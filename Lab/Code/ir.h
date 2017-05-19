#ifndef __IR_H__
#define __IR_H__

#include "tree.h"

typedef enum { VARIABLE, CONSTANT, ADDRESS, TEMP, LABEL} OperandKind;
typedef enum { ASSIGN, ADD, SUB, MUL, DIVIDE, LABELOP, IFOP, GOTO} OperationKind;

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
		struct {Operand op; }sigop;
		struct {Operand op1, op2, label; char* relop; }ifop;
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
