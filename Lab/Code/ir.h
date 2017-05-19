#ifndef __IR_H__
#define __IR_H__

#include "tree.h"

typedef enum { VARIABLE, CONSTANT, ADDRESS, TEMP, LABEL} OperandKind;
typedef enum { ASSIGN, ADD, SUB, MUL, DIVIDE } OperationKind;

typedef union {
	char* varName;
	int var_no;
	int value;
}OperandVal;

typedef struct Operand_* Operand;
struct Operand_ {
	OperandKind kind;
	OperandVal u;
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
