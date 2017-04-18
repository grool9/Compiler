#include "common.h"

int semanticErrorCount = 0;

void depthTraversal(struct Node* );
void __specifier_(struct Node*);

void extdef__specifier_extdeclist_semi(struct Node*);
void extdef__specifier_fundec_compst(struct Node*);
void extdeclist__vardec(struct Node*);
void extdeclist__vardec_comma_extdeclist(struct Node*);
void specifier__type(struct Node*);
void specifier__structspecifier(struct Node*);
void structspecifier__struct_opttag_lc_deflist_rc(struct Node*);
void structspecifier__struct_tag(struct Node*);
void opttag__id(struct Node*);
void tag__id(struct Node*);
void vardec__id(struct Node*);
void vardec__vardec_lb_int_rb(struct Node*);
void fundec__id_lp_varlist_rp(struct Node*);
void fundec__id_lp_rp(struct Node*);
void varlist__paramdec_comma_varlist(struct Node*);
void varlist__paramdec(struct Node*);
void paramdec__specifier_vardec(struct Node*);
void compst__lc_deflist_stmtlist_rc(struct Node*);
void stmt__exp_semi(struct Node*);
void stmt__compst(struct Node*);
void stmt__return_exp_semi(struct Node*);
void stmt__if_lp_exp_rp_stmt(struct Node*);
void stmt__if_lp_exp_rp_stmt_else_stmt(struct Node*);
void stmt__while_lp_exp_rp_stmt(struct Node*);
void def__specifier_declist_semi(struct Node*);
void declist__dec(struct Node*);
void declist__dec_comma_declist(struct Node*);
void dec__vardec(struct Node*);
void dec__vardec_assignop_exp(struct Node*);
void exp__id(struct Node*);

// print error msg
void semanticError(int lineno, char* msg) {
	semanticErrorCount ++;
	printf("[Error type %d] line : %d  %s\n", semanticErrorCount, lineno, msg);
}

// 遍历语法树
void semanticAnalysis(struct Node* root){
	if(root==NULL)return;
	
	switch(root->rule){
	//case ExtDef__Specifier_ExtDecList_SEMI:break;
	//case ExtDef__Specifier_FunDec_Compst:break;
	//case ExtDecList__VarDec:break;
	//case ExtDecList__VarDec_COMMA_ExtDecList:break;
	//case Specifier__TYPE:break;
	//case Specifier__StructSpecifier:break;
	//case StructSpecifier__STRUCT_OptTag_LC_DefList_RC:break; 
	//case StructSpecifier__STRUCT_Tag:break;
	//case OptTag__ID:break;
	//case Tag__ID:break;
	case VarDec__ID:vardec__id(root); break;
	case VarDec__VarDec_LB_int_RB:vardec__vardec_lb_int_rb(root); break;
	//case FunDec__ID_LP_VarList_RP:break;
	//case FunDec__ID_LP_RP:break;
	//case VarList__ParamDec_COMMA_VarList:break;
	//case VarList__ParamDec:break;
	//case ParamDec__Specifier_VarDec:break;
	//case Compst__LC_DefList_StmtList_RC:break; 
	//case Stmt__Exp_SEMI:break;
	//case Stmt__Compst:break;
	//case Stmt__RETURN_Exp_SEMI:break; 
	//case Stmt__IF_LP_Exp_RP_Stmt:break; 
	//case Stmt__IF_LP_Exp_RP_Stmt_else_Stmt:break;
	//case Stmt__WHILE_LP_Exp_RP_Stmt:break;
	case Def__Specifier_DecList_SEMI:def__specifier_declist_semi(root); break;
	case DecList__Dec:declist__dec(root); break;
	case DecList__Dec_COMMA_DecList:declist__dec_comma_declist(root); break;
	case Dec__VarDec:dec__vardec(root); break;
	case Dec__VarDec_ASSIGNOP_Exp:dec__vardec_assignop_exp(root); break;
	//case Exp__Exp_ASSIGNOP_Exp:break;
	//case Exp__Exp_AND_Exp:break;
	//case Exp__Exp_OR_Exp:break; 
	//case Exp__Exp_RELOP_Exp:break;
	//case Exp__Exp_PLUS_Exp:break;
	//case Exp__Exp_MINUS_Exp:break;
	//case Exp__Exp_STAR_Exp:break;
	//case Exp__Exp_DIV_Exp:break;
	//case Exp__LP_Exp_RP:break;
	//case Exp__MINUS_Exp:break;
	//case Exp__NOT_Exp:break;
	//case Exp__ID_LP_Args_RP:break;
	//case Exp__ID_LP_RP:break;
	//case Exp__Exp_LB_Exp_RB:break;
	//case Exp__Exp_DOT_ID:break;
	case Exp__ID:exp__id(root); break;
	//case Exp__INT:break;
	//case Exp__FLOAT:break;
	//case Args__Exp_COMMA_Args:break;
	//case Args__Exp:break;
	
	default:depthTraversal(root);break;
	}
}

//从左到右深度遍历
void depthTraversal(struct Node* root) {
	struct Node* child = root->child;
	while(child!=NULL) {
		semanticAnalysis(child);
		child = child->nextSibling;
	}
}

void __specifier_(struct Node* root) {
	struct Node* specifier = root->child;
	struct Node* other = specifier->nextSibling;
	semanticAnalysis(specifier);

	Type t= specifier->type;
	while(other!=NULL) {
		//inh
		other->type = t;
		semanticAnalysis(other);
		other = other->nextSibling;
	}
}

