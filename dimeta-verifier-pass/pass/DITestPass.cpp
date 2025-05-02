#include "Dimeta.h"
#include "DimetaData.h"
#include "DimetaIO.h"

#include "TypeLayout.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/Support/Casting.h>
#include <optional>
#include <variant>

namespace ditest {

class TestPass : public llvm::PassInfoMixin<TestPass> {
public:
  llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &) {
    // auto cu_types = dimeta::compile_unit_types(&M);
    // if (cu_types) {
    //   for (const auto &type_list : cu_types.value()) {
    //     for (const auto &type_ : type_list.types) {
    //       if (auto *pval = std::get_if<dimeta::QualifiedCompound>(&type_)) {
    //         ditest::print_layout(llvm::errs(), *pval);
    //         llvm::errs() << "\n";
    //       }
    //     }
    //   }
    // }

    llvm::for_each(M.functions(), [&](auto &func) { return runOnFunc(func); });
    return llvm::PreservedAnalyses::all();
  }

  static void print_as_yaml(llvm::raw_ostream &out,
                            const dimeta::LocatedType &ditype) {
    std::string yaml_string;
    llvm::raw_string_ostream yaml_oss(yaml_string);
    dimeta::io::emit(yaml_oss, ditype);
    out << yaml_oss.str();
  }

  template <typename LLVMType> static void print_type(const LLVMType *type) {
    auto ditype = dimeta::located_type_for(type);
    if (ditype) {
      if (std::holds_alternative<dimeta::QualifiedCompound>(ditype->type)) {
        // print_as_yaml(llvm::errs(), ditype.value());
        print_struct_layout(llvm::errs(),
                            std::get<dimeta::QualifiedCompound>(ditype->type));
      } else {
        print_fundamental(llvm::errs(),
                          std::get<dimeta::QualifiedFundamental>(ditype->type));
      }
    }
  }

  static void runOnFunc(llvm::Function &func) {
    if (func.isDeclaration()) {
      return;
    }

    for (auto &inst : llvm::instructions(func)) {
      if (auto *call_inst = llvm::dyn_cast<llvm::CallBase>(&inst)) {
        print_type(call_inst);
      } else if (auto *alloca_inst = llvm::dyn_cast<llvm::AllocaInst>(&inst)) {
        // print_iff_compound_type(alloca_inst);
      }
    }
  }
};

} // namespace ditest

#define DEBUG_TYPE "ditest-pass"

llvm::PassPluginLibraryInfo getPassPluginInfo() {
  const auto callback = [](llvm::PassBuilder &PB) {
    PB.registerPipelineStartEPCallback(
        [&](llvm::ModulePassManager &MPM, llvm::OptimizationLevel) {
          MPM.addPass(ditest::TestPass());
        });
  };

  return {LLVM_PLUGIN_API_VERSION, DEBUG_TYPE, "0.0.1", callback};
}

extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getPassPluginInfo();
}
