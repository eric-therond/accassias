cmake_minimum_required(VERSION 2.6) 
PROJECT(ACCASSIAS)

set (CMAKE_BUILD_TYPE debug)
  set (CMAKE_CXX_FLAGS  "-Wall -W")

include_directories (${ACCASSIAS_SOURCE_DIR} ${DATA_STRUCTURES_SOURCE_DIR}) 
link_directories (${ACCASSIAS_SOURCE_DIR}/static_analysis ${ACCASSIAS_SOURCE_DIR}/console ${ACCASSIAS_SOURCE_DIR}/virtual_machine ${ACCASSIAS_SOURCE_DIR}/compiler/lexical ${ACCASSIAS_SOURCE_DIR}/compiler/syntax)

  add_subdirectory (console)
  add_subdirectory (data_structures)
  add_subdirectory (multi_precision)
  add_subdirectory (compiler)
  add_subdirectory (virtual_machine)
add_subdirectory (static_analysis)

  add_executable (accassias main.cpp) 
target_link_libraries (accassias t_console t_vm t_lexical t_syntax) 










