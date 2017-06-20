#ifndef T_TOKEN_H
#define T_TOKEN_H

/* tokens (lexical analysis) */
enum t_token 
{
  T_READPOL, // 0
  T_PRINT, // 8
  T_CLASS, // 10
  T_NEW, // 11
  T_FUNCTION, // 12
  T_IDENTIFIER, // 13
  T_STRING,  // 14
  T_DECLR,  // 15
  T_VARIABLE,  // 16
  T_NUMBER,  // 17
  T_COMMA,  // 18
  T_DOTCOMMA,  // 19
  T_RIGHTBRACKET,  // 20
  T_LEFTBRACKET,  // 21
  T_RIGHTBRACE,  // 22
  T_LEFTBRACE,  // 23
  T_DOUBLEQUOTE,  // 24
  T_RIGHTPARENTHESIS,  // 25
  T_LEFTPARENTHESIS,  // 26
  T_EQUAL,  // 27
  T_ADD,  // 28
  T_SUB,  // 29
  T_DIV,  // 30
  T_MUL,  // 31
  T_UNDEF,  // 32
  T_END,  // 33
  T_ERROR,  // 34
  T_RETURN,  // 35
  T_UP_ARROW,  // 36
  T_DOWN_ARROW,  // 37
  T_TAB,  // 38
  T_IF,  // 40
  T_ELSEIF,  // 41
  T_ELSE,  // 42
  T_FOR,  // 43
  T_DO,  // 44
  T_WHILE,  // 45
  T_THIS,  // 46
  T_SUP,  // 47
  T_INF,  // 48
  T_SUPEQUAL,  // 49
  T_INFEQUAL,  // 50
  T_SYSTEM,  // 51
  T_FPUTS,  // 52
  T_CFG_DOT,  // 53
  T_CFG_COMPUTE,  // 54
  T_AST_DOT,  // 53
  T_INCLUDE  // 53
};

#endif

