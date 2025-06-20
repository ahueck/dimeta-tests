/* 
 * shade.c - This file contains the functions that perform surface shading.
 *
 *  $Id: shade.c 499 2006-10-06 19:58:28Z keeper $
 */

#include "machine.h"
#include "types.h"
#include "macros.h"
#include "threads.h"
#include "light.h"
#include "intersect.h"
#include "vector.h"
#include "trace.h"
#include "shade.h"


/*
 * Lowest Quality Shader - Returns the raw color of an object.
 *
 */

color lowest_shader(ray * incident) {
  int numints;
  object const * obj;
  flt t = FHUGE;
  color col;

  numints=closest_intersection(&t, &obj, incident);
                /* find the number of intersections */
                /* and return the closest one.      */

  if (numints < 1) {
    /* if there weren't any object intersections then return the */
    /* background color for the pixel color.                     */
    return incident->scene->background;
  }

  col.r = 1.0;
  col.g = 1.0;
  col.b = 1.0;

  return col;
}


/*
 * Low Quality Shader - Returns raw texture color of objects hit, nothing else.
 *
 */

color low_shader(ray * incident) {
  int numints;
  object const * obj;
  vector hit;
  flt t = FHUGE;

  numints=closest_intersection(&t, &obj, incident);
                /* find the number of intersections */
                /* and return the closest one.      */

  if (numints < 1) {
    /* if there weren't any object intersections then return the */
    /* background color for the pixel color.                     */
    return incident->scene->background;
  }

  RAYPNT(hit, (*incident), t) /* find the point of intersection from t */
  incident->opticdist = FHUGE; 
  return obj->tex->texfunc(&hit, obj->tex, incident);
}



/*
 * Medium Quality Shader - Includes a subset of the rendering features
 *
 */

color medium_shader(ray * incident) {
  color col, diffuse, phongcol; 
  shadedata shadevars;
  flt inten;
  flt t = FHUGE;
  object const * obj;
  int numints;
  list * cur;

  numints=closest_intersection(&t, &obj, incident);  
		/* find the number of intersections */
                /* and return the closest one.      */

  if (numints < 1) {         
    /* if there weren't any object intersections then return the */
    /* background color for the pixel color.                     */
    col = incident->scene->background;

    /* calculate fog effects */
    if (incident->scene->fog.fog_fctn != NULL) {
      col = incident->scene->fog.fog_fctn(&incident->scene->fog, col, t);
    }

    return col;
  }

  RAYPNT(shadevars.hit, (*incident), t) /* find point of intersection from t */ 
  incident->opticdist += t;
  obj->methods->normal(obj, &shadevars.hit, incident, &shadevars.N);  /* find the surface normal */

  /* Flip surface normal to point toward the viewer if necessary */
  if (VDot(&shadevars.N, &(incident->d)) > 0.0) {
    shadevars.N.x=-shadevars.N.x;
    shadevars.N.y=-shadevars.N.y;
    shadevars.N.z=-shadevars.N.z;
  }

  /* execute the object's texture function */
  col = obj->tex->texfunc(&shadevars.hit, obj->tex, incident); 

  if (obj->tex->flags & RT_TEXTURE_ISLIGHT) {  
                  /* if the current object is a light, then we  */
    return col;   /* will only use the object's base color      */
  }

  diffuse.r = 0.0; 
  diffuse.g = 0.0; 
  diffuse.b = 0.0; 
  phongcol = diffuse;

  if ((obj->tex->diffuse > MINCONTRIB) || (obj->tex->phong > MINCONTRIB)) {  
    cur = incident->scene->lightlist;
    while (cur != NULL) {              /* loop for light contributions */
      light * li=(light *) cur->item;  /* set li=to the current light  */
      inten = li->shade_diffuse(li, &shadevars);

      /* add in diffuse lighting for this light if we're facing it */ 
      if (inten > MINCONTRIB) {            
        /* XXX now that opacity is in the code, have to be more careful */
        ColorAddS(&diffuse, &((standard_texture *)li->tex)->col, inten);

        /* phong type specular highlights */
        if (obj->tex->phong > MINCONTRIB) {
          flt phongval;
          phongval = incident->scene->phongfunc(incident, &shadevars, obj->tex->phongexp); 
          if (obj->tex->phongtype == RT_PHONG_METAL) 
            ColorAddS(&phongcol, &col, phongval * obj->tex->phong);
          else
            ColorAddS(&phongcol, &((standard_texture *)li->tex)->col, phongval * obj->tex->phong);
        }
      }  

      cur = cur->next;
    } 
  }

  ColorScale(&diffuse, obj->tex->diffuse);

  col.r *= (diffuse.r + obj->tex->ambient); /* do a product of the */
  col.g *= (diffuse.g + obj->tex->ambient); /* diffuse intensity with  */
  col.b *= (diffuse.b + obj->tex->ambient); /* object color + ambient  */

  if (obj->tex->phong > MINCONTRIB) {
    ColorAccum(&col, &phongcol);
  }

  /* spawn reflection rays if necessary */
  /* note: this will overwrite the old intersection list */
  if (obj->tex->specular > MINCONTRIB) {    
    color specol;
    specol = shade_reflection(incident, &shadevars, obj->tex->specular);
    ColorAccum(&col, &specol);
  }

  /* spawn transmission rays / refraction */
  /* note: this will overwrite the old intersection list */
  if (obj->tex->opacity < (1.0 - MINCONTRIB)) {      
    color transcol;
    transcol = shade_transmission(incident, &shadevars, 1.0 - obj->tex->opacity);
    if (incident->scene->transmode == RT_TRANS_VMD) 
      ColorScale(&col, obj->tex->opacity);

    ColorAccum(&col, &transcol);
  }

  /* calculate fog effects */
  if (incident->scene->fog.fog_fctn != NULL) {
    col = incident->scene->fog.fog_fctn(&incident->scene->fog, col, t);
  }

  return col;    /* return the color of the shaded pixel... */
}



