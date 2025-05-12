

// RUN: %apply-dimeta-mpicc -O0 %s |& %filecheck %s

// ground truth (unordered): %clang-record-decl-mpicc %s

#include "include/types.h"

#include <stdlib.h>

texture g_texture;
standard_texture g_standard_texture;
vcstri_texture g_vcstri_texture;
object_methods g_object_methods;
clip_group g_clip_group;
object g_object;
intersection g_intersection;
intersectstruct g_intersectstruct;
camdef g_camdef;
fogdata g_fogdata;
nodeinfo g_nodeinfo;
list g_list;
shadedata g_shadedata;
displist g_displist;
scenedef g_scenedef;
ray g_ray;

void foo() {
  // clang-format off
  texture *t = (texture *)malloc(sizeof(texture));
  standard_texture *st = (standard_texture *)malloc(sizeof(standard_texture));
  vcstri_texture *vt = (vcstri_texture *)malloc(sizeof(vcstri_texture));
  object_methods *om = (object_methods *)malloc(sizeof(object_methods));
  clip_group *cg = (clip_group *)malloc(sizeof(clip_group));
  object *obj = (object *)malloc(sizeof(object));
  intersection *isect = (intersection *)malloc(sizeof(intersection));
  intersectstruct *is = (intersectstruct *)malloc(sizeof(intersectstruct));
  camdef *cam = (camdef *)malloc(sizeof(camdef));
  fogdata *fog = (fogdata *)malloc(sizeof(fogdata));
  nodeinfo *ni = (nodeinfo *)malloc(sizeof(nodeinfo));
  list *lst = (list *)malloc(sizeof(list));
  shadedata *sd = (shadedata *)malloc(sizeof(shadedata));
  displist *dp = (displist *)malloc(sizeof(displist));
  scenedef *scene = (scenedef *)malloc(sizeof(scenedef));
  ray *r = (ray *)malloc(sizeof(ray));
}

// CHECK:         0 | texture
// CHECK-NEXT:         0 |   fptr * texfunc
// CHECK-NEXT:         8 |   texture_methods * methods
// CHECK-NEXT:        16 |   unsigned int flags
// CHECK-NEXT:        20 |   float ambient
// CHECK-NEXT:        24 |   float diffuse
// CHECK-NEXT:        28 |   float phong
// CHECK-NEXT:        32 |   float phongexp
// CHECK-NEXT:        36 |   int phongtype
// CHECK-NEXT:        40 |   float specular
// CHECK-NEXT:        44 |   float opacity
// CHECK-NEXT:           | [sizeof=48]

// CHECK:         0 | standard_texture
// CHECK-NEXT:         0 |   fptr * texfunc
// CHECK-NEXT:         8 |   texture_methods * methods
// CHECK-NEXT:        16 |   unsigned int flags
// CHECK-NEXT:        20 |   float ambient
// CHECK-NEXT:        24 |   float diffuse
// CHECK-NEXT:        28 |   float phong
// CHECK-NEXT:        32 |   float phongexp
// CHECK-NEXT:        36 |   int phongtype
// CHECK-NEXT:        40 |   float specular
// CHECK-NEXT:        44 |   float opacity
// CHECK-NEXT:        48 |   color col
// CHECK-NEXT:        48 |     float r
// CHECK-NEXT:        52 |     float g
// CHECK-NEXT:        56 |     float b
// CHECK-NEXT:        64 |   vector ctr
// CHECK-NEXT:        64 |     flt x
// CHECK-NEXT:        72 |     flt y
// CHECK-NEXT:        80 |     flt z
// CHECK-NEXT:        88 |   vector rot
// CHECK-NEXT:        88 |     flt x
// CHECK-NEXT:        96 |     flt y
// CHECK-NEXT:       104 |     flt z
// CHECK-NEXT:       112 |   vector scale
// CHECK-NEXT:       112 |     flt x
// CHECK-NEXT:       120 |     flt y
// CHECK-NEXT:       128 |     flt z
// CHECK-NEXT:       136 |   vector uaxs
// CHECK-NEXT:       136 |     flt x
// CHECK-NEXT:       144 |     flt y
// CHECK-NEXT:       152 |     flt z
// CHECK-NEXT:       160 |   vector vaxs
// CHECK-NEXT:       160 |     flt x
// CHECK-NEXT:       168 |     flt y
// CHECK-NEXT:       176 |     flt z
// CHECK-NEXT:       184 |   void * img
// CHECK-NEXT:       192 |   void * obj
// CHECK-NEXT:           | [sizeof=200]

