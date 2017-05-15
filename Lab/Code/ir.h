#ifndef __IR_H__
#define __IR_H__

#include "tree.h"

typedef struct Operand_* Operand;
struct Operand_ {
	enum { VARIABLE, CONSTANT, ADDRESS} kind;
	union {
		int var_no;
		int value;
	}u;
};

struct InterCode {
	enum { ASSIGN, ADD, SUB, MUL, DIVIDE } kind;
	union {
		struct {Operand right, left; }assign;
		struct {Operand result, op1, op2; }binop;
	}u;
};

struct InterCodes {
	struct InterCode code;
	struct InterCodes* prev, *next;
};

extern struct InterCodes* icHead;

void printInterCodes(); 

#endif
