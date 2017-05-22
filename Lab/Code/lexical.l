%{
#include <stdio.h>
#include <string.h>
#include "syntax.tab.h"
#include "common.h"

	int isWrong = 0;
	void setVal(char*, char*);
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
"struct"	{setVal("STRUCT",yytext); return STRUCT; }
"return"	{setVal("RETURN",yytext);return RETURN; }
"if"	{setVal("IF",yytext); return IF; }
"else"	{setVal("ELSE",yytext); return ELSE; }
"while"	{setVal("WHILE",yytext); return WHILE; }
{type}	{setVal("TYPE",yytext); return TYPE; }
{int10} {setVal("INT",yytext);return INT; }
{int8}  {setVal("INT",yytext);return INT; }
{int16} {setVal("INT",yytext);return INT; }
{float}	{setVal("FLOAT",yytext);return FLOAT; }
{id}	{setVal("ID",yytext);return ID; }
"+"	{setVal("PLUS",yytext);return PLUS; }
"-"	{setVal("MINUS",yytext);return MINUS; }
"*"	{setVal("STAR",yytext);return STAR; }
"/"	{setVal("DIV",yytext);return DIV; }
"="	{setVal("ASSIGNOP",yytext);return ASSIGNOP; }
">"|"<"|">="|"<="|"=="|"!="	{  setVal("RELOP",yytext);return RELOP; }
"&&"	{setVal("AND",yytext);return AND; }
"||"	{setVal("OR",yytext);return OR; }
"."	{setVal("DOT",yytext);return DOT; }
"!"	{setVal("NOT",yytext);return NOT; }
"("	{setVal("LP",yytext);return LP; } 
")"	{setVal("RP",yytext);return RP; } 
"["	{setVal("LB",yytext);return LB; }
"]"	{setVal("RB",yytext);return RB; }
"{"	{setVal("LC",yytext);return LC; } 
"}"	{setVal("RC",yytext);return RC; } 
";"	{setVal("SEMI",yytext);return SEMI; }
","	{setVal("COMMA",yytext);return COMMA; }
{wrongint16}    { isWrong=1; printf("Error type A at Line %d: Illegal hexadecimal number \'%s\'.\n", yylineno, yytext); return INT; }
{wrongint8}   { isWrong=1; printf("Error type A at Line %d: Illegal octal number \'%s\'.\n", yylineno, yytext); return INT; } 
. { isWrong=1; printf("Error type A at Line %d: Mysterious characters \'%s\'.\n", yylineno, yytext);  }
%%

void setVal(char* token, char* lexeme){
	yylval=(struct Node*)malloc(sizeof(struct Node));
	yylval->terminal=1;
	yylval->token=(char*)malloc(sizeof(char)*LEN);
	yylval->lexeme=(char*)malloc(sizeof(char)*LEN);
	strcpy(yylval->token, token);
	strcpy(yylval->lexeme,lexeme);
	yylval->lineno=yylineno;
	yylval->child=NULL;
	yylval->nextSibling=NULL;

	//lab2
	yylval->type = none_type;
	yylval->rule = No_Rule;
	yylval->isLeftVal = false;

	//lab3
	yylval->isConstant = false;
}
