#include "Dimeta.h"
#include "DimetaIO.h"

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

namespace ditest {

class TestPass : public llvm::PassInfoMixin<TestPass> {
 public:
  llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager&) {
    llvm::for_each(M.functions(), [&](auto& func) { return runOnFunc(func); });
    return llvm::PreservedAnalyses::all();
  }

  static void runOnFunc(llvm::Function& func) {
    if (func.isDeclaration()) {
      return;
    }

    for (auto& inst : llvm::instructions(func)) {
      if (auto* call_inst = llvm::dyn_cast<llvm::CallBase>(&inst)) {
        auto ditype = dimeta::located_type_for(call_inst);
        if (ditype) {
          std::string yaml_string;
          llvm::raw_string_ostream yaml_oss(yaml_string);
          dimeta::io::emit(yaml_oss, ditype.value());
          llvm::errs() << yaml_oss.str();
        }
      }
    }
  }
};

}  // namespace ditest

#define DEBUG_TYPE "ditest-pass"

llvm::PassPluginLibraryInfo getPassPluginInfo() {
  const auto callback = [](llvm::PassBuilder& PB) {
    PB.registerPipelineStartEPCallback([&](llvm::ModulePassManager& MPM, llvm::OptimizationLevel) {
      MPM.addPass(ditest::TestPass());
    });
  };

  return {LLVM_PLUGIN_API_VERSION, DEBUG_TYPE, "0.0.1", callback};
}

extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getPassPluginInfo();
}
