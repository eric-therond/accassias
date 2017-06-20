#include "t_class.h"
#include <iostream>
#include <sstream>

  t_class::t_class() 
: t_identifier()
{
  this->nbargs = 0;
  this->nblocalvariables = 0; 
  this->methods = new std::map<std::string, t_function *>;
  this->variables = new std::map<std::string, t_identifier *>;
}

  t_class::t_class(unsigned int line, unsigned int column, s_symbol symbol, int addr) 
: t_identifier(line, column, symbol)
{
  this->addr = addr;
  this->nbargs = 0;
  this->nblocalvariables = 0;
  this->methods = new std::map<std::string, t_function *>;
  this->variables = new std::map<std::string, t_identifier *>;
}

  t_class::t_class(unsigned int line, unsigned int column, s_symbol symbol, std::string &lexeme) 
: t_identifier(line, column, symbol, lexeme)
{
  this->nbargs = 0;
  this->nblocalvariables = 0;
  this->methods = new std::map<std::string, t_function *>;
  this->variables = new std::map<std::string, t_identifier *>;
}

  t_class::t_class(unsigned int line, unsigned int column, s_symbol symbol, std::string &lexeme, int addr) 
: t_identifier(line, column, symbol, lexeme, addr)
{
  this->nbargs = 0;
  this->nblocalvariables = 0;
  this->methods = new std::map<std::string, t_function *>;
  this->variables = new std::map<std::string, t_identifier *>;
}

  t_class::t_class(unsigned int line, unsigned int column, s_symbol symbol, std::string &lexeme, int addr, unsigned int nbargs) 
: t_identifier(line, column, symbol, lexeme, addr)
{
  this->nbargs = nbargs;
  this->nblocalvariables = 0;
  this->methods = new std::map<std::string, t_function *>;
  this->variables = new std::map<std::string, t_identifier *>;
}

  t_class::t_class(unsigned int line, unsigned int column, s_symbol symbol, std::string &lexeme, int addr, unsigned int nbargs, unsigned int nblocalvariables) 
: t_identifier(line, column, symbol, lexeme, addr)
{
  this->nbargs = nbargs;
  this->nblocalvariables = nblocalvariables; 
  this->methods = new std::map<std::string, t_function *>;
  this->variables = new std::map<std::string, t_identifier *>;
}

std::map<std::string, t_function *> *t_class::getmethods()
{
  return this->methods;
}

void t_class::setmethods(std::map<std::string, t_function *> *methods)
{
  this->methods = methods;
}

std::map<std::string, t_identifier *> *t_class::getvariables()
{
  return this->variables;
}

void t_class::setvariables(std::map<std::string, t_identifier *> *variables)
{
  this->variables = variables;
}

unsigned int t_class::getnblocalvariables()
{
  return this->nblocalvariables;
}

void t_class::setnblocalvariables(unsigned int nblocalvariables)
{
  this->nblocalvariables = nblocalvariables;
}



