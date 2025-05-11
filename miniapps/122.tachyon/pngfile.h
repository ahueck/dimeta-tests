/*
 *  pngfile.h - This file deals with PNG format image files (reading/writing)
 *
 *  $Id: pngfile.h 499 2006-10-06 19:58:28Z keeper $
 */ 

int readpng(char * name, int * xres, int * yres, unsigned char **imgdata);
int writepng(char * name, int xres, int yres, unsigned char *imgdata);
