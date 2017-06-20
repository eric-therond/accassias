#ifndef T_CONSOLE_H 
#define T_CONSOLE_H 

#include <string>
#include <fstream>
#include "virtual_machine/t_vm.h"
#include "console/t_time.h"

class t_console 
{
  public:
    t_console();
    void read();
    void start();
    t_vm *get_vm();

  private:
    t_vm *vm;
    timestamp_t t0;
    timestamp_t t1;
};

#endif

