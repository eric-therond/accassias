#include "t_identifiervertex.h"

t_identifiervertex::t_identifiervertex()
{

}

void t_identifiervertex::put(t_identifier *identifier)
{
  table.push_back(identifier);
}

unsigned int t_identifiervertex::getsize()
{
  return table.size();
}

t_identifier *t_identifiervertex::search(std::string &str)
{
  std::vector<t_identifier *>::iterator it;
  for(it = table.begin(); it < table.end(); it++)
  {
    if((*it)->getlexeme() == str)
      return *it;
  }

  return NULL;
}



