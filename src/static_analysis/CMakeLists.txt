add_library (t_static_analysis t_control_flow_graph.cpp t_basicblock.cpp t_data_flow.cpp t_flow.cpp)

target_link_libraries (t_static_analysis) 
