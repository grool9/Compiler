#include "common.h"

#define DEBUG

void depthTraversal(struct Node* );
bool isTypeEquals(Type t1, Type t2);

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
void stmtlist__stmt_stmtlist(struct Node*);
void stmt__exp_semi(struct Node*);
void stmt__compst(struct Node*);
void stmt__return_exp_semi(struct Node*);
void stmt__if_lp_exp_rp_stmt(struct Node*);
void stmt__if_lp_exp_rp_stmt_else_stmt(struct Node*);
void stmt__while_lp_exp_rp_stmt(struct Node*);
void deflist__def_deflist(struct Node*);
void def__specifier_declist_semi(struct Node*);
void declist__dec(struct Node*);
void declist__dec_comma_declist(struct Node*);
void dec__vardec(struct Node*);
void dec__vardec_assignop_exp(struct Node*);
void exp__exp_assignop_exp(struct Node*);
void exp__exp_op_exp(struct Node*);
void exp__id_lp_args_rp(struct Node*);
void exp__exp_lb_exp_rb(struct Node*);
void exp__exp_dot_id(struct Node*);
void exp__id(struct Node*);
void exp__int(struct Node*);
void exp__float(struct Node*);
void args__exp_comma_args(struct Node*);
void args__exp(struct Node*);

