#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "tree.h"

#define N 16384

//符号
struct Symbol{
	char* name;
	Type type;//pointer
	int dimension;
	int argc;
	int addr;
	struct Symbol* next;
};

struct Symbol* table[N];

void initTable();
void addElement(struct Node* node);
struct Symbol* lookupIDTable(char* name);
void delElement(struct Node* node);

#endif
