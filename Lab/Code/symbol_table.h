#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "tree.h"

#define N 16384

//符号
struct Symbol{
	char* name;
	IDKind kind;//名字的种类

	union{
		// variable
		Type type;// pointer
	
		// function
		struct {
			Type retType;
			int argc;
			Type* argv;
		};
	};
	
	int addr;
	struct Symbol* next;
};

struct Symbol* table[N];

void initTable();
void addElement(struct Node* node);
struct Symbol* lookupIDTable(char* name);
void delElement(struct Node* node);

#endif
