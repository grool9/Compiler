%{
#define YYSTYPE struct Node*

#include "lex.yy.c"

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
Program			:	ExtDefList					{$$=insert("Program",1,$1); root=$$;}
				;
ExtDefList		:	ExtDef ExtDefList			{$$=insert("ExtDefList",2,$1,$2);}
				|	/* empty */					{$$=NULL;}
				;
ExtDef			:	Specifier ExtDecList SEMI	{$$=insert("ExtDef",3,$1,$2,$3);}
				|	Specifier SEMI				{$$=insert("ExtDef",2,$1,$2);}
				|	Specifier FunDec CompSt		{$$=insert("ExtDef",3,$1,$2,$3);}
				|	error SEMI					{yyerrok;}
				|	Specifier error				{yyerrok;}
				;
ExtDecList		:	VarDec						{$$=insert("ExtDecList",1,$1);}
				|	VarDec COMMA ExtDecList		{$$=insert("ExtDecList",3,$1,$2,$3);}
				;
Specifier		:	TYPE						{$$=insert("Specifier",1,$1);}
				|	StructSpecifier				{$$=insert("Specifier",1,$1);}
				;
StructSpecifier	:	STRUCT OptTag LC DefList RC	{$$=insert("StructSpecifier",5,$1,$2,$3,$4,$5);}
				|	STRUCT Tag					{$$=insert("StructSpecifier",2,$1,$2);}
				;
OptTag			:	ID							{$$=insert("OptTag",1,$1);}
				|	/* empty */					{$$=NULL;}
				;
Tag				:	ID							{$$=insert("Tag",1,$1);}
				;
VarDec			:	ID							{$$=insert("VarDec",1,$1);}
				|	VarDec LB INT RB			{$$=insert("VarDec",4,$1,$2,$3,$4);}
				|	VarDec LB error RB			{yyerrok;}
				;
FunDec			:	ID LP VarList RP			{$$=insert("FunDec",4,$1,$2,$3,$4);}
				|	ID LP RP					{$$=insert("FunDec",3,$1,$2,$3);}
				|	error RP					{yyerrok;}
				|	ID LP error					{yyerrok;}
				;
VarList			:	ParamDec COMMA VarList		{$$=insert("VarList",3,$1,$2,$3);}
				|	ParamDec					{$$=insert("VarList",1,$1);}
				;
ParamDec		:	Specifier VarDec			{$$=insert("ParamDec",2,$1,$2);}
				;
CompSt			:	LC DefList StmtList RC		{$$=insert("CompSt",4,$1,$2,$3,$4);}
				; 
StmtList		:	Stmt StmtList				{$$=insert("StmtList",2,$1,$2);}
				|	/* empty */					{$$=NULL;}
				; 
Stmt			:	Exp SEMI					{$$=insert("Stmt",2,$1,$2);}
				|	CompSt						{$$=insert("Stmt",1,$1);}
				|	RETURN Exp SEMI				{$$=insert("Stmt",3,$1,$2,$3);}
				|	IF LP Exp RP Stmt	%prec LOWER_THAN_ELSE	{$$=insert("Stmt",5,$1,$2,$3,$4,$5);}
				|	IF LP Exp RP Stmt ELSE Stmt	{$$=insert("Stmt",7,$1,$2,$3,$4,$5,$6,$7);}
				|	WHILE LP Exp RP Stmt		{$$=insert("Stmt",5,$1,$2,$3,$4,$5);}
				|	error SEMI					{yyerrok;}
				|	error						{}
				;
DefList			:	Def DefList					{$$=insert("DefList",2,$1,$2);}
				|	/* empty */					{$$=NULL;}
				;
Def				:	Specifier DecList SEMI		{$$=insert("Def",3,$1,$2,$3);}
				|	Specifier error SEMI		{yyerrok;}
				|	Specifier DecList error		{yyerrok;}
				;
DecList			:	Dec							{$$=insert("DecList",1,$1);}
				|	Dec COMMA DecList			{$$=insert("DecList",3,$1,$2,$3);}
				;
Dec				:	VarDec						{$$=insert("Dec",1,$1);}
				|	VarDec ASSIGNOP Exp			{$$=insert("Dec",3,$1,$2,$3);}
				;
Exp				:	Exp ASSIGNOP Exp			{$$=insert("Exp",3,$1,$2,$3);}
				|	Exp AND Exp					{$$=insert("Exp",3,$1,$2,$3);}
				|	Exp OR	Exp					{$$=insert("Exp",3,$1,$2,$3);}
				|	Exp RELOP Exp				{$$=insert("Exp",3,$1,$2,$3);}
				|	Exp PLUS Exp				{$$=insert("Exp",3,$1,$2,$3);}
				|	Exp MINUS Exp				{$$=insert("Exp",3,$1,$2,$3);}
				|	Exp STAR Exp				{$$=insert("Exp",3,$1,$2,$3);}
				|	Exp DIV Exp					{$$=insert("Exp",3,$1,$2,$3);}
				|	LP Exp RP					{$$=insert("Exp",3,$1,$2,$3);}
				|	MINUS Exp	%prec UMINUS	{$$=insert("Exp",2,$1,$2);}
				|	NOT Exp						{$$=insert("Exp",2,$1,$2);}
				|	ID LP Args RP				{$$=insert("Exp",4,$1,$2,$3,$4);}
				|	ID LP RP					{$$=insert("Exp",3,$1,$2,$3);}
				|	Exp LB Exp RB				{$$=insert("Exp",4,$1,$2,$3,$4);}
				|	Exp DOT ID					{$$=insert("Exp",3,$1,$2,$3);}
				|	ID							{$$=insert("Exp",1,$1); }
				|	INT							{$$=insert("Exp",1,$1); }
				|	FLOAT						{$$=insert("Exp",1,$1);}
				;
Args			:	Exp COMMA Args				{$$=insert("Args",3,$1,$2,$3);}
				|	Exp							{$$=insert("Args",1,$1);}
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

