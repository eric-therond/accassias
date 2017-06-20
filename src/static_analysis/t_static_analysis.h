#ifndef T_STATIC_ANALYSIS_H
#define T_STATIC_ANALYSIS_H

#include "data_structures/tree/t_tree.h"
#include "compiler/gencode/t_threeaddresscode.h"
#include "compiler/gencode/t_threeaddress.h"

typedef t_graph<t_threeaddresscode> t_cfg;
typedef std::vector<t_threeaddress>::iterator t_statement;
typedef unsigned int t_label;
typedef t_node<t_threeaddresscode> *t_basic_block;

#endif

