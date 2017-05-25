#include "common.h"

#define DEBUG

// block
bool isHeadCode(struct InterCodeNode* code) {
	if(code->prev == NULL)return true;
	if(code->code.kind == LABELOP)return true;
	if(code->prev->code.kind == GOTO || code->prev->code.kind == IFOP)return true;
	return false;
}

void del_code(struct InterCodeNode* code) {
	if(code == NULL)return;

	struct InterCodeNode* d = code;
	struct InterCodeNode* pre = code->prev;

	code = code->next;
	if(pre == NULL && code == NULL);
	else if(pre == NULL) {
		code->prev = NULL;
	}
	else if(code == NULL) {
		pre->next = NULL;
	}
	else {
		pre->next = code;
		code->prev = pre;
	}
	free(d);
}

/* 常量合并的实现
 * merge the constant
 */
void const_folding(struct Node* node) {
	switch(node->rule) {
		case Exp__Exp_PLUS_Exp:
		case Exp__Exp_MINUS_Exp:
		case Exp__Exp_STAR_Exp:
		case Exp__Exp_DIV_Exp:
		case Exp__Exp_AND_Exp:
		case Exp__Exp_OR_Exp:
		case Exp__Exp_RELOP_Exp:{
			struct Node* exp1 = node->child;
			struct Node* op = exp1->nextSibling;
			struct Node* exp2 = op->nextSibling;

			const_folding(exp1);
			const_folding(exp2);

			if(exp1->isConstant && exp2->isConstant) {
				node->isConstant = true;
				
				switch(node->rule) {
					case Exp__Exp_PLUS_Exp:node->value = exp1->value + exp2->value;break;
					case Exp__Exp_MINUS_Exp:node->value = exp1->value - exp2->value;break;
					case Exp__Exp_STAR_Exp:node->value = exp1->value * exp2->value;break;
					case Exp__Exp_DIV_Exp:node->value = exp1->value / exp2->value;break;
					case Exp__Exp_AND_Exp:node->value = exp1->value && exp2->value;break;
					case Exp__Exp_OR_Exp:node->value = exp1->value || exp2->value;break;
					case Exp__Exp_RELOP_Exp:{
						if(strcmp(op->lexeme, ">")==0)node->value = exp1->value > exp2->value;
						else if(strcmp(op->lexeme, "<")==0)node->value = exp1->value < exp2->value;
						else if(strcmp(op->lexeme, ">=")==0)node->value = exp1->value >= exp2->value;
						else if(strcmp(op->lexeme, "<=")==0)node->value = exp1->value <= exp2->value;
						else if(strcmp(op->lexeme, "==")==0)node->value = exp1->value == exp2->value;
						else node->value = exp1->value != exp2->value;break;
					
						break;
					}
				}
				
				node->rule = Exp__INT;

				node->child->token = "INT";
				node->child->value = node->value;
				node->child->terminal = 1;
				node->child->child = NULL;
				node->child->nextSibling = NULL;
			}
			break;
		}
		case Exp__MINUS_Exp:
		case Exp__NOT_Exp: {
			struct Node* exp = node->child->nextSibling;
			const_folding(exp);
			
			if(exp->isConstant) {
				node->isConstant = true;
				
				switch(node->rule) {
					case Exp__MINUS_Exp:node->value = 0-exp->value;break;
					case Exp__NOT_Exp:node->value = !exp->value;break;
				}
				node->rule = Exp__INT;

				node->child->token = "INT";
				node->child->value = node->value;
				node->child->terminal = 1;
				node->child->child = NULL;
				node->child->nextSibling = NULL;
			}
			break;
		}
		case Exp__INT: {
			node->isConstant = true;
			node->value = my_atoi(node->child->lexeme);	
			break;
		}
		default:{ //depth travesal
			struct Node* child = node->child;
			while(child != NULL) {
				const_folding(child);
				child = child->nextSibling;
			}
		}
	}
}

/* 控制流语句优化
 * reduce operation GOTO
 */
