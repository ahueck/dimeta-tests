/*
 *  winbmp.h - This file deals with Windows Bitmap image files 
 *             (reading/writing)
 *
 *  $Id: winbmp.h 499 2006-10-06 19:58:28Z keeper $
 */ 

int writebmp(char * name, int xres, int yres, unsigned char *imgdata);
