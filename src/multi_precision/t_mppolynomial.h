#ifndef T_MPPOLYNOMIAL_H 
#define T_MPPOLYNOMIAL_H

#define MAX(a, b) (a < b ? b : a)
#define MIN(a, b) (a < b ? a : b)

#include <vector>
#include <string>
#include "t_mpgeneric.h"
#include "t_mpinteger.h"
#include "t_string.h"

class t_mppolynomial : public t_mpgeneric
{
  public:
    t_mppolynomial();
    t_mppolynomial(unsigned int);
    static std::vector<int> consttocode(std::string &, unsigned int);
    static t_mppolynomial *div(t_mppolynomial *, t_mppolynomial *);
    static t_mppolynomial *mul(t_mppolynomial *, t_mppolynomial *);
    static t_mppolynomial *sub(t_mppolynomial *, t_mppolynomial *);
    static t_mppolynomial *add(t_mppolynomial *, t_mppolynomial *);
    static t_mppolynomial *fromcode(std::vector<int> &, unsigned int);
    static t_mppolynomial *fromstring(std::string);
    static t_mppolynomial *read(char, t_string *);

    void setcoefficient(int, t_mpinteger *);
    void setdegree(int);

    t_mpinteger *getcoefficient(int);
    int getdegree();

  private:
    std::vector<t_mpinteger *> *coefficients;
    int degree;
};

#endif

