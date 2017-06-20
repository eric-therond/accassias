#ifndef T_GENCODE_H
#define T_GENCODE_H

#include "compiler/syntax/t_syntax.h"
#include "t_genthreeaddresscodetreevisitor.h"
#include "t_opcode.h"
#include <map>

#define SIZE_WORD sizeof(unsigned int)

class t_gencode
{
  public:
    t_gencode();
    bool start();
    std::vector<int> &get_code();
    t_syntax *get_syntax();
    t_genthreeaddresscode *get_gentac();
    std::string code_to_string(unsigned int, unsigned int);
    std::string show_code_of_nextintruction(unsigned int &);

  private:
    std::vector<int> code;
    t_syntax *syntax;
    t_genthreeaddresscode *gentac;
    std::map<t_node<t_symbol> *, t_color> nodes_color;
    unsigned int imem;
    unsigned int last_stack;
    std::map<std::string, t_function *> *current_functions;
};

//std::ostream &operator<<(std::ostream &, t_gencode &);

#endif

