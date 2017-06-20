#ifndef T_NODE_H
#define T_NODE_H

#include "t_color.h"
#include <vector>
#include <string>
#include <iostream>

template <typename V>
class t_node
{
  public:
    t_node(const char *label){

      this->id = t_node::next_id++;
      this->label = label;
      this->nb_children = 0;
      this->children = new std::vector<t_node *>;	
    };

    t_node(const char *label, V *vertex){
      this->id = t_node::next_id++;
      this->vertex = vertex;
      this->label = label;
      this->nb_children = 0;
      this->children = new std::vector<t_node *>;
    };

    t_node(t_node<V> const &node){
      this->id = t_node::next_id++;
      label = node->label;
      nb_children = node->nb_children;
      children = node->children;
      vertex = node->vertex;
    };

    std::string &getlabel(){
      return label;
    };

    void setlabel(const char *label){
      this->label = label;
    };

    unsigned int getnbchildren(){
      return nb_children;
    };

    t_node<V> *getchild(unsigned int index){
      return (*children)[index]; 
    };

    std::vector<t_node *> *getchildren(){
      return children; 
    };

    void addchild(t_node<V> *node){
      this->children->push_back(node);
      this->nb_children ++;
    };

    void deletechild(){
      this->children->pop_back();
      this->nb_children --;

    };

    t_node<V> *addchild(const char *label){
      t_node *node = new t_node<V>(label);
      this->addchild(node);

      return node;
    };

    t_node<V> *addchild(const char *label, V *vertex){
      t_node *node = this->addchild(label);
      node->setvertex(vertex);

      return node;
    };

    void setvertex(V *vertex){
      this->vertex = vertex;
    };

    V *getvertex(){
      return vertex;
    };

    unsigned int getid(){
      return this->id;
    };

    bool isleaf(){
      return nb_children == 0;
    } ;      

  private:
    unsigned int nb_children;
    std::vector<t_node *> *children;
    std::string label;	
    V *vertex;
    unsigned int id;
    static unsigned int next_id;

};

template <typename V>
unsigned int t_node<V>::next_id = 0;

#endif

