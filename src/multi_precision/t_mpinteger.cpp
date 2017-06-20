#include "t_mpinteger.h"
#include <iostream>

t_mpinteger::t_mpinteger()
{
  this->setbase(10);
  this->settype(MP_INTEGER);
}

t_mpinteger::t_mpinteger(unsigned int base)
{
  this->setbase(base);
}

std::vector<int> t_mpinteger::consttocode(std::string &str, unsigned int base)
{
  unsigned int i;
  std::vector<int> code;

  code.push_back(base);
  code.push_back(str.length());
  for(i = 0; i < str.length(); i++)
    code.push_back(str[i]);

  return code;
}

t_mpinteger *t_mpinteger::fromstring(std::string str)
{
  t_mpinteger *mpint = new t_mpinteger;
  mpint->setbase(10);
  //mpint->setsize(str.length());
  mpint->setdigits(str);

  return mpint;
}

t_mpinteger *t_mpinteger::fromcode(std::vector<int> &code, unsigned int addr)
{
  t_mpinteger *mpint = new t_mpinteger;
  mpint->setbase(code[addr]);
  //mpint->setsize(code[addr + 1]);
  mpint->setdigits(code, code[addr + 1], addr + 2, 48);

  return mpint;
}


t_mpinteger *t_mpinteger::div(t_mpinteger *mpint1, t_mpinteger *mpint2)
{
  t_mpinteger *mpint3 = new t_mpinteger;
  unsigned int i;
  unsigned int tmp;
  unsigned int carry = 0;
  unsigned int k = mpint1->getsize();
  unsigned int l = mpint2->getsize();
  unsigned int m = k - l;

  for(i = m; i > 0; i --) 
  {
    tmp = mpint1->digits[i] + mpint2->digits[i] + carry;
    carry = tmp / mpint1->getbase();
    mpint3->digits[i] = tmp % mpint1->getbase();
  }

  if(carry)
    mpint3->digits.push_back(carry);

  return mpint3;
}


t_mpinteger *t_mpinteger::mul(t_mpinteger *mpint1, t_mpinteger *mpint2)
{
  t_mpinteger *mpint_temp;
  t_mpinteger *mpint3 = new t_mpinteger;
  unsigned int i, j;
  unsigned int tmp;
  unsigned int carry = 0;
  unsigned int k = mpint1->getsize();
  unsigned int l = mpint2->getsize();
  unsigned int m_temp;

  if(k < l)
  {
    mpint_temp = mpint1;
    mpint1 = mpint2;
    mpint2 = mpint_temp;

    m_temp = k;
    k = l;
    l = m_temp;
  }

  mpint3->digits.resize(k + l);

  for(i = 0; i < (k + l); i ++) 
    mpint3->digits[i] = 0;

  for(i = 0; i < k; i ++)
  {
    carry = 0;

    for(j = 0; j < l; j ++)
    {
      tmp = mpint1->digits[i]*mpint2->digits[j] + mpint3->digits[i + j] + carry;
      carry = tmp / mpint1->getbase();
      mpint3->digits[i + j] = tmp % mpint1->getbase();
    }

    if(carry)
      mpint3->digits[i + l] = carry;
  }

  return mpint3;
}


t_mpinteger *t_mpinteger::sub(t_mpinteger *mpint1, t_mpinteger *mpint2)
{
  t_mpinteger *mpint_temp;
  t_mpinteger *mpint3 = new t_mpinteger;
  unsigned int i;
  int tmp;
  int carry = 0;
  unsigned int k = mpint1->getsize();
  unsigned int l = mpint2->getsize();
  unsigned int m_temp;

  if(k < l)
  {
    mpint_temp = mpint1;
    mpint1 = mpint2;
    mpint2 = mpint_temp;

    m_temp = k;
    k = l;
    l = m_temp;
  }

  for(i = 0; i < l; i ++) 
  {
    tmp = mpint1->digits[i] - mpint2->digits[i] + carry;
    carry = tmp / mpint1->getbase();
    mpint3->digits.push_back(tmp % mpint1->getbase());
  }

  for(i = l; i < k; i ++)
  {
    tmp = mpint1->digits[i] + carry;
    carry = tmp / mpint1->getbase();
    mpint3->digits.push_back(tmp % mpint1->getbase());
  }

  if(carry)
    mpint3->digits.push_back(carry);

  return mpint3;
}



t_mpinteger *t_mpinteger::add(t_mpinteger *mpint1, t_mpinteger *mpint2)
{
  t_mpinteger *mpint_temp;
  t_mpinteger *mpint3 = new t_mpinteger;
  unsigned int i;
  unsigned int tmp;
  unsigned int carry = 0;
  unsigned int k = mpint1->getsize();
  unsigned int l = mpint2->getsize();
  unsigned int m_temp;

  if(k < l)
  {
    mpint_temp = mpint1;
    mpint1 = mpint2;
    mpint2 = mpint_temp;

    m_temp = k;
    k = l;
    l = m_temp;
  }

  for(i = 0; i < l; i ++) 
  {
    tmp = mpint1->digits[i] + mpint2->digits[i] + carry;
    carry = tmp / mpint1->getbase();
    mpint3->digits.push_back(tmp % mpint1->getbase());
  }

  for(i = l; i < k; i ++)
  {
    tmp = mpint1->digits[i] + carry;
    carry = tmp / mpint1->getbase();
    mpint3->digits.push_back(tmp % mpint1->getbase());
  }

  if(carry)
    mpint3->digits.push_back(carry);

  return mpint3;
}







