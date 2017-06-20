#include "t_string.h"
#include <iostream>

t_string::t_string()
{
  this->settype(MP_STRING);
}

std::vector<int> t_string::consttocode(std::string &str)
{
  unsigned int i;
  std::vector<int> code;

  code.push_back(str.length());
  for(i = 0; i < str.length(); i++)
    code.push_back(str[i]);

  return code;
}


t_string *t_string::removespaces(t_string *tstr)
{
  unsigned int i;
  unsigned int j = 0;
  t_string *copystr = new t_string;
  copystr->setsize(tstr->getsize());

  for(i = 0; i < tstr->getsize(); i ++)
  {
    if(tstr->digits[i] != ' ')
      copystr->digits[j++] = tstr->digits[i];
  }

  copystr->setsize(j);

  return copystr;
}


t_string *t_string::fromcode(std::vector<int> &code, unsigned int addr)
{
  t_string *str = new t_string;
  str->setdigits(code, code[addr + 1], addr + 2);

  return str;
}


