#ifndef DF7E1C25_6192_40DE_BB69_6C1EEAC70464
#define DF7E1C25_6192_40DE_BB69_6C1EEAC70464

#include "Dimeta.h"
#include "DimetaData.h"

namespace llvm {
class raw_ostream;
}

namespace ditest {

/*
 Emulates Clang's `-Xclang -fdump-record-layouts` with some simplifications.
 Does not yet handle C++ inheritance.
*/
void print_layout(llvm::raw_ostream &, const dimeta::QualifiedCompound &);

} // namespace ditest

#endif /* DF7E1C25_6192_40DE_BB69_6C1EEAC70464 */
