#ifndef __TREE_H__
#define __TREE_H__

#define LEN 32

struct Node{
	int terminal;
	char* type;
	char* lexeme;
	int lineno;
	struct Node* child;
	struct Node* nextNeighbor;
};

// parse tree root
struct Node* root;

struct Node* insert(char* type, int argc, ...);
void outputTree(struct Node*, int n);

#endif
