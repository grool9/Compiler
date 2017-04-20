#ifndef __TREE_H__
#define __TREE_H__

typedef enum{
	No_Rule,//terminal token
	Program__ExtDefList, 
	ExtDefList__ExtDef_ExtDefList, 
	ExtDef__Specifier_ExtDecList_SEMI, 
	ExtDef__Specifier_SEMI, 
	ExtDef__Specifier_FunDec_Compst, 
	ExtDecList__VarDec, 
	ExtDecList__VarDec_COMMA_ExtDecList, 
	Specifier__TYPE, 
	Specifier__StructSpecifier, 
	StructSpecifier__STRUCT_OptTag_LC_DefList_RC, 
	StructSpecifier__STRUCT_Tag, 
	OptTag__ID, 
	Tag__ID, 
	VarDec__ID, 
	VarDec__VarDec_LB_int_RB, 
	FunDec__ID_LP_VarList_RP, 
	FunDec__ID_LP_RP, 
	VarList__ParamDec_COMMA_VarList, 
	VarList__ParamDec, 
	ParamDec__Specifier_VarDec, 
	Compst__LC_DefList_StmtList_RC, 
	StmtList__Stmt_StmtList, 
	Stmt__Exp_SEMI, 
	Stmt__Compst, 
	Stmt__RETURN_Exp_SEMI, 
	Stmt__IF_LP_Exp_RP_Stmt, 
	Stmt__IF_LP_Exp_RP_Stmt_else_Stmt, 
	Stmt__WHILE_LP_Exp_RP_Stmt, 
	DefList__Def_DefList, 
	Def__Specifier_DecList_SEMI, 
	DecList__Dec, 
	DecList__Dec_COMMA_DecList, 
	Dec__VarDec, 
	Dec__VarDec_ASSIGNOP_Exp, 
	Exp__Exp_ASSIGNOP_Exp, 
	Exp__Exp_AND_Exp, 
	Exp__Exp_OR_Exp, 
	Exp__Exp_RELOP_Exp,
	Exp__Exp_PLUS_Exp,
	Exp__Exp_MINUS_Exp,
	Exp__Exp_STAR_Exp,
	Exp__Exp_DIV_Exp,
	Exp__LP_Exp_RP,
	Exp__MINUS_Exp,
	Exp__NOT_Exp,
	Exp__ID_LP_Args_RP,
	Exp__ID_LP_RP,
	Exp__Exp_LB_Exp_RB,
	Exp__Exp_DOT_ID,
	Exp__ID,
	Exp__INT,
	Exp__FLOAT,
	Args__Exp_COMMA_Args,
	Args__Exp
}Rule;

typedef enum {_CONST_, _VARIABLE_, _FUNCTION_, _TYPE_} IDKind;

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

//类型
struct Type_
{
	enum {_BASIC_, _ARRAY_, _STRUCTURE_}kind;
	union{
		enum {_INT_, _FLOAT_}basic;
		struct {Type elem; int size; }array;
		FieldList structure;
	}u;
};
//域
struct FieldList_
{
	char* name;
	Type type;
	FieldList tail;
};

struct Node{
	int terminal;
	char* token;
	char* lexeme;// =>name
	int lineno;
	struct Node* child;
	struct Node* nextSibling;

	// lab2
	Rule rule;

	IDKind kind;
	union {
		Type type;
		struct {
			Type retType;//function
			int argc;
			Type* argv;
		};
	};
};

// parse tree root
extern struct Node* root;

// lab1
int my_atoi(char* );
struct Node* insert(Rule rule, char* type, int argc, ...);
void outputTree(struct Node*, int n);

#endif
