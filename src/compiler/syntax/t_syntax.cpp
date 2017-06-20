#include "t_syntax.h"
#include "data_structures/graph/t_node.h"
#include "t_function.h"
#include "t_variable.h"
#include "compiler/lexical/t_token.h"
#include "t_symbol.h"
#include "data_structures/tree/t_dottreevisitor.h"
#include "data_structures/graph/t_breadth_first_search.h"
#include "data_structures/graph/t_depth_first_search.h"
#include <iostream>
#include <sstream>
#include <string>

t_syntax::t_syntax()
{
  this->nb_global_functions = 0;
  this->nb_global_class = 0;
  this->nbexprstermsfactors_prog = 0;
  this->nbexprstermsfactors_class = 0;
  this->backnode_id = 0;
  this->backnode_functions_id = 0;
  this->backnode_classes_id = 0;
  this->last_nb_nodes = 0;
  this->lexical = new t_lexical();

  this->global_variables = new std::map<std::string, t_identifier *>;
  this->global_functions = new std::map<std::string, t_function *>;
  this->global_class = new std::map<std::string, t_class *>;
  this->global_instances_class = new std::map<std::string, t_class *>;

  this->syntaxtree_prog = new t_tree<t_symbol>;

  t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_PROG);
  this->prognode = new t_node<t_symbol>(L_PROG, symbol);
  this->syntaxtree_prog->addnode(prognode);

  t_symbol *symbol_instructions = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_LIST_INSTRUCTIONS);
  this->instructionsnode = prognode->addchild(L_LIST_INSTRUCTIONS, symbol_instructions);
  this->syntaxtree_prog->addnode(instructionsnode);

  t_symbol *symbol_exit = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_EXIT);
  t_node<t_symbol> *exit_node = prognode->addchild(L_EXIT, symbol_exit);
  this->syntaxtree_prog->addnode(exit_node);

  t_symbol *symbol_classes = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_LIST_CLASSES);
  this->classesnode = prognode->addchild(L_LIST_CLASSES, symbol_classes);
  this->syntaxtree_prog->addnode(classesnode);

  t_symbol *functions_symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_LIST_FUNCTIONS);
  this->functions =  prognode->addchild(L_LIST_FUNCTIONS, functions_symbol);
  this->syntaxtree_prog->addnode(functions);

  this->syntaxtree_prog->setroot(prognode);
}

t_lexical *t_syntax::get_lexical()
{
  return lexical;
}

std::map<std::string, t_function *> *t_syntax::get_functions()
{
  return this->global_functions;
}

std::map<std::string, t_class *> *t_syntax::get_classes()
{
  return this->global_class;
}

bool t_syntax::getexistsyntaxerror()
{
  return existsyntaxerror;
}

bool t_syntax::start()
{
  if(lexical->get_source() == FROM_STDIN)
  {
    lexical->read_next_line();
    if(lexical->get_current_token() == T_TAB)
      return true;
  }

  lexical->next_token();
  this->existsyntaxerror = false;  

  this->nbexprstermsfactors_prog = 0;

  t_symbol *symbol_subinstructions = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_SUBLIST_INSTRUCTIONS);
  t_node<t_symbol> *subinstructionsnode = this->instructionsnode->addchild(L_SUBLIST_INSTRUCTIONS, symbol_subinstructions);
  subinstructionsnode->getvertex()->setnblocalvariables(0);

  this->instructions(subinstructionsnode, this->functions, this->classesnode);

  if(this->last_nb_nodes == this->gettree()->getnodes().size())
    return false;

  this->syntaxtree_prog->addnode(subinstructionsnode);

  this->last_nb_nodes = this->gettree()->getnodes().size();

  return true;
}

void t_syntax::syntaxerror(const std::string &debugstring, const unsigned int debugline, const std::string &errorstring)
{
  std::cout << "      Erreur de syntaxe" << std::endl;
  std::cout << "      Line = " << lexical->get_current_line() << ", column = " << lexical->get_current_column() << std::endl;
  std::cout << "      Debug : " << debugstring << ", line " << debugline << ", lexeme : " << lexical->get_current_lexeme() << ", token : " << lexical->get_current_token() << std::endl; 
  std::cout << "      " << errorstring << "\n" << std::endl;
  this->existsyntaxerror = true;
}

t_syntaxtree *t_syntax::gettree()
{
  return this->syntaxtree_prog;
}

unsigned int t_syntax::getnblocalvariables()
{
  return this->syntaxtree_prog->getnodes()[1]->getvertex()->getnblocalvariables();
}

