/****************** io_nonansi.c *****************************************/
/* MIMD version 6 */
/* Parallel I/O routines for the SU3 program */
/*   MIMD version 6. */

/* Wrappers for parallel file access. */
/* This version uses non ANSI "open" "close" "read" "write" and "lseek" */
/* This is the version we need for the T3E */

/* These are patterned after stdio fopen, fseek, fwrite, fread, fclose */
/* We need them because some systems (e.g. Intel Paragon and SP2) 
   use home-made routines, rather than using ANSI standard calls.

   NOTE: Once a file is opened with g_open, it should not be accessed
   with ANSI routines, even though it pretends to return a FILE * pointer */

#include "generic_includes.h"
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
/*#include <unistd.h>*/
/*Change made for SPEC CPU2004*/
#ifndef SPEC_MPI_WINDOWS
#include <unistd.h>
#endif

FILE *g_open(const char *filename, const char *mode)
{
  int fd,oflg;
  int *fp;

  /* Here we support only three ways to open the file:
     wb : create a new binary file for writing
     rb : open an existing binary file for reading
     rb+: open an existing binary file for reading and/or writing  */

  /* Decide on the mode for opening */

  if(mode[0] == 'a')
    {
      printf("g_open: Node %d. Append not supported in PIOFS\n",this_node);
      return NULL;
    }
  
  else if(mode[0] == 'w')
    {
      oflg = O_WRONLY | O_CREAT;
    }
  
  else if(mode[0] == 'r')
    {
      oflg = O_RDONLY;
      if(strchr(mode,'+') != NULL)
	oflg = O_RDWR;
    }

  else
    {
      printf("g_open: Node %d. mode %s not recognized\n",this_node,mode);
      return NULL;
    }

  /* Now open the file */

  if( (fd = open(filename, oflg, 00644)) == -1)
    {
      printf("g_open: Node %d error %d opening %s\n",
	     this_node,errno,filename);
      return NULL;
    }
  
  /* Set up a dummy file pointer structure */
  /* Since the structure FILE is system dependent, we use a pointer
     to an integer instead, and pretend that it is a FILE 
     No routines other than those in this package are supposed
     to look for "members" of "FILE" */

  fp = (int *)malloc(sizeof(int));
  if(fp == NULL)
    {
      printf("g_open: Node %d can't malloc fp\n",this_node);
      fflush(stdout); terminate(1);
    }

  *fp = fd;

  return (FILE *)fp;
}

int g_seek(FILE *stream, off_t offset, int whence)
{
  int fd;
  fd = *((int *)stream);

  return lseek(fd, (off_t)offset, whence );
}

size_t g_write(const void *ptr, size_t size, size_t nmemb,FILE *stream)
{
  int fd;
  fd = *((int *)stream);

  return write( fd, ptr, (int)(size*nmemb) )/size;
}

size_t g_read(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  int fd;
  fd = *((int *)stream);

  return read( fd, ptr, (int)(size*nmemb) )/size;
}

int g_close(FILE *stream)
{
  int fd, status;
  fd = *((int *)stream);

  status = close(fd);
  free(stream);
  return status;
}

