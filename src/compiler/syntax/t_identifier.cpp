#include "t_identifier.h"

  t_identifier::t_identifier()
: t_symbol()
{
  this->nb_flows = 0;
  this->isaclass = false;
  this->global = false;
}

  t_identifier::t_identifier(unsigned int line, unsigned int column, s_symbol symbol)
: t_symbol(line, column, symbol)
{
  this->nb_flows = 0;
  this->isaclass = false;
  this->global = false;
}

  t_identifier::t_identifier(unsigned int line, unsigned int column, s_symbol symbol, std::string &lexeme)
: t_symbol(line, column, symbol, lexeme)
{
  this->nb_flows = 0;
  this->isaclass = false;
  this->global = false;
}

  t_identifier::t_identifier(unsigned int line, unsigned int column, s_symbol symbol, int addr)
: t_symbol(line, column, symbol)
{
  this->nb_flows = 0;
  this->addr = addr;
  this->isaclass = false;
  this->global = false;
}

  t_identifier::t_identifier(unsigned int line, unsigned int column, s_symbol symbol, std::string &lexeme, int addr)
: t_symbol(line, column, symbol, lexeme)
{
  this->nb_flows = 0;
  this->addr = addr;
  this->isaclass = false;
  this->global = false;
}


  t_identifier::t_identifier(const t_identifier &vertex)
: t_symbol(vertex)
{
  this->nb_flows = vertex.nb_flows;
  this->addr = vertex.addr;
  this->isaclass = vertex.isaclass;
  this->global = vertex.global;
  this->classref = vertex.classref;
}

void t_identifier::addflow(unsigned int idflow)
{
  this->flows.push(idflow);
  this->nb_flows ++;
}

std::stack<unsigned int> t_identifier::getflows()
{
  return this->flows;
}

void t_identifier::setflows(std::stack<unsigned int> flows)
{
  this->flows = flows;
}

int t_identifier::getaddr() 
{
  return this->addr; 
}

void t_identifier::setaddr(int addr) 
{
  this->addr = addr;
}

void t_identifier::setclass(t_class *classref) 
{
  this->classref = classref;
  this->isaclass = true;
}

t_class *t_identifier::getclass() 
{
  return this->classref;
}

bool t_identifier::isclass() 
{
  return this->isaclass;
}

bool t_identifier::isglobal() 
{
  return this->global;
}

void t_identifier::setglobal() 
{
  this->global= true;
}

void t_identifier::setlocal() 
{
  this->global = false;
}

unsigned int t_identifier::getnbflows() 
{
  return this->nb_flows; 
}

void t_identifier::setnbflows(unsigned int nb_flows) 
{
  this->nb_flows = nb_flows;
}