void t_syntax::instructions(t_node<t_symbol> *syntaxnode, t_node<t_symbol> *syntaxnode_functions, t_node<t_symbol> *syntaxnode_classes)
{
  this->insidefunction = false;
  //this->global_variables = this->chained_variables[0];

  while(this->lexical->get_current_token() != T_END)
  {
    this->for_instruction(this->gettree(), syntaxnode);
    this->if_instruction(this->gettree(), syntaxnode);

    this->system_instruction(this->gettree(), syntaxnode);
    this->readpol_instruction(this->gettree(), syntaxnode);

    this->include_instruction(this->gettree(), syntaxnode);
    this->print_instruction(this->gettree(), syntaxnode);
    this->fputs_instruction(this->gettree(), syntaxnode);
    this->cfg_dot_instruction(this->gettree(), syntaxnode);
    this->cfg_compute_instruction(this->gettree(), syntaxnode);
    this->ast_dot_instruction(this->gettree(), syntaxnode);

    this->return_instruction(this->gettree(), syntaxnode);
    this->call_instruction(this->gettree(), syntaxnode, false, this->global_functions);
    this->variable(this->gettree(), syntaxnode, false, false);
    this->variable_declaration(this->gettree(), syntaxnode, false);
    this->function_declaration(this->gettree(), syntaxnode_functions);
    this->class_declaration(this->gettree(), syntaxnode_classes);

    this->lexical->next_token();
  }

  // syntaxnode = L_SUBLIST_INSTRUCTIONS
  syntaxnode->getvertex()->setnblocalvariables(
      syntaxnode->getvertex()->getnblocalvariables() + this->nbexprstermsfactors_prog);
}


void t_syntax::bloc_classe(t_node<t_symbol> *syntaxnode, t_node<t_symbol> *syntaxnode_functions)
{	
  this->lexical->next_token();
  if(this->lexical->get_current_token() != T_LEFTBRACE)
    return syntaxerror(DEBUG_BLOC, 165, ERROR_BLOC_LEFT_BRACE);

  this->nbexprstermsfactors_class = 0;

  while(this->lexical->get_current_token() != T_END && this->lexical->get_current_token() != T_RIGHTBRACE)
  {
    this->function_declaration(this->gettree(), syntaxnode_functions);
    this->variable_declaration(this->gettree(), syntaxnode, false);
    this->variable(this->gettree(), syntaxnode, false, false);
    this->lexical->next_token();
  }

  this->nbexprstermsfactors_prog = this->nbexprstermsfactors_prog - this->nbexprstermsfactors_class;   

  if(this->lexical->get_current_token() != T_RIGHTBRACE)
    return syntaxerror(DEBUG_BLOC, 180, ERROR_BLOC_RIGHT_BRACE); 
}


void t_syntax::bloc_instructions(t_node<t_symbol> *syntaxnode)
{	
  this->lexical->next_token();
  if(this->lexical->get_current_token() != T_LEFTBRACE)
    return syntaxerror(DEBUG_BLOC, 188, ERROR_BLOC_LEFT_BRACE);

  this->nbexprstermsfactors = 0;

  while(this->lexical->get_current_token() != T_END && this->lexical->get_current_token() != T_RIGHTBRACE)
  {
    this->for_instruction(this->gettree(), syntaxnode);
    this->if_instruction(this->gettree(), syntaxnode);

    this->system_instruction(this->gettree(), syntaxnode); 
    this->readpol_instruction(this->gettree(), syntaxnode);

    this->include_instruction(this->gettree(), syntaxnode);
    this->print_instruction(this->gettree(), syntaxnode);
    this->fputs_instruction(this->gettree(), syntaxnode);
    this->cfg_dot_instruction(this->gettree(), syntaxnode);
    this->cfg_compute_instruction(this->gettree(), syntaxnode);
    this->ast_dot_instruction(this->gettree(), syntaxnode);
    this->return_instruction(this->gettree(), syntaxnode);
    this->call_instruction(this->gettree(), syntaxnode, false, this->global_functions);
    this->variable(this->gettree(), syntaxnode, false, false);
    this->variable_declaration(this->gettree(), syntaxnode, false);
    this->lexical->next_token();
  }

  this->nbexprstermsfactors_prog = this->nbexprstermsfactors_prog - this->nbexprstermsfactors;   

  if(this->lexical->get_current_token() != T_RIGHTBRACE)
    return syntaxerror(DEBUG_BLOC, 215, ERROR_BLOC_RIGHT_BRACE); 
}

void t_syntax::for_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  unsigned int id_var;

  if(this->lexical->get_current_token() == T_FOR)
  { 
    if(this->lexical->next_token() != T_LEFTPARENTHESIS)
      return syntaxerror(DEBUG_FOR, 225, ERROR_FOR_LEFT_PARENTHESIS);

    this->lexical->next_token();
    this->variable(tree, syntaxnode, false, false);

    if(this->lexical->get_current_token() != T_DOTCOMMA)
      return syntaxerror(DEBUG_FOR, 231, ERROR_FOR_FIRST_DOTCOMMA);

    t_identifier *startfor_identifier = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_STARTFOR);
    t_node<t_symbol> *startfor_node = syntaxnode->addchild(L_STARTFOR, startfor_identifier);
    tree->addnode(startfor_node);

    this->lexical->next_token();
    id_var = this->expression(tree, syntaxnode);

    t_identifier *for_identifier = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_FOR, id_var);
    t_node<t_symbol> *for_node = syntaxnode->addchild(L_FOR, for_identifier);
    tree->addnode(for_node);

    if(this->lexical->get_current_token() != T_DOTCOMMA)
      return syntaxerror(DEBUG_FOR, 245, ERROR_FOR_SECOND_DOTCOMMA);

    this->lexical->next_token();
    this->variable(tree, syntaxnode, true, false);

    if(this->lexical->get_current_token() != T_RIGHTPARENTHESIS)
      return syntaxerror(DEBUG_FOR, 251, ERROR_FOR_RIGHT_PARENTHESIS);

    this->bloc_instructions(for_node);

    t_identifier *endfor_identifier = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_ENDFOR);
    t_node<t_symbol> *endfor_node = syntaxnode->addchild(L_ENDFOR, endfor_identifier);
    tree->addnode(endfor_node);
  }
}

