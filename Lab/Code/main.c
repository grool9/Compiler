#include <stdio.h>
#include "common.h"

extern int yylineno;
extern void yyrestart(FILE*);
extern void yyparse();

int main(int argc, char** argv) {
	// init
	none_type = (Type)malloc(sizeof(struct Type_));
	none_type->kind = _NONE_;

	if (argc <= 1)return 1;
	FILE* f = fopen(argv[1], "r");
	if (!f) {
		perror(argv[1]);
		return 1;
	}

	// lab1
	yylineno=1;
	yyrestart(f);
	yyparse();

	// lab2
	initTable();
	if(!isWrong)semanticAnalysis(root);

	// lab3
	if(!isWrong) {
		//printf("hello lab3\n");
		generateIR(root, argv[2]);
	}

	return 0;
} 
