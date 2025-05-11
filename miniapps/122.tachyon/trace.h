/* 
 * trace.h - This file contains the declarations for the main tracing calls.
 *
 *   $Id: trace.h 499 2006-10-06 19:58:28Z keeper $
 */

typedef struct {
  int tid;
  int nthr;
  scenedef * scene;
  unsigned long * local_mbox;
  unsigned long serialno; 
  int startx;
  int stopx;
  int xinc;
  int starty;
  int stopy;
  int yinc;
  rt_barrier_t * runbar; /* Thread barrier */
} thr_parms;

color trace(ray *);
void * thread_trace(thr_parms *); 