void t_syntax::if_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  unsigned int id_var;

  if(this->lexical->get_current_token() == T_IF)
  { 
    if(this->lexical->next_token() != T_LEFTPARENTHESIS)
      return syntaxerror(DEBUG_IF, 268, ERROR_IF_LEFT_PARENTHESIS);

    this->lexical->next_token();
    id_var = this->expression(tree, syntaxnode);

    if(this->lexical->get_current_token() != T_RIGHTPARENTHESIS)
      return syntaxerror(DEBUG_IF, 274, ERROR_IF_RIGHT_PARENTHESIS);

    t_identifier *if_identifier = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_IF, id_var);
    t_node<t_symbol> *if_node = syntaxnode->addchild(L_IF, if_identifier);
    tree->addnode(if_node);

    t_identifier *if_identifiertrue = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_IFTRUE, id_var);
    t_node<t_symbol> *iftrue_node = if_node->addchild(L_IFTRUE, if_identifiertrue);
    tree->addnode(iftrue_node);

    this->bloc_instructions(iftrue_node);
    this->lexical->next_token();

    t_identifier *endif_identifier = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_ENDIF);
    t_node<t_symbol> *endif_node = if_node->addchild(L_ENDIF, endif_identifier);
    if_identifier->addflow(endif_node->getid());
    tree->addnode(endif_node);

    while(this->lexical->get_current_token() == T_ELSEIF)
    {
      if(this->lexical->next_token() != T_LEFTPARENTHESIS)
        return syntaxerror(DEBUG_IF, 295, ERROR_IF_LEFT_PARENTHESIS);

      this->lexical->next_token();
      id_var = this->expression(tree, syntaxnode);

      if(this->lexical->get_current_token() != T_RIGHTPARENTHESIS)
        return syntaxerror(DEBUG_IF, 301, ERROR_IF_RIGHT_PARENTHESIS);

      t_identifier *elseif_identifier = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_ELSEIF, id_var);
      t_node<t_symbol> *elseif_node = endif_node->addchild(L_ELSEIF, elseif_identifier);
      tree->addnode(elseif_node);

      this->bloc_instructions(elseif_node);
      this->lexical->next_token();

      t_identifier *endelseif_identifier = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_ENDELSEIF);
      t_node<t_symbol> *endelseif_node = endif_node->addchild(L_ENDELSEIF, endelseif_identifier);
      if_identifier->addflow(endelseif_node->getid());
      tree->addnode(endelseif_node);
    }

    if(this->lexical->get_current_token() == T_ELSE)
    {
      t_identifier *else_identifier = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_ELSE);
      t_node<t_symbol> *else_node = endif_node->addchild(L_ELSE, else_identifier);
      tree->addnode(else_node);

      this->bloc_instructions(else_node);

      t_identifier *endelse_identifier = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_ENDELSE);
      t_node<t_symbol> *endelse_node = else_node->addchild(L_ENDELSE, endelse_identifier);
      if_identifier->addflow(endelse_node->getid());
      tree->addnode(endelse_node);
    }
    else
    {
      t_identifier *noelse_identifier = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_NOELSE);
      t_node<t_symbol> *noelse_node = endif_node->addchild(L_NOELSE, noelse_identifier);
      if_identifier->addflow(noelse_node->getid());
      tree->addnode(noelse_node);
    }

  }
}

void t_syntax::include_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  if(this->lexical->get_current_token() == T_INCLUDE)
  {  
    t_symbol *symbol = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_INCLUDE);
    t_node<t_symbol> *node_id = syntaxnode->addchild(L_INCLUDE, symbol);
    tree->addnode(node_id);
  }
}

void t_syntax::system_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  if(this->lexical->get_current_token() == T_SYSTEM)
  {  
    t_symbol *symbol = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_SYSTEM);
    t_node<t_symbol> *node_id = syntaxnode->addchild(L_SYSTEM, symbol);
    tree->addnode(node_id);
  }
}

void t_syntax::readpol_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  if(this->lexical->get_current_token() == T_READPOL)
  {   
    t_symbol *symbol = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_READPOL);
    t_node<t_symbol> *node_id = syntaxnode->addchild(L_READPOL, symbol);
    tree->addnode(node_id);
  }
}

void t_syntax::print_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  if(this->lexical->get_current_token() == T_PRINT)
  {  
    t_symbol *symbol = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_PRINT);
    t_node<t_symbol> *node_id = syntaxnode->addchild(L_PRINT, symbol);
    tree->addnode(node_id);
  }
}

void t_syntax::fputs_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  if(this->lexical->get_current_token() == T_FPUTS)
  {  
    t_symbol *symbol = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_FPUTS);
    t_node<t_symbol> *node_id = syntaxnode->addchild(L_FPUTS, symbol);
    tree->addnode(node_id);
  }
}

