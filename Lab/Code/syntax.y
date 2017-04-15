%{
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <ctype.h>
#include<stdarg.h>
#include"common.h"

#define YYSTYPE struct Node*

#include "lex.yy.c"

struct Node* root = NULL;

int pasterrline=0;
//char pasttext[LEN]="\0";

%}

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc TYPE STRUCT ID LC RC INT FLOAT RETURN IF WHILE

%token COMMA
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT
%left LP RP LB RB DOT
%left SEMI

%%
Program			:	ExtDefList					{$$=insert(_Program_,"Program",1,$1); root=$$;}
				;
ExtDefList		:	ExtDef ExtDefList			{$$=insert(_ExtDefList_,"ExtDefList",2,$1,$2);}
				|	/* empty */					{$$=NULL;}
				;
ExtDef			:	Specifier ExtDecList SEMI	{$$=insert(_ExtDef_,"ExtDef",3,$1,$2,$3);}
				|	Specifier SEMI				{$$=insert(_ExtDef_,"ExtDef",2,$1,$2);}
				|	Specifier FunDec CompSt		{$$=insert(_ExtDef_,"ExtDef",3,$1,$2,$3);}
				|	error SEMI					{yyerrok;}
				|	Specifier error				{yyerrok;}
				;
ExtDecList		:	VarDec						{$$=insert(_ExtDecList_,"ExtDecList",1,$1);}
				|	VarDec COMMA ExtDecList		{$$=insert(_ExtDecList_,"ExtDecList",3,$1,$2,$3);}
				;
Specifier		:	TYPE						{$$=insert(_Specifier_,"Specifier",1,$1);}
				|	StructSpecifier				{$$=insert(_Specifier_,"Specifier",1,$1);}
				;
StructSpecifier	:	STRUCT OptTag LC DefList RC	{$$=insert(_StructSpecifier_,"StructSpecifier",5,$1,$2,$3,$4,$5);}
				|	STRUCT Tag					{$$=insert(_StructSpecifier_,"StructSpecifier",2,$1,$2);}
				;
OptTag			:	ID							{$$=insert(_OptTag_,"OptTag",1,$1);}
				|	/* empty */					{$$=NULL;}
				;
Tag				:	ID							{$$=insert(_Tag_,"Tag",1,$1);}
				;
VarDec			:	ID							{$$=insert(_VarDec_,"VarDec",1,$1);}
				|	VarDec LB INT RB			{$$=insert(_VarDec_,"VarDec",4,$1,$2,$3,$4);}
				|	VarDec LB error RB			{yyerrok;}
				;
FunDec			:	ID LP VarList RP			{$$=insert(_FunDec_,"FunDec",4,$1,$2,$3,$4);}
				|	ID LP RP					{$$=insert(_FunDec_,"FunDec",3,$1,$2,$3);}
				|	error RP					{yyerrok;}
				|	ID LP error					{yyerrok;}
				;
VarList			:	ParamDec COMMA VarList		{$$=insert(_VarList_,"VarList",3,$1,$2,$3);}
				|	ParamDec					{$$=insert(_VarList_,"VarList",1,$1);}
				;
ParamDec		:	Specifier VarDec			{$$=insert(_ParamDec_,"ParamDec",2,$1,$2);}
				;
CompSt			:	LC DefList StmtList RC		{$$=insert(_CompSt_,"CompSt",4,$1,$2,$3,$4);}
				; 
StmtList		:	Stmt StmtList				{$$=insert(_StmtList_,"StmtList",2,$1,$2);}
				|	/* empty */					{$$=NULL;}
				; 
Stmt			:	Exp SEMI					{$$=insert(_Stmt_,"Stmt",2,$1,$2);}
				|	CompSt						{$$=insert(_Stmt_,"Stmt",1,$1);}
				|	RETURN Exp SEMI				{$$=insert(_Stmt_,"Stmt",3,$1,$2,$3);}
				|	IF LP Exp RP Stmt	%prec LOWER_THAN_ELSE	{$$=insert(_Stmt_,"Stmt",5,$1,$2,$3,$4,$5);}
				|	IF LP Exp RP Stmt ELSE Stmt	{$$=insert(_Stmt_,"Stmt",7,$1,$2,$3,$4,$5,$6,$7);}
				|	WHILE LP Exp RP Stmt		{$$=insert(_Stmt_,"Stmt",5,$1,$2,$3,$4,$5);}
				|	error SEMI					{yyerrok;}
				|	error						{}
				;
DefList			:	Def DefList					{$$=insert(_DefList_,"DefList",2,$1,$2);}
				|	/* empty */					{$$=NULL;}
				;
Def				:	Specifier DecList SEMI		{$$=insert(_Def_,"Def",3,$1,$2,$3);}
				|	Specifier error SEMI		{yyerrok;}
				|	Specifier DecList error		{yyerrok;}
				;
