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
						p->code.u.ifop.label->u.var_no = label2;

						// delete the 2 codes
						p->next = pnn->next;
						if(pnn->next != NULL)pnn->next->prev = p;
					}
				
					else if(label2 == label3) {
						// delete the 2 codes
						p->next = pnn->next;
						if(pnn->next != NULL)pnn->next->prev = p;
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
						//delete this 2 codes
						if(pre == NULL) {
							p = pn->next;
							icHead = p;
						}
						else {
							p = pn->next;
							pre->next = p;
							pn->next->prev = pre;
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
void clean_temp_var() {
	struct InterCodeNode* pre = NULL;
	struct InterCodeNode* p = icHead;
	while(p!= NULL) {
		bool todelete = false;
		if(p->code.kind == ASSIGN && p->code.u.assign.left->kind == TEMP) {
			Operand temp = p->code.u.assign.left;
			Operand right = p->code.u.assign.right;

			if(p->next != NULL) {
				struct InterCodeNode* pn = p->next;
				OperationKind kind = pn->code.kind;
				if(kind == ASSIGN) {
					if(pn->code.u.assign.left == temp) {
						pn->code.u.assign.left = right;
						todelete = true;
					}
					if(pn->code.u.assign.right == temp){
						pn->code.u.assign.right = right;
						todelete = true;
					}
				}
			}
		}
			
		if(todelete){
			if(pre == NULL) {
				p = p->next;
				icHead = p;
			}
			else {
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
}
