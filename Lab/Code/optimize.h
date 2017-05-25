#ifndef __OPTIMIZE_H__
#define __OPTIMIZE_H__

#include "tree.h"

void const_folding(struct Node* node);

void optimize_control();
void clean_temp_var();

void optimize_algebra();
void remove_equals();
#endif
