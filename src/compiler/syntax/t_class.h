#ifndef t_class_H
#define t_class_H

#include "t_identifier.h"
#include "t_function.h"
#include <map>
#include <string>

class t_class : public t_identifier
{
  public:
    t_class();
    t_class(unsigned int, unsigned int, s_symbol, int);
    t_class(unsigned int, unsigned int, s_symbol, std::string &);
    t_class(unsigned int, unsigned int, s_symbol, std::string &, int);
    t_class(unsigned int, unsigned int, s_symbol, std::string &, int, unsigned int);
    t_class(unsigned int, unsigned int, s_symbol, std::string &, int, unsigned int, unsigned int);

    std::map<std::string, t_function *> *getmethods();
    void setmethods(std::map<std::string, t_function *> *);

    std::map<std::string, t_identifier *> *getvariables();
    void setvariables(std::map<std::string, t_identifier *> *);

    virtual unsigned int getnblocalvariables();
    virtual void setnblocalvariables(unsigned int);

  private:
    unsigned int nbargs;
    unsigned int nblocalvariables;
    std::map<std::string, t_function *> *methods;
    std::map<std::string, t_identifier *> *variables;

};

#endif
