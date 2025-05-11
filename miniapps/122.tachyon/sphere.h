/* 
 * sphere.h - This file contains the defines for spheres etc.
 *
 *  $Id: sphere.h 499 2006-10-06 19:58:28Z keeper $
 */

object * newsphere(void *, vector, flt);

#ifdef SPHERE_PRIVATE

typedef struct {
  RT_OBJECT_HEAD
  vector ctr;
  flt rad;
} sphere;

static int sphere_bbox(void * obj, vector * min, vector * max);
static void sphere_intersect(const sphere *, ray *);
static void sphere_normal(const sphere *, const vector *, const ray *, vector *);

#endif /* SPHERE_PRIVATE */

