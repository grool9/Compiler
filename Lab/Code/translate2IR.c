#include "common.h"

static void depthTraversal(struct Node* );

static void extdef__specifier_extdeclist_semi(struct Node*);
static void extdef__specifier_fundec_compst(struct Node*);
static void extdef__specifier_semi(struct Node*);
static void extdeclist__vardec(struct Node*);
static void extdeclist__vardec_comma_extdeclist(struct Node*);
static void specifier__type(struct Node*);
static void specifier__structspecifier(struct Node*);
static void structspecifier__struct_opttag_lc_deflist_rc(struct Node*);
static void structspecifier__struct_tag(struct Node*);
static void opttag__id(struct Node*);
static void tag__id(struct Node*);
static void vardec__id(struct Node*);
static void vardec__vardec_lb_int_rb(struct Node*);
static void fundec__id_lp_varlist_rp(struct Node*);
static void fundec__id_lp_rp(struct Node*);
static void varlist__paramdec_comma_varlist(struct Node*);
static void varlist__paramdec(struct Node*);
static void paramdec__specifier_vardec(struct Node*);
static void compst__lc_deflist_stmtlist_rc(struct Node*);
static void stmtlist__stmt_stmtlist(struct Node*);
static void stmt__exp_semi(struct Node*);
static void stmt__compst(struct Node*);
static void stmt__return_exp_semi(struct Node*);
static void stmt__ifwhile(struct Node*);
static void deflist__def_deflist(struct Node*);
static void def__specifier_declist_semi(struct Node*);
static void declist__dec(struct Node*);
static void declist__dec_comma_declist(struct Node*);
static void dec__vardec(struct Node*);
static void dec__vardec_assignop_exp(struct Node*);
static void exp__exp_assignop_exp(struct Node*);
static void exp__exp_logicalop_exp(struct Node*);
static void exp__exp_relationop_exp(struct Node*);
static void exp__exp_arithop_exp(struct Node*);
static void exp__lp_exp_rp(struct Node*);
static void exp__minus_exp(struct Node*);
static void exp__not_exp(struct Node*);
static void exp__id_lp_args_rp(struct Node*);
static void exp__exp_lb_exp_rb(struct Node*);
static void exp__exp_dot_id(struct Node*);
static void exp__id(struct Node*);
static void exp__int(struct Node*);
static void exp__float(struct Node*);
static void args__exp_comma_args(struct Node*);
static void args__exp(struct Node*);

static void translate(struct Node* root) {
	if(root==NULL)return;
	
	switch(root->rule){
	/*case ExtDef__Specifier_ExtDecList_SEMI: extdef__specifier_extdeclist_semi(root); break;
	case ExtDef__Specifier_FunDec_Compst: extdef__specifier_fundec_compst(root); break;
	case ExtDef__Specifier_SEMI: extdef__specifier_semi(root); break;
	case ExtDecList__VarDec: extdeclist__vardec(root); break;
	case ExtDecList__VarDec_COMMA_ExtDecList: extdeclist__vardec_comma_extdeclist(root); break;
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
	case Stmt__Exp_SEMI:break;
	case Stmt__Compst: stmt__compst(root); break;
	case Stmt__RETURN_Exp_SEMI: stmt__return_exp_semi(root); break; 
	case Stmt__IF_LP_Exp_RP_Stmt: stmt__ifwhile(root); break; 
	case Stmt__IF_LP_Exp_RP_Stmt_else_Stmt:stmt__ifwhile(root); break;
	case Stmt__WHILE_LP_Exp_RP_Stmt:stmt__ifwhile(root); break;
	case DefList__Def_DefList: deflist__def_deflist(root); break;
	case Def__Specifier_DecList_SEMI:def__specifier_declist_semi(root); break;
	case DecList__Dec:declist__dec(root); break;
	case DecList__Dec_COMMA_DecList:declist__dec_comma_declist(root); break;
	case Dec__VarDec:dec__vardec(root); break;
	case Dec__VarDec_ASSIGNOP_Exp:dec__vardec_assignop_exp(root); break;
	case Exp__Exp_ASSIGNOP_Exp: exp__exp_assignop_exp(root); break;//=
	case Exp__Exp_AND_Exp:
	case Exp__Exp_OR_Exp:exp__exp_logicalop_exp(root); break;
	case Exp__Exp_RELOP_Exp: exp__exp_relationop_exp(root); break;//关系操作
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
	case Exp__ID:exp__id(root); break;*/
	case Exp__INT: exp__int(root); break;
	//case Exp__FLOAT: exp__float(root); break;
	//case Args__Exp_COMMA_Args: args__exp_comma_args(root); break;
	//case Args__Exp:args__exp(root); break;

	default:depthTraversal(root);break;
  	}
}

void generateIR(struct Node* root, char* filename) {
	translate(root);
}

static void depthTraversal(struct Node* root) {
	struct Node* child = root->child;
	while(child != NULL) {
		translate(child);
		child = child->nextSibling;
	}
}

static void exp__int(struct Node* root) {
	//int value = root->child->lexeme;
}
