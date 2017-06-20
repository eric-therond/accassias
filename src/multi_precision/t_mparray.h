#ifndef T_MPARRAY_H 
#define T_MPARRAY_H

#include <vector>
#include <string>
#include "t_mpgeneric.h"

class t_mparray : public t_mpgeneric
{
  public:
    t_mparray();
    static t_mparray *fromcode(std::vector<int> &, unsigned int, std::vector<t_mpgeneric *> &);

};

#endif

