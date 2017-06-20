#ifndef T_COMPILER_H
#define T_COMPILER_H

#include "compiler/syntax/t_symbol.h"
#include "compiler/gencode/t_genthreeaddresscodetreevisitor.h"
#include "data_structures/graph/t_node.h"
#include "data_structures/tree/t_tree.h"
#include "data_structures/tree/t_dottreevisitor.h"
#include "data_structures/tree/t_dotcfgtreevisitor.h"
#include "compiler/gencode/t_threeaddresscode.h"

#define DEBUG true

typedef t_tree<t_symbol> t_syntaxtree;
typedef t_dottreevisitor<t_node<t_symbol> > t_dotsyntaxtree;
typedef t_dotcfgtreevisitor<t_node<t_threeaddresscode> > t_dotcfgtree;

#endif

