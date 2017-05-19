#include "common.h"

// function
Operand newOperand(OperandKind kind, char* varName, int val) {
	Operand operand = (Operand)malloc(sizeof(struct Operand_));
	operand->kind = kind;

	if(kind == VARIABLE) {
		operand->u.varName = varName;
	}
	else operand->u.value = val;

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

Operand new_temp() {
	static int no = 1;
	Operand operand = (Operand)malloc(sizeof(struct Operand_));
	operand->kind = TEMP;
	operand->u.var_no = no;
	no++;
	return operand;
}

Operand new_label() {
	static int no = 1;
	Operand operand = (Operand)malloc(sizeof(struct Operand_));
	operand->kind = LABEL;
	operand->u.var_no = no;
	no++;
	return operand;
}

struct InterCodeNode* concat(int number, ...) {
	va_list p;
	va_start(p, number);

	struct InterCodeNode* head = va_arg(p, struct InterCodeNode*);
	struct InterCodeNode* tail = head;
	while(tail->next != NULL)tail = tail->next;

	int i = 1;
	for(;i<number;i++) {
		struct InterCodeNode* temp = va_arg(p, struct InterCodeNode*);
		if(temp!=NULL) {
			tail->next = temp;
			temp->prev = tail;

			tail = temp;
			while(tail->next != NULL) tail = tail->next;
		}
	}

	va_end(p);

	return head;
}

// constant
Operand constant0 = newOperand(CONSTANT, NULL, 0);
Operand constant1 = newOperand(CONSTANT, NULL, 1);

// tanslate function
static struct InterCodeNode* exp__int(struct Node* root, Operand place) {
	struct Node* intNode = root->child;
	int value = my_atoi(intNode->lexeme);
	Operand right = newOperand(CONSTANT, NULL, value);
	return newInterCodeNode(ASSIGN, place, right, NULL);
}

static struct InterCodeNode* exp__id(struct Node* root, Operand place) {
	struct Node* id = root->child;
	//struct Symbol* sym = lookupVariable(id->lexeme);
	Operand right = newOperand(VARIABLE, id->lexeme, 0);
	return newInterCodeNode(ASSIGN, place, right, NULL);
}

static struct InterCodeNode* exp__exp_assignop_exp(struct Node* root, Operand place) {
	struct Node* exp1 = root->child;
	struct Node* exp2 = exp1->nextSibling->nextSibling;
	
	struct Node* variable = exp1->child;
	
	Operand t1 = new_temp();
	Operand var = newOperand(VARIABLE, variable->lexeme, 0);

	struct InterCodeNode* c1 = newInterCodeNode(ASSIGN, var ,t1, NULL);
	struct InterCodeNode* c2 = newInterCodeNode(ASSIGN, place, var, NULL);

	struct InterCodeNode* code1 = translate_Exp(exp2, t1);
	struct InterCodeNode* code2 = concat(2, c1, c2);

	return concat(2, code1, code2);
}

static struct InterCodeNode* exp__exp_plus_exp(struct Node* root, Operand place) {
	struct Node* exp1 = root->child;
	struct Node* exp2 = exp1->nextSibling->nextSibling;

	Operand t1 = new_temp();
	Operand t2 = new_temp();
	struct InterCodeNode* code1 = translate_Exp(exp1, t1);
	struct InterCodeNode* code2 = translate_Exp(exp2, t2);
	struct InterCodeNode* code3 = newInterCodeNode(ADD, place, t1, t2);

	return concat(3, code1, code2, code3);
}

static struct InterCodeNode* exp__minus_exp(struct Node* root, Operand place){
	struct Node* exp1 = root->child->nextSibling;

	Operand t1 = new_temp();

	struct InterCodeNode* code1 = translate_Exp(exp1, t1);
	struct InterCodeNode* code2 = newInterCodeNode(SUB, place, constant0, t1);
	return concat(2, code1, code2);
}

static struct InterCodeNode* exp__logicalop(struct Node* root, Operand place) {
	Operand label1 = new_label();
	Operand label2 = new_label();

	struct InterCodeNode* code0 = newInterCodeNode(ASSIGN, place, constant0, NULL);
	struct InterCodeNode* code1 = translate_Cond(root, label1, label2);
	struct InterCodeNode* code2 = concat();

	return concat(4,
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
