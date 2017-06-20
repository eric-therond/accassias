#include "t_mpgeneric.h"
#include <iostream>

t_mpgeneric::t_mpgeneric()
{
  this->setsize(0);
}

t_mpgeneric::t_mpgeneric(mp_type type)
{
  this->setsize(0);
  this->settype(type);
}

void t_mpgeneric::setbase(unsigned int base)
{
  this->base = base;
}

unsigned int t_mpgeneric::getbase()
{
  return this->base;
}

void t_mpgeneric::settype(mp_type type)
{
  this->type = type;
}

mp_type t_mpgeneric::gettype()
{
  return this->type;
}

void t_mpgeneric::setsize(unsigned int size)
{
  //this->size = size;
  this->digits.resize(size);
}

unsigned int t_mpgeneric::getsize()
{
  return this->digits.size();
}

unsigned int t_mpgeneric::toint()
{
  unsigned int i;
  unsigned int nb = 0;

  for(i = this->getsize(); i > 0; i --)
  {
    //std::cout << "digits[i] = " << this->digits[i - 1] << std::endl;
    if((i - this->getsize()) > 0)
      nb += (this->digits[i - 1] * (i - this->getsize())  * 10);
    else
      nb += (this->digits[i - 1]);
  }

  return nb;	
}


void t_mpgeneric::setcases(std::vector<int> &code, unsigned int addr, std::vector<t_mpgeneric *> &mpgenerics)
{
  unsigned int i;

  for(i = 0; i < this->getsize(); i ++)
    this->cases.push_back(mpgenerics[code[addr + i]]);
}

void t_mpgeneric::setcase(unsigned int index, t_mpgeneric *mpgen)
{
  if(this->cases.size() <= index)
    this->cases.resize(index + 1);

  this->cases[index] = mpgen;
}

t_mpgeneric *t_mpgeneric::getcase(unsigned int index)
{
  return this->cases[index];
}

std::vector<t_mpgeneric *> t_mpgeneric::getcases()
{
  return this->cases;
}

std::vector<int> &t_mpgeneric::getdigits()
{
  return this->digits;
}

void t_mpgeneric::setdigits(std::vector<int> &digits, unsigned int size)
{
  this->setdigits(digits, size, 0);
}

void t_mpgeneric::setdigits(std::vector<int> &digits, unsigned int size, unsigned int start)
{
  this->setdigits(digits, size, start, 0);
}

void t_mpgeneric::setdigits(std::vector<int> &digits, unsigned int size, unsigned int start, unsigned int ascii)
{
  unsigned int i;

  for(i = start + size - 1; i >= start; i --)
    this->digits.push_back(digits[i] - ascii);
}

void t_mpgeneric::setdigits(std::string &str)
{
  unsigned int i;

  for(i = 0; i < str.length(); i ++)
    this->digits.push_back(str[i]);
}

bool t_mpgeneric::equal(t_mpgeneric *mpgen1, t_mpgeneric *mpgen2)
{
  unsigned int i;

  if(mpgen1->getsize() != mpgen2->getsize())
    return false;

  for(i = mpgen1->getsize() - 1; i > 0; i --)
    if(mpgen1->digits[i] != mpgen2->digits[i])
      return false;

  return true;
}

bool t_mpgeneric::sup(t_mpgeneric *mpgen1, t_mpgeneric *mpgen2)
{
  unsigned int i;

  if(mpgen1->getsize() == mpgen2->getsize())
  {
    for(i = mpgen1->getsize(); i > 0; i --)
    {
      if(mpgen1->digits[i - 1] <= mpgen2->digits[i - 1])
      {
        return false;
      }  
    }
  }

  if(mpgen1->getsize() < mpgen2->getsize())
    return false;

  return true;
}

bool t_mpgeneric::supequal(t_mpgeneric *mpgen1, t_mpgeneric *mpgen2)
{
  unsigned int i;

  if(mpgen1->getsize() == mpgen2->getsize())
  {
    for(i = mpgen1->getsize(); i > 0; i --)
      if(mpgen1->digits[i - 1] < mpgen2->digits[i - 1])
        return false;
  }

  if(mpgen1->getsize() < mpgen2->getsize())
    return false;

  return true;
}

bool t_mpgeneric::inf(t_mpgeneric *mpgen1, t_mpgeneric *mpgen2)
{
  unsigned int i;

  if(mpgen1->getsize() == mpgen2->getsize())
  {
    for(i = mpgen1->getsize(); i > 0; i --)
      if(mpgen1->digits[i - 1] >= mpgen2->digits[i - 1])
        return false;
  }

  if(mpgen1->getsize() > mpgen2->getsize())
    return false;

  return true;
}

bool t_mpgeneric::infequal(t_mpgeneric *mpgen1, t_mpgeneric *mpgen2)
{
  unsigned int i;

  if(mpgen1->getsize() == mpgen2->getsize())
  {
    for(i = mpgen1->getsize(); i > 0; i --)
      if(mpgen1->digits[i - 1] > mpgen2->digits[i - 1])
        return false;
  }

  if(mpgen1->getsize() > mpgen2->getsize())
    return false;

  return true;
}


bool t_mpgeneric::istrue()
{
  if(digits.size() == 0 || (digits.size() == 1 && digits[0] == 0))
    return false;

  return true;
}

std::ostream &operator<<(std::ostream &out, t_mpgeneric &mpgeneric)
{
  std::vector<int>::reverse_iterator it;

  for(it = mpgeneric.getdigits().rbegin(); it < mpgeneric.getdigits().rend(); it++)
  {
    switch(mpgeneric.gettype())
    {
      case MP_STRING:
        {
          out << static_cast<char>(*it);
          break;
        }

      case MP_INTEGER:
        {
          out << *it;
          break;
        }

      default:
        break;
    }
  }

  //out << std::endl;

  return out;
}



