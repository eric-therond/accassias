link_directories (${ACCASSIAS_SOURCE_DIR}/compiler/gencode ${ACCASSIAS_SOURCE_DIR}/multi_precision) 
  add_library (t_vm t_vm.cpp)
target_link_libraries (t_vm t_gencode t_multi_precision) 

