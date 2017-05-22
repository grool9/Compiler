#include "common.h"

FILE* fp = NULL;

struct InterCodeNode* icHead = NULL;

void printOp2File(Operand op) {
	if(op == NULL)return;
	switch(op->kind) {
		case VARIABLE: fprintf(fp, "v%d", op->u.var_no);break;
		case CONSTANT: fprintf(fp, "#%d", op->u.value);break;
		case ADDRESS: fprintf(fp, "&v%d", op->u.var_no); break;
		case TEMP: fprintf(fp, "t%d", op->u.var_no); break;
		case LABEL: fprintf(fp, "label%d", op->u.var_no); break;
		case VPOINTER: fprintf(fp, "*v%d", op->u.var_no);break;
		case TPOINTER: fprintf(fp, "*t%d", op->u.var_no);break;
		default : printf("UNK operand\n");
	}
}

void outputIR2File(char* filename) {
	fp = fopen(filename, "w");
	
	struct InterCodeNode* p = icHead;
	for(; p != NULL; p = p->next) {
		switch(p->code.kind) {
			case ASSIGN: {
							 printOp2File(p->code.u.assign.left);
							 fprintf(fp, " := ");
							 printOp2File(p->code.u.assign.right);
							 break;
						 }
			case ADD: {
						  printOp2File(p->code.u.binop.result);
						  fprintf(fp, " := ");
						  printOp2File(p->code.u.binop.op1);
						  fprintf(fp, " + ");
						  printOp2File(p->code.u.binop.op2);
						  break;
					  }
			case SUB: {
						  printOp2File(p->code.u.binop.result);
						  fprintf(fp, " := ");
						  printOp2File(p->code.u.binop.op1);
						  fprintf(fp, " - ");
						  printOp2File(p->code.u.binop.op2);
						  break;
					  }
			case MUL: {
						  printOp2File(p->code.u.binop.result);
						  fprintf(fp, " := ");
						  printOp2File(p->code.u.binop.op1);
						  fprintf(fp, " * ");
						  printOp2File(p->code.u.binop.op2);
						  break;
					  }
			case DIVIDE: {
						  printOp2File(p->code.u.binop.result);
						  fprintf(fp, " := ");
						  printOp2File(p->code.u.binop.op1);
						  fprintf(fp, " / ");
						  printOp2File(p->code.u.binop.op2);
						  break;
					  }
			case LABELOP: {
							  fprintf(fp, "LABEL ");
							  printOp2File(p->code.u.sigop.op);
							  fprintf(fp, " :");
							  break;
						  }
			case IFOP: {
						   fprintf(fp, "IF ");
						   printOp2File(p->code.u.ifop.op1);
						   fprintf(fp, " %s ", p->code.u.ifop.relop);
						   printOp2File(p->code.u.ifop.op2);
						   fprintf(fp, " GOTO ");
						   printOp2File(p->code.u.ifop.label);
						   break;
					   }
			case GOTO: {
						   fprintf(fp, "GOTO ");
						   printOp2File(p->code.u.sigop.op);
						   break;
					   }
			case RETURNOP: {
							   fprintf(fp, "RETURN ");
							   printOp2File(p->code.u.sigop.op);
							   break;
						   }
			case READ: {
						   fprintf(fp, "READ ");
						   printOp2File(p->code.u.sigop.op);
						   break;
					   }
			case WRITE: {
							fprintf(fp, "WRITE ");
							printOp2File(p->code.u.sigop.op);
							break;
						}
			case CALL: {
						   printOp2File(p->code.u.callop.result);
						   fprintf(fp, " := CALL %s", p->code.u.callop.name);
						   break;
					   }
			case ARG: {
						  fprintf(fp, "ARG ");
						  printOp2File(p->code.u.sigop.op);
						  break;
					  }
			case FUNCTION: {
							   fprintf(fp, "FUNCTION %s :", p->code.u.funop.name);
							   break;
						   }
			case PARAM: {
							fprintf(fp, "PARAM ");
							printOp2File(p->code.u.sigop.op);
							break;
						}
			default: {
				printf("UNK operation!\n");
						}
			}
		fprintf(fp, "\n");	
	}
	
	fclose(fp);
}


