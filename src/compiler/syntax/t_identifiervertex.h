#ifndef T_IDENTIFIERVERTEX_H
#define T_IDENTIFIERVERTEX_H

#include "t_identifier.h"
#include <string>
#include <vector>

class t_identifiervertex
{
  public:
    t_identifiervertex();

    void put(t_identifier *);
    t_identifier *search(std::string &);
    unsigned int getsize();

  private:
    std::vector<t_identifier *> table;

};

#endif
