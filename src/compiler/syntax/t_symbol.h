#ifndef T_SYMBOL_H
#define T_SYMBOL_H

#include <ostream>
#include <string>

/* syntactic symbols */ 
enum s_symbol
{
  S_READPOL,
  S_NEW_INSTANCE_CLASS,
  S_SHOW_TAC,	    
  S_HIDE_TAC,
  S_SHOW_CODE,
  S_HIDE_CODE,  
  S_SHOW_STATEVM,
  S_HIDE_STATEVM,
  S_METHOD,
  S_CALL,
  S_START_CALL,
  S_IDENTIFIER,
  S_CLASS,
  S_CLASS_END,
  S_FUNCTION,
  S_RETURN,
  S_RETURN_WITHOUT_VALUE,
  S_VARIABLE_LOCAL,
  S_VARIABLE_GLOBAL,
  S_NUMBER,
  S_COMMA,
  S_DOTCOMMA,
  S_RIGHTBRACKET,
  S_LEFTBRACKET,
  S_RIGHTBRACE,
  S_LEFTBRACE,
  S_RIGHTPARENTHESIS,
  S_LEFTPARENTHESIS,
  S_EQUAL,
  S_ADD,
  S_SUB,
  S_DIV,
  S_MUL,
  S_UNDEF,
  S_END,
  S_ERROR,
  S_PROG,
  S_LIST_CLASSES,
  S_LIST_FUNCTIONS,
  S_LIST_INSTRUCTIONS,
  S_SUBLIST_INSTRUCTIONS,
  S_DECLARATION,
  S_AFFECTATION,
  S_EXPRESSION,
  S_TERM,
  S_FACTOR,
  S_STRING,
  S_PRINT,
  S_EXIT,
  S_RCCONF,
  S_TIMER,
  S_PARAM,
  S_IF,
  S_IFTRUE,
  S_ELSEIF,
  S_ELSE,
  S_ENDIF,
  S_ENDELSEIF,
  S_ENDELSE,
  S_NOELSE,
  S_FOR,
  S_STARTFOR,
  S_ENDFOR,
  S_INF,
  S_INFEQUAL,
  S_SUP,
  S_SUPEQUAL,
  S_ARRAY,
  S_SYSTEM,
  S_FPUTS,
  S_CFG_DOT,
  S_CFG_COMPUTE,
  S_AST_DOT,
  S_INCLUDE
};

#define L_READPOL "readpol"
#define L_NEW_INSTANCE_CLASS "instance_class"
#define L_SHOW_TAC "show_tac"
#define L_HIDE_TAC "hide_tac"
#define L_SHOW_CODE "show_code" 
#define L_HIDE_CODE "hide_code"
#define L_SHOW_STATEVM "show_statevm"
#define L_HIDE_STATEVM "hide_statevm"
#define L_CALL "call"
#define L_START_CALL "start_call"
#define	L_IDENTIFIER "identifier"
#define L_METHOD "method"
#define L_CLASS "class"
#define L_CLASS_END "end_class"
#define L_FUNCTION "function"
#define L_RETURN "return"
#define L_RETURN_WITHOUT_VALUE "return_without_value"
#define L_VARIABLE_LOCAL "variable_locale"
#define L_VARIABLE_GLOBAL "variable_globale"
#define L_NUMBER "number"
#define L_COMMA "comma"
#define L_DOTCOMMA "dotcomma"
#define L_RIGHTBRACKET "rightbracket"
#define L_LEFTBRACKET "leftbracket"
#define L_RIGHTBRACE "rightbrace"
#define L_LEFTBRACE "leftbrace"
#define L_RIGHTPARENTHESIS "rightparenthesis"
#define L_LEFTPARENTHESIS "leftparenthesis"
#define L_EQUAL "equal"
#define L_ADD "add"
#define L_SUB "sub"
#define L_DIV "div"
#define L_MUL "mul"
#define L_UNDEF "undef"
#define L_END "end"
#define L_ERROR "error"
#define L_PROG "prog"
#define L_LIST_CLASSES "list_global_class"
#define L_LIST_FUNCTIONS "list_global_functions"
#define L_LIST_INSTRUCTIONS "list_global_instructions"
#define L_SUBLIST_INSTRUCTIONS "list_sub_instructions"
#define L_DECLARATION "declaration"
#define L_AFFECTATION "affectation"
#define L_EXPRESSION "expression"
#define L_TERM "term"
#define L_FACTOR "factor"
#define L_STRING "string"
#define L_PRINT "print"
#define L_EXIT "exit"
#define L_RCCONF "rcconf"
#define L_TIMER "timer"
#define L_PARAM "param"
#define L_IF "if"
#define L_IFTRUE "iftrue"
#define L_ELSEIF "elseif"
#define L_ELSE "else"
#define L_ENDIF "end_if"
#define L_ENDELSEIF "end_elseif"
#define L_ENDELSE "end_else"
#define L_NOELSE "no_else"
#define L_FOR "for"
#define L_ENDFOR "end_for"
#define L_STARTFOR "start_for"
#define L_INF "inf"
#define L_INFEQUAL "inf equal"
#define L_SUP "sup"
#define L_SUPEQUAL "sup equal"
#define L_ARRAY "array"
#define L_SYSTEM "system"
#define L_FPUTS "fputs"
#define L_CFG_DOT "cfg_dot"
#define L_CFG_COMPUTE "cfg_compute"
#define L_AST_DOT "ast_dot"
#define L_INCLUDE "include"

class t_symbol
{
  public:
    t_symbol();
    t_symbol(unsigned int, unsigned int, s_symbol);
    t_symbol(unsigned int, unsigned int, s_symbol, std::string &); 
    t_symbol(const t_symbol &);

    void setsymbol(s_symbol);
    s_symbol getsymbol();
    unsigned int getline();
    unsigned int getcolumn();
    void setlexeme(std::string &lexeme);
    std::string &getlexeme();

    virtual int getaddr();
    virtual void setaddr(int);

    virtual unsigned int getnbargs();
    virtual void setnbargs(unsigned int);

    virtual unsigned int getnblocalvariables();
    virtual void setnblocalvariables(unsigned int);

  protected:
    s_symbol symbol;
    unsigned int line;
    unsigned int column;
    std::string lexeme;
    unsigned int nblocalvariables;
};

std::ostream &operator<<(std::ostream &, t_symbol &);

#endif
