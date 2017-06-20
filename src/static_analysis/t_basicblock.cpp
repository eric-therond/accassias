#include "t_basicblock.h"

t_basicblock::t_basicblock()
{
}

t_basicblock::t_basicblock(std::vector<t_threeaddress>::iterator it_start)
{
  this->it_start = it_start;
}

t_basicblock::t_basicblock(std::vector<t_threeaddress>::iterator it_start, t_node<t_threeaddresscode> *node)
{
  this->it_start = it_start;
  this->node = node;
}

t_basicblock::t_basicblock(std::vector<t_threeaddress>::iterator it_start, std::vector<t_threeaddress>::iterator it_end, t_node<t_threeaddresscode> *node)
{
  this->it_start = it_start;
  this->it_end = it_end;
  this->node = node;
}

t_basicblock::t_basicblock(const t_basicblock &bb)
{
  this->it_start = bb.it_start;
  this->it_end = bb.it_end;
  this->node = bb.node;
}

void t_basicblock::set_it_start(std::vector<t_threeaddress>::iterator it_start)
{
  this->it_start = it_start;
}

void t_basicblock::set_it_end(std::vector<t_threeaddress>::iterator it_end)
{
  this->it_start = it_end;
}

std::vector<t_threeaddress>::iterator t_basicblock::get_it_start()
{
  return this->it_start;
}

std::vector<t_threeaddress>::iterator t_basicblock::get_it_end()
{
  return this->it_end;
}

t_node<t_threeaddresscode> *t_basicblock::getnode()
{
  return this->node;
}

void t_basicblock::setnode(t_node<t_threeaddresscode> *node)
{
  this->node = node;
}

std::ostream &operator<<(std::ostream &out, t_basicblock &bb)
{
  for(std::vector<t_threeaddress>::iterator it = bb.get_it_start(); it != bb.get_it_end(); ++it)
    std::cout << (*it) << std::endl;

  return out;
}


