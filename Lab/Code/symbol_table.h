#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#define N 16384
typedef enum { false, true } bool;

struct Symbol{
	char name[30];
	int type;
	int addr;
	struct Symbol* next;
};

struct Symbol* table[N];

void initTable();
void addElement(struct Symbol* sym);
bool findElement(char* name);

#endif
