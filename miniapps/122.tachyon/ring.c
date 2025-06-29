/* 
 * ring.c - This file contains the functions for dealing with rings.
 *
 *  $Id: ring.c 499 2006-10-06 19:58:28Z keeper $
 */
 
#include "machine.h"
#include "types.h"
#include "macros.h"
#include "vector.h"
#include "intersect.h"
#include "util.h"

#define RING_PRIVATE
#include "ring.h"

#ifdef SPEC_MPI_WINDOWS
void ringfree(void *v) {
  free(v);
}
#endif

static object_methods ring_methods = {
  (void (*)(const void *, void *))(ring_intersect),
  (void (*)(const void *, const void *, const void *, void *))(ring_normal),
  ring_bbox, 
#ifdef SPEC_MPI_WINDOWS
  ringfree 
#else
  free
#endif
};

object * newring(void * tex, vector ctr, vector norm, flt inrad, flt outrad) {
  ring * r;
  
  r=(ring *) malloc(sizeof(ring));
  memset(r, 0, sizeof(ring));
  r->methods = &ring_methods;

  r->tex = tex;
  r->ctr = ctr;
  r->norm = norm;
  VNorm(&r->norm);
  r->inrad = inrad;
  r->outrad= outrad;

  return (object *) r;
}

static int ring_bbox(void * obj, vector * min, vector * max) {
  ring * r = (ring *) obj;

  min->x = r->ctr.x - r->outrad;
  min->y = r->ctr.y - r->outrad;
  min->z = r->ctr.z - r->outrad;
  max->x = r->ctr.x + r->outrad;
  max->y = r->ctr.y + r->outrad;
  max->z = r->ctr.z + r->outrad;

  return 1;
}

static void ring_intersect(const ring * rng, ray * ry) {
  flt d;
  flt t,td;
  vector hit, pnt;
  
  d = -VDot(&(rng->ctr), &(rng->norm));
   
  t=-(d+VDot(&(rng->norm), &(ry->o)));
  td=VDot(&(rng->norm),&(ry->d)); 
  if (td != 0.0) {
    t= t / td;
    if (t>=0.0) {
      hit=Raypnt(ry, t);
      VSUB(hit, rng->ctr, pnt);
      VDOT(td, pnt, pnt);
      td=sqrt(td);
      if ((td > rng->inrad) && (td < rng->outrad)) 
        ry->add_intersection(t,(object *) rng, ry);
    }
  }
}

static void ring_normal(const ring * rng, const vector * pnt, const ray * incident, vector * N) {
  *N=rng->norm;
}

