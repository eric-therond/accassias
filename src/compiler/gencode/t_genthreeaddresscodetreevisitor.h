#ifndef T_GENTHREEADDRESSCODETREEVISITOR_H
#define T_GENTHREEADDRESSCODETREEVISITOR_H

#include "compiler/syntax/t_symbol.h"
#include "t_tacsymbol.h"
#include "data_structures/graph/t_node.h"
#include "data_structures/tree/t_treevisitor.h"
#include "t_threeaddress.h"
#include "t_threeaddresscode.h"
#include "compiler/syntax/t_function.h"
#include "compiler/t_compiler.h"
#include <stdlib.h>

template <typename N>
class t_genthreeaddresscodetreevisitor : public t_treevisitor<N>
{
  public:
    t_genthreeaddresscodetreevisitor()
    {
      this->threeaddresscode = new t_threeaddresscode;
    };

    void initialize()
    {
      this->threeaddresscode->get_code()->clear(); 
      flowin = 0;
    };

    void initialize_node(N *node){
      (void) node;
    };

    void examine_node(N *node){

      switch(node->getvertex()->getsymbol())
      {
        case S_SUBLIST_INSTRUCTIONS:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_SUBINSTRUCTIONS, node->getvertex()->getaddr(), node->getvertex()->getnblocalvariables(), 0));
            break;
          }

        case S_LIST_INSTRUCTIONS:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_INSTRUCTIONS, node->getvertex()->getaddr(), node->getvertex()->getnblocalvariables(), 0));
            break;
          }

        case S_PROG:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_PROG_JUMP, node->getvertex()->getaddr(), 0, 0));
            break;
          }

        case S_CLASS:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_CLASS, node->getvertex()->getaddr(), node->getvertex()->getnblocalvariables(), node->getvertex()->getlexeme())); 
            break; 
          }

        case S_CLASS_END:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_ENDCLASS, node->getid(), node->getid()));
            break;
          }

        case S_FUNCTION:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_FUNCTION, node->getvertex()->getaddr(), node->getvertex()->getnblocalvariables(), node->getvertex()->getlexeme())); 
            break; 
          }

        case S_NEW_INSTANCE_CLASS:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_INSTANCE_CLASS, node->getvertex()->getaddr(), node->getvertex()->getnblocalvariables(), node->getvertex()->getlexeme())); 
            break; 
          }

        case S_STARTFOR:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_STARTFOR, node->getvertex()->getaddr(), node->getid()));
            break;
          }

        case S_FOR:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_FOR, node->getvertex()->getaddr(), node->getid()));
            break;
          }

        case S_ENDFOR:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_ENDFOR, node->getid(), node->getid()));
            break;
          }

        case S_IF:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_IF, node->getvertex()->getaddr(), node->getid()));
            break;
          }

        case S_IFTRUE:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_IFTRUE, node->getvertex()->getaddr(), node->getid()));
            break;
          }

        case S_ELSEIF:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_ELSEIF, node->getvertex()->getaddr(), node->getid()));
            break;
          }

        case S_ELSE:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_ELSE, node->getid(), node->getid()));
            break;
          }

        case S_ENDIF:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_ENDIF, node->getid(), node->getid()));
            break;
          }

        case S_ENDELSEIF:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_ENDELSEIF, node->getid(), node->getid()));
            break;
          }

        case S_ENDELSE:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_ENDELSE, node->getid(), node->getid()));
            break;
          }

        case S_NOELSE:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_NOELSE, node->getid(), node->getid()));
            break;
          }

        case S_METHOD:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_METHOD, node->getid(), node->getid(), node->getvertex()->getlexeme()));
            break;
          }
        default:
          break;
      }
    };

    void examine_node_target(N *node, N *targetnode){
      (void) node;
      (void) targetnode;
    };

    void finish_node(N *node){

      switch(node->getvertex()->getsymbol())
      {		
        case S_START_CALL:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_STARTCALL, node->getvertex()->getaddr(), node->getvertex()->getnbargs(), node->getvertex()->getlexeme()));
            break;
          }

        case S_CALL:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_CALL, node->getvertex()->getaddr(), node->getvertex()->getnbargs(), node->getvertex()->getlexeme()));
            break;
          }

        case S_INCLUDE:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_INCLUDE, node->getid(), node->getid()));
            break; 
          }

        case S_PRINT:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_PRINT, node->getid(), node->getid()));
            break; 
          }

        case S_FPUTS:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_FPUTS, node->getid(), node->getid()));
            break; 
          }

        case S_AST_DOT:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_AST_DOT, node->getid(), node->getid()));
            break; 
          }

        case S_CFG_DOT:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_CFG_DOT, node->getid(), node->getid()));
            break; 
          }

        case S_CFG_COMPUTE:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_CFG_COMPUTE, node->getid(), node->getid()));
            break; 
          }

        case S_SYSTEM:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_SYSTEM, node->getid(), node->getid()));
            break; 
          }

        case S_EXIT:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_EXIT, node->getid(), node->getid()));
            break; 
          }

        case S_READPOL:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_READPOL, node->getid(), node->getid()));
            break; 
          }

        case S_RETURN_WITHOUT_VALUE:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_RETURN_WITHOUT_VALUE, node->getvertex()->getaddr(), node->getvertex()->getnbargs(), node->getvertex()->getlexeme())); 

            break; 
          }

        case S_RETURN:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_RETURN, node->getvertex()->getaddr(), node->getvertex()->getnbargs(), node->getvertex()->getlexeme())); 

            break; 
          }

        case S_PARAM:
          {
            this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_PARAM, node->getvertex()->getaddr(), node->getid())); 

            break; 
          }

        case S_DECLARATION:
          {
            if(node->getnbchildren() > 2 && node->getchild(3)->getvertex()->getsymbol() == S_ARRAY)
              this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_DECLR_ARRAY, node->getvertex()->getaddr(), node->getchild(3)->getvertex()->getaddr()));

            else
              this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_DECLR, node->getvertex()->getaddr(), 0));

            break;
          }

        case S_AFFECTATION:
          {
            if(node->getnbchildren() > 1 && node->getchild(1)->getvertex()->getsymbol() == S_ARRAY)
            {
              if(node->getnbchildren() > 1 && node->getchild(2)->getvertex()->getsymbol() == S_EQUAL)
                this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_ARRAY, node->getvertex()->getaddr(), node->getchild(1)->getvertex()->getaddr(), node->getchild(2)->getchild(0)->getvertex()->getaddr()));
            }

            if(node->getchild(0)->getvertex()->getsymbol() == S_EQUAL)
              this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGNV, node->getvertex()->getaddr(), node->getchild(0)->getchild(0)->getvertex()->getaddr()));

            break;
          }

        case S_FACTOR:
          {
            switch(node->getchild(0)->getvertex()->getsymbol())
            {
              case S_START_CALL:
                {
                  this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGNCA, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getnbargs(), node->getchild(0)->getvertex()->getlexeme()));

                  break; 
                }

              case  S_VARIABLE_GLOBAL:
                {
                  if(node->getnbchildren() > 1 && node->getchild(2)->getvertex()->getsymbol() == S_ARRAY)
                    this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_FACTOR_ARRAY, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getaddr(), node->getchild(2)->getvertex()->getaddr()));

                  else
                    this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGNV, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getaddr()));

                  break;
                }


              case  S_VARIABLE_LOCAL:
                {
                  if(node->getnbchildren() > 1 && node->getchild(2)->getvertex()->getsymbol() == S_ARRAY)
                    this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_FACTOR_ARRAY, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getaddr(), node->getchild(2)->getvertex()->getaddr()));

                  else
                    this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGNV, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getaddr()));

                  break;
                }
              case S_NUMBER:
                {
                  this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGNC, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getlexeme()));
                  break;
                }

              case S_STRING:
                {
                  this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGNS, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getlexeme()));
                  break;
                }

              default:
                break;
            }
            break;
          }

        case S_EXPRESSION:
          {
            if(node->getnbchildren() > 1)
            {
              t_tacsymbol tacsymbol;
              switch(node->getchild(1)->getvertex()->getsymbol())
              {
                case S_ADD: tacsymbol = TAC_ASSIGN_ADD; break;
                case S_SUB: tacsymbol = TAC_ASSIGN_SUB; break;
                case S_INF: tacsymbol = TAC_ASSIGN_INF; break;
                case S_INFEQUAL: tacsymbol = TAC_ASSIGN_INFEQUAL; break;
                case S_SUP: tacsymbol = TAC_ASSIGN_SUP; break;
                case S_SUPEQUAL: tacsymbol = TAC_ASSIGN_SUPEQUAL; break;
                default: break;
              }

              this->threeaddresscode->get_code()->push_back(t_threeaddress(tacsymbol, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getaddr(), node->getchild(2)->getvertex()->getaddr())); 
            }
            else
              this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGNV, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getaddr())); 

            break;
          }

        case S_TERM:
          { 
            if(node->getnbchildren() > 1)
            {
              t_tacsymbol tacsymbol;
              switch(node->getchild(1)->getvertex()->getsymbol())
              {
                case S_DIV: tacsymbol = TAC_ASSIGN_DIV;
                case S_MUL: tacsymbol = TAC_ASSIGN_MUL;
                default: break;
              }

              this->threeaddresscode->get_code()->push_back(t_threeaddress(tacsymbol, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getaddr(), node->getchild(2)->getvertex()->getaddr())); 

            }
            else
              this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGNV, node->getvertex()->getaddr(), node->getchild(0)->getvertex()->getaddr())); 

            break;
          }

        default:
          //this->threeaddresscode->get_code()->push_back(t_threeaddress(TAC_ASSIGN_NULL, node->getvertex()->getaddr(), 0, 0));

          break;
      }
    };
    void finish(){
    };

    t_threeaddresscode *get_tac()
    {
      return threeaddresscode;
    };

  private:
    t_threeaddresscode *threeaddresscode;
    unsigned int flowin;
};

typedef t_genthreeaddresscodetreevisitor<t_node<t_symbol> > t_genthreeaddresscode;

#endif