// used to debug
void printOp(Operand op) {
	if(op == NULL)return;
	switch(op->kind) {
		case VARIABLE: printf("v%d", op->u.var_no);break;
		case CONSTANT: printf("#%d", op->u.value);break;
		case ADDRESS: printf("&v%d", op->u.var_no); break;
		case TEMP: printf("t%d", op->u.var_no); break;
		case LABEL: printf("label%d", op->u.var_no); break;
		case VPOINTER: printf("*v%d", op->u.var_no);break;
		case TPOINTER: printf("*t%d", op->u.var_no);break;
		default : printf("UNK operand\n");
	}
}

void outputIR(struct InterCodeNode* head) {	
	struct InterCodeNode* p = head;
	for(; p != NULL; p = p->next) {
		switch(p->code.kind) {
			case ASSIGN: {
							 printOp(p->code.u.assign.left);
							 printf(" := ");
							 printOp(p->code.u.assign.right);
							 break;
						 }
			case ADD: {
						  printOp(p->code.u.binop.result);
						  printf(" := ");
						  printOp(p->code.u.binop.op1);
						  printf(" + ");
						  printOp(p->code.u.binop.op2);
						  break;
					  }
			case SUB: {
						  printOp(p->code.u.binop.result);
						  printf(" := ");
						  printOp(p->code.u.binop.op1);
						  printf(" - ");
						  printOp(p->code.u.binop.op2);
						  break;
					  }
			case MUL: {
						  printOp(p->code.u.binop.result);
						  printf(" := ");
						  printOp(p->code.u.binop.op1);
						  printf(" * ");
						  printOp(p->code.u.binop.op2);
						  break;
					  }
			case DIVIDE: {
						  printOp(p->code.u.binop.result);
						  printf(" := ");
						  printOp(p->code.u.binop.op1);
						  printf(" / ");
						  printOp(p->code.u.binop.op2);
						  break;
					  }
			case LABELOP: {
							  printf("LABEL ");
							  printOp(p->code.u.sigop.op);
							  printf(" :");
							  break;
						  }
			case IFOP: {
						   printf("IF ");
						   printOp(p->code.u.ifop.op1);
						   printf(" %s ", p->code.u.ifop.relop);
						   printOp(p->code.u.ifop.op2);
						   printf(" GOTO ");
						   printOp(p->code.u.ifop.label);
						   break;
					   }
			case GOTO: {
						   printf("GOTO ");
						   printOp(p->code.u.sigop.op);
						   break;
					   }
			case RETURNOP: {
							   printf("RETURN ");
							   printOp(p->code.u.sigop.op);
							   break;
						   }
			case READ: {
						   printf("READ ");
						   printOp(p->code.u.sigop.op);
						   break;
					   }
			case WRITE: {
							printf("WRITE ");
							printOp(p->code.u.sigop.op);
							break;
						}
			case CALL: {
						   printOp(p->code.u.callop.result);
						   printf(" := CALL %s", p->code.u.callop.name);
						   break;
					   }
			case ARG: {
						  printf("ARG ");
						  printOp(p->code.u.sigop.op);
						  break;
					  }
			case FUNCTION: {
							   printf("FUNCTION %s :", p->code.u.funop.name);
							   break;
						   }
			case PARAM: {
							printf("PARAM ");
							printOp(p->code.u.sigop.op);
							break;
						}
			case DEC: {
						  printf("DEC ");
						  printOp(p->code.u.decop.op);
						  printf(" %d", p->code.u.decop.size);
						  break;
					  }
			default: {
				printf("UNK operation!\n");
						}
			}
		printf("\n");	
	}
}
