/*
 *  ppm.h - This file deals with PPM format image files (reading/writing)
 *
 *  $Id: ppm.h 499 2006-10-06 19:58:28Z keeper $
 */ 

/* For our puposes, we're interested only in the 3 byte per pixel 24 bit
   truecolor sort of file..  Probably won't implement any decent checking
   at this point, probably choke on things like the # comments.. */

int readppm(char * name, int * xres, int * yres, unsigned char **imgdata);
int writeppm(char *name, int xres, int yres, unsigned char *imgdata);