void t_syntax::cfg_dot_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  if(this->lexical->get_current_token() == T_CFG_DOT)
  {  
    t_symbol *symbol = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_CFG_DOT);
    t_node<t_symbol> *node_id = syntaxnode->addchild(L_CFG_DOT, symbol);
    tree->addnode(node_id);
  }
}

void t_syntax::cfg_compute_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  if(this->lexical->get_current_token() == T_CFG_COMPUTE)
  {  
    t_symbol *symbol = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_CFG_COMPUTE);
    t_node<t_symbol> *node_id = syntaxnode->addchild(L_CFG_COMPUTE, symbol);
    tree->addnode(node_id);
  }
}

void t_syntax::ast_dot_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  if(this->lexical->get_current_token() == T_AST_DOT)
  {  
    t_symbol *symbol = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_AST_DOT);
    t_node<t_symbol> *node_id = syntaxnode->addchild(L_AST_DOT, symbol);
    tree->addnode(node_id);
  }
}

void t_syntax::class_declaration(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{	
  if(this->lexical->get_current_token() == T_CLASS)
  {
    if(lexical->next_token() != T_IDENTIFIER)
      return syntaxerror(DEBUG_CLASS, 417, ERROR_CLASS_IDENTIFIER);

    if(this->global_class->find(lexical->get_current_lexeme()) != this->global_class->end())
      return syntaxerror(DEBUG_CLASS, 420, ERROR_CLASS_IDENTIFIER_EXIST);

    t_class *classe = new t_class(lexical->get_current_line(), lexical->get_current_column(), S_CLASS, lexical->get_current_lexeme(), this->nb_global_class);
    this->global_class->insert(std::pair<std::string, t_class *>(lexical->get_current_lexeme(), classe));
    this->current_classe = classe;

    this->nb_global_class ++;
    t_node<t_symbol> *node = syntaxnode->addchild(L_CLASS, classe);
    tree->addnode(node);

    this->insideclass = true;

    this->temp_variables = this->current_classe->getvariables();
    this->bloc_classe(node, node);
    this->temp_variables = this->global_variables;

    this->insideclass = false;

    classe->setnblocalvariables(this->nbexprstermsfactors_class);

    t_symbol *symbol1 = new t_symbol(this->lexical->get_current_line(), this->lexical->get_current_column(), S_CLASS_END);
    t_node<t_symbol> *node1 = syntaxnode->addchild(L_CLASS_END, symbol1);
    tree->addnode(node1);
  }
}

void t_syntax::function_declaration(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{	
  if(this->lexical->get_current_token() == T_FUNCTION)
  {
    this->insidefunction = true;

    if(lexical->next_token() != T_IDENTIFIER)
      return syntaxerror(DEBUG_FUNCTION, 456, ERROR_FUNCTION_IDENTIFIER);

    if(global_functions->find(lexical->get_current_lexeme()) != global_functions->end())
      return syntaxerror(DEBUG_FUNCTION, 459, ERROR_FUNCTION_IDENTIFIER_EXIST);

    if(global_class->find(lexical->get_current_lexeme()) != global_class->end())
      return syntaxerror(DEBUG_FUNCTION, 462, ERROR_FUNCTION_IDENTIFIER_EXIST);

    t_function *function = new t_function(lexical->get_current_line(), lexical->get_current_column(), S_FUNCTION, lexical->get_current_lexeme(), this->nb_global_functions);
    if(this->insideclass)
    {
      if(this->current_classe->getmethods()->find(lexical->get_current_lexeme()) != this->current_classe->getmethods()->end())
        return syntaxerror(DEBUG_FUNCTION, 468, ERROR_FUNCTION_IDENTIFIER_EXIST);

      this->current_classe->getmethods()->insert(std::pair<std::string, t_function *>(lexical->get_current_lexeme(), function));

    }
    else
    {
      this->global_functions->insert(std::pair<std::string, t_function *>(lexical->get_current_lexeme(), function));
    }

    this->current_function = function;
    this->nb_global_functions ++;
    t_node<t_symbol> *node = syntaxnode->addchild(L_FUNCTION, function);
    tree->addnode(node);

    this->temp_variables = this->current_function->getvariables();

    if(this->lexical->next_token() != T_LEFTPARENTHESIS)
      return syntaxerror(DEBUG_FUNCTION, 488, ERROR_FUNCTION_LEFT_PARENTHESIS);

    while(this->lexical->get_current_token() != T_END && this->lexical->get_current_token() != T_RIGHTPARENTHESIS)
    {
      this->lexical->next_token();
      if(lexical->get_current_token() == T_DECLR)
      {
        this->variable_declaration(tree, node, true);
        function->setnbargs(function->getnbargs() + 1);
      }
    }

    std::map<std::string, t_identifier *>::iterator it;
    for(it = this->temp_variables->begin(); it != this->temp_variables->end(); it++)
      ((*it).second)->setaddr((((*it).second)->getaddr()*3) - ((function->getnbargs()*3) + 1));

    this->existreturn = false;

    this->bloc_instructions(node);


    function->setnblocalvariables(this->nbexprstermsfactors);

    if(!existreturn)
    { 
      t_function *function1 = new t_function(lexical->get_current_line(), lexical->get_current_column(), S_RETURN_WITHOUT_VALUE, function->getlexeme(), function->getaddr(), function->getnbargs(), function->getnblocalvariables());
      t_node<t_symbol> *node1 = node->addchild(L_RETURN_WITHOUT_VALUE, function1);
      tree->addnode(node1);
    }

    this->insidefunction = false;
    this->temp_variables = this->global_variables;
  }
}

void t_syntax::return_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{	  
  if(lexical->get_current_token() == T_RETURN)
  {
    this->lexical->next_token();
    unsigned int id_variable = this->expression(tree, syntaxnode);

    t_function *function1 = new t_function(this->lexical->get_current_line(), this->lexical->get_current_column(), S_RETURN, current_function->getlexeme(), id_variable, current_function->getnbargs(), current_function->getnblocalvariables());
    t_node<t_symbol> *node = syntaxnode->addchild(L_RETURN, function1);
    tree->addnode(node);

    if(this->lexical->get_current_token() != T_DOTCOMMA) 
      return syntaxerror(DEBUG_RETURN, 535, ERROR_RETURN_DOTCOMMA);

    t_symbol *symbol1 = new t_symbol(this->lexical->get_current_line(), this->lexical->get_current_column(), S_DOTCOMMA);
    t_node<t_symbol> *node1 = syntaxnode->addchild(L_DOTCOMMA, symbol1);
    tree->addnode(node1);

    this->existreturn = true;	 
  }
}

void t_syntax::call_instruction(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode, bool isexpression, std::map<std::string, t_function *> *functions_temp)
{
  if(this->lexical->get_current_token() == T_IDENTIFIER)
  {
    if(functions_temp->find(lexical->get_current_lexeme()) == functions_temp->end())
      return syntaxerror(DEBUG_CALL, 550, ERROR_CALL_IDENTIFIER_DOESNT_EXIST);

    t_function *function_found = (*functions_temp)[lexical->get_current_lexeme()];

    if(this->lexical->next_token() != T_LEFTPARENTHESIS)
      return syntaxerror(DEBUG_CALL, 555, ERROR_CALL_LEFT_PARENTHESIS);

    t_function *call_start = new t_function(this->lexical->get_current_line(), this->lexical->get_current_column(), S_START_CALL, function_found->getlexeme(), function_found->getaddr(), function_found->getnbargs());
    t_node<t_symbol> *node1 = syntaxnode->addchild(L_START_CALL, call_start);
    tree->addnode(node1);

    t_identifier *param;
    t_node<t_symbol> *node_param;
    unsigned int i;
    unsigned int id_param;
    for(i = 0; i < function_found->getnbargs(); i ++)
    {
      this->lexical->next_token();

      id_param = this->expression(tree, syntaxnode);

      param = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_PARAM, id_param);
      node_param = syntaxnode->addchild(L_PARAM, param);
      tree->addnode(node_param); 

      if(i < (function_found->getnbargs() - 1) && this->lexical->get_current_token() != T_COMMA)
        return syntaxerror(DEBUG_CALL, 576, ERROR_CALL_ARG);
    }

    if((function_found->getnbargs() && this->lexical->get_current_token() != T_RIGHTPARENTHESIS) || 
        (!function_found->getnbargs() && this->lexical->next_token() != T_RIGHTPARENTHESIS))
      return syntaxerror(DEBUG_CALL, 581, ERROR_CALL_RIGHT_PARENTHESIS);

    if(isexpression && this->lexical->next_token() != T_DOTCOMMA)
      return syntaxerror(DEBUG_CALL, 584, ERROR_CALL_DOTCOMMA);

    if(!isexpression)
    {
      t_function *call = new t_function(this->lexical->get_current_line(), this->lexical->get_current_column(), S_CALL, function_found->getlexeme(), function_found->getaddr(), function_found->getnbargs());
      t_node<t_symbol> *node2 = syntaxnode->addchild(L_CALL, call);
      tree->addnode(node2);
    }
  }
}

void t_syntax::variable_declaration(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode, bool paramsfunctions)
{
  unsigned int id_var;

  if(lexical->get_current_token() == T_DECLR)
  {
    this->nbexprstermsfactors ++;
    this->nbexprstermsfactors_prog ++;
    this->nbexprstermsfactors_class ++;

    if(lexical->next_token() != T_VARIABLE)
      return syntaxerror(DEBUG_VARDEC, 606, ERROR_VARDEC_DOLLAR);

    if(lexical->next_token() != T_IDENTIFIER)
      return syntaxerror(DEBUG_VARDEC, 609, ERROR_VARDEC_IDENTIFIER);


    t_identifier *identifier;

    if(this->insideclass && !this->insidefunction)
    {
      identifier = new t_identifier(lexical->get_current_line(), lexical->get_current_column(), S_DECLARATION, this->current_classe->getvariables()->size());
      identifier->setglobal();

      if(this->current_classe->getvariables()->find(lexical->get_current_lexeme()) != this->current_classe->getvariables()->end())
        return syntaxerror(DEBUG_FUNCTION, 619, ERROR_FUNCTION_IDENTIFIER_EXIST);

      this->current_classe->getvariables()->insert(std::pair<std::string, t_identifier *>(lexical->get_current_lexeme(), identifier));
    }
    else if(this->insidefunction)
    {
      identifier = new t_identifier(lexical->get_current_line(), lexical->get_current_column(), S_DECLARATION, this->current_function->getvariables()->size());
      identifier->setlocal();

      if(this->current_function->getvariables()->find(lexical->get_current_lexeme()) != this->current_function->getvariables()->end())
        return syntaxerror(DEBUG_FUNCTION, 619, ERROR_FUNCTION_IDENTIFIER_EXIST);

      this->current_function->getvariables()->insert(std::pair<std::string, t_identifier *>(lexical->get_current_lexeme(), identifier));
    }
    else
    {
      identifier = new t_identifier(lexical->get_current_line(), lexical->get_current_column(), S_DECLARATION, this->global_variables->size());
      identifier->setglobal();

      if(this->global_variables->find(lexical->get_current_lexeme()) != this->global_variables->end())
        return syntaxerror(DEBUG_VARDEC, 628, ERROR_VARDEC_IDENTIFIER_EXIST);

      this->global_variables->insert(std::pair<std::string, t_identifier *>(lexical->get_current_lexeme(), identifier));
    }

    t_node<t_symbol> *node = syntaxnode->addchild(L_DECLARATION, identifier);
    tree->addnode(node);

    t_symbol *symbol = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_IDENTIFIER);
    t_node<t_symbol> *node_id = node->addchild(L_IDENTIFIER, symbol);
    tree->addnode(node_id);

    if(lexical->next_token() == T_LEFTBRACKET)
    {
      this->lexical->next_token();
      id_var = this->expression(tree, node);
      if(lexical->get_current_token() != T_RIGHTBRACKET)
        return syntaxerror(DEBUG_VARDEC, 645, ERROR_VARDEC_RIGHTBRACKET);

      t_symbol *symbol_array = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_ARRAY, id_var);
      t_node<t_symbol> *node_array = node->addchild(L_ARRAY, symbol_array);
      tree->addnode(node_array);
      this->lexical->next_token();
    }

    this->variable_end(tree, syntaxnode, paramsfunctions);
  }
}

