#include "common.h"

//hash function
unsigned int hash_pjw(char* name){
	unsigned int val=0,i;
	for(;*name;++name){
		val=(val<<2)+ *name;
		if(i=val&~0x3fff)val=(val^(i>>12))&0x3fff;
	}
	return val;
}

//init
void initTable(){
	int i=0;
	for(;i<N;i++){
		table[i]=NULL;
	}
}

//add
void addField(struct Node* node, char* structName){
	char* name = node->lexeme;
	int pos = hash_pjw(name);

	struct Symbol* sym = (struct Symbol*)malloc(sizeof(struct Symbol));
	sym->name = (char*)malloc(LEN);//变量名称
	strcpy(sym->name, name);
	sym->idkind = node->idkind;
	sym->type = node->type;//变量类型
	sym->addr = (char*)malloc(LEN);
	strcpy(sym->addr, structName);

	sym->next=table[pos];
	table[pos]=sym;
}

//add
void addElement(struct Node* node){
	char* name = node->lexeme;
	int pos=hash_pjw(name);

#ifdef DEBUG
	printf("add element:%s\t pos:%d\n",name,pos);
#endif

	struct Symbol* sym = (struct Symbol*)malloc(sizeof(struct Symbol));
	sym->name = (char*)malloc(LEN);//变量名称
	strcpy(sym->name, name);
	sym->idkind = node->idkind;

	if(sym->idkind == _FUNCTION_) {
		sym->retType = node->retType;
		sym->argc = node->argc;
		sym->argv = node->argv;
	}
	else sym->type = node->type;//变量类型

	sym->addr = "";
	sym->next=table[pos];
	table[pos]=sym;
}


//find
// 查找方程／变量
struct Symbol* lookupFunction(char* name) {
	int pos = hash_pjw(name);

	struct Symbol* p = table[pos];
	struct Symbol* outcome = NULL;
	for(;p!=NULL;p=p->next) {
		if(strcmp(name, p->name) == 0) {
			outcome = p;
			if(p->idkind == _FUNCTION_)break;
		}
	}

	return outcome;
}
// 查找变量
struct Symbol* lookupVariable(char* name) {
	int pos = hash_pjw(name);

	struct Symbol* p = table[pos];
	for(;p!=NULL;p=p->next) {
		if(strcmp(name, p->name) == 0 && p->idkind != _FUNCTION_) break;
	}
	return p;
}

void printType(Type type) {
	int k = type->kind;
	printf("type kind:");
	
	if(k == _BASIC_){
		printf("basic -- ");
		int b = type->u.basic;
		if(b==_INT_)printf("int\n");
		else printf("float\n");
	}
	else if(k == _ARRAY_) {
		printf("array size: %d\n", type->u.array.size);
		printType(type->u.array.elem);
	}
	else if(k == _STRUCTURE_) {
		printf("structure\n");
		FieldList p = type->u.structure;
		while(p != NULL) {
			printf("name: %d\n", p->name);
			printType(p->type);
			p = p->tail;
		}
	}
	else printf("none\n");
}

void printArgv(int argc, Type* argv) {
	printf("argc: %d\n", argc);

	int i = 0;
	for(; i<argc; i++ ){
		printType(argv[i]);
	}
}
