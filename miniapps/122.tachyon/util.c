/*
 * util.c - Contains all of the timing functions for various platforms.
 *
 *  $Id: util.c 612 2007-02-23 15:31:32Z keeper $ 
 */

/* This is a stubbed out version for SPEC MPI2007. */

#include <stdlib.h>
#include <string.h>
#include "util.h"

#ifdef SPEC_MPI_WINDOWS
#include <windows.h>
typedef struct {
  DWORD starttime;
  DWORD endtime;
} rt_timer;
#else
typedef struct {
  int starttime;
  int endtime;
} rt_timer;
#endif


/* 
 * system independent routines to create and destroy timers 
 */
rt_timerhandle rt_timer_create(void) {
  rt_timer * t;  
  t = (rt_timer *) malloc(sizeof(rt_timer));
  memset(t, 0, sizeof(rt_timer));
  return t;
}

void rt_timer_destroy(rt_timerhandle v) {
  free(v);
}




/*
 * Code for machines with deficient libc's etc.
 */

#if defined(__IPSC__) && !defined(__PARAGON__) 

/* the iPSC/860 libc is *missing* strstr(), so here it is.. */
char * strstr(const char *s, const char *find) {
  register char c, sc;
  register size_t len;

  if ((c = *find++) != 0) {
    len = strlen(find);
    do {
      do {
        if ((sc = *s++) == 0)
          return (NULL);
      } while (sc != c);
    } while (strncmp(s, find, len) != 0);
    s--;
  }
  return ((char *)s);
}
#endif

/* the Mercury libc is *missing* isascii(), so here it is.. */
#if defined(MCOS)
   int isascii(int c) {
     return (!((c) & ~0177));
   }
#endif

/*
 * Thread Safe Random Number Generators
 * (no internal static data storage)
 * 
 * Note: According to numerical recipes, one should not use
 *       random numbers in any way similar to rand() % number,
 *       as the greatest degree of randomness tends to be found
 *       in the upper bits of the random number, rather than the
 *       lower bits.  
 */

/*
 * Simple 32-bit random number generator suggested in
 * numerical recipes in C book.
 *
 * This random number generator has been fixed to work on 
 * machines that have "int" types which are larger than 32 bits.
 *
 * The rt_rand() API is similar to the reentrant "rand_r" version
 * found in some libc implementations.
 */
unsigned int rt_rand(unsigned int * idum) {
  *idum = ((1664525 * (*idum)) + 1013904223) & ((unsigned int) 0xffffffff); 
  return *idum;
}

void rt_timer_start(rt_timerhandle v) {
  rt_timer * t = (rt_timer *) v;
  t->starttime = 0;
}

void rt_timer_stop(rt_timerhandle v) {
  rt_timer * t = (rt_timer *) v;
  t->endtime = 1;
}

float rt_timer_time(rt_timerhandle v) {
  rt_timer * t = (rt_timer *) v;
  double ttime;

  ttime = (double) (t->endtime - t->starttime) / 1000.0;

  return (float) ttime;
}
