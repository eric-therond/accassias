#ifndef T_BASICBLOCK_H
#define T_BASICBLOCK_H

#include "compiler/gencode/t_threeaddresscode.h"
#include "data_structures/graph/t_node.h"

class t_basicblock
{
  public:
    t_basicblock();
    t_basicblock(std::vector<t_threeaddress>::iterator);
    t_basicblock(std::vector<t_threeaddress>::iterator, t_node<t_threeaddresscode> *); 
    t_basicblock(std::vector<t_threeaddress>::iterator, std::vector<t_threeaddress>::iterator, t_node<t_threeaddresscode> *); 
    t_basicblock(const t_basicblock &);

    void set_it_start(std::vector<t_threeaddress>::iterator);
    void set_it_end(std::vector<t_threeaddress>::iterator);
    std::vector<t_threeaddress>::iterator get_it_start();
    std::vector<t_threeaddress>::iterator get_it_end();

    void setnode(t_node<t_threeaddresscode> *);
    t_node<t_threeaddresscode> *getnode();

  private:
    std::vector<t_threeaddress>::iterator it_start;
    std::vector<t_threeaddress>::iterator it_end;
    t_node<t_threeaddresscode> *node;
};

std::ostream &operator<<(std::ostream &, t_basicblock &);

#endif
