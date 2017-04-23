#include "common.h"

//#define DEBUG

bool isBuildingStruct = false;
int top = -1;

void depthTraversal(struct Node* );
bool isTypeEquals(Type t1, Type t2);

void extdef__specifier_extdeclist_semi(struct Node*);
void extdef__specifier_fundec_compst(struct Node*);
void extdef__specifier_semi(struct Node*);
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
void stmt__other(struct Node*);
void deflist__def_deflist(struct Node*);
void def__specifier_declist_semi(struct Node*);
void declist__dec(struct Node*);
void declist__dec_comma_declist(struct Node*);
void dec__vardec(struct Node*);
void dec__vardec_assignop_exp(struct Node*);
void exp__exp_assignop_exp(struct Node*);
void exp__exp_logicalop_exp(struct Node*);
void exp__exp_arithop_exp(struct Node*);
void exp__lp_exp_rp(struct Node*);
void exp__minus_exp(struct Node*);
void exp__not_exp(struct Node*);
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
	case ExtDef__Specifier_SEMI: extdef__specifier_semi(root); break;
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
	case Stmt__IF_LP_Exp_RP_Stmt: stmt__other(root); break; 
	case Stmt__IF_LP_Exp_RP_Stmt_else_Stmt:stmt__other(root); break;
	case Stmt__WHILE_LP_Exp_RP_Stmt:stmt__other(root); break;
	case DefList__Def_DefList: deflist__def_deflist(root); break;
	case Def__Specifier_DecList_SEMI:def__specifier_declist_semi(root); break;
	case DecList__Dec:declist__dec(root); break;
	case DecList__Dec_COMMA_DecList:declist__dec_comma_declist(root); break;
	case Dec__VarDec:dec__vardec(root); break;
	case Dec__VarDec_ASSIGNOP_Exp:dec__vardec_assignop_exp(root); break;
	case Exp__Exp_ASSIGNOP_Exp: exp__exp_assignop_exp(root); break;
	case Exp__Exp_AND_Exp:
	case Exp__Exp_OR_Exp:
	case Exp__Exp_RELOP_Exp: exp__exp_logicalop_exp(root); break;
	case Exp__Exp_PLUS_Exp:
	case Exp__Exp_MINUS_Exp:
	case Exp__Exp_STAR_Exp:
	case Exp__Exp_DIV_Exp: exp__exp_arithop_exp(root); break;
	case Exp__LP_Exp_RP: exp__lp_exp_rp(root); break;
	case Exp__MINUS_Exp: exp__minus_exp(root); break;
	case Exp__NOT_Exp: exp__not_exp(root); break;
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
	printf("enter extdef__specifier_extdeclist_semi()\n");

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

void extdef__specifier_semi(struct Node* root) {
	struct Node* specifier = root->child;
	semanticAnalysis(specifier);
}