DecList			:	Dec							{$$=insert(_DecList_,"DecList",1,$1);}
				|	Dec COMMA DecList			{$$=insert(_DecList_,"DecList",3,$1,$2,$3);}
				;
Dec				:	VarDec						{$$=insert(_Dec_,"Dec",1,$1);}
				|	VarDec ASSIGNOP Exp			{$$=insert(_Dec_,"Dec",3,$1,$2,$3);}
				;
Exp				:	Exp ASSIGNOP Exp			{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	Exp AND Exp					{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	Exp OR	Exp					{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	Exp RELOP Exp				{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	Exp PLUS Exp				{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	Exp MINUS Exp				{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	Exp STAR Exp				{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	Exp DIV Exp					{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	LP Exp RP					{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	MINUS Exp	%prec UMINUS	{$$=insert(_Exp_,"Exp",2,$1,$2);}
				|	NOT Exp						{$$=insert(_Exp_,"Exp",2,$1,$2);}
				|	ID LP Args RP				{$$=insert(_Exp_,"Exp",4,$1,$2,$3,$4);}
				|	ID LP RP					{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	Exp LB Exp RB				{$$=insert(_Exp_,"Exp",4,$1,$2,$3,$4);}
				|	Exp DOT ID					{$$=insert(_Exp_,"Exp",3,$1,$2,$3);}
				|	ID							{$$=insert(_Exp_,"Exp",1,$1); }
				|	INT							{$$=insert(_Exp_,"Exp",1,$1); }
				|	FLOAT						{$$=insert(_Exp_,"Exp",1,$1);}
				;
Args			:	Exp COMMA Args				{$$=insert(_Args_,"Args",3,$1,$2,$3);}
				|	Exp							{$$=insert(_Args_,"Args",1,$1);}
				;
%%
yyerror(char* msg) {
	isWrong = 1;
	if(yylineno!=pasterrline/*||strcmp(yytext,pasttext)!=0*/){
		if(strlen(yytext)!=0)fprintf(stderr,"Error type B at Line %d: Unexpected token '%s'\n",yylineno,yytext);
		else fprintf(stderr,"Error type B at Line %d: Unexpected $end\n",yylineno);
		pasterrline=yylineno;
		//strcpy(pasttext,yytext);
	}
}

int my_atoi(char* str){
	if(strlen(str)==1||str[0]!='0'){//10
		return atoi(str);
	}
	else if(str[0]=='0'&&(str[1]=='X'||str[1]=='x')){//16
		int sum=0;
		int i=2;
		for(;str[i]!='\0';i++){
			sum*=16;
			if(isdigit(str[i]))sum+=str[i]-'0';
			else if(isupper(str[i]))sum+=str[i]-'A'+10;
			else sum+=str[i]-'a'+10;
		}
		return sum;
	}
	else{//8
		int sum=0;
		int i=1;
		for(;str[i]!='\0';i++){
			sum*=8;
			sum+=str[i]-'0';
		}
		return sum;
	}
}

void outputTree(struct Node* root,int n){
	if(root==NULL)return;
	
	struct Node* p=root;
	for(;p!=NULL;p=p->nextSibling){
		int i=0;
		for(;i<n;i++)printf("  ");
		
		if(!p->terminal){
			printf("%s (%d)\n",p->type,p->lineno);
		}
		else {
			printf("%s",p->type);
			if(strcmp(p->type,"ID")==0){
				printf(": %s", p->lexeme);
			}
			else if(strcmp(p->type, "TYPE")==0) {
				printf(": %s", p->lexeme);
			}
			else if(strcmp(p->type, "INT")==0) {
				printf(": %d", my_atoi(p->lexeme));//10
			}
			else if(strcmp(p->type, "FLOAT")==0) {
				printf(": %f", atof(p->lexeme));
			}
			printf("\n");
		}

		outputTree(p->child,n+1);
	}
}

struct Node* insert(enum TypeNo no, char* type, int argc, ...){
	struct Node* head=(struct Node*)malloc(sizeof(struct Node));
	head->terminal=0;
	head->type=(char*)malloc(sizeof(char)*LEN); 
	head->lexeme=NULL;
	strcpy(head->type,type);
	head->child=NULL;
	head->nextSibling=NULL;

	head->typeno=no;

	struct Node* current=head;

	va_list p;
	va_start(p, argc);
	current->child=va_arg(p, struct Node*);//not null
	current=current->child;

	head->lineno=current->lineno;

	int i=1;
	for(;i<argc;i++){
		struct Node* temp=va_arg(p,struct Node*);
		if(temp!=NULL){
			current->nextSibling=temp;
			current=current->nextSibling;
		}
	}
	
	va_end(p);

	return head;
}
