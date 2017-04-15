#ifndef __TREE_H__
#define __TREE_H__

#define LEN 32

enum TypeNo{_Program_,_ExtDefList_,_ExtDef_,_ExtDecList_,_Specifier_,_StructSpecifier_, _OptTag_,_Tag_,_VarDec_,_FunDec_,_VarList_, _ParamDec_, _CompSt_,_StmtList_,_Stmt_,_DefList_,_Def_,_DecList_,_Dec_,_Exp_,_Args_,
_ELSE_,_TYPE_,_STRUCT_,_ID_,_LC_,_RC_,_INT_,_FLOAT_,_RETURN_,_IF_,_WHILE_,_COMMA_,_ASSIGNOP_,_OR_,_AND_,_RELOP_,_PLUS_,_MINUS_,_STAR_,_DIV_,_NOT_,_LP_,_RP_,_LB_,_RB_,_DOT_,_SEMI_
};

enum Rule{
	program__extdeflist
};

struct Node{
	int terminal;
	char* type;
	char* lexeme;
	int lineno;
	struct Node* child;
	struct Node* nextSibling;
	enum TypeNo typeno;
	
	enum Rule rule;
};

// parse tree root
extern struct Node* root;

// lab1
struct Node* insert(enum TypeNo no, char* type, int argc, ...);
void outputTree(struct Node*, int n);

#endif
