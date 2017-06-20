#ifndef T_PRINT_H 
#define T_PRINT_H 

#include <vector>
#include <string>
#include "compiler/gencode/t_threeaddress.h"

class t_vm;
class t_gencode;
#define SIZE_LINES 20

class t_print 
{
  public:
    t_print();
    static std::string printvar(int);

    bool get_print_tac();
    void set_print_tac(bool);
    bool get_print_code();
    void set_print_code(bool);
    void set_print_statevm(bool);
    bool get_print_statevm();

    void statevm_start();
    void statevm_end();
    void statevm(t_vm *);
    void tac(std::vector<t_threeaddress> *);
    void code(t_gencode *);
    void stack(t_vm *);
    void space(std::string);
    void space(unsigned int);

  private:
    bool print_statevm;
    bool print_tac;
    bool print_code;
};

#endif

