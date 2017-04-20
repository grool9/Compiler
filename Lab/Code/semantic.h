#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "tree.h"

struct {
	Type structType;
	char* structName;
} structinfo[100];
int top;

void semanticAnalysis(struct Node* root);

#endif
