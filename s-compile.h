/*
    Very Simple Compiler
	$Id$
 */

#include <stdio.h>

extern char *ptr,*last_ptr,*before;
extern int  value,lvalue;
extern int  last_token;
extern int  label;
extern char *comments;

enum opcode {
    O_SUB = 1,
    O_ADD = 2,
    O_MUL = 3,
    O_DIV = 4,

    O_MOD = 5,
    O_AND = 6,
    O_OR  = 7,
    O_XOR = 8,

    O_SUB_R = 9,
    O_DIV_R = 10,
} ;

extern void error(char *);

extern void emit_push();
extern void emit_compare() ;
extern void emit_store(int);
extern void emit_push();
extern void emit_calc(enum opcode);
extern void emit_value(int) ;
extern void emit_load(int);
extern void emit_comment();
extern void emit_intro();
extern void emit_ending();
extern void emit_print();

extern int token();