void extdef__specifier_extdeclist_semi(struct Node* root) {
	struct Node* specifier = root->child;
	struct Node* extdeclist = specifier->nextSibling;

	semanticAnalysis(specifier);

	extdeclist->type = specifier->type;
	semanticAnalysis(extdeclist);
}

void extdef__specifier_fundec_compst(struct Node* root) {
	struct Node* specifier = root->child;
	struct Node* fundec = specifier->nextSibling;
	struct Node* compst = fundec->nextSibling;

	semanticAnalysis(specifier);

	fundec->type = specifier->type;
	compst->type = specifier->type;

	semanticAnalysis(fundec);
	semanticAnalysis(compst);
}

void extdeclist__vardec(struct Node* root) {
	struct Node* vardec = root->child;
	
	vardec->type = root->type;
	semanticAnalysis(vardec);
}

void extdeclist__vardec_comma_extdeclist(struct Node* root) {
	struct Node* vardec = root->child;
	struct Node* extdeclist = vardec->nextSibling->nextSibling;

	vardec->type = root->type;
	extdeclist->type = root->type;

	semanticAnalysis(vardec);
	semanticAnalysis(extdeclist);
}

void specifier__type(struct Node* root) {
	struct Node* nodetype = root->child;
	char* text = nodetype->lexeme;

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

void specifier__structspecifier(struct Node* root) {
	struct Node* structspecifier = root->child;

	semanticAnalysis(structspecifier);
	root->type = structspecifier->type;
}

void structspecifier__struct_opttag_lc_deflist_rc(struct Node* root) {
	struct Node* opttag = root->child->nextSibling;
	struct Node* deflist = opttag->nextSibling->nextSibling;

	semanticAnalysis(opttag);
	root->lexeme = opttag->lexeme;

	Type t = (Type)malloc(sizeof(struct Type_));
	t->kind = _STRUCTURE_;
	t->u.structure = (FieldList)malloc(sizeof(struct FieldList_));
	deflist->type = t;

	semanticAnalysis(deflist);

	root->type = deflist->type;
}

void structspecifier__struct_tag(struct Node* root) {
	struct Node* tag = root->child->nextSibling;

	semanticAnalysis(tag);
	root->lexeme = tag->lexeme;

	Type t = (Type)malloc(sizeof(struct Type_));
	t->kind = _STRUCTURE_;
	t->u.structure = NULL;
	root->type = t;
}

void opttag__id(struct Node* root) {
	struct Node* id = root->child;

	root->lexeme = id->lexeme;
}

void tag__id(struct Node* root) {
	struct Node* id = root->child;
	root->lexeme = id->lexeme;
}

void vardec__id(struct Node* root) {
	struct Node* id = root->child;
	
	id->type = root->type;
	addElement(id);
#ifdef DEBUG
	printf("vardec__id:%s\n",id->lexeme);
#endif
}

void vardec__vardec_lb_int_rb(struct Node* root) {
	struct Node* vardec = root->child;
	int size = my_atoi(vardec->nextSibling->nextSibling->lexeme);

	Type t = (Type)malloc(sizeof(struct Type_));
	t->kind = _ARRAY_;
	t->u.array.elem = root->type;
	t->u.array.size = size;

	vardec->type = t;
	semanticAnalysis(vardec);

	root->type = vardec->type;
}

void fundec__id_lp_varlist_rp(struct Node* root) {

}

void fundec__id_lp_rp(struct Node* root) {
}

void varlist__paramdec_comma_varlist(struct Node* root) {

}

void varlist__paramdec(struct Node* root) {
}

void stmt__return_exp_semi(struct Node* root) {
	//check type
}

//......................

void def__specifier_declist_semi(struct Node* root) {
	struct Node* specifier = root->child;
	struct Node* declist = specifier->nextSibling;

	semanticAnalysis(specifier);

	declist->type = specifier->type;
	semanticAnalysis(declist);
}

void declist__dec(struct Node* root) {
	struct Node* dec = root->child;
	dec->type = root->type;

	semanticAnalysis(dec);
}

void declist__dec_comma_declist(struct Node* root) {
	struct Node* dec = root->child;
	struct Node* declist = dec->nextSibling->nextSibling;

	dec->type = root->type;
	declist->type = root->type;

	semanticAnalysis(dec);
	semanticAnalysis(declist);
}

void dec__vardec(struct Node* root) {
	struct Node* vardec = root->child;
	
	vardec->type = root->type;
	semanticAnalysis(vardec);
}

void dec__vardec_assignop_exp(struct Node* root) {
	struct Node* vardec = root->child;
	struct Node* exp = vardec->nextSibling->nextSibling;

	vardec->type = root->type;
	semanticAnalysis(vardec);

	semanticAnalysis(exp);
	//check type
	//if(exp->type != vardec->type){
	//	semanticError(vardec->lineno, "wrong type");
	//}

}

void exp__exp_assignop_exp(struct Node* root) {
	struct Node* exp1 = root->child;
	struct Node* exp2 = exp1->nextSibling->nextSibling;
	
	semanticAnalysis(exp1);
	semanticAnalysis(exp2);

	//等号两边类型匹配
	if(exp1->type != exp2->type) {
	}
}

void exp__exp_and_exp(struct Node* root) {
}

void exp__id(struct Node* root) {
	struct Node* id = root->child;

	// check
	char* text = id->lexeme;
	if(lookupIDTable(text)==NULL) {
		semanticError(root->lineno, text);
	}
}

//..................