// 遍历语法树
void semanticAnalysis(struct Node* root){
	if(root==NULL)return;
	
	switch(root->rule){
	case ExtDef__Specifier_ExtDecList_SEMI: extdef__specifier_extdeclist_semi(root); break;
	case ExtDef__Specifier_FunDec_Compst: extdef__specifier_fundec_compst(root); break;
	//case ExtDecList__VarDec:break;
	//case ExtDecList__VarDec_COMMA_ExtDecList:break;
	case Specifier__TYPE: specifier__type(root); break;
	case Specifier__StructSpecifier: specifier__structspecifier(root); break;
	case StructSpecifier__STRUCT_OptTag_LC_DefList_RC: structspecifier__struct_opttag_lc_deflist_rc(root); break; 
	case StructSpecifier__STRUCT_Tag: structspecifier__struct_tag(root); break;
	case OptTag__ID: opttag__id(root); break;
	case Tag__ID: tag__id(root); break;
	case VarDec__ID:vardec__id(root); break;
	case VarDec__VarDec_LB_int_RB:vardec__vardec_lb_int_rb(root); break;
	case FunDec__ID_LP_VarList_RP: fundec__id_lp_varlist_rp(root); break;
	case FunDec__ID_LP_RP: fundec__id_lp_rp(root); break;
	case VarList__ParamDec_COMMA_VarList: varlist__paramdec_comma_varlist(root); break;
	case VarList__ParamDec: varlist__paramdec(root); break;
	case ParamDec__Specifier_VarDec: paramdec__specifier_vardec(root); break;
	case Compst__LC_DefList_StmtList_RC: compst__lc_deflist_stmtlist_rc(root); break;
	case StmtList__Stmt_StmtList: stmtlist__stmt_stmtlist(root); break;
	//case Stmt__Exp_SEMI:break;
	//case Stmt__Compst:break;
	case Stmt__RETURN_Exp_SEMI: stmt__return_exp_semi(root); break; 
	//case Stmt__IF_LP_Exp_RP_Stmt:break; 
	//case Stmt__IF_LP_Exp_RP_Stmt_else_Stmt:break;
	//case Stmt__WHILE_LP_Exp_RP_Stmt:break;
	case DefList__Def_DefList: deflist__def_deflist(root); break;
	case Def__Specifier_DecList_SEMI:def__specifier_declist_semi(root); break;
	case DecList__Dec:declist__dec(root); break;
	case DecList__Dec_COMMA_DecList:declist__dec_comma_declist(root); break;
	case Dec__VarDec:dec__vardec(root); break;
	case Dec__VarDec_ASSIGNOP_Exp:dec__vardec_assignop_exp(root); break;
	case Exp__Exp_ASSIGNOP_Exp: exp__exp_assignop_exp(root); break;
	//case Exp__Exp_AND_Exp:break;
	//case Exp__Exp_OR_Exp:break; 
	case Exp__Exp_RELOP_Exp:
	case Exp__Exp_PLUS_Exp:
	case Exp__Exp_MINUS_Exp:
	case Exp__Exp_STAR_Exp:
	case Exp__Exp_DIV_Exp: exp__exp_op_exp(root); break;
	//case Exp__LP_Exp_RP:break;
	//case Exp__MINUS_Exp:break;
	//case Exp__NOT_Exp:break;
	case Exp__ID_LP_Args_RP: exp__id_lp_args_rp(root); break;
	case Exp__ID_LP_RP: exp__id_lp_args_rp(root); break;
	case Exp__Exp_LB_Exp_RB: exp__exp_lb_exp_rb(root); break;
	case Exp__Exp_DOT_ID: exp__exp_dot_id(root); break;
	case Exp__ID:exp__id(root); break;
	case Exp__INT: exp__int(root); break;
	case Exp__FLOAT: exp__float(root); break;
	case Args__Exp_COMMA_Args: args__exp_comma_args(root); break;
	case Args__Exp:args__exp(root); break;
	
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

bool isTypeEquals(Type t1, Type t2) {
	if(t1==NULL && t2==NULL)return true;
	if(t1==NULL || t2==NULL)return false;

	if(t1->kind != t2->kind)return false;

	switch(t1->kind) {
		case _BASIC_:{
						 if(t1->u.basic == t2->u.basic)return true;
						 return false;
					 }
		case _ARRAY_:{
						 if(isTypeEquals(t1->u.array.elem, t2->u.array.elem) && t1->u.array.size == t2->u.array.size)return true;
						 return false;
					 }
		case _STRUCTURE_:{
							 FieldList p = t1->u.structure;
							 FieldList q = t2->u.structure;

							 while(p!=NULL&&q!=NULL) {
								 if(!isTypeEquals(p->type, q->type))return false;
								 p = p->tail;
								 q = q->tail;
							 }
							 if(p!=NULL||q!=NULL)return false;
							 return true;
						 }
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
		root->type->u.basic=_FLOAT_;
	}
	else {
		root->type = NULL;
		//wrong type
	}
}

void specifier__structspecifier(struct Node* root) {
	struct Node* structspecifier = root->child;

	semanticAnalysis(structspecifier);
	root->type = structspecifier->type;
}
// need to modify //////////////////////////////////
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
	id->kind = _VARIABLE_;
	// redefination
	char* name = id->lexeme;
	if(lookupIDTable(name) !=NULL) {
		printf("Error type 3 at Line %d: Redefined variable \"%s\"\n",root->lineno, name);
	}
	else addElement(id);
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
	struct Node* id = root->child;
	struct Node* varlist = id->nextSibling->nextSibling;

	id->kind = _FUNCTION_;
	id->retType = root->retType;
	
	id->argv = (Type*)malloc(sizeof(Type)*MAXARGC);

	varlist->argc = 0; //inh
	varlist->argv = id->argv;//inh
	semanticAnalysis(varlist);

	id->argc = varlist->argc;//syn

	//redefination
	char* name = id->lexeme;
	if(lookupIDTable(name) != NULL){
		printf("Error type 4 at Line %d: Redefined function \"%s\"\n", root->lineno, name);
	}
	else addElement(id);
}

void fundec__id_lp_rp(struct Node* root) {
	struct Node* id = root->child;

	id->retType = root->retType;
	id->kind = _FUNCTION_;

	//redefination
	char* name = id->lexeme;
	if(lookupIDTable(name) != NULL){
		printf("Error type 4 at Line %d: Redefined function \"%s\"\n", root->lineno, name);
	}
	else addElement(id);
}

void varlist__paramdec_comma_varlist(struct Node* root) {
	struct Node* paramdec = root->child;
	struct Node* varlist = paramdec->nextSibling->nextSibling;

	paramdec->argc = root->argc;
	paramdec->argv = root->argv;
	semanticAnalysis(paramdec);

	varlist->argc = paramdec->argc;
	varlist->argv = paramdec->argv;
	semanticAnalysis(varlist);

	root->argc = varlist->argc;//syn
}

void varlist__paramdec(struct Node* root) {
	struct Node* paramdec = root->child;

	paramdec->argc = root->argc;
	paramdec->argv = root->argv;
	semanticAnalysis(paramdec);

	root->argc = paramdec->argc;
}

void paramdec__specifier_vardec(struct Node* root) {
	struct Node* specifier = root->child;
	struct Node* vardec = specifier->nextSibling;

	semanticAnalysis(specifier);

	vardec->type = specifier->type;
	semanticAnalysis(vardec);

	root->argv[root->argc] = specifier->type;
	root->argc++;
}

void compst__lc_deflist_stmtlist_rc(struct Node* root) {
#ifdef DEBUG
	printf("enter compst__lc_deflist_stmtlist_rc\n");
#endif

	//注意deflist和stmtlist可以为空
	struct Node* p = root->child;
	for(;p!=NULL;p = p->nextSibling) {
		p->retType = root->retType;
		semanticAnalysis(p);
	}
}

void stmtlist__stmt_stmtlist(struct Node* root) {
	struct Node* stmt = root->child;
	struct Node* stmtlist = stmt->nextSibling;

	stmt->retType = root->retType;
	semanticAnalysis(stmt);

	if(stmtlist != NULL) {
		stmtlist->retType = root->retType;
		semanticAnalysis(stmtlist);
	}
}

void stmt__return_exp_semi(struct Node* root) {
#ifdef DEBUG
	printf("enter stmt__return_exp_semi()\n");
#endif
	struct Node* exp = root->child->nextSibling;

	semanticAnalysis(exp);

	//check type
	if(!isTypeEquals(root->retType, exp->type)) {
		printf("Error type 8 at Line %d: Type mismatched for return.\n", root->lineno);
	}
}

void deflist__def_deflist(struct Node* root) {
	struct Node* def = root->child;
	struct Node* deflist = def->nextSibling;

	def->type = root->type;
	semanticAnalysis(def);

	if(deflist != NULL) {
		deflist->type = root->type;
		semanticAnalysis(deflist);
	}
}

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

	//等号左边为右值
	if(exp1->kind != _VARIABLE_) {
		printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", root->lineno);
	}

	//等号两边类型匹配
	if(!isTypeEquals(exp1->type, exp2->type)) {
		printf("Error type 5 at Line %d: Type mismatched for assignment.\n", root->lineno);
	}
}

