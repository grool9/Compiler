#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <ctype.h>
#include<stdarg.h>
#include"symbol_table.h"
#include"tree.h"

//遍历语法树，插入符号表
void traversalTree(struct Node* root){
	if(root==NULL)return;

	if(strcmp(root->type,"ExtDef")==0||strcmp(root->type,"Def")==0){
	}
	else if(strcmp(root->type,"Exp")==0){
	}
}


int my_atoi(char* str){
	if(strlen(str)==1||str[0]!='0'){//10
		return atoi(str);
	}
	else if(str[0]=='0'&&(str[1]=='X'||str[1]=='x')){//16
		int sum=0;
		int i=2;
		for(;str[i]!='\0';i++){
			sum*=16;
			if(isdigit(str[i]))sum+=str[i]-'0';
			else if(isupper(str[i]))sum+=str[i]-'A'+10;
			else sum+=str[i]-'a'+10;
		}
		return sum;
	}
	else{//8
		int sum=0;
		int i=1;
		for(;str[i]!='\0';i++){
			sum*=8;
			sum+=str[i]-'0';
		}
		return sum;
	}
}

void outputTree(struct Node* root,int n){
	if(root==NULL)return;
	
	struct Node* p=root;
	for(;p!=NULL;p=p->nextNeighbor){
		int i=0;
		for(;i<n;i++)printf("  ");
		
		if(!p->terminal){
			printf("%s (%d)\n",p->type,p->lineno);
		}
		else {
			printf("%s",p->type);
			if(strcmp(p->type,"ID")==0){
				printf(": %s", p->lexeme);
			}
			else if(strcmp(p->type, "TYPE")==0) {
				printf(": %s", p->lexeme);
			}
			else if(strcmp(p->type, "INT")==0) {
				printf(": %d", my_atoi(p->lexeme));//10
			}
			else if(strcmp(p->type, "FLOAT")==0) {
				printf(": %f", atof(p->lexeme));
			}
			printf("\n");
		}

		outputTree(p->child,n+1);
	}
}

struct Node* insert(char* type, int argc, ...){
	struct Node* head=(struct Node*)malloc(sizeof(struct Node));
	head->terminal=0;
	head->type=(char*)malloc(sizeof(char)*LEN); 
	head->lexeme=NULL;
	strcpy(head->type,type);
	head->child=NULL;
	head->nextNeighbor=NULL;

	struct Node* current=head;

	va_list p;
	va_start(p, argc);
	current->child=va_arg(p, struct Node*);//not null
	current=current->child;

	head->lineno=current->lineno;

	int i=1;
	for(;i<argc;i++){
		struct Node* temp=va_arg(p,struct Node*);
		if(temp!=NULL){
			current->nextNeighbor=temp;
			current=current->nextNeighbor;
		}
	}
	
	va_end(p);

	return head;
}
