#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "tree.h"

#define N 16384

//符号
struct Symbol{
	char* name;
	IDKind idkind;//名字的种类
	bool isLeftVal;

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
	
	char* addr;
	struct Symbol* next;
};

struct Symbol* table[N];

void initTable();
void addField(struct Node*, char*);
void addElement(struct Node* node);
struct Symbol* lookupFunction(char* name);
struct Symbol* lookupVariable(char* name);

void printType(Type type);
void printArgv(int argc, Type* argv);

#endif
