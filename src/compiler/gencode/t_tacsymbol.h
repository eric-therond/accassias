#ifndef T_TACSYMBOL_H
#define T_TACSYMBOL_H

enum t_tacsymbol
{

  TAC_ASSIGN_READPOL, // 1	
  TAC_ASSIGN_SUBINSTRUCTIONS, // 2	
  TAC_ASSIGN_INSTRUCTIONS, // 3
  TAC_ASSIGN_PROG_JUMP, // 4
  TAC_ASSIGN_STARTCALL, // 5
  TAC_ASSIGN_CALL, // 6
  TAC_ASSIGN_FUNCTION, // 7
  TAC_ASSIGN_METHOD, // 8
  TAC_ASSIGN_INSTANCE_CLASS, // 9
  TAC_ASSIGN_CLASS, // 10
  TAC_ASSIGN_ENDCLASS, // 11
  TAC_ASSIGN_RETURN, // 12
  TAC_ASSIGN_RETURN_WITHOUT_VALUE, // 13
  TAC_ASSIGN_DECLR_ARRAY, // 14
  TAC_ASSIGN_DECLR, // 15
  TAC_ASSIGN_PRINT, // 16
  TAC_ASSIGN_EXIT, // 17
  TAC_ASSIGN_SYSTEM, // 18
  TAC_ASSIGNV,  // 19
  TAC_ASSIGNL,  // 20
  TAC_ASSIGNC, // 21
  TAC_ASSIGNS, // 22
  TAC_ASSIGNCA, // 23
  TAC_ASSIGN_ADD, // 24
  TAC_ASSIGN_SUB, // 25
  TAC_ASSIGN_DIV, // 26
  TAC_ASSIGN_MUL, // 27
  TAC_ASSIGN_INF, // 28
  TAC_ASSIGN_SUP, // 29
  TAC_ASSIGN_INFEQUAL, // 30
  TAC_ASSIGN_SUPEQUAL, // 31
  TAC_ASSIGN_PARAM, // 32
  TAC_ASSIGN_IF, // 33
  TAC_ASSIGN_IFTRUE, // 34
  TAC_ASSIGN_ELSEIF, // 35
  TAC_ASSIGN_ELSE, // 36
  TAC_ASSIGN_ENDIF, // 37
  TAC_ASSIGN_ENDELSEIF, // 38
  TAC_ASSIGN_ENDELSE, // 39
  TAC_ASSIGN_NOELSE, // 40
  TAC_ASSIGN_STARTFOR, // 41
  TAC_ASSIGN_FOR, // 42
  TAC_ASSIGN_ENDFOR,// 43
  TAC_ASSIGN_ARRAY, // 44
  TAC_ASSIGN_FACTOR_ARRAY, // 45
  TAC_ASSIGN_NULL, // 46
  TAC_ASSIGN_FPUTS,
  TAC_ASSIGN_CFG_DOT,
  TAC_ASSIGN_CFG_COMPUTE,
  TAC_ASSIGN_AST_DOT,
  TAC_ASSIGN_INCLUDE
};

#endif

