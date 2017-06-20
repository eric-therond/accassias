#include "t_gencode.h"
#include "data_structures/graph/t_depth_first_search.h"
#include "compiler/syntax/t_syntax.h"
#include "compiler/t_compiler.h"
#include "multi_precision/t_mpinteger.h"
#include "multi_precision/t_string.h"
#include "static_analysis/t_data_flow.h"
#include <iostream>
#include <sstream>

t_gencode::t_gencode()
{
  this->syntax = new t_syntax();
  this->gentac = new t_genthreeaddresscode;
  this->last_stack = 0;
}

t_syntax *t_gencode::get_syntax()
{
  return syntax;
}

bool t_gencode::start()
{
  std::stack<unsigned int> stack_temp_addr_if;
  std::stack<unsigned int> stack_temp_addr_elseif;
  std::stack<unsigned int> stack_temp_addr_else;
  std::stack<unsigned int> stack_temp_addr_endif;
  std::stack<unsigned int> stack_temp_addr_for;
  std::stack<unsigned int> stack_temp_addr_startfor;

  //std::map<t_function *, unsigned int> addr_functions;
  std::map<unsigned int, t_function *> addr_functions;

  this->imem = 2;
  this->code.clear();
  this->nodes_color.clear();
  this->gentac->get_tac()->get_code()->clear();

  if(!this->syntax->start())
    return false;

  if(this->syntax->getexistsyntaxerror())
    return false;

  t_depth_first_search::visit<t_genthreeaddresscode, t_syntaxtree, t_symbol>(gentac, syntax->gettree(), nodes_color);  	
  syntax->gettree()->setcolorize(false);

  t_threeaddresscode *threeaddresscode = gentac->get_tac();

  this->current_functions = this->syntax->get_functions();
  this->code.push_back(JUMP);
  this->code.push_back(0);

  std::vector<int> tmpcode;
  std::vector<t_threeaddress>::iterator it, itstart;
  for(it = threeaddresscode->get_code()->begin(); it < threeaddresscode->get_code()->end(); it++)
  {
    switch((*it).gettacsymbol())
    {
      case TAC_ASSIGN_SUBINSTRUCTIONS:
        {
          itstart = it;
          this->code.push_back(INSTRUCTIONS);
          this->code.push_back(PILE);
          this->code.push_back((*it).getaddrarg1());
          imem += 3;
          break;
        }

      case TAC_ASSIGN_INSTRUCTIONS:
        {
          break;
        }

      case TAC_ASSIGN_STARTFOR:
        {
          stack_temp_addr_startfor.push(imem);

          break;
        }

      case TAC_ASSIGN_FOR:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrresult());
          this->code.push_back(IFFALSE);
          this->code.push_back(0);

          stack_temp_addr_for.push(imem + 3);

          imem += 4;

          break;
        }

      case TAC_ASSIGN_ENDFOR:
        {
          if(stack_temp_addr_for.empty())
            exit(0);

          this->code[stack_temp_addr_for.top()] = imem + 2;
          stack_temp_addr_for.pop();

          this->code.push_back(JUMP);

          if(stack_temp_addr_startfor.empty())
            exit(0);

          this->code.push_back(stack_temp_addr_startfor.top());
          stack_temp_addr_startfor.pop();

          break;
        }

      case TAC_ASSIGN_IF:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrresult());
          this->code.push_back(IFFALSE); 
          this->code.push_back(0);

          stack_temp_addr_if.push(imem + 3);

          imem += 4;

          break;
        }

      case TAC_ASSIGN_IFTRUE:
        {
          break;
        }

      case TAC_ASSIGN_ENDIF:
        {
          if(stack_temp_addr_if.empty())
            exit(0);

          this->code[stack_temp_addr_if.top()] = imem + 2;
          stack_temp_addr_if.pop();

          break;
        }

      case TAC_ASSIGN_ELSEIF:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrresult());
          this->code.push_back(IFFALSE);
          this->code.push_back(0);

          stack_temp_addr_elseif.push(imem + 3);

          imem += 4;

          break;
        }

      case TAC_ASSIGN_ENDELSEIF:
        {
          if(stack_temp_addr_elseif.empty())
            exit(0);

          this->code[stack_temp_addr_elseif.top()] = imem + 2;
          stack_temp_addr_elseif.pop();

          break;
        }

      case TAC_ASSIGN_ELSE:
        {
          this->code.push_back(JUMP);
          this->code.push_back(0);

          stack_temp_addr_else.push(imem + 1);
          imem += 2;

          break;
        }

      case TAC_ASSIGN_ENDELSE:
        {
          if(stack_temp_addr_else.empty())
            exit(0);

          this->code[stack_temp_addr_else.top()] = imem;
          stack_temp_addr_else.pop();

          break;
        }

      case TAC_ASSIGN_NOELSE:
        {

          break;
        }

      case TAC_ASSIGN_PROG_JUMP:
        {
          this->code[1] = imem;
          break;
        }

      case TAC_ASSIGN_READPOL:
        {
          this->code.push_back(READPOL);
          this->code.push_back(-3);
          imem += 2;
          break;
        }

      case TAC_ASSIGN_STARTCALL:
        {
          this->code.push_back(PILE);
          this->code.push_back(1);
          imem += 2;
          break;
        }

      case TAC_ASSIGN_CALL:
        {
          //addr_functions[(*this->current_functions)[(*it).getconst1()]] = imem+1;
          addr_functions[imem+1] = (*this->current_functions)[(*it).getconst1()];

          this->code.push_back(CALL);
          this->code.push_back((*this->current_functions)[(*it).getconst1()]->getaddr());   
          this->code.push_back(PILE);
          this->code.push_back(-1*((*it).getaddrarg1())); // nbargs
          imem += 4;

          this->current_functions = this->syntax->get_functions();
          break;
        }

      case TAC_ASSIGN_METHOD:
        {
          this->current_functions = (*this->syntax->get_classes())[(*it).getconst1()]->getmethods();
          break;
        }

      case TAC_ASSIGN_CLASS:
        {
          this->current_functions = (*this->syntax->get_classes())[(*it).getconst1()]->getmethods();
          this->code.push_back(CLASS);
          this->code.push_back((*it).getaddrresult());
          //this->code.push_back(PILE);
          //this->code.push_back((*it).getaddrarg1());
          //imem += 4;
          imem += 2;
          break;
        }

      case TAC_ASSIGN_INSTANCE_CLASS:
        {
          //this->current_functions = (*this->syntax->get_classes())[(*it).getconst1()]->getmethods();
          this->code.push_back(INSTANCE_CLASS);
          this->code.push_back((*it).getaddrresult()); // addr
          //this->code.push_back(PILE);
          this->code.push_back((*it).getaddrarg1()); // nblocalvariables
          imem += 3;
          break;
        }

      case TAC_ASSIGN_ENDCLASS:
        {
          this->current_functions = this->syntax->get_functions();
          break;
        }

      case TAC_ASSIGN_FUNCTION:
        {
          (*this->current_functions)[(*it).getconst1()]->setaddr(imem);
          this->code.push_back(FUNCTION);
          this->code.push_back((*it).getaddrresult());
          this->code.push_back(PILE);
          this->code.push_back((*it).getaddrarg1());
          imem += 4;
          break;
        }

      case TAC_ASSIGN_RETURN:
        {    	
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrresult());
          this->code.push_back(RETURN);
          this->code.push_back((*it).getaddrarg1());
          imem += 4;
          break;
        }

      case TAC_ASSIGN_RETURN_WITHOUT_VALUE:
        {    	
          this->code.push_back(RETURN);
          this->code.push_back((*it).getaddrarg1());
          imem += 2;
          break;
        }

      case TAC_ASSIGN_DECLR_ARRAY:
        {
          break;
        }

      case TAC_ASSIGN_DECLR:
        {
          break;
        }

      case TAC_ASSIGN_EXIT:
        {

          this->code.push_back(EXIT);
          imem ++;
          break;
        }

      case TAC_ASSIGN_INCLUDE:
        {

          this->code.push_back(INCLUDE);
          this->code.push_back(-3);
          imem += 2;
          break;
        }

      case TAC_ASSIGN_SYSTEM:
        {

          this->code.push_back(SYSTEM);
          this->code.push_back(-3);
          imem += 2;
          break;
        }

      case TAC_ASSIGN_PRINT:
        {

          this->code.push_back(PRINT);
          this->code.push_back(-3);
          imem += 2;
          break;
        }

      case TAC_ASSIGN_FPUTS:
        {
          this->code.push_back(FPUTS);
          this->code.push_back(-3);
          imem += 2;
          break;
        }

      case TAC_ASSIGN_CFG_DOT:
        {

          this->code.push_back(CFG_DOT);
          this->code.push_back(-3);
          imem += 2;
          break;
        }

      case TAC_ASSIGN_CFG_COMPUTE:
        {

          this->code.push_back(CFG_COMPUTE);
          imem += 1;
          break;
        }


      case TAC_ASSIGN_AST_DOT:
        {

          this->code.push_back(AST_DOT);
          this->code.push_back(-3);
          imem += 2;
          break;
        }

      case TAC_ASSIGN_PARAM:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrresult());
          imem += 2;
          break;
        }

      case TAC_ASSIGN_FACTOR_ARRAY:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg2()); // i
          this->code.push_back(EMPT);
          this->code.push_back((*it).getaddrarg1());
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());
          imem += 6;
          break;
        }

      case TAC_ASSIGN_ARRAY:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1()); // i
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg2()); // v
          this->code.push_back(DEPT);
          this->code.push_back((*it).getaddrresult());
          imem += 6;
          break;
        }

      case TAC_ASSIGNV:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1());
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());
          imem += 4;
          break;
        }

      case TAC_ASSIGNCA:
        {

          addr_functions[imem+1] = (*this->current_functions)[(*it).getconst1()];

          this->code.push_back(CALL);
          this->code.push_back((*this->current_functions)[(*it).getconst1()]->getaddr());
          this->code.push_back(PILE);
          this->code.push_back(-1*((*it).getaddrarg1())); // nbargs       
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());
          imem += 6;
          break;
        }

      case TAC_ASSIGNL:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1());
          imem += 2;
          break;
        }

      case TAC_ASSIGNC:
        {
          this->code.push_back(EMPC);
          tmpcode = t_mpinteger::consttocode((*it).getconst1(), 10);
          this->code.insert(this->code.end(), tmpcode.begin(), tmpcode.end());
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());
          imem = imem + 3 + tmpcode.size();

          break;
        }

      case TAC_ASSIGNS:
        {
          this->code.push_back(EMPS);
          tmpcode = t_string::consttocode((*it).getconst1());
          this->code.insert(this->code.end(), tmpcode.begin(), tmpcode.end());
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());

          imem = imem + 3 + tmpcode.size();

          break;
        }

      case TAC_ASSIGN_INF:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1());
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg2());
          this->code.push_back(INF);
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());

          imem += 7;
          break;
        }

      case TAC_ASSIGN_INFEQUAL:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1());
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg2());
          this->code.push_back(INFEQUAL);
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());

          imem += 7;
          break;
        }

      case TAC_ASSIGN_SUP:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1());
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg2());
          this->code.push_back(SUP);
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());

          imem += 7;
          break;
        }

      case TAC_ASSIGN_SUPEQUAL:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1());
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg2());
          this->code.push_back(SUPEQUAL);
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());

          imem += 7;
          break;
        }

      case TAC_ASSIGN_ADD:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1());
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg2());
          this->code.push_back(ADD);
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());

          imem += 7;
          break;
        }

      case TAC_ASSIGN_SUB:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1());
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg2());
          this->code.push_back(SUB);
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());
          imem += 7;
          break;
        }

      case TAC_ASSIGN_DIV:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1());
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg2());
          this->code.push_back(DIV);
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());
          imem += 7;
          break;
        }

      case TAC_ASSIGN_MUL:
        {
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg1());
          this->code.push_back(EMPL);
          this->code.push_back((*it).getaddrarg2());
          this->code.push_back(MUL);
          this->code.push_back(DEPL);
          this->code.push_back((*it).getaddrresult());
          imem += 7;
          break;
        }

      case TAC_ASSIGN_NULL:
        {
          break;
        }
    }
  }

  t_function *fd;
  for (std::map< unsigned int, t_function *>::iterator it = addr_functions.begin(); it != addr_functions.end(); ++it)
  {
    fd = it->second;
    this->code[it->first] = fd->getaddr();
  }

  return true;
}

