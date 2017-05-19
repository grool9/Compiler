%{
#include"common.h"

#define YYSTYPE struct Node*

#include "lex.yy.c"

struct Node* root = NULL;

int pasterrline=0;

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
Program			:	ExtDefList					{$$=insert(Program__ExtDefList,"Program",1,$1); root=$$;}
				;
ExtDefList		:	ExtDef ExtDefList			{$$=insert(ExtDefList__ExtDef_ExtDefList,"ExtDefList",2,$1,$2);}
				|	/* empty */					{$$=NULL;}
				;
ExtDef			:	Specifier ExtDecList SEMI	{$$=insert(ExtDef__Specifier_ExtDecList_SEMI,"ExtDef",3,$1,$2,$3);}
				|	Specifier SEMI				{$$=insert(ExtDef__Specifier_SEMI,"ExtDef",2,$1,$2);}
				|	Specifier FunDec CompSt		{$$=insert(ExtDef__Specifier_FunDec_Compst,"ExtDef",3,$1,$2,$3);}
				|	error SEMI					{yyerrok;}
				|	Specifier error				{yyerrok;}
				;
ExtDecList		:	VarDec						{$$=insert(ExtDecList__VarDec,"ExtDecList",1,$1);}
				|	VarDec COMMA ExtDecList		{$$=insert(ExtDecList__VarDec_COMMA_ExtDecList,"ExtDecList",3,$1,$2,$3);}
				;
Specifier		:	TYPE						{$$=insert(Specifier__TYPE,"Specifier",1,$1);}
				|	StructSpecifier				{$$=insert(Specifier__StructSpecifier,"Specifier",1,$1);}
				;
StructSpecifier	:	STRUCT OptTag LC DefList RC	{$$=insert(StructSpecifier__STRUCT_OptTag_LC_DefList_RC,"StructSpecifier",5,$1,$2,$3,$4,$5);}
				|	STRUCT Tag					{$$=insert(StructSpecifier__STRUCT_Tag,"StructSpecifier",2,$1,$2);}
				;
OptTag			:	ID							{$$=insert(OptTag__ID,"OptTag",1,$1);}
				|	/* empty */					{$$=NULL;}
				;
Tag				:	ID							{$$=insert(Tag__ID,"Tag",1,$1);}
				;
VarDec			:	ID							{$$=insert(VarDec__ID,"VarDec",1,$1);}
				|	VarDec LB INT RB			{$$=insert(VarDec__VarDec_LB_int_RB,"VarDec",4,$1,$2,$3,$4);}
				|	VarDec LB error RB			{yyerrok;}
				;
FunDec			:	ID LP VarList RP			{$$=insert(FunDec__ID_LP_VarList_RP,"FunDec",4,$1,$2,$3,$4);}
				|	ID LP RP					{$$=insert(FunDec__ID_LP_RP,"FunDec",3,$1,$2,$3);}
				|	error RP					{yyerrok;}
				|	ID LP error					{yyerrok;}
				;
VarList			:	ParamDec COMMA VarList		{$$=insert(VarList__ParamDec_COMMA_VarList,"VarList",3,$1,$2,$3);}
				|	ParamDec					{$$=insert(VarList__ParamDec,"VarList",1,$1);}
				;
ParamDec		:	Specifier VarDec			{$$=insert(ParamDec__Specifier_VarDec,"ParamDec",2,$1,$2);}
				;
CompSt			:	LC DefList StmtList RC		{$$=insert(	Compst__LC_DefList_StmtList_RC,"CompSt",4,$1,$2,$3,$4);}
				; 
StmtList		:	Stmt StmtList				{$$=insert(StmtList__Stmt_StmtList,"StmtList",2,$1,$2);}
				|	/* empty */					{$$=NULL;}
				; 
Stmt			:	Exp SEMI					{$$=insert(Stmt__Exp_SEMI,"Stmt",2,$1,$2);}
				|	CompSt						{$$=insert(Stmt__Compst,"Stmt",1,$1);}
				|	RETURN Exp SEMI				{$$=insert(Stmt__RETURN_Exp_SEMI,"Stmt",3,$1,$2,$3);}
				|	IF LP Exp RP Stmt	%prec LOWER_THAN_ELSE	{$$=insert(Stmt__IF_LP_Exp_RP_Stmt,"Stmt",5,$1,$2,$3,$4,$5);}
				|	IF LP Exp RP Stmt ELSE Stmt	{$$=insert(Stmt__IF_LP_Exp_RP_Stmt_else_Stmt,"Stmt",7,$1,$2,$3,$4,$5,$6,$7);}
				|	WHILE LP Exp RP Stmt		{$$=insert(Stmt__WHILE_LP_Exp_RP_Stmt,"Stmt",5,$1,$2,$3,$4,$5);}
				|	error SEMI					{yyerrok;}
				|	error						{}
				;
DefList			:	Def DefList					{$$=insert(DefList__Def_DefList,"DefList",2,$1,$2);}
				|	/* empty */					{$$=NULL;}
				;
Def				:	Specifier DecList SEMI		{$$=insert(Def__Specifier_DecList_SEMI,"Def",3,$1,$2,$3);}
				|	Specifier error SEMI		{yyerrok;}
				|	Specifier DecList error		{yyerrok;}
				;
DecList			:	Dec							{$$=insert(DecList__Dec,"DecList",1,$1);}
				|	Dec COMMA DecList			{$$=insert(DecList__Dec_COMMA_DecList,"DecList",3,$1,$2,$3);}
				;
Dec				:	VarDec						{$$=insert(Dec__VarDec,"Dec",1,$1);}
				|	VarDec ASSIGNOP Exp			{$$=insert(Dec__VarDec_ASSIGNOP_Exp,"Dec",3,$1,$2,$3);}
				;
Exp				:	Exp ASSIGNOP Exp			{$$=insert(Exp__Exp_ASSIGNOP_Exp,"Exp",3,$1,$2,$3);}
				|	Exp AND Exp					{$$=insert(Exp__Exp_AND_Exp,"Exp",3,$1,$2,$3);}
				|	Exp OR	Exp					{$$=insert(Exp__Exp_OR_Exp,"Exp",3,$1,$2,$3);}
				|	Exp RELOP Exp				{$$=insert(Exp__Exp_RELOP_Exp,"Exp",3,$1,$2,$3);}
				|	Exp PLUS Exp				{$$=insert(Exp__Exp_PLUS_Exp,"Exp",3,$1,$2,$3);}
				|	Exp MINUS Exp				{$$=insert(Exp__Exp_MINUS_Exp,"Exp",3,$1,$2,$3);}
				|	Exp STAR Exp				{$$=insert(Exp__Exp_STAR_Exp,"Exp",3,$1,$2,$3);}
				|	Exp DIV Exp					{$$=insert(Exp__Exp_DIV_Exp,"Exp",3,$1,$2,$3);}
				|	LP Exp RP					{$$=insert(Exp__LP_Exp_RP,"Exp",3,$1,$2,$3);}
				|	MINUS Exp	%prec UMINUS	{$$=insert(Exp__MINUS_Exp,"Exp",2,$1,$2);}
				|	NOT Exp						{$$=insert(Exp__NOT_Exp,"Exp",2,$1,$2);}
				|	ID LP Args RP				{$$=insert(Exp__ID_LP_Args_RP,"Exp",4,$1,$2,$3,$4);}
				|	ID LP RP					{$$=insert(Exp__ID_LP_RP,"Exp",3,$1,$2,$3);}
				|	Exp LB Exp RB				{$$=insert(Exp__Exp_LB_Exp_RB,"Exp",4,$1,$2,$3,$4);}
				|	Exp DOT ID					{$$=insert(Exp__Exp_DOT_ID,"Exp",3,$1,$2,$3);}
				|	ID							{$$=insert(Exp__ID,"Exp",1,$1); }
				|	INT							{$$=insert(Exp__INT,"Exp",1,$1); }
				|	FLOAT						{$$=insert(Exp__FLOAT,"Exp",1,$1);}
				;
Args			:	Exp COMMA Args				{$$=insert(Args__Exp_COMMA_Args,"Args",3,$1,$2,$3);}
				|	Exp							{$$=insert(Args__Exp,"Args",1,$1);}
				;
%%
yyerror(char* msg) {
	isWrong = 1;
	if(yylineno!=pasterrline/*||stRCmp(yytext,pasttext)!=0*/){
		if(strlen(yytext)!=0)fprintf(stderr,"Error type B at Line %d: UnExpected token '%s'\n",yylineno,yytext);
		else fprintf(stderr,"Error type B at Line %d: UnExpected $end\n",yylineno);
		pasterrline=yylineno;
		//stRCpy(pasttext,yytext);
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
			printf("%s (%d)\n",p->token,p->lineno);
		}
		else {
			printf("%s",p->token);
			if(strcmp(p->token,"ID")==0){
				printf(": %s", p->lexeme);
			}
			else if(strcmp(p->token, "TYPE")==0) {
				printf(": %s", p->lexeme);
			}
			else if(strcmp(p->token, "INT")==0) {
				printf(": %d", my_atoi(p->lexeme));//10
			}
			else if(strcmp(p->token, "FLOAT")==0) {
				printf(": %f", atof(p->lexeme));
			}
			printf("\n");
		}

		outputTree(p->child,n+1);
	}
}

struct Node* insert(Rule rule, char* token, int argc, ...){
	struct Node* head=(struct Node*)malloc(sizeof(struct Node));
	head->terminal=0;
	head->token=(char*)malloc(sizeof(char)*LEN); 
	head->lexeme=NULL;
	strcpy(head->token,token);
	head->child=NULL;
	head->nextSibling=NULL;

	//lab2
	head->rule = rule;
	head->type = none_type;
	head->isLeftVal = false;

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
