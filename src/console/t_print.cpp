#include "t_print.h"
#include "virtual_machine/t_vm.h"
#include "compiler/gencode/t_gencode.h"

t_print::t_print()
{
  this->print_statevm = false;
  this->print_tac = false;
  this->print_code = false;
}

void t_print::set_print_statevm(bool state)
{
  this->print_statevm = state;
}

bool t_print::get_print_statevm()
{
  return this->print_statevm;
}

std::string t_print::printvar(int addr)
{
  std::stringstream streamtemp;
  std::string temp("temp");

  streamtemp << addr;
  temp += streamtemp.str();

  return temp;
}

bool t_print::get_print_tac()
{
  return print_tac;
}

void t_print::set_print_tac(bool state)
{
  this->print_tac = state;
}

void t_print::set_print_code(bool state)
{
  this->print_code = state;
}

bool t_print::get_print_code()
{
  return this->print_code;
}

void t_print::code(t_gencode *gencode)
{
  if(this->get_print_code())
  {
    unsigned int temp_co = 0;

    std::vector<int> code = gencode->get_code();
    std::cout << std::endl << " : CODE : \033[1;35mcode here\033[0m" << std::endl << std::endl;

    while(temp_co < code.size())
    {
      std::cout << temp_co << "    ";
      std::cout << gencode->show_code_of_nextintruction(temp_co) << std::endl;

    }
  }
}

void t_print::tac(std::vector<t_threeaddress> *code)
{
  if(this->get_print_tac())
  {	  
    std::cout << std::endl << " : THREEADDRESSCODE : \033[1;35mthree address code here\033[0m" << std::endl << std::endl;

    std::vector<t_threeaddress>::iterator it;
    for(it = code->begin(); it < code->end(); it++)
      std::cout << *it << std::endl;
  }
}

void t_print::stack(t_vm *vm)
{
  unsigned int j = 0;
  unsigned int i;

  for(i = vm->get_sp(); i >= SIZE_MEM - SIZE_SECTION_STACK; i --)
  {
    std::cout << " " << vm->get_mem()[i];
    if(j++ > 20)
      break;
  }
}

void t_print::statevm_start()
{
  if(this->get_print_statevm())
  { 
    std::cout << std::endl << " : VM : \033[1;35mstate and stack here\033[0m" << std::endl << std::endl;
    std::cout << "---------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "|         co |        bel  |         sp | instruction                    |  stack                        " << std::endl;
    std::cout << "---------------------------------------------------------------------------------------------------------------------------------" << std::endl;		
  }
}

void t_print::statevm_end()
{
  if(this->get_print_statevm())
  { 
    std::cout << "---------------------------------------------------------------------------------------------------------------------------------" << std::endl;		
  }
}

void t_print::statevm(t_vm *vm)
{
  if(this->get_print_statevm())
  { 	
    unsigned int temp_co = vm->get_co();
    std::cout << "| "; this->space(vm->get_co());
    std::cout << " | "; this->space(vm->get_bel());
    std::cout << "  | "; this->space(vm->get_sp());
    std::cout << " | "; this->space(vm->get_gencode()->show_code_of_nextintruction(temp_co));
    std::cout << " | "; this->stack(vm); 
    std::cout << std::endl;
  }
}

void t_print::space(std::string str)
{
  unsigned int size = 30 - str.size();
  unsigned int i;

  std::cout << str;	

  for(i = 0; i < size; i ++)
    std::cout << " ";
}

void t_print::space(unsigned int var)
{
  if(var < 10)
    std::cout << "         ";
  else if(var >= 10 && var < 100)
    std::cout << "        ";
  else if(var >= 100 && var < 1000)
    std::cout << "       ";
  else if(var >= 1000 && var < 10000)
    std::cout << "      ";
  else if(var >= 10000 && var < 100000)
    std::cout << "     ";
  else if(var >= 100000 && var < 1000000)
    std::cout << "    ";
  else if(var >= 1000000 && var < 10000000)
    std::cout << "   ";
  else if(var >= 10000000 && var < 100000000)
    std::cout << "  ";
  else if(var >= 100000000 && var < 1000000000)
    std::cout << " ";

  std::cout << var;
}