void optimize_control() {
	struct InterCodeNode* p = icHead;
	for(;p!= NULL;p=p->next) {
		if(p->code.kind == IFOP) {
			if(p->next != NULL && p->next->next != NULL) {
				struct InterCodeNode* pn = p->next;
				struct InterCodeNode* pnn = pn->next;

				if(pn->code.kind == GOTO && pnn->code.kind == LABELOP) {
					int label1 = p->code.u.ifop.label->u.var_no;
					int label2 = pn->code.u.sigop.op->u.var_no;
					int label3 = pnn->code.u.sigop.op->u.var_no;
					
					if(label1 == label3) {
						// change relop
						char* relop = p->code.u.ifop.relop;
						if(strcmp(relop, ">") == 0)p->code.u.ifop.relop = "<=";
					  	else if(strcmp(relop, "<") == 0)p->code.u.ifop.relop = ">=";
						else if(strcmp(relop, ">=") == 0)p->code.u.ifop.relop = "<";
						else if(strcmp(relop, "<=") == 0)p->code.u.ifop.relop = ">";
						else if(strcmp(relop, "==") == 0)p->code.u.ifop.relop = "!=";
						else p->code.u.ifop.relop = "==";

						// change label no
						// cannot change directly!!!!!!!
						//p->code.u.ifop.label->u.var_no = label2;
						p->code.u.ifop.label = newOperand(LABEL, label2);

						// delete the goto code -- pn
						del_code(pn);
					}
				
					else if(label2 == label3) {
						// delete the goto code
						del_code(pn);
					}
				}
			}
		}
		else if(p->code.kind == GOTO) {
			if(p->next != NULL) {
				struct InterCodeNode* pn = p->next;

				if(pn->code.kind == LABELOP) {
					int label1 = p->code.u.sigop.op->u.var_no;
					int label2 = pn->code.u.sigop.op->u.var_no;

					if(label1 == label2) {
						//delete the goto code
						del_code(p);
						p = pn;
					}
				}
			}
		}

	}
}


void optimize_algebra() {
	struct InterCodeNode* p = icHead;
	for(; p != NULL; p = p->next) {
		OperationKind kind = p->code.kind;
		switch(kind) {
			case ADD: {
							 Operand result = p->code.u.binop.result;
							 Operand op1 = p->code.u.binop.op1;
							 Operand op2 = p->code.u.binop.op2;

							 if(op1->kind == CONSTANT && op1->u.value == 0) {
								 p->code.kind = ASSIGN;
								 p->code.u.assign.left = result;
								 p->code.u.assign.right = op2;
							 }
							 else if(op2->kind == CONSTANT && op2->u.value == 0) {
								 p->code.kind = ASSIGN;
								 p->code.u.assign.left = result;
								 p->code.u.assign.right = op1;
							 }
							 break;
					  }
			case SUB:{
							 Operand result = p->code.u.binop.result;
							 Operand op1 = p->code.u.binop.op1;
							 Operand op2 = p->code.u.binop.op2;

							 if(op2->kind == CONSTANT && op2->u.value == 0) {
								 p->code.kind = ASSIGN;
								 p->code.u.assign.left = result;
								 p->code.u.assign.right = op1;
							 }
							 break;
					 }
			case MUL:{
						 Operand result = p->code.u.binop.result;
						 Operand op1 = p->code.u.binop.op1;
						 Operand op2 = p->code.u.binop.op2;
						
						 if((op1->kind == CONSTANT && op1->u.value == 0) || (op2->kind == CONSTANT && op2->u.value == 0)) {
							p->code.kind = ASSIGN;
							p->code.u.assign.left = result;
							p->code.u.assign.right = newOperand(CONSTANT, 0);
							}
						 else if(op1->kind == CONSTANT) {
							 if(op1->u.value == 1) {
								 p->code.kind = ASSIGN;
								 p->code.u.assign.left = result;
								 p->code.u.assign.right = op2;
							 }
							 else if(op1->u.value == 2) {
								 p->code.kind = ADD;
								 p->code.u.binop.op1 = op2;
							 }
						 }

						 else if(op2->kind == CONSTANT) {
							if(op2->u.value == 1) {
								 p->code.kind = ASSIGN;
								 p->code.u.assign.left = result;
								 p->code.u.assign.right = op1;
							 }
							 else if(op2->u.value == 2) {
								 p->code.kind = ADD;
								 p->code.u.binop.op2 = op1;
							 }
						 }
						 break;
					 }
		case DIVIDE: {
						Operand result = p->code.u.binop.result;
						 Operand op1 = p->code.u.binop.op1;
						 Operand op2 = p->code.u.binop.op2;
						
						 if((op1->kind == CONSTANT && op1->u.value == 0)) {
							p->code.kind = ASSIGN;
							p->code.u.assign.left = result;
							p->code.u.assign.right = newOperand(CONSTANT, 0);
							}

						 else if(op2->kind == CONSTANT) {
							if(op2->u.value == 1) {
								 p->code.kind = ASSIGN;
								 p->code.u.assign.left = result;
								 p->code.u.assign.right = op1;
							 }
						 }
						 break;
					 }
		}
		
	}
}