// CHECK:         0 | vcstri_texture
// CHECK-NEXT:         0 |   fptr * texfunc
// CHECK-NEXT:         8 |   texture_methods * methods
// CHECK-NEXT:        16 |   unsigned int flags
// CHECK-NEXT:        20 |   float ambient
// CHECK-NEXT:        24 |   float diffuse
// CHECK-NEXT:        28 |   float phong
// CHECK-NEXT:        32 |   float phongexp
// CHECK-NEXT:        36 |   int phongtype
// CHECK-NEXT:        40 |   float specular
// CHECK-NEXT:        44 |   float opacity
// CHECK-NEXT:        48 |   void * obj
// CHECK-NEXT:        56 |   color c0
// CHECK-NEXT:        56 |     float r
// CHECK-NEXT:        60 |     float g
// CHECK-NEXT:        64 |     float b
// CHECK-NEXT:        68 |   color c1
// CHECK-NEXT:        68 |     float r
// CHECK-NEXT:        72 |     float g
// CHECK-NEXT:        76 |     float b
// CHECK-NEXT:        80 |   color c2
// CHECK-NEXT:        80 |     float r
// CHECK-NEXT:        84 |     float g
// CHECK-NEXT:        88 |     float b
// CHECK-NEXT:           | [sizeof=96]

// CHECK:         0 | object_methods
// CHECK-NEXT:         0 |   fptr * intersect
// CHECK-NEXT:         8 |   fptr * normal
// CHECK-NEXT:        16 |   fptr * bbox
// CHECK-NEXT:        24 |   fptr * freeobj
// CHECK-NEXT:           | [sizeof=32]

// CHECK:         0 | clip_group
// CHECK-NEXT:         0 |   int numplanes
// CHECK-NEXT:         8 |   flt * planes
// CHECK-NEXT:           | [sizeof=16]

// CHECK:         0 | object
// CHECK-NEXT:         0 |   unsigned int id
// CHECK-NEXT:         8 |   void * nextobj
// CHECK-NEXT:        16 |   object_methods * methods
// CHECK-NEXT:        24 |   clip_group * clip
// CHECK-NEXT:        32 |   texture * tex
// CHECK-NEXT:           | [sizeof=40]

// CHECK:         0 | intersection
// CHECK-NEXT:         0 |   const object * obj
// CHECK-NEXT:         8 |   flt t
// CHECK-NEXT:           | [sizeof=16]

// CHECK:         0 | intersectstruct
// CHECK-NEXT:         0 |   int num
// CHECK-NEXT:         8 |   intersection closest
// CHECK-NEXT:         8 |     const object * obj
// CHECK-NEXT:        16 |     flt t
// CHECK-NEXT:           | [sizeof=24]

// CHECK:         0 | camdef
// CHECK-NEXT:         0 |   int projection
// CHECK-NEXT:         8 |   vector center
// CHECK-NEXT:         8 |     flt x
// CHECK-NEXT:        16 |     flt y
// CHECK-NEXT:        24 |     flt z
// CHECK-NEXT:        32 |   vector viewvec
// CHECK-NEXT:        32 |     flt x
// CHECK-NEXT:        40 |     flt y
// CHECK-NEXT:        48 |     flt z
// CHECK-NEXT:        56 |   vector rightvec
// CHECK-NEXT:        56 |     flt x
// CHECK-NEXT:        64 |     flt y
// CHECK-NEXT:        72 |     flt z
// CHECK-NEXT:        80 |   vector upvec
// CHECK-NEXT:        80 |     flt x
// CHECK-NEXT:        88 |     flt y
// CHECK-NEXT:        96 |     flt z
// CHECK-NEXT:       104 |   flt camzoom
// CHECK-NEXT:       112 |   flt px
// CHECK-NEXT:       120 |   flt py
// CHECK-NEXT:       128 |   flt psx
// CHECK-NEXT:       136 |   flt psy
// CHECK-NEXT:       144 |   flt focallength
// CHECK-NEXT:       152 |   flt left
// CHECK-NEXT:       160 |   flt right
// CHECK-NEXT:       168 |   flt top
// CHECK-NEXT:       176 |   flt bottom
// CHECK-NEXT:       184 |   flt aperture
// CHECK-NEXT:       192 |   vector projcent
// CHECK-NEXT:       192 |     flt x
// CHECK-NEXT:       200 |     flt y
// CHECK-NEXT:       208 |     flt z
// CHECK-NEXT:       216 |   fptr * cam_ray
// CHECK-NEXT:       224 |   vector lowleft
// CHECK-NEXT:       224 |     flt x
// CHECK-NEXT:       232 |     flt y
// CHECK-NEXT:       240 |     flt z
// CHECK-NEXT:       248 |   vector iplaneright
// CHECK-NEXT:       248 |     flt x
// CHECK-NEXT:       256 |     flt y
// CHECK-NEXT:       264 |     flt z
// CHECK-NEXT:       272 |   vector iplaneup
// CHECK-NEXT:       272 |     flt x
// CHECK-NEXT:       280 |     flt y
// CHECK-NEXT:       288 |     flt z
// CHECK-NEXT:           | [sizeof=296]

