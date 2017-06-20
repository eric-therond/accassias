include_directories (${ACCASSIAS_SOURCE_DIR} ${DATA_STRUCTURES_SOURCE_DIR}) 

add_library (t_syntax t_class.cpp t_function.cpp t_variable.cpp t_syntax.cpp t_symbol.cpp t_identifier.cpp t_identifiervertex.cpp) 

target_link_libraries (t_syntax) 
