#include "common"

void outputIR2File(char* filename) {
	fp = fopen(filename, "w");
	while(p != NULL){
	ircode_print(p->code);
	p = p->next;
	}
	close(fp);
}
