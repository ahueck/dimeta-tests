/*
 * vol.h - Volume rendering definitions etc.
 *
 *
 *  $Id: vol.h 499 2006-10-06 19:58:28Z keeper $
 */


void * newscalarvol(void * intex, vector min, vector max, 
                    int xs, int ys, int zs, 
                    const char * fname, scalarvol * invol);

void  LoadVol(scalarvol *);
color scalar_volume_texture(const vector *, const texture *, ray *);

