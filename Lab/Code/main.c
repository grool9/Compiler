#include <stdio.h>
#include "common.h"

extern int isWrong;
extern int yylineno;
extern void yyrestart(FILE*);
extern void yyparse();

int main(int argc, char** argv) {
	if (argc <= 1)return 1;
	FILE* f = fopen(argv[1], "r");
	if (!f) {
		perror(argv[1]);
		return 1;
	}

	yylineno=1;
	yyrestart(f);
	yyparse();
	
	//if(!isWrong)outputTree(root,0);
	initTable();
	semanticAnalysis(root);

	return 0;
} 
