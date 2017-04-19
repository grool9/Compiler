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
void addElement(struct Node* node){
	char* name = node->lexeme;
	int pos=hash_pjw(name);

#ifdef DEBUG
	printf("add element:%s\t pos:%d\n",name,pos);
#endif

	struct Symbol* sym = (struct Symbol*)malloc(sizeof(struct Symbol));
	sym->name = (char*)malloc(LEN);//变量名称

	strcpy(sym->name, name);
	sym->kind = node->kind;

	sym->type = node->type;//变量类型

	sym->next=table[pos];
	table[pos]=sym;
}


//find
struct Symbol* lookupIDTable(struct Node* node){
	char* name = node->lexeme;
	IDKind kind = node->kind;

	int pos=hash_pjw(name);
#ifdef DEBUG
	printf("look up:%s \t pos:%d\n", name, pos);
#endif	

	struct Symbol* p=table[pos];
	for(;p!=NULL;p=p->next){
		if(p->kind == kind && strcmp(p->name, name)==0)break;
	}

	return p;
}

//delete
void delElement(struct Node* node) {
}