/*
 * Full Quality Shader - Includes all possible rendering features
 *
 */

color full_shader(ray * incident) {
  color col, diffuse, phongcol; 
  shadedata shadevars;
  ray shadowray;
  flt inten;
  flt t = FHUGE;
  object const * obj;
  int numints;
  list * cur;

  numints=closest_intersection(&t, &obj, incident);  
		/* find the number of intersections */
                /* and return the closest one.      */

  if (numints < 1) {         
    /* if there weren't any object intersections then return the */
    /* background color for the pixel color.                     */
    col = incident->scene->background;

    /* calculate fog effects */
    if (incident->scene->fog.fog_fctn != NULL) {
      col = incident->scene->fog.fog_fctn(&incident->scene->fog, col, t);
    }

    return col;
  }

  RAYPNT(shadevars.hit, (*incident), t) /* find point of intersection from t */ 
  incident->opticdist += t;
  obj->methods->normal(obj, &shadevars.hit, incident, &shadevars.N);  /* find the surface normal */

  /* Flip surface normal to point toward the viewer if necessary */
  if (VDot(&shadevars.N, &(incident->d)) > 0.0) { 
    shadevars.N.x=-shadevars.N.x;
    shadevars.N.y=-shadevars.N.y;
    shadevars.N.z=-shadevars.N.z;
  }

  /* execute the object's texture function */
  col = obj->tex->texfunc(&shadevars.hit, obj->tex, incident); 

  if (obj->tex->flags & RT_TEXTURE_ISLIGHT) {  
                  /* if the current object is a light, then we  */
    return col;   /* will only use the object's base color      */
  }

  diffuse.r = 0.0; 
  diffuse.g = 0.0; 
  diffuse.b = 0.0; 
  phongcol = diffuse;

  if ((obj->tex->diffuse > MINCONTRIB) || (obj->tex->phong > MINCONTRIB)) {  
    cur = incident->scene->lightlist;
    while (cur != NULL) {              /* loop for light contributions */
      light * li=(light *) cur->item;  /* set li=to the current light  */
      inten = li->shade_diffuse(li, &shadevars);

      /* add in diffuse lighting for this light if we're facing it */ 
      if (inten > MINCONTRIB) {            
        /* test for a shadow */
        shadowray.o   = shadevars.hit;
        shadowray.d   = shadevars.L;      
        shadowray.maxdist = shadevars.Llen;

        if (incident->scene->flags & RT_SHADE_CLIPPING) {
          shadowray.add_intersection = add_clipped_shadow_intersection;
        } else {
          shadowray.add_intersection = add_shadow_intersection;
        }

        shadowray.flags = RT_RAY_SHADOW;
        incident->serial++;
        shadowray.serial = incident->serial;
        shadowray.mbox = incident->mbox;
        shadowray.scene = incident->scene;
        intersect_objects(&shadowray); /* trace the shadow ray */

        if (!shadow_intersection(&shadowray)) {
          /* XXX now that opacity is in the code, have to be more careful */
          ColorAddS(&diffuse, &((standard_texture *)li->tex)->col, inten);

          /* phong type specular highlights */
          if (obj->tex->phong > MINCONTRIB) {
            flt phongval;
            phongval = incident->scene->phongfunc(incident, &shadevars, obj->tex->phongexp); 
            if (obj->tex->phongtype == RT_PHONG_METAL) 
              ColorAddS(&phongcol, &col, phongval * obj->tex->phong);
            else
              ColorAddS(&phongcol, &((standard_texture *)li->tex)->col, phongval * obj->tex->phong);
          }
        }
      }  

      cur = cur->next;
    } 
  }

  ColorScale(&diffuse, obj->tex->diffuse);

  col.r *= (diffuse.r + obj->tex->ambient); /* do a product of the */
  col.g *= (diffuse.g + obj->tex->ambient); /* diffuse intensity with  */
  col.b *= (diffuse.b + obj->tex->ambient); /* object color + ambient  */

  if (obj->tex->phong > MINCONTRIB) {
    ColorAccum(&col, &phongcol);
  }

  /* spawn reflection rays if necessary */
  /* note: this will overwrite the old intersection list */
  if (obj->tex->specular > MINCONTRIB) {    
    color specol;
    specol = shade_reflection(incident, &shadevars, obj->tex->specular);
    ColorAccum(&col, &specol);
  }

  /* spawn transmission rays / refraction */
  /* note: this will overwrite the old intersection list */
  if (obj->tex->opacity < (1.0 - MINCONTRIB)) {      
    color transcol;
    transcol = shade_transmission(incident, &shadevars, 1.0 - obj->tex->opacity);
    if (incident->scene->transmode == RT_TRANS_VMD) 
      ColorScale(&col, obj->tex->opacity);

    ColorAccum(&col, &transcol);
  }

  /* calculate fog effects */
  if (incident->scene->fog.fog_fctn != NULL) {
    col = incident->scene->fog.fog_fctn(&incident->scene->fog, col, t);
  }

  return col;    /* return the color of the shaded pixel... */
}