// CHECK:         0 | struct fogdata_t
// CHECK-NEXT:         0 |   fptr * fog_fctn
// CHECK-NEXT:         8 |   color col
// CHECK-NEXT:         8 |     float r
// CHECK-NEXT:        12 |     float g
// CHECK-NEXT:        16 |     float b
// CHECK-NEXT:        24 |   flt start
// CHECK-NEXT:        32 |   flt end
// CHECK-NEXT:        40 |   flt density
// CHECK-NEXT:           | [sizeof=48]

// CHECK:         0 | nodeinfo
// CHECK-NEXT:         0 |   int numcpus
// CHECK-NEXT:         8 |   flt cpuspeed
// CHECK-NEXT:        16 |   flt nodespeed
// CHECK-NEXT:        24 |   char[512] machname
// CHECK-NEXT:           | [sizeof=536]

// CHECK:         0 | struct list
// CHECK-NEXT:         0 |   void * item
// CHECK-NEXT:         8 |   struct list * next
// CHECK-NEXT:           | [sizeof=16]

// CHECK:         0 | shadedata
// CHECK-NEXT:         0 |   vector hit
// CHECK-NEXT:         0 |     flt x
// CHECK-NEXT:         8 |     flt y
// CHECK-NEXT:        16 |     flt z
// CHECK-NEXT:        24 |   vector N
// CHECK-NEXT:        24 |     flt x
// CHECK-NEXT:        32 |     flt y
// CHECK-NEXT:        40 |     flt z
// CHECK-NEXT:        48 |   vector L
// CHECK-NEXT:        48 |     flt x
// CHECK-NEXT:        56 |     flt y
// CHECK-NEXT:        64 |     flt z
// CHECK-NEXT:        72 |   flt Llen
// CHECK-NEXT:           | [sizeof=80]

// CHECK:         0 | displist
// CHECK-NEXT:         0 |   object * boundedobj
// CHECK-NEXT:         8 |   object * unboundedobj
// CHECK-NEXT:        16 |   int numobjects
// CHECK-NEXT:           | [sizeof=24]

