#include <stdio.h>
#include "common.h"

extern int isWrong;
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

	yylineno=1;
	yyrestart(f);
	yyparse();
	
	//if(!isWrong)outputTree(root,0);
	initTable();
	if(!isWrong)semanticAnalysis(root);

	return 0;
} 
