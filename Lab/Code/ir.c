#include "common.h"

struct InterCodes* icHead = NULL;

void printOp(Operand op) {
	int kind = op->kind;
	printf("kind:%d\n", kind);

	printf("var_no:%d\n", op->u.var_no);
	printf("value:%d\n", op->u.value);
}

void printInterCodes() {
	struct InterCodes* cur = icHead;
	while(cur != NULL) {
		// kind
		int kind = cur->code.kind;
		printf("kind:%d\n", kind);
		// u
		if(kind == 0){
			printf("right op:");
			printOp(cur->code.u.assign.right);
			printf("\nleft op:");
			printOp(cur->code.u.assign.left);
		}
		else {
			printf("result:");
			printOp(cur->code.u.binop.result);
			printf("\nop1:");
			printOp(cur->code.u.binop.op1);
			printf("\nop2:");
			printOp(cur->code.u.binop.op2);
		}
		printf("\n");
	}
}
