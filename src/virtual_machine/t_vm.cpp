#include "t_vm.h"
#include "multi_precision/t_mparray.h"
#include "multi_precision/t_mpinteger.h"
#include "multi_precision/t_string.h"
#include "multi_precision/t_mppolynomial.h"
#include "data_structures/graph/t_depth_first_search.h"
#include "console/t_time.h"
#include "console/t_print.h"
#include "static_analysis/t_data_flow.h"
#include <iostream>
#include <sstream>

t_vm::t_vm()
{
  this->co = 0;
  this->nbmpgenerics = 0;
  this->sp = SIZE_MEM - SIZE_SECTION_STACK;
  this->beg = SIZE_MEM - SIZE_SECTION_STACK - SIZE_SECTION_HEAP;
  this->bel = 0;

  this->mem.resize(SIZE_MEM);
  this->gencode = new t_gencode();
  this->control_flow_graph = NULL;
  this->print = new t_print;
}

t_gencode *t_vm::get_gencode()
{
  return this->gencode;
}

std::vector<int> &t_vm::get_mem()
{
  return this->mem;
}

unsigned int t_vm::get_sp()
{
  return this->sp;
}

unsigned int t_vm::get_co()
{
  return this->co;
}

void t_vm::set_co(unsigned int co)
{
  this->co = co;
}

unsigned int t_vm::get_executed_co()
{
  return this->executed_co;
}

void t_vm::set_executed_co(unsigned int executed_co)
{
  this->executed_co = executed_co;
}

unsigned int t_vm::get_bel()
{
  return this->bel;
}

unsigned int t_vm::get_beg()
{
  return this->beg;
}

bool t_vm::get_endwhile()
{
  return this->endwhile;
}

void t_vm::set_endwhile(bool endwhile)
{
  this->endwhile = endwhile;
}

t_print *t_vm::get_print()
{
  return this->print;
}

void t_vm::copy_code_to_mem(std::vector<int> &code)
{
  unsigned int i = 0;
  std::vector<int>::iterator it;

  for(it = code.begin(); it < code.end(); it++)
    this->mem[i ++] = *it;

  if(i > SIZE_SECTION_CODE)
    std::cout << "ALERT" << std::endl;
}

void t_vm::include(std::string file)
{
  //std::cout << std::endl << file << " has been included" << std::endl; 
  this->get_gencode()->get_syntax()->get_lexical()->set_file_source(file.c_str());
  this->start();
}

void t_vm::start()
{
  if(!this->get_gencode()->start())
    return;

  if(this->get_gencode()->get_syntax()->get_lexical()->get_current_token() == T_TAB)
  {
    unsigned int nb_comp = 0;
    std::map<std::string, t_function *> *functions = this->get_gencode()->get_syntax()->get_functions();
    std::map<std::string, t_function *>::iterator it;
    unsigned int nb_char = this->get_gencode()->get_syntax()->get_lexical()->get_current_line_string().length();
    std::string str =  this->get_gencode()->get_syntax()->get_lexical()->get_current_line_string();
    std::string str_found;

    std::cout << std::endl;

    for(it = functions->begin(); it != functions->end(); it ++)
    {
      if((*it).first.substr(0, nb_char).compare(str) == 0)
      {
        str_found = (*it).first;
        nb_comp ++;
      }
    }

    if(nb_comp)
      std::cout << str_found << std::endl;
  }
  else
  {
    this->get_print()->tac(this->get_gencode()->get_gentac()->get_tac()->get_code());
    this->get_print()->code(this->get_gencode());
    this->get_print()->statevm_start();

    this->copy_code_to_mem(this->get_gencode()->get_code());
    this->set_endwhile(false);

    while(this->get_co() < SIZE_SECTION_CODE && !this->get_endwhile())
    {	
      this->get_print()->statevm(this);
      this->execute();
    }

    this->get_print()->statevm_end();
  }
}

