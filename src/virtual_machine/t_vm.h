#ifndef T_VM_H
#define T_VM_H

#include "console/t_print.h"
#include "compiler/gencode/t_gencode.h"
#include "compiler/gencode/t_opcode.h"
#include "multi_precision/t_mpgeneric.h"
#include "static_analysis/t_control_flow_graph.h"

#define SIZE_SECTION_CODE 10000
#define SIZE_SECTION_STACK 10000
#define SIZE_SECTION_HEAP 10000
#define SIZE_MEM SIZE_SECTION_CODE+SIZE_SECTION_STACK+SIZE_SECTION_HEAP

class t_vm
{
  public:
    t_vm();
    void include(std::string);
    void start();
    t_opcode execute();
    std::vector<int> &get_mem();
    unsigned int get_sp();
    unsigned int get_co();
    unsigned int get_executed_co();
    void set_executed_co(unsigned int);
    void set_co(unsigned int);
    unsigned int get_bel();
    unsigned int get_beg();
    bool get_endwhile();
    void set_endwhile(bool);
    void copy_code_to_mem(std::vector<int> &);
    t_gencode *get_gencode();
    t_print *get_print();

  private:
    t_print *print;
    std::vector<int> mem;

    bool endwhile;
    bool print_statevm;
    unsigned int executed_co;
    unsigned int co;
    unsigned int bel;
    unsigned int beg;
    unsigned int sp;
    t_gencode *gencode;
    bool wantreloadconsole;

    std::vector<t_mpgeneric *> mpgenerics;
    unsigned int nbmpgenerics;

    t_control_flow_graph *control_flow_graph;
};

#endif

