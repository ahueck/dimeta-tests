/* 
 * ring.h - This file contains the defines for rings etc.
 *
 *  $Id: ring.h 499 2006-10-06 19:58:28Z keeper $
 */

object * newring(void * tex, vector ctr, vector norm, flt in, flt out);

#ifdef RING_PRIVATE 
typedef struct {
  RT_OBJECT_HEAD
  vector ctr;
  vector norm;
  flt inrad;
  flt outrad;
} ring; 

static int ring_bbox(void * obj, vector * min, vector * max);
static void ring_intersect(const ring *, ray *);
static void ring_normal(const ring *, const vector *, const ray * incident, vector *);
#endif

