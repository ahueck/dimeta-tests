/* 
 * quadric.c - This file contains the functions for dealing with quadrics.
 *
 *  $Id: quadric.c 499 2006-10-06 19:58:28Z keeper $
 */

#include "machine.h"
#include "types.h"
#include "macros.h"
#include "quadric.h"
#include "vector.h"
#include "intersect.h"
#include "util.h"

int quadric_bbox(void * obj, vector * min, vector * max) {
  return 0;
}

#ifdef SPEC_MPI_WINDOWS
void quadricfree(void *v) {
  free(v);
}
#endif

static object_methods quadric_methods = {
  (void (*)(const void *, void *))(quadric_intersect),
  (void (*)(const void *, const void *, const void *, void *))(quadric_normal),
  quadric_bbox, 
#ifdef SPEC_MPI_WINDOWS
  quadricfree 
#else
  free
#endif
};
 
quadric * newquadric(void) {
  quadric * q;
 
  q=(quadric *) malloc(sizeof(quadric));
  memset(q, 0, sizeof(quadric));
  q->ctr.x=0.0;
  q->ctr.y=0.0;
  q->ctr.z=0.0;
  q->methods = &quadric_methods;
 
  return q;
}

void quadric_intersect(const quadric * q, ray * ry) {
  flt Aq, Bq, Cq;
  flt t1, t2;
  flt disc;
  vector rd;
  vector ro;
 
  rd=ry->d;
  VNorm(&rd);

  ro.x =  ry->o.x - q->ctr.x;
  ro.y =  ry->o.y - q->ctr.y;
  ro.z =  ry->o.z - q->ctr.z;


  Aq = (q->mat.a*(rd.x * rd.x)) +
        (2.0 * q->mat.b * rd.x * rd.y) +
        (2.0 * q->mat.c * rd.x * rd.z) +
        (q->mat.e * (rd.y * rd.y)) +
        (2.0 * q->mat.f * rd.y * rd.z) +
        (q->mat.h * (rd.z * rd.z));

  Bq = 2.0 * (
        (q->mat.a * ro.x * rd.x) +
        (q->mat.b * ((ro.x * rd.y) + (rd.x * ro.y))) +
        (q->mat.c * ((ro.x * rd.z) + (rd.x * ro.z))) +
        (q->mat.d * rd.x) +
        (q->mat.e * ro.y * rd.y) +
        (q->mat.f * ((ro.y * rd.z) + (rd.y * ro.z))) +
        (q->mat.g * rd.y) +
        (q->mat.h * ro.z * rd.z) +
        (q->mat.i * rd.z)
        );

  Cq = (q->mat.a * (ro.x * ro.x)) +
        (2.0 * q->mat.b * ro.x * ro.y) +
        (2.0 * q->mat.c * ro.x * ro.z) +
        (2.0 * q->mat.d * ro.x) +
        (q->mat.e * (ro.y * ro.y)) +
        (2.0 * q->mat.f * ro.y * ro.z) +
        (2.0 * q->mat.g * ro.y) +
        (q->mat.h * (ro.z * ro.z)) +
        (2.0 * q->mat.i * ro.z) +
        q->mat.j;

  if (Aq == 0.0) {
          t1 = - Cq / Bq;
          ry->add_intersection(t1, (object *) q, ry);
          }
  else {
    disc=(Bq*Bq - 4.0 * Aq * Cq);
    if (disc > 0.0) {
          disc=sqrt(disc);
          t1 = (-Bq + disc) / (2.0 * Aq);
          t2 = (-Bq - disc) / (2.0 * Aq);
          ry->add_intersection(t1, (object *) q, ry);
          ry->add_intersection(t2, (object *) q, ry); 
          }
  }
}

void quadric_normal(const quadric * q, const vector * pnt, const ray * incident, vector * N) {
  flt len;

  N->x = (q->mat.a*(pnt->x - q->ctr.x) + 
	  q->mat.b*(pnt->y - q->ctr.y) + 
	  q->mat.c*(pnt->z - q->ctr.z) + q->mat.d);

  N->y = (q->mat.b*(pnt->x - q->ctr.x) + 
	  q->mat.e*(pnt->y - q->ctr.y) + 
	  q->mat.f*(pnt->z - q->ctr.z) + q->mat.g);

  N->z = (q->mat.c*(pnt->x - q->ctr.x) + 
	  q->mat.f*(pnt->y - q->ctr.y) + 
	  q->mat.h*(pnt->z - q->ctr.z) + q->mat.i);

  len = sqrt(N->x*N->x + N->y*N->y + N->z*N->z);
  N->x /= len;
  N->y /= len;
  N->z /= len;
}
 

