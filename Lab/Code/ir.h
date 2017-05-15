#ifndef __IR_H__
#define __IR_H__

typedef struct Operand_* Operand;
struct Operand_ {
	enum { VARIABLE, CONSTANT, ADDRESS} kind;
	union {
		int var_no;
		int value;
	}u;
};

struct InterCode {
	enum { ASSIGN, ADD, SUB, MUL, DIV } kind;
	union {
		struct {Operand right, left; }assign;
		struct {Operand result, op1, op2; }binop;
	}u;
};

struct InterCodes {
	InterCode code;
	struct InterCodes* prev, *next;
};

#endif
