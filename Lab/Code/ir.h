#ifndef __IR_H__
#define __IR_H__

#include "tree.h"

typedef enum { VARIABLE, CONSTANT, ADDRESS, TEMP, LABEL, VPOINTER, TPOINTER} OperandKind;
typedef enum { ASSIGN, ADD, SUB, MUL, DIVIDE, LABELOP, IFOP, GOTO, RETURNOP, READ, WRITE, CALL, ARG, FUNCTION, PARAM, DEC} OperationKind;

typedef union {
	int var_no;
	int value;
}OperandVal;

typedef struct Operand_* Operand;
struct Operand_ {
	OperandKind kind;
	OperandVal u;
};

struct OperandNode {
	Operand op;
	struct OperandNode* next;
};

struct InterCode {
	OperationKind kind;
	union {
		struct {Operand left, right; }assign;
		struct {Operand result, op1, op2; }binop;
		struct {Operand op; }sigop;
		struct {Operand op1, op2, label; char* relop; }ifop;
		struct {char* name; } funop;
		struct {Operand result; char* name; }callop;
		struct {Operand op; int size; }decop;
	}u;
};

struct InterCodeNode {
	struct InterCode code;
	struct InterCodeNode* prev, *next;
};

extern struct InterCodeNode* icHead;

void generateIR(struct Node* root, char* filename);
void outputIR2File(char* filename);

void outputIR(struct InterCodeNode* head);
void printOp(Operand op);

#endif
