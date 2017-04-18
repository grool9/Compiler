#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "tree.h"

#define N 16384

typedef enum {_VARIABLE_, _FUNCTION_} IDKind;

//符号
struct Symbol{
	char* name;
	IDKind kind;//名字的种类

	union{
		// variable
		Type type;// pointer
	
		// function
		struct {
			int argc;
			Type* argv;
			Type retType;
		};
	};
	
	int addr;
	struct Symbol* next;
};

struct Symbol* table[N];

void initTable();
void addVariable(struct Node* node);
void addFunction(struct Node* node);
struct Symbol* lookupIDTable(IDKind kind, char* name);
void delElement(struct Node* node);

#endif
