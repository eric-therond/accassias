#ifndef T_OPCODE_H
#define T_OPCODE_H

enum t_opcode
{
  JUMP, // 0
  EMPC, // 1
  EMPS, // 2
  EMPL, // 3
  EMPG, // 4
  DEPL, // 5
  EMPT, // 6
  DEPT, // 7
  ADD, // 8
  SUB, // 9
  MUL, // 10
  DIV, // 11
  INF, // 12
  INFEQUAL, // 13
  SUP, // 14
  SUPEQUAL, // 15
  CALL, // 16
  PILE, // 17
  FUNCTION, // 18
  INSTRUCTIONS, // 19
  RETURN, // 20
  PRINT, // 22
  EXIT, // 22
  READPOL, // 29
  END, // 30
  IFFALSE, // 32
  CLASS, // 32
  INSTANCE_CLASS, // 33
  SYSTEM, // 35
  FPUTS,
  CFG_DOT,
  CFG_COMPUTE,
  AST_DOT,
  INCLUDE

};

#endif
