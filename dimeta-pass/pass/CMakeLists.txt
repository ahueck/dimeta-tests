#### llvm-dimeta consumer pass
add_llvm_pass_plugin(ditest_pass
  DITestPass.cpp
  LINK_LIBS LLVMDemangle 
)

target_include_directories(ditest_pass
  SYSTEM
  PRIVATE
  ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(ditest_pass
  PRIVATE
  LLVM_VERSION_MAJOR=${LLVM_VERSION_MAJOR}
)

### Link llvm-dimeta librarys
target_link_libraries(ditest_pass PRIVATE dimeta::Types)
