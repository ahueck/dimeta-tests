/*
 * parvol.h - Volume rendering definitions etc.
 *
 *
 *  $Id: parvol.h 499 2006-10-06 19:58:28Z keeper $
 */

typedef struct {
  RT_OBJECT_HEAD
  vector min;
  vector max;
  flt ambient;
  flt diffuse;
  flt opacity;
  int samples;
  flt (* evaluator)(flt, flt, flt);
} parvol;

parvol * newparvol();
color par_volume_texture(vector *, texture *, ray *);


