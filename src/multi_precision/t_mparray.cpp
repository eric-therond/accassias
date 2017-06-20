#include "t_mparray.h"
#include <iostream>

t_mparray::t_mparray()
{
  this->settype(MP_ARRAY);
  //this->cases.resize(0);
}

t_mparray *t_mparray::fromcode(std::vector<int> &code, unsigned int addr, std::vector<t_mpgeneric *> &mpgenerics)
{
  t_mparray *mparray = new t_mparray;
  mparray->setsize(code[addr]);
  mparray->setcases(code, addr + 1, mpgenerics);

  return mparray;
}



