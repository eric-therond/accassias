#ifndef T_MPINTEGER_H 
#define T_MPINTEGER_H

#define MAX(a, b) (a < b ? b : a)
#define MIN(a, b) (a < b ? a : b)

#include <vector>
#include <string>
#include "t_mpgeneric.h"

class t_mpinteger : public t_mpgeneric
{
  public:
    t_mpinteger();
    t_mpinteger(unsigned int);
    static std::vector<int> consttocode(std::string &, unsigned int);
    static t_mpinteger *div(t_mpinteger *, t_mpinteger *);
    static t_mpinteger *mul(t_mpinteger *, t_mpinteger *);
    static t_mpinteger *sub(t_mpinteger *, t_mpinteger *);
    static t_mpinteger *add(t_mpinteger *, t_mpinteger *);
    static t_mpinteger *fromcode(std::vector<int> &, unsigned int);
    static t_mpinteger *fromstring(std::string);
};

#endif

