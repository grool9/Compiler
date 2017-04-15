#include "common.h"

int semanticErrorCount = 0;

// print error msg
void semanticError(int lineno) {
	semanticErrorCount ++;
	printf("[Error type %d] line : %d\n", semanticErrorCount, lineno);
}

// 从左到右深度优先遍历语法树
void traversalTree(struct Node* root){
	if(root==NULL)return;

	int no = root->typeno;
	struct Node* current = root->child;
	switch(no){
		_ExtDef_:
		{
			for(;current!=NULL;current=current->nextSibling){
				traversalTree(current);

			}
			break;
		}
		_Def_:break;
		default:break;
	}
}
