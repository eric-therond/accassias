#ifndef T_THREEADDRESS_H
#define T_THREEADDRESS_H

#include <ostream>
#include "t_tacsymbol.h"

class t_threeaddress
{
  public:
    t_threeaddress(t_tacsymbol, int, int);
    t_threeaddress(t_tacsymbol, int, int, int);
    t_threeaddress(t_tacsymbol, int, int, std::string &);
    t_threeaddress(t_tacsymbol, int, std::string &);
    t_threeaddress(t_threeaddress const &);

    std::string &getconst1();
    int getaddrresult();
    int getaddrarg1();
    int getaddrarg2();
    unsigned int getnbargs();
    t_tacsymbol gettacsymbol();

    void setconst1(const std::string &);
    void setaddrresult(int);
    void setaddrarg1(int);
    void setaddrarg2(int);

  private:
    t_tacsymbol tacsymbol;
    std::string const1;
    int addr_result;
    int addr_arg1;
    int addr_arg2;
    unsigned int nbargs;

};

std::string printvar(unsigned int);
std::ostream &operator<<(std::ostream &, t_threeaddress &);

#endif





