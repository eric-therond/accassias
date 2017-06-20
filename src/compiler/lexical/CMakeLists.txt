include_directories (${ACCASSIAS_SOURCE_DIR} ${DATA_STRUCTURES_SOURCE_DIR}) 

  add_library (t_lexical t_lexical.cpp)
target_link_libraries (t_lexical ncurses) 


