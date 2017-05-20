#include "common.h"

struct InterCodeNode* icHead = NULL;

void printOp(Operand op) {
	printf("!!%d\n", op);
	int kind = op->kind;
	printf("kind:%d\t", kind);

	if(kind == VARIABLE) {
		printf("%s\n", op->u.varName);
	}
	else {
		printf("%d\n", op->u.value);
	}
}

void printInterCodes(struct InterCodeNode* root) {
	struct InterCodeNode* cur = root;
	while(cur != NULL) {
		// kind
		int kind = cur->code.kind;
		printf("kind:%d\n", kind);
		// u
		if(kind == ASSIGN){
			printf("left op:");
			printOp(cur->code.u.assign.left);
			printf("right op:");
			printOp(cur->code.u.assign.right);
		}
		else if(kind == ADD || kind == SUB || kind == MUL || kind == DIVIDE ) {
			printf("result:");
			printOp(cur->code.u.binop.result);
			printf("op1:");
			printOp(cur->code.u.binop.op1);
			printf("op2:");
			printOp(cur->code.u.binop.op2);
		}
		else if(kind == LABELOP || kind == GOTO || kind == RETURNOP || kind == READ || kind == WRITE || kind == ARG || kind == PARAM) {
			printf("op:");
			printOp(cur->code.u.sigop.op);
		}
		else if(kind == IFOP) {
			printf("op1:");
			printOp(cur->code.u.ifop.op1);
			printf("%s\n", cur->code.u.ifop.relop);
			printf("op2");
			printOp(cur->code.u.ifop.op2);
			printf("label:");
			printOp(cur->code.u.ifop.label);
		}
		else if(kind == CALL || kind == FUNCTION) {
			printf("function:%s", cur->code.u.funop.name);
		}
		else if(kind == ASSIGNCALL) {
			printf("result:");
			printOp(cur->code.u.assigncall.result);
			printf("name:%s", cur->code.u.assigncall.name);
		}

		printf("\n");

		cur = cur->next;
	}
}
