#ifndef T_IDENTIFIER_H
#define T_IDENTIFIER_H

#include "t_symbol.h"
#include <string>
#include <stack>

class t_class;

class t_identifier : public t_symbol
{
  public:
    t_identifier();
    t_identifier(unsigned int, unsigned int, s_symbol);
    t_identifier(unsigned int, unsigned int, s_symbol, std::string &);
    t_identifier(unsigned int, unsigned int, s_symbol, int);
    t_identifier(unsigned int, unsigned int, s_symbol, std::string &, int);
    t_identifier(const t_identifier &);

    void setclass(t_class *);
    t_class *getclass();
    bool isclass();
    bool isglobal();
    void setglobal();
    void setlocal(); 

    int getaddr();
    void setaddr(int);

    void addflow(unsigned int);

    std::stack<unsigned int> getflows();
    void setflows(std::stack<unsigned int>);

    unsigned int getnbflows(); 
    void setnbflows(unsigned int);

  protected:
    int addr;
    std::stack<unsigned int> flows;
    unsigned int nb_flows;
    t_class *classref;
    bool isaclass;
    bool global;

};

#endif