// CHECK:         0 | scenedef
// CHECK-NEXT:         0 |   char[256] outfilename
// CHECK-NEXT:       256 |   int writeimagefile
// CHECK-NEXT:       264 |   unsigned char * img
// CHECK-NEXT:       272 |   int imginternal
// CHECK-NEXT:       276 |   int imgformat
// CHECK-NEXT:       280 |   int numthreads
// CHECK-NEXT:       284 |   int nodes
// CHECK-NEXT:       288 |   int mynode
// CHECK-NEXT:       296 |   nodeinfo * cpuinfo
// CHECK-NEXT:       304 |   int hres
// CHECK-NEXT:       308 |   int vres
// CHECK-NEXT:       312 |   flt aspectratio
// CHECK-NEXT:       320 |   int raydepth
// CHECK-NEXT:       324 |   int antialiasing
// CHECK-NEXT:       328 |   int verbosemode
// CHECK-NEXT:       332 |   int boundmode
// CHECK-NEXT:       336 |   int boundthresh
// CHECK-NEXT:       344 |   struct list * texlist
// CHECK-NEXT:       352 |   struct list * cliplist
// CHECK-NEXT:       360 |   unsigned int flags
// CHECK-NEXT:       368 |   camdef camera
// CHECK-NEXT:       368 |     int projection
// CHECK-NEXT:       376 |     vector center
// CHECK-NEXT:       376 |       flt x
// CHECK-NEXT:       384 |       flt y
// CHECK-NEXT:       392 |       flt z
// CHECK-NEXT:       400 |     vector viewvec
// CHECK-NEXT:       400 |       flt x
// CHECK-NEXT:       408 |       flt y
// CHECK-NEXT:       416 |       flt z
// CHECK-NEXT:       424 |     vector rightvec
// CHECK-NEXT:       424 |       flt x
// CHECK-NEXT:       432 |       flt y
// CHECK-NEXT:       440 |       flt z
// CHECK-NEXT:       448 |     vector upvec
// CHECK-NEXT:       448 |       flt x
// CHECK-NEXT:       456 |       flt y
// CHECK-NEXT:       464 |       flt z
// CHECK-NEXT:       472 |     flt camzoom
// CHECK-NEXT:       480 |     flt px
// CHECK-NEXT:       488 |     flt py
// CHECK-NEXT:       496 |     flt psx
// CHECK-NEXT:       504 |     flt psy
// CHECK-NEXT:       512 |     flt focallength
// CHECK-NEXT:       520 |     flt left
// CHECK-NEXT:       528 |     flt right
// CHECK-NEXT:       536 |     flt top
// CHECK-NEXT:       544 |     flt bottom
// CHECK-NEXT:       552 |     flt aperture
// CHECK-NEXT:       560 |     vector projcent
// CHECK-NEXT:       560 |       flt x
// CHECK-NEXT:       568 |       flt y
// CHECK-NEXT:       576 |       flt z
// CHECK-NEXT:       584 |     fptr * cam_ray
// CHECK-NEXT:       592 |     vector lowleft
// CHECK-NEXT:       592 |       flt x
// CHECK-NEXT:       600 |       flt y
// CHECK-NEXT:       608 |       flt z
// CHECK-NEXT:       616 |     vector iplaneright
// CHECK-NEXT:       616 |       flt x
// CHECK-NEXT:       624 |       flt y
// CHECK-NEXT:       632 |       flt z
// CHECK-NEXT:       640 |     vector iplaneup
// CHECK-NEXT:       640 |       flt x
// CHECK-NEXT:       648 |       flt y
// CHECK-NEXT:       656 |       flt z
// CHECK-NEXT:       664 |   fptr * shader
// CHECK-NEXT:       672 |   fptr * phongfunc
// CHECK-NEXT:       680 |   int transmode
// CHECK-NEXT:       684 |   color background
// CHECK-NEXT:       684 |     float r
// CHECK-NEXT:       688 |     float g
// CHECK-NEXT:       692 |     float b
// CHECK-NEXT:       696 |   struct fogdata_t fog
// CHECK-NEXT:       696 |     fptr * fog_fctn
// CHECK-NEXT:       704 |     color col
// CHECK-NEXT:       704 |       float r
// CHECK-NEXT:       708 |       float g
// CHECK-NEXT:       712 |       float b
// CHECK-NEXT:       720 |     flt start
// CHECK-NEXT:       728 |     flt end
// CHECK-NEXT:       736 |     flt density
// CHECK-NEXT:       744 |   displist objgroup
// CHECK-NEXT:       744 |     object * boundedobj
// CHECK-NEXT:       752 |     object * unboundedobj
// CHECK-NEXT:       760 |     int numobjects
// CHECK-NEXT:       768 |   struct list * lightlist
// CHECK-NEXT:       776 |   int numlights
// CHECK-NEXT:       780 |   int scenecheck
// CHECK-NEXT:       784 |   void * parbuf
// CHECK-NEXT:       792 |   void * threads
// CHECK-NEXT:       800 |   void * threadparms
// CHECK-NEXT:       808 |   clip_group * curclipgroup
// CHECK-NEXT:           | [sizeof=816]

// CHECK:         0 | struct ray_t
// CHECK-NEXT:         0 |   vector o
// CHECK-NEXT:         0 |     flt x
// CHECK-NEXT:         8 |     flt y
// CHECK-NEXT:        16 |     flt z
// CHECK-NEXT:        24 |   vector d
// CHECK-NEXT:        24 |     flt x
// CHECK-NEXT:        32 |     flt y
// CHECK-NEXT:        40 |     flt z
// CHECK-NEXT:        48 |   flt maxdist
// CHECK-NEXT:        56 |   flt opticdist
// CHECK-NEXT:        64 |   fptr * add_intersection
// CHECK-NEXT:        72 |   intersectstruct intstruct
// CHECK-NEXT:        72 |     int num
// CHECK-NEXT:        80 |     intersection closest
// CHECK-NEXT:        80 |       const object * obj
// CHECK-NEXT:        88 |       flt t
// CHECK-NEXT:        96 |   unsigned int depth
// CHECK-NEXT:       100 |   unsigned int flags
// CHECK-NEXT:       104 |   unsigned long serial
// CHECK-NEXT:       112 |   unsigned long * mbox
// CHECK-NEXT:       120 |   scenedef * scene
// CHECK-NEXT:       128 |   unsigned int randval
// CHECK-NEXT:           | [sizeof=136]