void t_syntax::variable_affectation(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode, t_identifier *identifier_found, bool paramsfunctions)
{
  if(this->lexical->get_current_token() == T_EQUAL)
  {
    t_identifier *identifier = new t_identifier(lexical->get_current_line(), lexical->get_current_column(), S_AFFECTATION, identifier_found->getaddr());
    t_node<t_symbol> *node = syntaxnode->addchild(L_AFFECTATION, identifier);

    t_symbol *symbol = new t_symbol(this->lexical->get_current_line(), this->lexical->get_current_column(), S_EQUAL);
    t_node<t_symbol> *node_equal = node->addchild(L_EQUAL, symbol);
    tree->addnode(node_equal);

    this->lexical->next_token();

    if(this->lexical->get_current_token() == T_NEW)
    {	
      this->classe_instance(tree, node_equal, identifier_found);
    }
    else
    {
      this->expression(tree, node_equal);
    }

    this->variable_end(tree, syntaxnode, paramsfunctions);

  }
} 

void t_syntax::variable_end(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode, bool paramsfunctions)
{
  t_node<t_symbol> *node_dc;

  if(paramsfunctions)
  {
    if(this->lexical->get_current_token() != T_COMMA && this->lexical->get_current_token() != T_RIGHTPARENTHESIS) 
      return syntaxerror(DEBUG_VAREND, 691, ERROR_VAREND_RIGHT_PARENTHESIS);

    t_symbol *symbol = new t_symbol(this->lexical->get_current_line(), this->lexical->get_current_column(), S_COMMA);
    node_dc = syntaxnode->addchild(L_COMMA, symbol);
    tree->addnode(node_dc);
  }
  else
  {
    if(this->lexical->get_current_token() != T_DOTCOMMA) 
      return syntaxerror(DEBUG_VAREND, 700, ERROR_VAREND_DOTCOMMA);

    t_symbol *symbol = new t_symbol(this->lexical->get_current_line(), this->lexical->get_current_column(), S_DOTCOMMA);
    node_dc = syntaxnode->addchild(L_DOTCOMMA, symbol);
    tree->addnode(node_dc);
  }
}

