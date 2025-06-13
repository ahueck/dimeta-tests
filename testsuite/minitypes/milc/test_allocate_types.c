// RUN: %apply-dimeta-mpicc %s |& %filecheck %s

// ground truth (unordered): %clang-record-decl-mpicc %s

#include "include/generic_quark_types.h"
#include "include/io_lat.h"
#include <stdlib.h>

quark_invert_control g_quark_invert_control;
dirac_clover_param g_dirac_clover_param;
dirac_wilson_param g_dirac_wilson_param;
dirac_ks_param g_dirac_ks_param;
wilson_quark_source g_wilson_quark_source;

gauge_header g_gauge_header;
gauge_check g_gauge_check;
gauge_header_1996 g_gauge_header_1996;
QCDheader g_QCDheader;

void allocate_structs() {
  quark_invert_control *qic =
      (quark_invert_control *)malloc(sizeof(quark_invert_control));
  dirac_clover_param *dcp =
      (dirac_clover_param *)malloc(sizeof(dirac_clover_param));
  dirac_wilson_param *dwp =
      (dirac_wilson_param *)malloc(sizeof(dirac_wilson_param));
  dirac_ks_param *dkp = (dirac_ks_param *)malloc(sizeof(dirac_ks_param));
  wilson_quark_source *wqs =
      (wilson_quark_source *)malloc(sizeof(wilson_quark_source));

  gauge_header *gh = (gauge_header *)malloc(sizeof(gauge_header));
  gauge_check *gc = (gauge_check *)malloc(sizeof(gauge_check));
  gauge_header_1996 *gh96 =
      (gauge_header_1996 *)malloc(sizeof(gauge_header_1996));
  QCDheader *qh = (QCDheader *)malloc(sizeof(QCDheader));
}

// CHECK:          0 | quark_invert_control
// CHECK-NEXT:          0 |   int min
// CHECK-NEXT:          4 |   int max
// CHECK-NEXT:          8 |   int nrestart
// CHECK-NEXT:         12 |   int parity
// CHECK-NEXT:         16 |   int start_flag
// CHECK-NEXT:         20 |   int nsrc
// CHECK-NEXT:         24 |   float resid
// CHECK-NEXT:         28 |   float size_r
// CHECK-NEXT:         32 |   int converged
// CHECK-NEXT:         36 |   field_offset wv1
// CHECK-NEXT:         40 |   field_offset wv2
// CHECK-NEXT:         44 |   field_offset wv3
// CHECK-NEXT:         48 |   field_offset wv4
// CHECK-NEXT:            | [sizeof=52]

// *** Dumping AST Record Layout
//          0 | quark_invert_control
//          0 |   int min
//          4 |   int max
//          8 |   int nrestart
//         12 |   int parity
//         16 |   int start_flag
//         20 |   int nsrc
//         24 |   float resid
//         28 |   float size_r
//         32 |   int converged
//         36 |   field_offset wv1
//         40 |   field_offset wv2
//         44 |   field_offset wv3
//         48 |   field_offset wv4
//            | [sizeof=52, align=4]

// CHECK:          0 | dirac_clover_param
// CHECK-NEXT:          0 |   float Kappa
// CHECK-NEXT:          4 |   float Clov_c
// CHECK-NEXT:          8 |   float U0
// CHECK-NEXT:         12 |   field_offset work_f_mn
// CHECK-NEXT:            | [sizeof=16]

// *** Dumping AST Record Layout
//          0 | dirac_clover_param
//          0 |   float Kappa
//          4 |   float Clov_c
//          8 |   float U0
//         12 |   field_offset work_f_mn
//            | [sizeof=16, align=4]

// CHECK:          0 | dirac_wilson_param
// CHECK-NEXT:          0 |   float Kappa
// CHECK-NEXT:            | [sizeof=4]

// CHECK:          0 | dirac_ks_param
// CHECK-NEXT:          0 |   float mass
// CHECK-NEXT:            | [sizeof=4]

// *** Dumping AST Record Layout
//          0 | dirac_wilson_param
//          0 |   float Kappa
//            | [sizeof=4, align=4]

