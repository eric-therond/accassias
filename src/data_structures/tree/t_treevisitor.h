#ifndef T_TREEVISITOR_H
#define T_TREEVISITOR_H

template <typename N>
class t_treevisitor
{
  public:
    virtual void initialize() {};
    virtual void initialize_node(N *) {};
    virtual void examine_node(N *) {};
    virtual void examine_node_target(N *, N *) {};
    virtual void finish_node(N *) {};
    virtual void finish() {};
};

#endif