void t_syntax::variable(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode, bool paramsfunctions, bool isexpression)
{
  if(lexical->get_current_token() == T_VARIABLE)
  {
    bool isclass = false;
    bool isfunction = false;
    std::map<std::string, t_identifier *> *where_variables = this->global_variables;
    std::map<std::string, t_function *> *where_functions;
    t_identifier *identifier_class_found;
    s_symbol symbol_variable = S_VARIABLE_GLOBAL;

    if(lexical->next_token() == T_IDENTIFIER)
    {
      // variables globales
      if(this->global_variables->find(lexical->get_current_lexeme()) != this->global_variables->end())
      {
        identifier_class_found = (*this->global_variables)[lexical->get_current_lexeme()];
        if(identifier_class_found->isclass())
        {
          isclass = true;
          where_variables = identifier_class_found->getclass()->getvariables();
          where_functions = identifier_class_found->getclass()->getmethods();
        }
      }
      // variables d'une fonction
      else if(this->global_variables->find(lexical->get_current_lexeme()) != this->global_variables->end())
      {
        isfunction = true;
        symbol_variable = S_VARIABLE_LOCAL;
        where_variables = this->current_function->getvariables();
      }
      else
        return syntaxerror(DEBUG_VAR, 730, ERROR_VAR_IDENTIFIER);
    }
    // variables d'une classe
    else if(lexical->get_current_token() == T_THIS)
    {
      isclass = true;
      where_variables = this->current_classe->getvariables();
      where_functions = this->current_classe->getmethods();
    }
    else
      return syntaxerror(DEBUG_VAR, 739, ERROR_VAR_IDENTIFIER);

    if(isclass)
    {
      if(this->lexical->next_token() != T_SUB)
        return syntaxerror(DEBUG_CALL, 744, ERROR_CALL_LEFT_PARENTHESIS);

      if(this->lexical->next_token() != T_SUP)
        return syntaxerror(DEBUG_CALL, 747, ERROR_CALL_LEFT_PARENTHESIS);

      lexical->next_token();
    }

    if(where_variables->find(lexical->get_current_lexeme()) != where_variables->end())
    {
      identifier_class_found = (*where_variables)[lexical->get_current_lexeme()];
      t_identifier *identifier = new t_identifier(lexical->get_current_line(), lexical->get_current_column(), symbol_variable, identifier_class_found->getaddr());
      syntaxnode->addchild(L_VARIABLE_GLOBAL, identifier);

      unsigned int id_var;

      if(lexical->next_token() == T_LEFTBRACKET)
      {
        this->lexical->next_token();
        id_var = this->expression(tree, syntaxnode);
        if(lexical->get_current_token() != T_RIGHTBRACKET)
          return syntaxerror(DEBUG_VARDEC, 765, ERROR_VARDEC_RIGHTBRACKET);

        t_symbol *symbol_array = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_ARRAY, id_var);
        t_node<t_symbol> *node_array = syntaxnode->addchild(L_ARRAY, symbol_array);
        tree->addnode(node_array);
      }

      variable_affectation(tree, syntaxnode, identifier_class_found, paramsfunctions);
    }
    else
    {
      t_identifier *method = new t_identifier(this->lexical->get_current_line(), this->lexical->get_current_column(), S_METHOD, identifier_class_found->getclass()->getlexeme());
      t_node<t_symbol> *method_node = syntaxnode->addchild(L_METHOD, method);
      tree->addnode(method_node);

      this->call_instruction(tree, method_node, isexpression, where_functions);
    }
  }
}