color shade_reflection(ray * incident, const shadedata * shadevars, flt specular) {
  ray specray;
  color col;
  vector R;

  /* Do recursion depth test immediately to early-exit ASAP */
  if (incident->depth <= 1) {
    /* if ray is truncated, return the background as its color */
    return incident->scene->background;
  }
  specray.depth=incident->depth - 1;     /* go up a level in recursion depth */
 
  VAddS(-2.0 * (incident->d.x * shadevars->N.x + 
                incident->d.y * shadevars->N.y + 
                incident->d.z * shadevars->N.z), &shadevars->N, &incident->d, &R);

  specray.o=shadevars->hit; 
  specray.d=R;			         /* reflect incident ray about normal */
  specray.o=Raypnt(&specray, EPSILON);   /* avoid numerical precision bugs */
  specray.maxdist = FHUGE;               /* take any intersection */
  specray.opticdist = incident->opticdist;
  specray.add_intersection=add_regular_intersection; /* ray type  */
  specray.flags = RT_RAY_REGULAR;        /* infinite ray, to start with */
  specray.serial = incident->serial + 1; /* next serial number */
  specray.mbox = incident->mbox; 
  specray.scene=incident->scene;         /* global scenedef info */
  specray.randval=incident->randval;     /* random number seed */

  /* inlined code from trace() to eliminate one level of recursion */
  intersect_objects(&specray);           /* trace specular reflection ray */
  col=specray.scene->shader(&specray);

  incident->serial = specray.serial;     /* update the serial number */

  ColorScale(&col, specular);

  return col;
}


color shade_transmission(ray * incident, const shadedata * shadevars, flt trans) {
  ray transray;
  color col;

  /* Do recursion depth test immediately to early-exit ASAP */
  if (incident->depth <= 1) {
    /* if ray is truncated, return the background as its color */
    return incident->scene->background;
  }
  transray.depth=incident->depth - 1;    /* go up a level in recursion depth */

  transray.o=shadevars->hit; 
  transray.d=incident->d;                /* ray continues on incident path */
  transray.o=Raypnt(&transray, EPSILON); /* avoid numerical precision bugs */
  transray.maxdist = FHUGE;              /* take any intersection */
  transray.opticdist = incident->opticdist;
  transray.add_intersection=add_regular_intersection; /* ray type  */
  transray.depth=incident->depth - 1;    /* go up a level in recursion depth */
  transray.flags = RT_RAY_REGULAR;       /* infinite ray, to start with */
  transray.serial = incident->serial + 1; /* update serial number */
  transray.mbox = incident->mbox;
  transray.scene=incident->scene;        /* global scenedef info */
  transray.randval=incident->randval;    /* random number seed */

  /* inlined code from trace() to eliminate one level of recursion */
  intersect_objects(&transray);          /* trace transmission ray */
  col=transray.scene->shader(&transray);

  incident->serial = transray.serial;

  ColorScale(&col, trans);

  return col;
}


/*
 * Phong shader, always returns 0.0, used for testing
 */
flt shade_nullphong(const ray * incident, const shadedata * shadevars, flt specpower) {
  return 0.0;
}

/*
 * Phong shader, implements specular highlight model
 * using Blinn's halfway vector dotted with the surface normal.
 * This is also the shading model used by OpenGL and Direct3D.
 */
