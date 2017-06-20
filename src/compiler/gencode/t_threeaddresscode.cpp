#include "t_threeaddresscode.h"
#include <iostream>

std::vector<t_threeaddress> *t_threeaddresscode::get_code()
{
  return code;
}

t_threeaddresscode::t_threeaddresscode()
{
  this->code = new std::vector<t_threeaddress>;
}

t_threeaddresscode::t_threeaddresscode(std::vector<t_threeaddress>::iterator it1, std::vector<t_threeaddress>::iterator it2)
{
  this->code = new std::vector<t_threeaddress>(it1, it2);
}

std::ostream &operator<<(std::ostream &out, t_threeaddresscode &tac)
{
  std::vector<t_threeaddress>::iterator it;
  for(it = tac.get_code()->begin(); it < tac.get_code()->end(); it++)
    out << *it << "\n";

  return out;
}


