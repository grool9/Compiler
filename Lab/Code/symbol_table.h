#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#define N 16384

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
//类型
struct Type_
{
	enum {BASIC, ARRAY, STRUCTURE }kind;
	union{
		int basic;
		struct {Type elem; int size; }array;
		FieldList structure;
	}u;
};
//域
struct FieldList_
{
	char* name;
	Type type;
	FieldList tail;
};
//符号
struct Symbol{
	char* name;
	Type type;//pointer
	int dimension;
	int argc;
	int value;
	int addr;
	struct Symbol* next;
};

struct Symbol* table[N];

void initTable();
void addElement(struct Symbol* sym);
struct Symbol* lookupIDTable(char* name);

#endif
