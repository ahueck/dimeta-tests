/* 
 * quadric.h - This file contains the defines for quadrics.
 *
 *  $Id: quadric.h 499 2006-10-06 19:58:28Z keeper $
 */

typedef struct {
  flt a; flt b; flt c;
  flt d; flt e; flt f;
  flt g; flt h; flt i; flt j;
} quadmatrix;

 
typedef struct {
  RT_OBJECT_HEAD
  vector ctr;
  quadmatrix mat;
} quadric; 


quadric * newquadric(void);
void quadric_intersect(const quadric *, ray *);
void quadric_normal(const quadric *, const vector *, const ray *, vector *);



