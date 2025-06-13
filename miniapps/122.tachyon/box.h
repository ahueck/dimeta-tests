/* 
 * box.h - This file contains the defines for boxes etc.
 *
 *  $Id: box.h 499 2006-10-06 19:58:28Z keeper $
 */

 
typedef struct {
  RT_OBJECT_HEAD
  vector min;
  vector max;
} box; 


box * newbox(void * tex, vector min, vector max);
void box_intersect(const box *, ray *);
void box_normal(const box *, const vector *, const ray *, vector *);
