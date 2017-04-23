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
	sym->type = node->type;//变量类型
	sym->addr = NULL;
	sym->next=table[pos];
	table[pos]=sym;
}


//find
struct Symbol* lookupIDTable(struct Node* node){
	char* name = node->lexeme;
	int pos=hash_pjw(name);

#ifdef DEBUG
	printf("look up:%s \t pos:%d\n", name, pos);
#endif	

	struct Symbol* p=table[pos];
	for(;p!=NULL;p=p->next){
		if(node->idkind == _FUNCTION_){
			if(p->idkind == _FUNCTION_ && strcmp(name, p->name) == 0)break;
		}
		else {//_CONST_不会存入符号表
			if(p->idkind != _FUNCTION_ && strcmp(name, p->name) == 0)break;
		}
	}

	return p;
}

//delete
void delElement(struct Node* node) {
}
