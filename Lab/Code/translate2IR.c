#include "common.h"

// declaration
struct InterCodeNode* translate_Exp(struct Node* root, Operand place);
struct InterCodeNode* translate_Cond(struct Node* root, Operand label_true, Operand label_false);

// constant defination
Operand constant0 = NULL;
Operand constant1 = NULL;

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

struct InterCodeNode* newInterCodeNode(OperationKind kind, Operand op1, Operand op2, Operand op3, char* relop) {
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
	else if(kind == LABELOP || kind == GOTO) {
		node->code.u.sigop.op = op1;
	}
	else if(kind == IFOP) {
		node->code.u.ifop.op1 = op1;
		node->code.u.ifop.op2 = op2;
		node->code.u.ifop.label = op3;
		node->code.u.ifop.relop = relop;
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

struct InterCodeNode* translate_Exp(struct Node* root, Operand place) {
	switch(root->rule) {
		case Exp__INT:{
			struct Node* intNode = root->child;
			int value = my_atoi(intNode->lexeme);
			Operand right = newOperand(CONSTANT, NULL, value);
			return newInterCodeNode(ASSIGN, place, right, NULL, NULL);
		}
		case Exp__ID:{
			struct Node* id = root->child;
			Operand right = newOperand(VARIABLE, id->lexeme, 0);
			return newInterCodeNode(ASSIGN, place, right, NULL, NULL);
		}
		case Exp__Exp_ASSIGNOP_Exp:{
			struct Node* exp1 = root->child;
			struct Node* exp2 = exp1->nextSibling->nextSibling;
	
			struct Node* variable = exp1->child;
	
			Operand t1 = new_temp();
			Operand var = newOperand(VARIABLE, variable->lexeme, 0);

			struct InterCodeNode* c1 = newInterCodeNode(ASSIGN, var ,t1, NULL, NULL);
			struct InterCodeNode* c2 = newInterCodeNode(ASSIGN, place, var, NULL, NULL);

			struct InterCodeNode* code1 = translate_Exp(exp2, t1);
			struct InterCodeNode* code2 = concat(2, c1, c2);

			return concat(2, code1, code2);
		}
		case Exp__Exp_PLUS_Exp:{
			struct Node* exp1 = root->child;
			struct Node* exp2 = exp1->nextSibling->nextSibling;

			Operand t1 = new_temp();
			Operand t2 = new_temp();
			struct InterCodeNode* code1 = translate_Exp(exp1, t1);
			struct InterCodeNode* code2 = translate_Exp(exp2, t2);
			struct InterCodeNode* code3 = newInterCodeNode(ADD, place, t1, t2, NULL);

			return concat(3, code1, code2, code3);
		}
		case Exp__MINUS_Exp:{
			struct Node* exp1 = root->child->nextSibling;

			Operand t1 = new_temp();

			struct InterCodeNode* code1 = translate_Exp(exp1, t1);
			struct InterCodeNode* code2 = newInterCodeNode(SUB, place, constant0, t1, NULL);
			return concat(2, code1, code2);
		}
		default:{
			Operand label1 = new_label();
			Operand label2 = new_label();

			struct InterCodeNode* code0 = newInterCodeNode(ASSIGN, place, constant0, NULL, NULL);
			struct InterCodeNode* code1 = translate_Cond(root, label1, label2);
			struct InterCodeNode* c1 = newInterCodeNode(LABELOP, label1, NULL, NULL, NULL);
			struct InterCodeNode* c2 = newInterCodeNode(ASSIGN, place, constant1, NULL, NULL);
			struct InterCodeNode* code2 = concat(2, c1, c2);
			struct InterCodeNode* code3 = newInterCodeNode(LABELOP, label2, NULL, NULL, NULL);

			return concat(4, code0 ,code1, code2, code3);
		}
	}
}

struct InterCodeNode* translate_Cond(struct Node* root, Operand label_true, Operand label_false) {
	switch(root->rule) {
		case Exp__Exp_RELOP_Exp:{
			struct Node* exp1 = root->child;
			struct Node* relop = exp1->nextSibling;
			struct Node* exp2 = relop->nextSibling;

			Operand t1 = new_temp();
			Operand t2 = new_temp();
			struct InterCodeNode* code1 = translate_Exp(exp1, t1);
			struct InterCodeNode* code2 = translate_Exp(exp2, t2);
			char* op = relop->lexeme;
			struct InterCodeNode* code3 = newInterCodeNode(IFOP, t1, t2, label_true, op);
			struct InterCodeNode* code4 = newInterCodeNode(GOTO, label_false, NULL, NULL ,NULL);
			return concat(4, code1, code2, code3, code4);
		}
		case Exp__NOT_Exp:{
			return translate_Cond(root->child->nextSibling, label_false, label_true);
		}
		case Exp__Exp_AND_Exp:{
			struct Node* exp1 = root->child;
			struct Node* exp2 = exp1->nextSibling->nextSibling;

			Operand label1 = new_label();
			struct InterCodeNode* code1 = translate_Cond(exp1, label1, label_false);
			struct InterCodeNode* code2 = translate_Cond(exp2, label_true, label_false);
			struct InterCodeNode* c = newInterCodeNode(LABELOP, label1, NULL, NULL, NULL);
			return concat(3, code1, c, code2);
		}
		case Exp__Exp_OR_Exp:{
			struct Node* exp1 = root->child;
			struct Node* exp2 = exp1->nextSibling->nextSibling;

			Operand label1 = new_label();
			struct InterCodeNode* code1 = translate_Cond(exp1, label_true, label1);
			struct InterCodeNode* code2 = translate_Cond(exp2, label_true, label_false);
			struct InterCodeNode* c = newInterCodeNode(LABELOP, label1, NULL, NULL, NULL);
			return concat(3, code1, c, code2);
		}
		default: {
					Operand t1 = new_temp();
					struct InterCodeNode* code1 = translate_Exp(root, t1);
					struct InterCodeNode* code2 = newInterCodeNode(IFOP, t1, constant0, label_true, "!=");
					struct InterCodeNode* code3 = newInterCodeNode(GOTO, label_false, NULL, NULL, NULL);

					return concat(3, code1, code2, code3);
				 }
	}
}

void generateIR(struct Node* root, char* filename) {
	constant0 = newOperand(CONSTANT, NULL, 0);
	constant1 = newOperand(CONSTANT, NULL, 1);
//	translate(root);

	// file...
}