void extdeclist__vardec(struct Node* root) {
	printf("enter extdeclist__vardec\n");
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

void structspecifier__struct_opttag_lc_deflist_rc(struct Node* root) {
#ifdef DEBUG
	printf("Enter structspecifer__struct_opttag_lc_deflist_rc\n");
#endif
	isBuildingStruct = true;

	struct Node* opttag = root->child->nextSibling;
	struct Node* deflist = opttag->nextSibling->nextSibling;

	// structspecifier idkind
	root->idkind = _TYPE_;
	
	// structspecifier name
	semanticAnalysis(opttag);
	root->lexeme = opttag->lexeme;

	// check table
	char* name = root->lexeme;
	if(lookupIDTable(root) != NULL) {
		printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", root->lineno, name);
		return;
	}

	top++;
	structinfo[top].structName = root->lexeme;
	structinfo[top].structType = (Type)malloc(sizeof(struct Type_));
	structinfo[top].structType->kind = _STRUCTURE_; // type kind
	structinfo[top].structType->u.structure = NULL;

	semanticAnalysis(deflist);
	
	//structspecifier type
	root->type = structinfo[top].structType;

	// add 2 symbol table
	addElement(root);

	// re-initial
	isBuildingStruct = false;
	top--;
}

void structspecifier__struct_tag(struct Node* root) {
	struct Node* tag = root->child->nextSibling;

	semanticAnalysis(tag);
	char* name = tag->lexeme;

	struct Symbol* sym = lookupIDTable(tag);
	if(sym == NULL) {
		printf("Error type 17 at Line %d: Undefined structure \"%s\".\n", root->lineno, name);
		return;
	}

	root->type = sym->type;
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
	char* name = id->lexeme;

	if(isBuildingStruct) {
		FieldList p = (FieldList)malloc(sizeof(struct FieldList_));
		p->name = name;
		p->type = id->type;
		p->tail = NULL;

		FieldList current = structinfo[top].structType->u.structure;
		if(current == NULL) {
			structinfo[top].structType->u.structure = p;
		}
		else {
			while(current->tail != NULL)current = current->tail;
			current->tail = p;
		}
	}
	
	// redefination
	struct Symbol* sym = lookupIDTable(id);
	if(sym == NULL) {
		if(isBuildingStruct)addField(id, structinfo[top].structName);
		else addElement(id);
	}
	else{
		if(isBuildingStruct && strcmp(sym->addr, structinfo[top].structName)==0) {
			printf("Error type 15 at Line %d: Redefined field \"%s\".\n", root->lineno, name);
		}
		else {
			printf("Error type 3 at Line %d: Redefined variable \"%s\"\n",root->lineno, name);
		}
	}
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
}

void fundec__id_lp_varlist_rp(struct Node* root) {
	struct Node* id = root->child;
	struct Node* varlist = id->nextSibling->nextSibling;

	id->idkind = _FUNCTION_;
	id->retType = root->retType;
	
	id->argv = (Type*)malloc(sizeof(Type)*MAXARGC);

	varlist->argc = 0; //inh
	varlist->argv = id->argv;//inh
	semanticAnalysis(varlist);

	id->argc = varlist->argc;//syn

	//redefination
	char* name = id->lexeme;
	struct Symbol* p = lookupIDTable(id);
	if(p != NULL && p->idkind == _FUNCTION_){
		printf("Error type 4 at Line %d: Redefined function \"%s\"\n", root->lineno, name);
	}
	else addElement(id);
}

void fundec__id_lp_rp(struct Node* root) {
	struct Node* id = root->child;

	id->retType = root->retType;
	id->idkind = _FUNCTION_;

	//redefination
	char* name = id->lexeme;
	if(lookupIDTable(id) != NULL){
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

	if(root->retType!=NULL){
		printf("stmt return type: %d\n",root->retType->u.basic);
	}
	if(exp->type !=NULL) {
		printf("exp type: %d\n", exp->type->u.basic);
	}

	//check type
	if(!isTypeEquals(root->retType, exp->type)) {
		printf("Error type 8 at Line %d: Type mismatched for return.\n", root->lineno);
	}
}

void stmt__other(struct Node* root) {
	struct Node* p = root->child;

	while(p != NULL){
		p->type = root->type;
		semanticAnalysis(p);

		p = p->nextSibling;
	}
}



void deflist__def_deflist(struct Node* root) {
#ifdef DEBUG
	printf("enter deflist__def_deflist\n");
#endif
	struct Node* def = root->child;
	struct Node* deflist = def->nextSibling;

	semanticAnalysis(def);

	if(deflist != NULL) {
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
#ifdef DEBUG
	printf("enter declist__dec()\n");
#endif
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
	//等号两边类型匹配
	if(!isTypeEquals(vardec->type, exp->type)) {
         printf("Error type 5 at Line %d: Type mismatched for assignment.\n",root->lineno);
     }
}

void exp__exp_assignop_exp(struct Node* root) {
	struct Node* exp1 = root->child;
	struct Node* exp2 = exp1->nextSibling->nextSibling;
	
	semanticAnalysis(exp1);
	semanticAnalysis(exp2);

	//等号左边为右值
	if(exp1->idkind != _VARIABLE_) {
		printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", root->lineno);
	}

	//等号两边类型匹配
	if(!isTypeEquals(exp1->type, exp2->type)) {
		printf("Error type 5 at Line %d: Type mismatched for assignment.\n", root->lineno);
	}
}

void exp__exp_logicalop_exp(struct Node* root) {
	struct Node* exp1 = root->child;
	struct Node* exp2 = exp1->nextSibling->nextSibling;

	semanticAnalysis(exp1);
	semanticAnalysis(exp2);

	if(exp1->type->kind != _BASIC_ || exp1->type->u.basic != _INT_) {
		printf("Error type 7 at Line %d: operand \"%s\" must be integer.\n", root->lineno, exp1->lexeme);
	}
	if(exp2->type->kind != _BASIC_ || exp2->type->u.basic != _INT_) {
         printf("Error type 7 at Line %d: operand \"%s\" must be integer.\n",     root->lineno, exp2->lexeme);
     }

	root->type = exp1->type;
}

void exp__exp_arithop_exp(struct Node* root) {
	struct Node* exp1 = root->child;
	struct Node* exp2 = exp1->nextSibling->nextSibling;

	semanticAnalysis(exp1);
	semanticAnalysis(exp2);


	if(exp1->type->kind != _BASIC_) {
		printf("Error type 7 at Line %d: operand \"%s\" must be integer or float.\n",root->lineno, exp1->lexeme);
     }
	if(exp2->type->kind != _BASIC_) {
		printf("Error type 7 at Line %d: operand \"%s\" must be integer or float.\n",root->lineno, exp1->lexeme);
     }

	// 类型匹配
	if(!isTypeEquals(exp1->type, exp2->type)) {
		printf("Error type 7 at Line %d: Type mismatched for operands.\n", root->lineno);
	}

	root->type = exp1->type;
}

void exp__lp_exp_rp(struct Node* root) {
	struct Node* exp1 = root->child->nextSibling;
	semanticAnalysis(exp1);

	root->type = exp1->type;
}

void exp__minus_exp(struct Node* root) {
	struct Node* exp1 = root->child->nextSibling;
	semanticAnalysis(exp1);
	if(exp1->type->kind != _BASIC_) {
		printf("Error type 7 at Line %d: operand \"%s\" must be integer or float.\n",root->lineno, exp1->lexeme);
     }

	root->type = exp1->type;
}

void exp__not_exp(struct Node* root) {
	struct Node* exp1 = root->child->nextSibling;
	semanticAnalysis(exp1);
	if(exp1->type->kind != _BASIC_ || exp1->type->u.basic != _INT_) {
		printf("Error type 7 at Line %d: operand \"%s\" must be integer.\n",root->lineno, exp1->lexeme);
	}

	root->type = exp1->type;
}

void exp__id_lp_args_rp(struct Node* root) {
	struct Node* id = root->child;
	struct Node* args = id->nextSibling->nextSibling;

	//方程未定义
	char* name = id->lexeme;
	struct Symbol* p = lookupIDTable(id);
	if(p == NULL) {
		printf("Error type 2 at Line %d: Undefined function \"%s\"\n", root->lineno, name);
		return;
	}

	//不是方程
	if(p->idkind != _FUNCTION_) {
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
	root->idkind = _CONST_;
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

	root->idkind = _VARIABLE_;
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
#ifdef DEBUG
	printf("Enter exp__exp_dot_id\n");
#endif
	struct Node* exp1 = root->child;
	struct Node* id = exp1->nextSibling->nextSibling;

	semanticAnalysis(exp1);

	// check type
	if(exp1->type->kind != _STRUCTURE_) {
		printf("Error type 13 at Line %d: Illegal use of \".\"\n", root->lineno);
	}

	// 结构中的域
	FieldList p = exp1->type->u.structure;
	bool found = false;
	while( p!=NULL) {
		if(strcmp(p->name, id->lexeme)==0){
				found = true;
				root->type = p->type;
				break;
		}
		p = p->tail;
	}
	if(found == false) {
		printf("Error type 14 at Line %d: Non-existent field \"%s\".\n", root->lineno, id->lexeme);
	}
}

void exp__id(struct Node* root) {
#ifdef DEBUG
	printf("enter exp__id\n");
#endif
	struct Node* id = root->child;

	// 符号未定义
	char* name = id->lexeme;
	struct Symbol* p = lookupIDTable(id);
	if(p == NULL) {
		printf("Error type 1 at Line %d: Undefined variable \"%s\"\n", root->lineno, name);
	}
	else {
		root->idkind = _VARIABLE_;
		root->type = p->type;
		root->lexeme = p->name;
	}
}

void exp__int(struct Node* root) {
	root->lexeme = root->child->lexeme;
	
	root->idkind = _CONST_;

	root->type = (Type)malloc(sizeof(struct Type_));
	root->type->kind = _BASIC_;
	root->type->u.basic = _INT_;
}

void exp__float(struct Node* root) {
	root->lexeme = root->child->lexeme;
		
	root->idkind = _CONST_;
	
	root->type = (Type)malloc(sizeof(struct Type_));
	root->type->kind = _BASIC_;
	root->type->u.basic = _FLOAT_;
}
