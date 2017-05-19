#include "common.h"

Operand newOperand(OperandKind kind, int v) {
	Operand operand = (Operand)malloc(sizeof(struct Operand_));
	operand->kind = kind;
	operand->u.value = v;
	return operand;// return the pointer
}

struct InterCodeNode* newInterCodeNode(OperationKind kind, Operand op1, Operand op2, Operand op3) {
	struct InterCodeNode* node = (struct InterCodeNode*)malloc(sizeof(struct InterCodeNode));
	node->code.kind = kind;
	
	if(kind == ASSIGN) {
		node->code.u.assign.left = op1;
		node->code.u.assign.right = op2;
	}
	else if(kind == ADD || kind == SUB || kind == MUL || kind == DIVIDE) {
		node->code.u.binop.result = op1;
		node->code.u.binop.op1 = op2;
		node->code.u.binop.op2 = op3;
	}

	node->prev = NULL;
	node->next = NULL;

	return node;
}

static struct InterCodeNode* exp__int(struct Node* root, Operand place) {
	struct Node* intNode = root->child;
	int value = my_atoi(intNode->lexeme);
	Operand right = newOperand(CONSTANT, value);
	return newInterCodeNode(ASSIGN, place, right, NULL);
}

static struct InterCodeNode* exp__id(struct Node* root, Operand place) {
	struct Node* intNode = root->child;
	int value = my_atoi(intNode->lexeme);
	Operand right = newOperand(CONSTANT, value);
	return newInterCodeNode(ASSIGN, place, right, NULL);
}

static struct InterCodeNode* exp__exp_assignop_exp(struct Node* root, Operand place) {
	struct Node* intNode = root->child;
	int value = my_atoi(intNode->lexeme);
	Operand right = newOperand(CONSTANT, value);
	return newInterCodeNode(ASSIGN, place, right, NULL);
}

static struct InterCodeNode* exp__exp_plus_exp(struct Node* root, Operand place) {
	struct Node* intNode = root->child;
	int value = my_atoi(intNode->lexeme);
	Operand right = newOperand(CONSTANT, value);
	return newInterCodeNode(ASSIGN, place, right, NULL);
}

static struct InterCodeNode* exp__logicalop(struct Node* root, Operand place) {
	struct Node* intNode = root->child;
	int value = my_atoi(intNode->lexeme);
	Operand right = newOperand(CONSTANT, value);
	return newInterCodeNode(ASSIGN, place, right, NULL);
}

static struct InterCodeNode* exp__minus_exp(struct Node* root, Operand place){
	struct Node* intNode = root->child;
	int value = my_atoi(intNode->lexeme);
	Operand right = newOperand(CONSTANT, value);
	return newInterCodeNode(ASSIGN, place, right, NULL);
}

struct InterCodeNode* translate_Exp(struct Node* root, Operand place) {
	switch(root->rule) {
		case Exp__INT:return exp__int(root, place);
		case Exp__ID:return exp__id(root, place);
		case Exp__Exp_ASSIGNOP_Exp:return exp__exp_assignop_exp(root, place);
		case Exp__Exp_PLUS_Exp:return exp__exp_plus_exp(root, place);
		case Exp__MINUS_Exp:return exp__minus_exp(root, place);
		default:return exp__logicalop(root, place);
	}
}

void generateIR(struct Node* root, char* filename) {
//	translate(root);

	// file...
}
