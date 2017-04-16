#include "common.h"

int semanticErrorCount = 0;

void extdef__specifier_extdeclist_semi(struct Node*);
void extdef__specifier_semi(struct Node*);
void extdef__specifier_fundec_compst(struct Node*);

// print error msg
void semanticError(int lineno, char* msg) {
	semanticErrorCount ++;
	printf("[Error type %d] line : %d  %s\n", semanticErrorCount, lineno, msg);
}

// 从左到右深度优先遍历语法树
void traversalTree(struct Node* root){
	if(root==NULL)return;
	
	switch(root->rule){
	case Program__ExtDefList:break;
	case ExtDefList__ExtDef_ExtDefList:break;
	case ExtDef__Specifier_ExtDecList_SEMI:extdef__specifier_extdeclist_semi(root);break; 
	case ExtDef__Specifier_SEMI:extdef__specifier_semi(root);break;
	case ExtDef__Specifier_FunDec_Compst:extdef__specifier_fundec_compst(root);break;
	case ExtDecList__VarDec:break;
	case ExtDecList__VarDec_COMMA_ExtDecList:break;
	case Specifier__TYPE:break;
	case Specifier__StructSpecifier:break;
	case StructSpecifier__STRUCT_OptTag_LC_DefList_RC:break; 
	case StructSpecifier__STRUCT_Tag:break;
	case OptTag__ID:break;
	case Tag__ID:break;
	case VarDec__ID:break;
	case VarDec__VarDec_LB_int_RB:break;
	case FunDec__ID_LP_VarList_RP:break;
	case FunDec__ID_LP_RP:break;
	case VarList__ParamDec_COMMA_VarList:break;
	case VarList__ParamDec:break;
	case ParamDec__Specifier_VarDec:break; 
	case Compst__LC_DefList_StmtList_RC:break; 
	case StmtList__Stmt_StmtList:break;
	case Stmt__Exp_SEMI:break;
	case Stmt__Compst:break;
	case Stmt__RETURN_Exp_SEMI:break; 
	case Stmt__IF_LP_Exp_RP_Stmt:break; 
	case Stmt__IF_LP_Exp_RP_Stmt_else_Stmt:break;
	case Stmt__WHILE_LP_Exp_RP_Stmt:break;
	case DefList__Def_DefList:break;
	case Def__Specifier_DecList_SEMI:break;
	case DecList__Dec:break;
	case DecList__Dec_COMMA_DecList:break;
	case Dec__VarDec:break;
	case Dec__VarDec_ASSIGNOP_Exp:break;
	case Exp__Exp_ASSIGNOP_Exp:break;
	case Exp__Exp_AND_Exp:break;
	case Exp__Exp_OR_Exp:break; 
	case Exp__Exp_RELOP_Exp:break;
	case Exp__Exp_PLUS_Exp:break;
	case Exp__Exp_MINUS_Exp:break;
	case Exp__Exp_STAR_Exp:break;
	case Exp__Exp_DIV_Exp:break;
	case Exp__LP_Exp_RP:break;
	case Exp__MINUS_Exp:break;
	case Exp__NOT_Exp:break;
	case Exp__ID_LP_Args_RP:break;
	case Exp__ID_LP_RP:break;
	case Exp__Exp_LB_Exp_RB:break;
	case Exp__Exp_DOT_ID:break;
	case Exp__ID:break;
	case Exp__INT:break;
	case Exp__FLOAT:break;
	case Args__Exp_COMMA_Args:break;
	case Args__Exp:break;
	default:break;
	}
}


void extdef__specifier_extdeclist_semi(struct Node* root) {
	struct Node* specifier = root->child;
	struct Node* extdeclist = specifier->nextSibling;
	
	traversalTree(specifier);

	extdeclist->type = specifier->type;
	traversalTree(extdeclist);
}

void extdeclist__vardec(struct Node* root) {
	struct Node* vardec = root->child;
	
	vardec->type = root->type;
	traversalTree(vardec);
}

void extdeclist__vardec_comma_extdeclist(struct Node* root) {
	struct Node* vardec = root->child;
	struct Node* extdeclist = vardec->nextSibling->nextSibling;

	vardec->type = root->type;
	extdeclist->type = root->type;

	traversalTree(vardec);
	traversalTree(extdeclist);
}

void vardec__id(struct Node* root) {
	struct Node* id = root->child;

	add2Table(id);
}

void vardec__vardec_lb_int_rb(struct Node* root) {
	struct Node* vardec = root->child;
	int size = vardec->nextSibling->nextSibling;

	Type t = (Type)malloc(sizeof(struct Type_));
	t->kind = _ARRAY_;
	t->u.array.elem = root->type;//
	t->u.array.size = size;

	vardec->type = t;
	traversalTree(vardec);
}

void specifier__type(struct Node* root) {
	char* text = root->lexeme;
	if(strcmp(text,"int")==0){
			root->type = (Type)malloc(sizeof(struct Type_));
			root->type->kind=_BASIC_;
			root->type->u.basic=_INT_;
	}
	else if(strcmp(text,"float")==0){
		root->type = (Type)malloc(sizeof(struct Type_));
		root->type->kind=_BASIC_;
		root->type->u.basic=_INT_;
	}
	else {
		root->type = NULL;
		semanticError(root->lineno, "wrong type");
	}
}

void extdef__specifier_semi(struct Node* root) {
}
void extdef__specifier_fundec_compst(struct Node* root) {
	struct Node* specifier = root->child;
	traversalTree(specifier);
	
	struct Node* fundec = specifier->nextSibling;
	fundec->type = specifier->type;
	traversalTree(fundec);	

	struct Node* compst = fundec->nextSibling;
	compst->type = specifier->type;
	traversalTree(compst);
}