std::vector<int> &t_gencode::get_code()
{
  return this->code;
}

t_genthreeaddresscode *t_gencode::get_gentac()
{
  return gentac;
}

std::string t_gencode::code_to_string(unsigned int nbwords, unsigned int start)
{
  std::string str;
  unsigned int i;

  for(i = 0; i < nbwords; i ++)
    str += static_cast<char>(this->code[i+start]);

  return str;
}

std::string t_gencode::show_code_of_nextintruction(unsigned int &address_instruction)
{
  std::ostringstream out;

  switch(code[address_instruction])
  {
    case INSTRUCTIONS:
      {
        out << "INSTRUCTIONS ";
        address_instruction ++;

        break;
      }

    case IFFALSE:
      {
        out << "IFFALSE ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case JUMP:
      {
        out << "JUMP ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case PILE:
      {
        out << "PILE ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case CALL:
      {
        out << "CALL ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case FUNCTION:
      {
        out << "FUNCTION ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case CLASS:
      {
        out << "CLASS ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case INSTANCE_CLASS:
      {
        out << "INSTANCE_CLASS ";
        address_instruction ++;
        out << code[address_instruction] << " ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case RETURN:
      {
        out << "RETURN ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case EMPT:
      {
        out << "EMPT ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case DEPT:
      {
        out << "DEPT ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case EMPC:
      {
        out << "EMPC ";
        address_instruction ++;
        out << code[address_instruction] << " ";
        address_instruction ++;
        out << code[address_instruction] << " ";
        address_instruction ++;
        out << this->code_to_string(code[address_instruction-1], address_instruction);
        address_instruction += code[address_instruction-1];

        break;
      }

    case EMPS:
      {
        out << "EMPS ";
        address_instruction ++;
        out << code[address_instruction] << " ";
        address_instruction ++;
        out << this->code_to_string(code[address_instruction-1], address_instruction);
        address_instruction += code[address_instruction-1];

        break;
      }

    case EMPL: 
      {
        out << "EMPL ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case EMPG:
      {
        out << "EMPG ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case DEPL:
      {
        out << "DEPL ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case INF:
      {
        out << "INF ";
        address_instruction ++;

        break;
      }

    case SUP:
      {
        out << "SUP ";
        address_instruction ++;

        break;
      }

    case INFEQUAL:
      {
        out << "INFEQUAL ";
        address_instruction ++;

        break;
      }

    case SUPEQUAL:
      {
        out << "SUPEQUAL ";
        address_instruction ++;

        break;
      }

    case ADD:
      {
        out << "ADD ";
        address_instruction ++;

        break;
      }

    case SUB:
      {
        out << "SUB ";
        address_instruction ++;

        break;
      }

    case MUL: 
      {
        out << "MUL ";
        address_instruction ++;

        break;
      }

    case DIV: 
      {
        out << "DIV ";
        address_instruction ++;

        break;
      }

    case END:
      {
        out << "END ";
        address_instruction ++;

        break;
      }

    case EXIT:
      {
        out << "EXIT ";
        address_instruction ++;

        break;
      }

    case INCLUDE:
      {
        out << "INCLUDE ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case SYSTEM:
      {
        out << "SYSTEM ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case PRINT:
      {
        out << "PRINT ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case READPOL:
      {
        out << "READPOL ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case FPUTS:
      {
        out << "FPUTS ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case CFG_DOT:
      {
        out << "CFG_DOT ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    case CFG_COMPUTE:
      {
        out << "CFG_COMPUTE ";
        address_instruction ++;

        break;
      }

    case AST_DOT:
      {
        out << "AST_DOT ";
        address_instruction ++;
        out << code[address_instruction];
        address_instruction ++;

        break;
      }

    default:
      {
        address_instruction ++; 

        break;
      }
  }

  return out.str();
}
/*
   std::ostream &operator<<(std::ostream &out, t_gencode &gencode)
   {
   unsigned int temp_co = 0;

   std::vector<int> &code = gencode.get_code();
   out << " : CODE : \033[1;35mcode here\033[0m" << std::endl << std::endl;

   while(temp_co < code.size())
   {
   out << temp_co << "    ";
   out << gencode.show_code_of_nextintruction(temp_co) << std::endl;

   }

   return out;
   }

 */

