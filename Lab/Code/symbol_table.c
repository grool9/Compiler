#include<stdio.h>
#include"symbol_table.h"

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
void addElement(struct Symbol* sym){
	int pos=hash_pjw(sym->name);
	sym->next=table[pos];
	table[pos]=sym;
}

//find
bool findElement(char* name){
	int pos=hash_pjw(name);
	
	struct Symbol* p=table[pos];
	for(;p!=NULL;p=p->next){
		if(strcmp(p->name, name)==0)return true;
	}
	return false;
}
