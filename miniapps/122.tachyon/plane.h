/* 
 * plane.h - This file contains the defines for planes etc.
 *
 *  $Id: plane.h 499 2006-10-06 19:58:28Z keeper $
 */

 
object * newplane(void * tex, vector ctr, vector norm);

#ifdef PLANE_PRIVATE
typedef struct {
  RT_OBJECT_HEAD
  flt d;
  vector norm;
} plane; 

static void plane_intersect(const plane *, ray *);
static int plane_bbox(void * obj, vector * min, vector * max);
static void plane_normal(const plane *, const vector *, const ray * incident, vector *);
#endif

