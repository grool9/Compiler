#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#define N 16384

struct Symbol{
	char ID[30];
	int type;
	int addr;
};

struct Symbol table[N];

#endif
