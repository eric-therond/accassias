#include "t_symbol.h"

t_symbol::t_symbol()
{
}

t_symbol::t_symbol(unsigned int line, unsigned int column, s_symbol symbol)
{
  this->line = line;
  this->column = column;
  this->symbol = symbol;
}

t_symbol::t_symbol(unsigned int line, unsigned int column, s_symbol symbol, std::string &lexeme)
{
  this->line = line;
  this->column = column;
  this->symbol = symbol;
  this->lexeme = lexeme;
}

t_symbol::t_symbol(const t_symbol &vertex)
{
  this->line = vertex.line;
  this->column = vertex.column;
  this->symbol = vertex.symbol;
}

void t_symbol::setsymbol(s_symbol symbol)
{
  this->symbol = symbol;
}

s_symbol t_symbol::getsymbol()
{
  return symbol;
}

unsigned int t_symbol::getline()
{
  return line;
}

unsigned int t_symbol::getcolumn()
{
  return column;
}

void t_symbol::setlexeme(std::string &lexeme)
{
  this->lexeme = lexeme;
}

std::string &t_symbol::getlexeme()
{
  return lexeme;
}

int t_symbol::getaddr() 
{
  return 0; 
}

void t_symbol::setaddr(int) 
{
}

unsigned int t_symbol::getnbargs() 
{
  return 0; 
}

void t_symbol::setnbargs(unsigned int) 
{
}

unsigned int t_symbol::getnblocalvariables()
{
  return this->nblocalvariables;
}

void t_symbol::setnblocalvariables(unsigned int nblocalvariables)
{
  this->nblocalvariables = nblocalvariables;
}

std::ostream &operator<<(std::ostream &out, t_symbol &symbol)
{
  out << symbol.getlexeme();

  return out;
}


