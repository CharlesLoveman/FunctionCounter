add_llvm_library( LLVMNumFuncs MODULE
  NumFuncs.cpp

  DEPENDS
  PLUGIN_TOOL
  opt
  )
