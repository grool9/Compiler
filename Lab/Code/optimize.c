#include "common.h"

#define DEBUG

void delNodeList(struct Node* node) {
	struct Node* p = node;
	while(p != NULL) {
		struct Node* d = p;
		p = p->nextSibling;
		free(d);
	}
}

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
		default:{
			struct Node* child = node->child;
			while(child != NULL) {
				const_folding(child);
				child = child->nextSibling;
			}
		}
	}
}
