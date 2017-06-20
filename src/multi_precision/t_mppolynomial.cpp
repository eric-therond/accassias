#include "t_mppolynomial.h"
#include <iostream>
#include <stdlib.h>

t_mppolynomial::t_mppolynomial()
{
  this->setbase(10);
  this->settype(MP_INTEGER);
  this->coefficients = new std::vector<t_mpinteger *>;
}

t_mppolynomial::t_mppolynomial(unsigned int base)
{
  this->setbase(base);
}

std::vector<int> t_mppolynomial::consttocode(std::string &str, unsigned int base)
{
  unsigned int i;
  std::vector<int> code;

  code.push_back(base);
  code.push_back(str.length());
  for(i = 0; i < str.length(); i++)
    code.push_back(str[i]);

  return code;
}

t_mppolynomial *t_mppolynomial::fromstring(std::string str)
{
  t_mppolynomial *mpint = new t_mppolynomial;
  mpint->setbase(10);
  //mpint->setsize(str.length());
  mpint->setdigits(str);

  return mpint;
}

t_mppolynomial *t_mppolynomial::fromcode(std::vector<int> &code, unsigned int addr)
{
  t_mppolynomial *mpint = new t_mppolynomial;
  mpint->setbase(code[addr]);
  //mpint->setsize(code[addr + 1]);
  mpint->setdigits(code, code[addr + 1], addr + 2, 48);

  return mpint;
}


t_mppolynomial *t_mppolynomial::div(t_mppolynomial *mpint1, t_mppolynomial *mpint2)
{
  (void) mpint2;
  return mpint1;
}


t_mppolynomial *t_mppolynomial::mul(t_mppolynomial *mpint1, t_mppolynomial *mpint2)
{
  (void) mpint2;
  return mpint1;
}

t_mppolynomial *t_mppolynomial::sub(t_mppolynomial *mpint1, t_mppolynomial *mpint2)
{
  (void) mpint2;
  return mpint1;
}

t_mppolynomial *t_mppolynomial::add(t_mppolynomial *mpint1, t_mppolynomial *mpint2)
{
  (void) mpint2;
  return mpint1;
}

// Enlever tout les espaces
t_mppolynomial *t_mppolynomial::read(char c, t_string *tstr)
{
  unsigned int i = 0;
  std::string exp;
  int expi;
  int max_expi;
  std::string coeff;
  bool coeffsign = false;

  t_mppolynomial *mppoly = new t_mppolynomial;
  t_string *ststr = t_string::removespaces(tstr);

  while(i < ststr->getsize())
  {
    while(ststr->digits[i] == ' ')
      i ++;

    if(ststr->digits[i] == '-' || ststr->digits[i] == '+')
    {
      coeffsign = true;	
      if(ststr->digits[i] == '-')
        coeffsign = false;

      i ++;
    }

    // 9
    if(ststr->digits[i] >= '0' && ststr->digits[i] <= '9')
    {
      exp = "0";
      while(ststr->digits[i] >= '0' && ststr->digits[i] <= '9')
      {
        coeff += ststr->digits[i];

        i ++;
      }

      // 9*
      if(ststr->digits[i] == '*')
      {	// 9*X
        if(ststr->digits[i] == c)
        {
          i ++;
          exp = "1";
          // 9*X^
          if(ststr->digits[i] == '^')
          {	// 9*X^5
            if(ststr->digits[i] >= '0' && ststr->digits[i] <= '9')
            {
              while(ststr->digits[i] >= '0' && ststr->digits[i] <= '9')
              {
                exp += ststr->digits[i];

                i ++;
              }
            }
          }
        }
      }

      t_mpinteger *mpcoeff = t_mpinteger::fromstring(coeff);
      expi = atoi(exp.c_str());
      //mppoly->setcoefficient(expi, mpcoeff);
      mppoly->setcase(expi, mpcoeff);

      if(expi > max_expi)
      {
        max_expi = expi;
        mppoly->setdegree(expi);
      }
    }

    i ++;
  }

  return mppoly;
}

int t_mppolynomial::getdegree()
{
  return this->degree;
}

void t_mppolynomial::setdegree(int degree)
{
  this->degree = degree;
}

t_mpinteger *t_mppolynomial::getcoefficient(int expi)
{
  return (*this->coefficients)[expi];
}

void t_mppolynomial::setcoefficient(int expi, t_mpinteger *coeff)
{
  this->coefficients->resize(expi);
  (*this->coefficients)[expi] = coeff;
}