void remove_equation() {
	struct InterCodeNode* p = icHead;
	while(p != NULL ) {
		bool autoAdd = true;
		if(p->code.kind == ASSIGN) {
			Operand left = p->code.u.assign.left;
			Operand right = p->code.u.assign.right;

			if(left->kind == right->kind && left->u.value == right->u.value) {
				// delete this code
				struct InterCodeNode* d = p;
				p = p->next;
				del_code(d);
				autoAdd = false;
			}
		}
		if(autoAdd) {
			p = p->next;
		}
	}
}

// remove useless LABEL
void remove_extralabel() {
	struct{
		int useless;
		int jumpto;
	} table[1000];
	int index = 0;
	int num = 0;

	int used[1000];
	int used_index = 0;
	int used_num = 0;

	// find the LABEL GOTO
	struct InterCodeNode* p = icHead;
	for( ;p != NULL; p = p->next) {
		if(p->code.kind == LABELOP && p->next != NULL){
			struct InterCodeNode* pn = p->next;

			int label1 = p->code.u.sigop.op->u.var_no;
			if(pn->code.kind == GOTO || pn->code.kind == LABELOP) {
				int label2 = pn->code.u.sigop.op->u.var_no;

				//update
				int i = 0;
				for(;i<num;i++) {
					if(table[i].jumpto == label1)table[i].jumpto = label2;
				}

				table[index].useless = label1;
				table[index].jumpto = label2;

				index = (index + 1)%1000;
				if(num<1000)num++;
			}
		}
	}		
#ifdef DEBUG
	//printf("table:\n");
	//int i = 0;
	//for(;i<num;i++){
	//	printf("%d	%d\n", table[i].useless, table[i].jumpto);
	//}
#endif

	// change the label no
	p = icHead;
	for(; p != NULL; p = p->next ) {
		// goto
		if(p->code.kind == GOTO || p->code.kind == IFOP) {
			int label;
			if(p->code.kind == GOTO) {
				label = p->code.u.sigop.op->u.var_no;

				// check the table
				int i = 0;
				for(;i<num;i++) {
					if( label == table[i].useless) break;
				}
				if(i < num) {
					// substitute
					p->code.u.sigop.op = newOperand(LABEL, table[i].jumpto);
				}

				label = p->code.u.sigop.op->u.var_no;
			}
			else {
				label = p->code.u.ifop.label->u.var_no;

				// check the table
				int i = 0;
				for(;i<num;i++) {
					if( label == table[i].useless) break;
				}
				if(i < num) {
					// substitute
					p->code.u.ifop.label = newOperand(LABEL, table[i].jumpto);
				}

				label = p->code.u.ifop.label->u.var_no;
			}
			// add to used
			int i = 0;
			for(; i<used_num;i++) {
				if(used[i] == label) break;
			}
			if(i == used_num) {
				used[used_index] = label;
				used_index = (used_index+1)%1000;
				if(used_num<1000)used_num++;
			}

		}
	}
#ifdef DEBUG
	//printf("used table:\n");
	//i = 0;
	//for(;i<used_num;i++){
	//	printf("%d\n", used[i]);
	//}
#endif
	// delete useless LABEL
	p = icHead;
	while(p!=NULL) {
		if(p->code.kind ==LABELOP) {
			int label = p->code.u.sigop.op->u.var_no;
			int i = 0;
			for(;i<used_num;i++){
				if(label == used[i])break;
			}
			if(i == used_num) {
				//delete this code
				struct InterCodeNode* d = p;
				p = p->next;
				del_code(d);
			}
			else {
				p = p->next;
			}
		}
		else {
			p = p->next;
		}
	}
}
