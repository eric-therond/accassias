#ifndef T_TREE_H
#define T_TREE_H

#include "data_structures/graph/t_graph.h"

template <typename T>
class t_tree: public t_graph<T>
{
  public:
    t_tree(){
      this->root = NULL;
      this->istree = true;
    };
};

#endif









