/*
 *  spectxt.c
 *  simple output filter that writes a text representation of the image
 *  for easy comparison in SPEC benchmark harness
 */ 

#include <stdio.h>
#include "machine.h"
#include "types.h"
#include "util.h"
#include "imageio.h" /* error codes etc */
#include "spectxt.h"

int writespectxt(char *name, int xres, int yres, unsigned char *imgdata)
{
  FILE *ofp;
  char txtbuf[80];
  int x, y;
  unsigned char r, g, b;
  double r_sum, g_sum, b_sum;

  if ((ofp = fopen(name, "wb")) != NULL)
    {
      for ( y = 0; y < yres; y++ )
        {
          r_sum = g_sum = b_sum = 0.0;
          for ( x = 0; x < (xres*3); x += 3 )
            {
              r = imgdata[(xres*3)*(yres - y - 1) + (x + 2)];
              g = imgdata[(xres*3)*(yres - y - 1) + (x + 1)];
              b = imgdata[(xres*3)*(yres - y - 1) + x];
              r_sum += (double)r;
              g_sum += (double)g;
              b_sum += (double)b;
            }
          sprintf(txtbuf, "%f %f %f\n", r_sum, g_sum, b_sum);
          fputs(txtbuf, ofp);
        }
      fclose(ofp);
    }
  return IMAGENOERR;
}