void t_syntax::classe_instance(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode, t_identifier *identifier)
{
  if(this->lexical->next_token() == T_IDENTIFIER)
  {
    if(this->global_class->find(lexical->get_current_lexeme()) == this->global_class->end())
      return syntaxerror(DEBUG_CLASS_INSTANCE, 790, ERROR_CLASS_IDENTIFIER_DOESNT_EXIST);

    t_class *classe_found = (*this->global_class)[lexical->get_current_lexeme()];

    identifier->setclass(classe_found);

    t_class *symbol_class = new t_class(this->lexical->get_current_line(), this->lexical->get_current_column(), S_NEW_INSTANCE_CLASS, classe_found->getlexeme(), classe_found->getaddr(), 0, classe_found->getvariables()->size());

    t_node<t_symbol> *node_class = syntaxnode->addchild(L_NEW_INSTANCE_CLASS, symbol_class);
    tree->addnode(node_class);


    t_identifier *var_class;
    std::map<std::string, t_identifier *>::iterator it;
    std::map<std::string, t_identifier *> variables_class = *(classe_found->getvariables());


    for(it = variables_class.begin(); it != variables_class.end(); it++)
    {
      var_class = it->second;
      var_class->setaddr(var_class->getaddr() + global_variables->size());

      t_identifier *identifier = new t_identifier(lexical->get_current_line(), lexical->get_current_column(), S_DECLARATION, var_class->getaddr());
      t_node<t_symbol> *node = node_class->addchild(L_DECLARATION, identifier);
      tree->addnode(node);
    }


    this->nbexprstermsfactors_prog = this->nbexprstermsfactors_prog + classe_found->getnblocalvariables();

    this->lexical->next_token();
  }
}

