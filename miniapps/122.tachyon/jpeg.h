/*
 *  jpeg.h - This file deals with JPEG format image files (reading/writing)
 *
 *  $Id: jpeg.h 499 2006-10-06 19:58:28Z keeper $
 */ 

int readjpeg(char * name, int * xres, int * yres, unsigned char **imgdata);
int writejpeg(char * name, int xres, int yres, unsigned char *imgdata);
