#ifndef __IR_H__
#define __IR_H__

#include "tree.h"

typedef enum { VARIABLE, CONSTANT, ADDRESS} OperandKind;
typedef enum { ASSIGN, ADD, SUB, MUL, DIVIDE } OperationKind;

typedef struct Operand_* Operand;
struct Operand_ {
	OperandKind kind;
	union {
		int var_no;
		int value;
	}u;
};

struct InterCode {
	OperationKind kind;
	union {
		struct {Operand left, right; }assign;
		struct {Operand result, op1, op2; }binop;
	}u;
};

struct InterCodeNode {
	struct InterCode code;
	struct InterCodeNode* prev, *next;
};

extern struct InterCodeNode* icHead;

void printInterCodes();

struct InterCodeNode* newInterCodeNode(); 

#endif
