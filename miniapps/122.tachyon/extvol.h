/*
 * vol.h - Volume rendering definitions etc.
 *
 *
 *  $Id: extvol.h 499 2006-10-06 19:58:28Z keeper $
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
} extvol;

extvol * newextvol(void * voidtex, vector min, vector max, 
                   int samples, flt (* evaluator)(flt, flt, flt));
color ext_volume_texture(const vector *, const texture *, ray *);