flt shade_blinn(const ray * incident, const shadedata * shadevars, flt specpower) {
  vector H;   /* Blinn's halfway vector */
  flt inten;  /* calculated intensity   */

  /* since incident ray is negated direction to viewer, we subtract... */
  /* sub. incoming ray dir. from light direction */
  H.x = shadevars->L.x - incident->d.x; 
  H.y = shadevars->L.y - incident->d.y;
  H.z = shadevars->L.z - incident->d.z;

  inten = shadevars->N.x * H.x + shadevars->N.y * H.y + shadevars->N.z * H.z;
  if (inten > MINCONTRIB) {
    /* normalize the previous dot product */
    inten /= sqrt(H.x * H.x + H.y * H.y + H.z * H.z);

    /* calculate specular exponent */
    inten = pow(inten, specpower);
  } else {
    inten = 0.0;
  }

  return inten;
}



/*
 * Phong shader, implements specular highlight model
 * using Blinn's halfway vector dotted with the surface normal.
 * This is also the shading model used by OpenGL and Direct3D.
 * Uses Graphics Gems IV chapter VI.1 algorithm for phong exponent
 * instead of the usual call to pow(). 
 */
flt shade_blinn_fast(const ray * incident, const shadedata * shadevars, flt specpower) {
  vector H;   /* Blinn's halfway vector */
  flt inten;  /* calculated intensity   */

  /* since incident ray is negated direction to viewer, we subtract... */
  /* sub. incoming ray dir. from light direction */
  H.x = shadevars->L.x - incident->d.x; 
  H.y = shadevars->L.y - incident->d.y;
  H.z = shadevars->L.z - incident->d.z;

  inten = shadevars->N.x * H.x + shadevars->N.y * H.y + shadevars->N.z * H.z;
  if (inten > 0.0) {
    /* normalize the previous dot product */
    inten /= sqrt(H.x * H.x + H.y * H.y + H.z * H.z);

    /* replace specular exponent with a simple approximation */
    inten = inten / (specpower - (specpower * inten) + inten);
  } else {
    inten = 0.0;
  }

  return inten;
} 

/*
 * Phong shader, implements a Phong specular highlight model
 * using the reflection vector about the surface normal, dotted
 * with the direction to the viewer.  This is the "classic" phong
 */
flt shade_phong(const ray * incident, const shadedata * shadevars, flt specpower) {
  vector R;   /* reflection vector      */
  vector V;   /* direction to viewpoint */
  vector LL;  /* reverse direction to light     */
  flt inten;  /* calculated intensity   */

  LL = shadevars->L;
  VScale(&LL, -1.0);
  VAddS(-2.0 * (LL.x * shadevars->N.x + 
                LL.y * shadevars->N.y + 
                LL.z * shadevars->N.z), &shadevars->N, &LL, &R);

  V = incident->d;
  VScale(&V, -1.0);
  VNorm(&R);            /* normalize reflection vector */
  inten = VDot(&V, &R); /* dot product of halfway vector and surface normal */
  if (inten > 0.0)     
    inten = pow(inten, specpower);
  else 
    inten = 0.0;

  return inten;
} 


/*
 * Fog functions
 */
color fog_color_linear(struct fogdata_t * fog, color col, flt z) {
  color c; 
  flt f;

  f = (fog->end - z) / (fog->end - fog->start) * fog->density;
  if (f > 1.0) {
    f = 1.0;
  } else if (f < 0.0) {
    f = 0.0;
  } 

  c.r = (f * col.r) + ((1 - f) * fog->col.r);
  c.g = (f * col.g) + ((1 - f) * fog->col.g);
  c.b = (f * col.b) + ((1 - f) * fog->col.b);

  return c;
}

color fog_color_exp(struct fogdata_t * fog, color col, flt z) {
  color c; 
  flt f, v;

  v = fog->density * (z - fog->start);
  f = exp(-v);
  if (f > 1.0) {
    f = 1.0;
  } else if (f < 0.0) {
    f = 0.0;
  } 

  c.r = (f * col.r) + ((1 - f) * fog->col.r);
  c.g = (f * col.g) + ((1 - f) * fog->col.g);
  c.b = (f * col.b) + ((1 - f) * fog->col.b);

  return c;
}

color fog_color_exp2(struct fogdata_t * fog, color col, flt z) {
  color c; 
  flt f, v;
  
  v = fog->density * (z - fog->start);
  f = exp(-v*v);
  if (f > 1.0) {
    f = 1.0;
  } else if (f < 0.0) {
    f = 0.0;
  } 

  c.r = (f * col.r) + ((1 - f) * fog->col.r);
  c.g = (f * col.g) + ((1 - f) * fog->col.g);
  c.b = (f * col.b) + ((1 - f) * fog->col.b);

  return c;
}

