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
void add2Table(struct Node* node){
	int pos=hash_pjw(node->lexeme);

	struct Symbol* sym = (struct Symbol*)malloc(sizeof(struct Symbol));
	sym->name = (char*)malloc(LEN);
	sym->type = (Type)malloc(sizeof(struct Type_));
	strcmp(sym->name, node->lexeme);
	*sym->type = *node->type;
	sym->next=table[pos];
	table[pos]=sym;
}

//find
struct Symbol* lookupIDTable(char* name){
	int pos=hash_pjw(name);
	
	struct Symbol* p=table[pos];
	for(;p!=NULL;p=p->next){
		if(strcmp(p->name, name)==0)break;
	}

	return p;
}