void exp__exp_and_exp(struct Node* root) {
}

void exp__exp_op_exp(struct Node* root) {
	struct Node* exp1 = root->child;
	struct Node* exp2 = exp1->nextSibling->nextSibling;

	semanticAnalysis(exp1);
	semanticAnalysis(exp2);

	// 类型匹配
	if(!isTypeEquals(exp1->type, exp2->type)) {
		printf("Error type 7 at Line %d: Type mismatched for operands.\n", root->lineno);
	}
}

void exp__id_lp_args_rp(struct Node* root) {
	struct Node* id = root->child;
	struct Node* args = id->nextSibling->nextSibling;

	//方程未定义
	char* name = id->lexeme;
	struct Symbol* p = lookupIDTable(name);
	if(p == NULL) {
		printf("Error type 2 at Line %d: Undefined function \"%s\"\n", root->lineno, name);
		return;
	}

	//不是方程
	if(p->kind != _FUNCTION_) {
		printf("Error type 11 at Line %d: \"%s\" is not a fuction.\n", root->lineno, name);
		return;
	}

	//实参
	//对于exp__id_lp_rp 这个实参为空 semanticAnalysis(rp);
	args->argc = 0;
	args->argv = (Type*)malloc(sizeof(Type)*MAXARGC);
	semanticAnalysis(args);

	//参数不对
	bool flag = true;
	if(p->argc != args->argc)flag = false;
	else {
		int i = 0;
		for(; i<args->argc; i++) {
			if(!isTypeEquals(p->argv[i], args->argv[i])) {
					flag = false;
					break;
			}
		}
	}
	if(!flag) {
		printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments.\n", root->lineno, name, root->lineno, name);
	}

	// 综合属性
	root->kind = _CONST_;
	root->retType = p->retType;
}