t_opcode t_vm::execute()
{
  this->endwhile = false;

  t_mpinteger *mpint1;
  t_mpinteger *mpint2;
  t_mparray *mparr;
  t_mpgeneric *mpgen1;
  //t_mpgeneric *mpgen2;
  t_string *str1;
  unsigned int i;
  unsigned int v;

  t_opcode opcode;
  std::ostringstream out;

  opcode = (t_opcode) this->mem[this->co];

  switch(opcode)
  {
    case JUMP:
      {
        this->co = this->mem[this->co + 1]; 
        break;
      }

    case EMPT:
      {
        i = this->mem[-- this->sp];

        mparr = (t_mparray *) this->mpgenerics[this->mem[this->co + 1]];
        mpgen1 = mparr->getcase(this->mpgenerics[i]->toint());

        this->mpgenerics.push_back(mpgen1);

        this->mem[this->sp ++] = nbmpgenerics ++;
        this->co += 2; 

        break;
      }

    case DEPT:
      {
        v = this->mem[-- this->sp];
        i = this->mem[-- this->sp];

        mparr = (t_mparray *) this->mpgenerics[this->mem[this->co + 1]];
        mparr->setcase(this->mpgenerics[i]->toint(), this->mpgenerics[v]);
        this->co += 2;

        break;
      }

    case DEPL:
      {
        this->mem[this->bel + this->mem[this->co + 1]] = this->mem[-- this->sp];
        this->co += 2;
        break;
      }

    case EMPL: 
      {
        this->mem[this->sp ++] = this->mem[this->bel + this->mem[this->co + 1]];
        this->co += 2;
        break;
      }

    case EMPG:
      {
        this->mem[this->sp ++] = this->mem[this->beg + this->mem[this->co + 1]];
        this->co += 2;
        break;
      }

    case EMPC:
      {
        mpint1 = t_mpinteger::fromcode(this->mem, this->co + 1);
        this->mpgenerics.push_back(mpint1);

        this->mem[this->sp ++] = nbmpgenerics ++;
        this->co = this->co + 3 + mpint1->getsize(); 

        break;
      }

    case EMPS:
      {
        str1 = t_string::fromcode(this->mem, this->co);
        this->mpgenerics.push_back(str1);

        this->mem[this->sp ++] = nbmpgenerics ++;
        this->co = this->co + 2 + str1->getsize(); 

        break;
      }

    case PILE: 
      {
        this->sp = this->sp + this->mem[co + 1];	
        this->co += 2;
        break;
      }

    case CALL:
      {
        this->mem[this->sp ++] = this->co + 2;
        this->co = this->mem[this->co + 1];
        break;
      }

    case FUNCTION:
      {
        this->mem[this->sp ++] = this->bel;
        this->bel = this->sp;
        this->co += 2;
        break;
      }

    case CLASS:
      {
        this->mem[this->sp ++] = this->bel;
        this->bel = this->sp;
        this->co += 2;
        break;
      }


    case INSTANCE_CLASS:
      {
        //unsigned int test1 = this->mem[this->co + 1];
        //unsigned int test2 = this->mem[this->co + 2];

        this->co += 3;
        break;
      }

    case INSTRUCTIONS:
      {
        this->mem[this->sp ++] = this->bel;
        this->bel = this->sp;
        this->co ++;
        break;
      }

    case INF:
      {
        mpint1 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 2]];
        mpint2 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 1]];

        this->mem[this->sp ++] = t_mpinteger::inf(mpint1, mpint2);
        this->co ++;
        break;
      }

    case INFEQUAL:
      {
        mpint1 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 2]];
        mpint2 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 1]];

        this->mem[this->sp ++] = t_mpinteger::infequal(mpint1, mpint2);
        this->co ++;
        break;
      }

    case SUP:
      {
        mpint1 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 2]];
        mpint2 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 1]];

        this->mem[this->sp ++] = t_mpinteger::sup(mpint1, mpint2);
        this->co ++;
        break;
      }

    case SUPEQUAL:
      {
        mpint1 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 2]];
        mpint2 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 1]];

        this->mem[this->sp ++] = t_mpinteger::supequal(mpint1, mpint2);
        this->co ++;
        break;
      }

    case ADD:
      {
        mpint1 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 2]];
        mpint2 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 1]];
        mpint2 = t_mpinteger::add(mpint1, mpint2);

        this->mpgenerics[this->mem[sp - 1]] = (t_mpgeneric *) mpint2;
        this->co ++;
        break;
      }

    case SUB:
      {
        mpint1 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 2]];
        mpint2 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 1]];
        mpint2 = t_mpinteger::sub(mpint1, mpint2);

        this->mpgenerics[this->mem[sp - 1]] = (t_mpgeneric *) mpint2;
        this->co ++;
        break;
      }

    case MUL: 
      {
        mpint1 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 2]];
        mpint2 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 1]];
        mpint2 = t_mpinteger::mul(mpint1, mpint2);

        this->mpgenerics[this->mem[sp - 1]] = (t_mpgeneric *) mpint2;
        this->co ++;
        break;
      }

    case DIV: 
      {
        mpint1 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 2]];
        mpint2 = (t_mpinteger *) this->mpgenerics[this->mem[sp - 1]];
        mpint2 = t_mpinteger::div(mpint1, mpint2);

        this->mpgenerics[this->mem[sp - 1]] = (t_mpgeneric *) mpint2;
        this->co ++;
        break;
      }

    case RETURN:
      { 
        this->mem[this->bel - (this->mem[this->co + 1] + 3)] = this->mem[-- this->sp];
        this->sp = this->bel;
        this->bel = this->mem[-- this->sp];
        this->co = this->mem[-- this->sp]; 
        break;
      }

    case IFFALSE:
      {
        if(!this->mem[sp - 1])
        {
          this->co = this->mem[this->co + 1];
          break;
        }

        this->co += 2;
        break;
      }

    case EXIT:
      {
        this->endwhile = true;
        this->co ++;
        break;
      }

    case SYSTEM:
      {
        std::ostringstream oss;
        oss << *mpgenerics[this->mem[this->bel + this->mem[this->co + 1]]];

        std::string str = oss.str();
        if(str == "exit")
        {
          this->endwhile = true;
        }

        else if(str == "show_tac")
        {
          this->get_print()->set_print_tac(true);
          this->get_print()->tac(get_gencode()->get_gentac()->get_tac()->get_code());
        }

        else if(str == "hide_tac")
        {
          this->get_print()->set_print_tac(false);

        }

        else if(str == "show_code")
        {
          this->get_print()->set_print_code(true);
          this->get_print()->code(get_gencode());
        }

        else if(str == "hide_code")
        {
          this->get_print()->set_print_code(false);
        }

        else if(str == "show_statevm")
        {
          this->get_print()->set_print_statevm(true);
          this->get_print()->statevm_start();
          this->get_print()->statevm(this);
          this->get_print()->statevm_end();
        }

        else if(str == "hide_statevm")
        {
          this->get_print()->set_print_statevm(false);

        }
        else if(str == "data_flow_get_flows")
        {
          this->control_flow_graph = new t_control_flow_graph(get_gencode()->get_gentac()->get_tac());
          this->control_flow_graph->start(get_gencode()->get_gentac()->get_tac()->get_code()->begin());

          t_data_flow *data_flow = new t_data_flow(this->control_flow_graph);
          data_flow->start();
          data_flow->get_flows();
        }
        else
        {
          std::cout << "ERREUR SYSTEM" << std::endl;
        }

        this->co += 2;
        break;
      }

    case INCLUDE:
      {
        std::ostringstream out1;
        out1 << *mpgenerics[this->mem[this->bel + this->mem[this->co + 1]]];

        this->include(out1.str());

        this->co += 2;
        break;
      }

    case PRINT:
      {
        unsigned int test = this->mem[this->bel + this->mem[this->co + 1]];
        std::cout << *mpgenerics[this->mem[this->bel + this->mem[this->co + 1]]] << std::endl;

        this->co += 2;
        break;
      }

    case READPOL:
      {
        //unsigned int test = this->bel + this->mem[this->co + 1];
        std::cout << *mpgenerics[this->mem[this->bel + this->mem[this->co + 1]]] << std::endl;
        //t_string *tstr = (t_string *) mpgenerics[this->mem[this->bel + this->mem[this->co + 1]]];
        //t_mppolynomial *mppol1 = (t_mppolynomial *) t_mppolynomial::read('X', tstr);

        this->co ++;
        break;
      }

    case END:
      {
        this->endwhile = true;
        break;
      }

    case FPUTS:
      {
        std::ostringstream out1;
        out1 << *mpgenerics[this->mem[this->bel + this->mem[this->co + 2]]];

        std::ostringstream out2;
        out2 << *mpgenerics[this->mem[this->bel + this->mem[this->co + 1]]];

        FILE *filept = fopen(out1.str().c_str(), "w");
        if(filept != NULL)
        {
          fputs(out2.str().c_str(), filept);
          fclose(filept);
        }

        this->co += 2;
        break;
      }

    case CFG_DOT:
      {
        if(this->control_flow_graph != NULL)
        {	
          std::ostringstream out1;
          out1 << *mpgenerics[this->mem[this->bel + this->mem[this->co + 1]]];

          t_dotcfgtree *dotcfgtreevisitor = new t_dotcfgtree;
          dotcfgtreevisitor->openfile(out1.str().c_str());
          t_depth_first_search::visit<t_dotcfgtree, t_cfg, t_threeaddresscode>(dotcfgtreevisitor, this->control_flow_graph->getgraph());
          std::cout << std::endl << "control flow graph has been printed into " << out1.str().c_str() << std::endl;
        }
        else
          std::cout << std::endl << "control flow graph must be computed" << std::endl;

        this->co += 2;
        break;
      }

    case CFG_COMPUTE:
      {
        this->control_flow_graph = new t_control_flow_graph(get_gencode()->get_gentac()->get_tac());
        this->control_flow_graph->start(get_gencode()->get_gentac()->get_tac()->get_code()->begin());

        this->co += 1;
        break;
      }


    case AST_DOT:
      {
        std::ostringstream out1;
        out1 << *mpgenerics[this->mem[this->bel + this->mem[this->co + 1]]];

        t_dotsyntaxtree *dotsyntaxtree = new t_dotsyntaxtree;
        dotsyntaxtree->openfile(out1.str().c_str());
        t_depth_first_search::visit<t_dotsyntaxtree, t_syntaxtree, t_symbol>(dotsyntaxtree, get_gencode()->get_syntax()->gettree());

        std::cout << std::endl << "abstract syntax tree has been printed into " << out1.str().c_str() << std::endl;

        this->co += 2;
        break;
      }

    default: 
      {
        this->co ++;
        break;
      }
  }



  return opcode;
}



