// RUN: DITESTPASS_YAML=1 %apply-dimeta-c %s |& %filecheck %s

#include <stdlib.h>
int main() {
  int *test = (int *)malloc(4);
  return 0;
}

// CHECK: SourceLoc:
// CHECK-NEXT:   File:            01_malloc_int.c
// CHECK-NEXT:   Function:        main
// CHECK-NEXT:   Line:            5
// CHECK-NEXT: Builtin:         true
// CHECK-NEXT: Type:
// CHECK-NEXT:   Fundamental:     { Name: int, Extent: 4, Encoding: signed_int }
// CHECK-NEXT:   Qualifiers:      [ ptr ]