unsigned int t_syntax::expression(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  this->nbexprstermsfactors ++;
  this->nbexprstermsfactors_prog ++;
  this->nbexprstermsfactors_class ++;

  unsigned int id_variable = this->temp_variables->size();

  t_identifier *identifier = new t_identifier(lexical->get_current_line(), lexical->get_current_column(), S_EXPRESSION, lexical->get_current_lexeme(), this->temp_variables->size());
  std::ostringstream out;
  out << "expression" << this->temp_variables->size();
  this->temp_variables->insert(std::pair<std::string, t_identifier *>(out.str(), identifier));

  t_node<t_symbol> *node_expr = syntaxnode->addchild(L_EXPRESSION, identifier);
  tree->addnode(node_expr);
  term(tree, node_expr);

  t_token token = lexical->get_current_token();
  if(token == T_ADD)
  {	
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_ADD);
    t_node<t_symbol> *node = node_expr->addchild(L_ADD, symbol);
    tree->addnode(node);
    lexical->next_token();
    term(tree, node_expr);
  }
  else if(token == T_SUB)
  {	
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_SUB);
    t_node<t_symbol> *node = node_expr->addchild(L_SUB, symbol);
    tree->addnode(node);
    lexical->next_token();
    term(tree, node_expr);
  }
  else if(token == T_INF)
  {	
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_INF);
    t_node<t_symbol> *node = node_expr->addchild(L_INF, symbol);
    tree->addnode(node);
    lexical->next_token();
    term(tree, node_expr);
  }
  else if(token == T_INFEQUAL)
  {	
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_INFEQUAL);
    t_node<t_symbol> *node = node_expr->addchild(L_INFEQUAL, symbol);
    tree->addnode(node);
    lexical->next_token();
    term(tree, node_expr);
  }
  else if(token == T_SUP)
  {	
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_SUP);
    t_node<t_symbol> *node = node_expr->addchild(L_SUP, symbol);
    tree->addnode(node);
    lexical->next_token();
    term(tree, node_expr);
  }
  else if(token == T_SUPEQUAL)
  {	
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_SUPEQUAL);
    t_node<t_symbol> *node = node_expr->addchild(L_SUPEQUAL, symbol);
    tree->addnode(node);
    lexical->next_token();
    term(tree, node_expr);
  }

  return id_variable;
}


void t_syntax::term(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  this->nbexprstermsfactors ++;
  this->nbexprstermsfactors_prog ++;
  this->nbexprstermsfactors_class ++;

  t_identifier *identifier = new t_identifier(lexical->get_current_line(), lexical->get_current_column(), S_TERM, lexical->get_current_lexeme(), this->temp_variables->size());
  std::ostringstream out;
  out << "term" << this->temp_variables->size();

  this->temp_variables->insert(std::pair<std::string, t_identifier *>(out.str(), identifier));

  t_node<t_symbol> *node_term = syntaxnode->addchild(L_TERM, identifier);
  tree->addnode(node_term);
  factor(tree, node_term);

  t_token token = lexical->get_current_token();
  if(token == T_MUL)
  {
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_MUL);
    t_node<t_symbol> *node = node_term->addchild(L_MUL, symbol);
    tree->addnode(node);
    lexical->next_token();
    factor(tree, node_term);
  }
  else if(token == T_DIV)
  {
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_DIV);
    t_node<t_symbol> *node = node_term->addchild(L_DIV, symbol);
    tree->addnode(node);
    lexical->next_token();
    factor(tree, node_term);
  }
}

void t_syntax::factor(t_syntaxtree *tree, t_node<t_symbol> *syntaxnode)
{
  this->nbexprstermsfactors ++;
  this->nbexprstermsfactors_prog ++;
  this->nbexprstermsfactors_class ++;

  t_identifier *identifier = new t_identifier(lexical->get_current_line(), lexical->get_current_column(), S_FACTOR, lexical->get_current_lexeme(), this->temp_variables->size());
  std::ostringstream out;
  out << "factor" << this->temp_variables->size();

  this->temp_variables->insert(std::pair<std::string, t_identifier *>(out.str(), identifier));

  t_node<t_symbol> *node_factor = syntaxnode->addchild(L_FACTOR, identifier);
  tree->addnode(node_factor);

  t_token token = lexical->get_current_token();
  if(token == T_NUMBER)
  {
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_NUMBER, lexical->get_current_lexeme());
    t_node<t_symbol> *node = node_factor->addchild(L_NUMBER, symbol);
    tree->addnode(node);
    lexical->next_token();
  }
  else if(token == T_STRING)
  { 
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_STRING, lexical->get_current_lexeme());
    t_node<t_symbol> *node = node_factor->addchild(L_STRING, symbol);
    tree->addnode(node);
    lexical->next_token(); 
  }
  else if(token == T_VARIABLE)
  {
    this->variable(tree, node_factor, false, true);
  }
  else if(token == T_IDENTIFIER)
  {
    this->call_instruction(tree, node_factor, true, this->global_functions);
  }
  else if(token == T_LEFTPARENTHESIS)
  {
    t_symbol *symbol = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_LEFTPARENTHESIS);
    t_node<t_symbol> *node_lb = node_factor->addchild(L_LEFTPARENTHESIS, symbol);
    tree->addnode(node_lb);
    expression(tree, node_factor);

    if(token != T_RIGHTPARENTHESIS)
      return syntaxerror(DEBUG_FACTOR, 976, ERROR_FACTOR_RIGHT_PARENTHESIS);

    t_symbol *symbol1 = new t_symbol(lexical->get_current_line(), lexical->get_current_column(), S_RIGHTPARENTHESIS);
    t_node<t_symbol> *node_lr = node_factor->addchild(L_RIGHTPARENTHESIS, symbol1);
    tree->addnode(node_lr);
  }
}










