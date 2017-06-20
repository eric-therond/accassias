#ifndef T_THREEADDRESSCODE_H
#define T_THREEADDRESSCODE_H

#include "t_threeaddress.h"
#include <vector>
#include <ostream>

class t_threeaddresscode
{
  public:
    t_threeaddresscode(std::vector<t_threeaddress>::iterator, std::vector<t_threeaddress>::iterator);
    t_threeaddresscode();

    std::vector<t_threeaddress> *get_code();

  private:
    std::vector<t_threeaddress> *code;

};

std::ostream &operator<<(std::ostream &, t_threeaddresscode &);

#endif





