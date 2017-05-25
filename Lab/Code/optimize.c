#include "common.h"

#define DEBUG

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
 * reduce operation GOTO and LABELOP
 */
void optimize_control() {
	struct InterCodeNode* pre = NULL;
	struct InterCodeNode* p = icHead;
	while( p!= NULL) {
		bool autoAdd = true;

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
						p->next = pnn;
						pnn->prev = p;
					}
				
					else if(label2 == label3) {
						// delete the goto code
						p->next = pnn;
						pnn->prev = p;
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
						if(pre == NULL) {
							p = pn;
							icHead = p;
						}
						else {
							p = pn;
							pre->next = p;
							pn->prev = pre;
						}
						autoAdd = false;
					}
				}
			}
		}

		if(autoAdd) {
			pre = p;
			p = p->next;
		}
	}
}

/* 优化临时变量
 */
void replaceTemp(struct InterCodeNode* p, int temp_no, Operand toChange){
	int kind = p->code.kind;
	if(kind == ASSIGN) {
		Operand left = p->code.u.assign.left;
		Operand right = p->code.u.assign.right;
		if(left->kind == TEMP && left->u.var_no == temp_no)p->code.u.assign.left = toChange;
		if(right->kind == TEMP && right->u.var_no == temp_no)p->code.u.assign.right = toChange;
	}
	else if(kind == ADD || kind == SUB || kind == MUL || kind == DIVIDE) {
		Operand result = p->code.u.binop.result;
		Operand op1 = p->code.u.binop.op1;
		Operand op2 = p->code.u.binop.op2;
		if(result->kind == TEMP && result->u.var_no == temp_no)p->code.u.binop.result = toChange;
		if(op1->kind == TEMP && op1->u.var_no == temp_no)p->code.u.binop.op1 = toChange;
		if(op2->kind == TEMP && op2->u.var_no == temp_no)p->code.u.binop.op2 = toChange;
	}
	else if(kind == LABELOP || kind == GOTO || kind == RETURNOP || kind == READ || kind == WRITE || kind == ARG || kind == PARAM) {
		Operand op = p->code.u.sigop.op;
		if(op->kind == TEMP && op->u.var_no == temp_no)p->code.u.sigop.op = toChange;
	}
	else if(kind == IFOP) {
		Operand op1 = p->code.u.ifop.op1;
		Operand op2 = p->code.u.ifop.op2;
		if(op1->kind == TEMP && op1->u.var_no == temp_no)p->code.u.ifop.op1 = toChange;
		if(op2->kind == TEMP && op2->u.var_no == temp_no)p->code.u.ifop.op2 = toChange;
	}
	else if(kind == CALL) {
		Operand op = p->code.u.callop.result;
		if(op->kind == TEMP && op->u.var_no == temp_no)p->code.u.callop.result = toChange;
	}
	else if(kind == DEC) {
		Operand op = p->code.u.decop.op;
		if(op->kind == TEMP && op->u.var_no == temp_no)p->code.u.decop.op = toChange;
	}
}
void clean_temp_var() {
	struct {
		int temp_no;
		Operand toChange;
	}table[1000];
	int index = 0;
	int num = 0;

	struct InterCodeNode* pre = NULL;
	struct InterCodeNode* p = icHead;
	while(p!=NULL) {
		if(p->code.kind == ASSIGN && p->code.u.assign.left->kind == TEMP) {
			table[index].temp_no = p->code.u.assign.left->u.var_no;
			table[index].toChange = p->code.u.assign.right;
			index = (index+1) % 1000;
			if(num<1000)num++;

			// delete this code
			if(pre == NULL) {
				p = p->next;
				icHead = p;
			}
			else{
				p = p->next;
				pre->next = p;
				p->prev = pre;
			}
		}
		else {
			pre = p;
			p = p->next;
		}
	}

	// replace the temp
	p = icHead;
	for(; p!=NULL;p=p->next) {
		int i = 0;
		for(;i<num;i++) {
			replaceTemp(p, table[i].temp_no, table[i].toChange);
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

void remove_equals() {
	struct InterCodeNode* pre = NULL;
	struct InterCodeNode* p = icHead;
	while(p != NULL ) {
		bool autoAdd = true;
		if(p->code.kind == ASSIGN) {
			Operand left = p->code.u.assign.left;
			Operand right = p->code.u.assign.right;

			if(left->kind == right->kind && left->u.value == right->u.value) {
				// delete this code
				if(pre == NULL) {
					p = p->next;
					icHead = p;
				}
				else {
					p = p->next;
					pre->next = p;
					if(p != NULL) p->prev = pre;
				}
				autoAdd = false;
			}
		}
		if(autoAdd) {
			pre = p;
			p = p->next;
		}
	}
}
