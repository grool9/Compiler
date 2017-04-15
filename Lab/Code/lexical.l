%{
#include <stdio.h>
#include <string.h>
#include "syntax.tab.h"
#include "common.h"

	int isWrong = 0;
	void setVal(enum TypeNo, char*, char*);
%}
%option yylineno

delim	[ \t\n]
ws		{delim}+
digit	[0-9]
letter	[a-zA-Z]
letter_	{letter}|_
id		{letter_}(({letter_}|{digit}){0,31})
int8	[0][0-7]+
int10	0|([1-9]{digit}{0,31})
int16	[0][xX][0-9A-Fa-f]+
wrongint16	0[xX]({digit}|{letter})*
wrongint8	0({digit}|{letter})*
float	({int10}\.({digit}+)?)|({int10}?\.{digit}+)
type	int|float

%%
{ws}	{ }
[\n\r]	{ }
"struct"	{setVal(_STRUCT_,"STRUCT",yytext); return STRUCT; }
"return"	{setVal(_RETURN_,"RETURN",yytext);return RETURN; }
"if"	{setVal(_IF_,"IF",yytext); return IF; }
"else"	{setVal(_ELSE_,"ELSE",yytext); return ELSE; }
"while"	{setVal(_WHILE_,"WHILE",yytext); return WHILE; }
{type}	{setVal(_TYPE_,"TYPE",yytext); return TYPE; }
{int10} {setVal(_INT_,"INT",yytext);return INT; }
{int8}  {setVal(_INT_,"INT",yytext);return INT; }
{int16} {setVal(_INT_,"INT",yytext);return INT; }
{float}	{setVal(_FLOAT_,"FLOAT",yytext);return FLOAT; }
{id}	{setVal(_ID_,"ID",yytext);return ID; }
"+"	{setVal(_PLUS_,"PLUS",yytext);return PLUS; }
"-"	{setVal(_MINUS_,"MINUS",yytext);return MINUS; }
"*"	{setVal(_STAR_,"STAR",yytext);return STAR; }
"/"	{setVal(_DIV_,"DIV",yytext);return DIV; }
"="	{setVal(_ASSIGNOP_,"ASSIGNOP",yytext);return ASSIGNOP; }
">"|"<"|">="|"<="|"=="|"!="	{  setVal(_RELOP_,"RELOP",yytext);return RELOP; }
"&&"	{setVal(_AND_,"AND",yytext);return AND; }
"||"	{setVal(_OR_,"OR",yytext);return OR; }
"."	{setVal(_DOT_,"DOT",yytext);return DOT; }
"!"	{setVal(_NOT_,"NOT",yytext);return NOT; }
"("	{setVal(_LP_,"LP",yytext);return LP; } 
")"	{setVal(_RP_,"RP",yytext);return RP; } 
"["	{setVal(_LB_,"LB",yytext);return LB; }
"]"	{setVal(_RB_,"RB",yytext);return RB; }
"{"	{setVal(_LC_,"LC",yytext);return LC; } 
"}"	{setVal(_RC_,"RC",yytext);return RC; } 
";"	{setVal(_SEMI_,"SEMI",yytext);return SEMI; }
","	{setVal(_COMMA_,"COMMA",yytext);return COMMA; }
{wrongint16}    { isWrong=1; printf("Error type A at Line %d: Illegal hexadecimal number \'%s\'.\n", yylineno, yytext); return INT; }
{wrongint8}   { isWrong=1; printf("Error type A at Line %d: Illegal octal number \'%s\'.\n", yylineno, yytext); return INT; } 
. { isWrong=1; printf("Error type A at Line %d: Mysterious characters \'%s\'.\n", yylineno, yytext);  }
%%

void setVal(enum TypeNo no, char* type, char* lexeme){
	yylval=(struct Node*)malloc(sizeof(struct Node));
	yylval->terminal=1;
	yylval->type=(char*)malloc(sizeof(char)*LEN);
	yylval->lexeme=(char*)malloc(sizeof(char)*LEN); 
	strcpy(yylval->type, type);
	strcpy(yylval->lexeme,lexeme);
	yylval->lineno=yylineno;
	yylval->child=NULL;
	yylval->nextSibling=NULL;

	yylval->typeno=no;
}
