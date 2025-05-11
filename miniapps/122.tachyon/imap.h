/*
 * imap.h - This file contains defines etc for doing image map type things.  
 *
 *  $Id: imap.h 499 2006-10-06 19:58:28Z keeper $
 */

void       ResetImage(void);
void       LoadImage(rawimage *);
rawimage * AllocateImage(const char *);
void       DeallocateImage(rawimage *);
void       ResetImages(void);
void       FreeImages(void);
rawimage * DecimateImage(const rawimage *);
mipmap *   LoadMIPMap(const char *);
mipmap *   CreateMIPMap(rawimage *);
void       FreeMIPMap(mipmap * mip);
color      MIPMap(const mipmap *, flt, flt, flt);
color      ImageMap(const rawimage *, flt, flt);
 
