#ifndef T_STRING_H
#define T_STRING_H

#include <string>
#include "t_mpgeneric.h"

class t_string : public t_mpgeneric
{
  public:
    t_string();

    static std::string inttostring(unsigned int);
    static std::vector<int> consttocode(std::string &);
    static t_string *fromcode(std::vector<int> &, unsigned int);
    static t_string *removespaces(t_string *);
};

#endif