// *** Dumping AST Record Layout
//          0 | dirac_ks_param
//          0 |   float mass
//            | [sizeof=4, align=4]

// CHECK:          0 | wilson_quark_source
// CHECK-NEXT:          0 |   int color
// CHECK-NEXT:          4 |   int spin
// CHECK-NEXT:          8 |   int type
// CHECK-NEXT:         12 |   char[30] descrp
// CHECK-NEXT:         44 |   int wall_cutoff
// CHECK-NEXT:         48 |   int parity
// CHECK-NEXT:         52 |   float r0
// CHECK-NEXT:         56 |   int x0
// CHECK-NEXT:         60 |   int y0
// CHECK-NEXT:         64 |   int z0
// CHECK-NEXT:         68 |   int t0
// CHECK-NEXT:         72 |   int src_pointer
// CHECK-NEXT:            | [sizeof=76]

// *** Dumping AST Record Layout
//          0 | wilson_quark_source
//          0 |   int color
//          4 |   int spin
//          8 |   int type
//         12 |   char[30] descrp
//         44 |   int wall_cutoff
//         48 |   int parity
//         52 |   float r0
//         56 |   int x0
//         60 |   int y0
//         64 |   int z0
//         68 |   int t0
//         72 |   int src_pointer
//            | [sizeof=76, align=4]

// CHECK:          0 | gauge_header
// CHECK-NEXT:          0 |   int32type magic_number
// CHECK-NEXT:          4 |   char[64] time_stamp
// CHECK-NEXT:         68 |   int32type[4] dims
// CHECK-NEXT:         84 |   int32type header_bytes
// CHECK-NEXT:         88 |   int32type order
// CHECK-NEXT:            | [sizeof=92]

// CHECK:          0 | gauge_check
// CHECK-NEXT:          0 |   u_int32type sum31
// CHECK-NEXT:          4 |   u_int32type sum29
// CHECK-NEXT:            | [sizeof=8]

// CHECK:          0 | gauge_header_1996
// CHECK-NEXT:          0 |   int32type magic_number
// CHECK-NEXT:          4 |   int32type[4] dims
// CHECK-NEXT:         20 |   int32type header_bytes
// CHECK-NEXT:         24 |   int32type order
// CHECK-NEXT:         28 |   anon_t gauge_field
// CHECK-NEXT:         28 |     int32type n_descript
// CHECK-NEXT:         32 |     char[200] descript
// CHECK-NEXT:        232 |     int32type n_param
// CHECK-NEXT:        236 |     float[2] param
// CHECK-NEXT:            | [sizeof=244]

// CHECK:          0 | QCDheader
// CHECK-NEXT:          0 |   int ntoken
// CHECK-NEXT:          8 |   char ** token
// CHECK-NEXT:         16 |   char ** value
// CHECK-NEXT:            | [sizeof=24]

// clang-format off
// *** Dumping AST Record Layout
//          0 | gauge_header
//          0 |   int32type magic_number
//          4 |   char[64] time_stamp
//         68 |   int32type[4] dims
//         84 |   int32type header_bytes
//         88 |   int32type order
//            | [sizeof=92, align=4]

// *** Dumping AST Record Layout
//          0 | gauge_check
//          0 |   u_int32type sum31
//          4 |   u_int32type sum29
//            | [sizeof=8, align=4]

// *** Dumping AST Record Layout
//          0 | gauge_header_1996
//          0 |   int32type magic_number
//          4 |   int32type[4] dims
//         20 |   int32type header_bytes
//         24 |   int32type order
//         28 |   struct gauge_header_1996::(unnamed at io_lat.h:203:3) gauge_field
//         28 |     int32type n_descript
//         32 |     char[200] descript
//        232 |     int32type n_param
//        236 |     float[2] param
//            | [sizeof=244, align=4]

// *** Dumping AST Record Layout
//          0 | QCDheader
//          0 |   int ntoken
//          8 |   char ** token
//         16 |   char ** value
//            | [sizeof=24, align=8]