#ifndef T_GRAPH_H
#define T_GRAPH_H

#include "t_node.h"
#include <stack>
#include <iostream>

template <typename T>
class t_graph
{
  public:
    t_graph(){
      this->root = NULL;
      this->istree = false;
    };

    t_node<T> *getroot(){
      return root;
    };

    void setroot(t_node<T> *node){
      root = node;
    };

    void addnode(t_node<T> *node) {
      this->nodes.push_back(node);
    };

    std::vector<t_node<T> *> &getnodes(){
      return this->nodes;
    };

    bool getcolorize(){
      return colorize;
    };

    void setcolorize(bool colorize){
      this->colorize = colorize;
    };

  protected:
    t_node<T> *root;
    unsigned int id;
    std::vector<t_node<T> *> nodes;
    bool colorize;
    bool istree;

};

#endif









