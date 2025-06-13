/* 
 * tgafile.h - this file contains defines and structures for tgafile.c
 *
 *  $Id: tgafile.h 499 2006-10-06 19:58:28Z keeper $
 */

/* declare other functions */
int createtgafile(char *, unsigned short, unsigned short);
void * opentgafile(char *);
void writetgaregion(void *, int, int, int, int, unsigned char *);
void closetgafile(void *);
int readtga(char * name, int * xres, int * yres, unsigned char **imgdata);
int writetga(char * name, int xres, int yres, unsigned char *imgdata);