void args__exp_comma_args(struct Node* root) {
	struct Node* exp = root->child;
	struct Node* args = exp->nextSibling->nextSibling;

	int argc = root->argc;
	Type* argv = root->argv;
	semanticAnalysis(exp);
	
	argv[argc] = exp->type;
	argc++;

	args->argc = argc;
	args->argv = argv;
	semanticAnalysis(args);

	root->argc = args->argc;
}

void args__exp(struct Node* root) {
	struct Node* exp = root->child;

	int argc = root->argc;
	Type* argv = root->argv;
	semanticAnalysis(exp);

	argv[argc] = exp->type;
	argc++;

	root->argc = argc;
}

void exp__exp_lb_exp_rb(struct Node* root) {
	struct Node* exp1 = root->child;
	struct Node* exp2 = exp1->nextSibling->nextSibling;

	semanticAnalysis(exp1);
	semanticAnalysis(exp2);

	root->kind = _VARIABLE_;
	root->type = exp1->type->u.array.elem;
	root->lexeme = exp1->lexeme;

	if(exp1->type->kind != _ARRAY_) {
		printf("Error type 10 at Line %d: \"%s\" is not an array.\n", root->lineno, exp1->lexeme);
		return;
	}
	if(exp2->type->kind != _BASIC_ || exp2->type->u.basic != _INT_) {
		printf("Error type 12 at Line %d: \"%s\" is not an integer.\n", root->lineno, exp2->lexeme);
		return;
	}
}

void exp__exp_dot_id(struct Node* root) {
	printf("Enter exp__exp_dot_id\n");

	struct Node* exp1 = root->child;
	struct Node* id = exp1->nextSibling->nextSibling;

	semanticAnalysis(exp1);

	printf("exp1->type == NULL:%d\n", exp1->type==NULL);

	// check type
	if(exp1->type->kind != _STRUCTURE_) {
		printf("Error type 13 at Line %d: Illegal use of \".\"\n", root->lineno);
	}

	printf("id:%s\n", id->lexeme);
}

void exp__id(struct Node* root) {
#ifdef DEBUG
	printf("enter exp__id\n");
#endif
	struct Node* id = root->child;

	// 符号未定义
	char* name = id->lexeme;
	struct Symbol* p = lookupIDTable(name);
	if(p == NULL) {
		printf("Error type 1 at Line %d: Undefined variable \"%s\"\n", root->lineno, name);
	}
	else {
		root->kind = _VARIABLE_;
		root->type = p->type;
		root->lexeme = p->name;
#ifdef DEBUG
		printf("exp__id: %s\n", p->name);
#endif
		printf("exp__id: root->lexeme:%s \t root->type==NULL:%d\n", root->lexeme, root->type == NULL);			
	}
}

void exp__int(struct Node* root) {
	root->lexeme = root->child->lexeme;
	
	root->kind = _CONST_;

	root->type = (Type)malloc(sizeof(struct Type_));
	root->type->kind = _BASIC_;
	root->type->u.basic = _INT_;
}

void exp__float(struct Node* root) {
	root->lexeme = root->child->lexeme;
		
	root->kind = _CONST_;
	
	root->type = (Type)malloc(sizeof(struct Type_));
	root->type->kind = _BASIC_;
	root->type->u.basic = _FLOAT_;
}
