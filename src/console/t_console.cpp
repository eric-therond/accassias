#include "t_console.h"

t_console::t_console()
{
  this->vm = new t_vm;

  this->t0 = 0;
  this->t1 = 0;
}

void t_console::start()
{		 
  timestamp_t t0_gen = get_timestamp();		

  this->get_vm()->start();

  timestamp_t t1_gen = get_timestamp();
  t1_gen = t1_gen - t0_gen;

  //std::cout << std::endl << "time = " << t1_gen << " ms" << std::endl;	

  std::cout << "\033[1;34maca\033[0m\033[1;36m$\033[0m ";
}

t_vm *t_console::get_vm()
{
  return this->vm;
}

void t_console::read()
{
  std::string contenu;
  std::ifstream is;

  std::string test = std::string(getenv("HOME")) + "/.accassias";
  is.open(test.c_str(), std::ifstream::in);

  std::cout << std::endl;

  if(is.good())
  {
    while(is.good())
    {
      getline(is, contenu);
      if(contenu == "auto_print_tac=yes")
      {
        this->get_vm()->get_print()->set_print_tac(true);
        std::cout << "     CONSOLE : \033[1;32mthree address code will be printed\033[0m" << std::endl;
      }

      if(contenu == "auto_print_code=yes")
      {
        this->get_vm()->get_print()->set_print_code(true);
        std::cout << "     CONSOLE : \033[1;32mcode will be printed\033[0m" << std::endl;
      }

      if(contenu == "auto_print_statevm=yes")
      {
        this->get_vm()->get_print()->set_print_statevm(true);
        std::cout << "     CONSOLE : \033[1;32mstate vm + stack will be printed\033[0m" << std::endl;
      }
    }

    is.close(); 
  }

  std::cout << std::endl;
  std::cout << std::endl;
}

