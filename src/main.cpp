#include "console/t_console.h"

int main(int argc, char **argv)
{

  t_console *console = new t_console;  
  console->read();

  console->get_vm()->include("./poly.aca");
  console->get_vm()->include("./stdlib.aca");
  console->get_vm()->include("./ast.aca");
  console->get_vm()->include("./cfg.aca");
  console->get_vm()->include("./data_flow.aca");

  if(argc > 0)
    console->get_vm()->get_gencode()->get_syntax()->get_lexical()->set_file_source(argv[1]);

  while(true)
    console->start();

  return 0;
}


