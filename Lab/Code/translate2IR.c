#include "common.h"

//#define DEBUG

// declaration
struct InterCodeNode* translate_ExtDefList(struct Node* root);
struct InterCodeNode* translate_ExtDef(struct Node* root);
struct InterCodeNode* translate_ExtDecList(struct Node* root);
struct InterCodeNode* translate_FunDec(struct Node* root);
struct InterCodeNode* translate_VarList(struct Node* root);
struct InterCodeNode* translate_ParamDec(struct Node* root);

struct InterCodeNode* translate_Exp(struct Node* root, Operand place);
struct InterCodeNode* translate_Stmt(struct Node* root);
struct InterCodeNode* translate_Cond(struct Node* root, Operand label_true, Operand label_false);
struct InterCodeNode* translate_Args(struct Node* root, struct OperandNode* arg_list);
struct InterCodeNode* translate_Compst(struct Node* root);
struct InterCodeNode* translate_DefList(struct Node* root);
struct InterCodeNode* translate_StmtList(struct Node* root);
struct InterCodeNode* translate_Def(struct Node* root);
struct InterCodeNode* translate_DecList(struct Node* root);
struct InterCodeNode* translate_Dec(struct Node* root);
struct InterCodeNode* translate_VarDec(struct Node* root);

// constant defination
Operand constant0 = NULL;
Operand constant1 = NULL;

// function
Operand newOperand(OperandKind kind, int val) {
	Operand operand = (Operand)malloc(sizeof(struct Operand_));
	operand->kind = kind;
	operand->u.value = val;

	return operand;// return the pointer
}

struct InterCodeNode* newInterCodeNode(OperationKind kind, Operand op1, Operand op2, Operand op3, char* name) {
	struct InterCodeNode* node = (struct InterCodeNode*)malloc(sizeof(struct InterCodeNode));
	node->code.kind = kind;
	
	if(kind == ASSIGN) {
		node->code.u.assign.left = op1;
		node->code.u.assign.right = op2;
	}
	else if(kind == ADD || kind == SUB || kind == MUL || kind == DIVIDE) {
		node->code.u.binop.result = op1;
		node->code.u.binop.op1 = op2;
		node->code.u.binop.op2 = op3;
	}
	else if(kind == LABELOP || kind == GOTO || kind == RETURNOP || kind == READ || kind == WRITE || kind == ARG || kind == PARAM) {
		node->code.u.sigop.op = op1;
	}
	else if(kind == IFOP) {
		node->code.u.ifop.op1 = op1;
		node->code.u.ifop.op2 = op2;
		node->code.u.ifop.label = op3;
		node->code.u.ifop.relop = name;
	}
	else if(kind == CALL|| kind == FUNCTION) {
		node->code.u.funop.name = name;
	}
	else if(kind == ASSIGNCALL) {
		node->code.u.assigncall.result = op1;
		node->code.u.assigncall.name = name;
	}

	node->prev = NULL;
	node->next = NULL;

	return node;
}

Operand new_temp() {
	static int no = 1;
	Operand operand = (Operand)malloc(sizeof(struct Operand_));
	operand->kind = TEMP;
	operand->u.var_no = no;
	no++;
	return operand;
}

Operand new_label() {
	static int no = 1;
	Operand operand = (Operand)malloc(sizeof(struct Operand_));
	operand->kind = LABEL;
	operand->u.var_no = no;
	no++;
	return operand;
}

struct InterCodeNode* concat(int number, ...) {
	va_list p;
	va_start(p, number);

	struct InterCodeNode* head = NULL;
	struct InterCodeNode* tail = NULL;
	int i = 0;
	for(;i<number;i++) {
		struct InterCodeNode* temp = va_arg(p, struct InterCodeNode*);
		if(temp!=NULL) {
			if(head == NULL) {
				head = temp;
				tail = head;
				while(tail->next != NULL) tail = tail->next;
			}
			else {
				tail->next = temp;
				temp->prev = tail;

				tail = temp;
				while(tail->next != NULL) tail = tail->next;
			}
		}
	}

	va_end(p);

	return head;
}

struct OperandNode* arg_concat(Operand t, struct OperandNode* arg_list) {
	struct OperandNode* temp = (struct OperandNode*)malloc(sizeof(struct OperandNode));
	temp->op = t;

	// connect
	struct OperandNode* first = arg_list;
	struct OperandNode* second = arg_list->next;

	first->next = temp;
	temp->next = second;
	return temp;
}

/*
 * translate functions
 */
struct InterCodeNode* translate_Exp(struct Node* root, Operand place) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_Exp():%s\t%s\n", root->token, root->lexeme);
#endif

	switch(root->rule) {
		case Exp__INT:{
			struct Node* intNode = root->child;
			int value = my_atoi(intNode->lexeme);
			Operand right = newOperand(CONSTANT, value);
			struct InterCodeNode* code = newInterCodeNode(ASSIGN, place, right, NULL, NULL);
			return code;
		}
		case Exp__ID:{
			struct Node* id = root->child;
			struct Symbol* sym = lookupVariable(id->lexeme);
			Operand right = newOperand(VARIABLE, sym->var_no);
#ifdef DEBUG
	printf("...EXP__ID\n");
	printf("%d, %d\n", place, right);
#endif
			return newInterCodeNode(ASSIGN, place, right, NULL, NULL);
		}
		case Exp__Exp_ASSIGNOP_Exp:{
			struct Node* exp1 = root->child;
			struct Node* exp2 = exp1->nextSibling->nextSibling;
	
			struct Node* variable = exp1->child;
	
			Operand t1 = new_temp();

			struct Symbol* sym = lookupVariable(variable->lexeme);
			Operand var = newOperand(VARIABLE, sym->var_no);

			struct InterCodeNode* c1 = newInterCodeNode(ASSIGN, var ,t1, NULL, NULL);
			struct InterCodeNode* c2 = newInterCodeNode(ASSIGN, place, var, NULL, NULL);

			struct InterCodeNode* code1 = translate_Exp(exp2, t1);
			struct InterCodeNode* code2 = concat(2, c1, c2);

			return concat(2, code1, code2);
		}
		case Exp__Exp_PLUS_Exp:{
			struct Node* exp1 = root->child;
			struct Node* exp2 = exp1->nextSibling->nextSibling;

			Operand t1 = new_temp();
			Operand t2 = new_temp();
			struct InterCodeNode* code1 = translate_Exp(exp1, t1);
			struct InterCodeNode* code2 = translate_Exp(exp2, t2);
			struct InterCodeNode* code3 = newInterCodeNode(ADD, place, t1, t2, NULL);

			return concat(3, code1, code2, code3);
		}
		case Exp__MINUS_Exp:{
			struct Node* exp1 = root->child->nextSibling;

#ifdef DEBUG
	printf("Exp__MINUS_Exp\n");
	printf("%s\n", exp1);
#endif

			Operand t1 = new_temp();

			struct InterCodeNode* code1 = translate_Exp(exp1, t1);
			struct InterCodeNode* code2 = newInterCodeNode(SUB, place, constant0, t1, NULL);
			return concat(2, code1, code2);
		}
		case Exp__Exp_RELOP_Exp:
		case Exp__NOT_Exp:
		case Exp__Exp_AND_Exp:
		case Exp__Exp_OR_Exp:{
			Operand label1 = new_label();
			Operand label2 = new_label();

			struct InterCodeNode* code0 = newInterCodeNode(ASSIGN, place, constant0, NULL, NULL);
			struct InterCodeNode* code1 = translate_Cond(root, label1, label2);
			struct InterCodeNode* c1 = newInterCodeNode(LABELOP, label1, NULL, NULL, NULL);
			struct InterCodeNode* c2 = newInterCodeNode(ASSIGN, place, constant1, NULL, NULL);
			struct InterCodeNode* code2 = concat(2, c1, c2);
			struct InterCodeNode* code3 = newInterCodeNode(LABELOP, label2, NULL, NULL, NULL);

			return concat(4, code0 ,code1, code2, code3);
		}
		case Exp__ID_LP_RP:{
			struct Node* id = root->child;
			char* function = id->lexeme;

			struct InterCodeNode* c1 = newInterCodeNode(READ, place, NULL, NULL, NULL);
			struct InterCodeNode* c2 = newInterCodeNode(CALL, NULL, NULL, NULL, function);

			if(strcmp(function, "read")==0) return c1;
			else return c2;
		}
		case Exp__ID_LP_Args_RP: {
			struct Node* id = root->child;
			struct Node* args = id->nextSibling->nextSibling;

			char* function = id->lexeme;

			struct OperandNode* arg_list = (struct OperandNode*)malloc(sizeof(struct OperandNode));
			arg_list->op = NULL;
			arg_list->next = NULL;
			
			struct InterCodeNode* code1 = translate_Args(args, arg_list);

#ifdef DEBUG
	printf("Exp__ID_LP_Args_RP\n");
	printInterCodes(code1);
#endif
			Operand first_arg = arg_list->next->op;
			struct InterCodeNode* c1 = newInterCodeNode(WRITE, first_arg, NULL, NULL, NULL);
			
#ifdef DEBUG
	printInterCodes(c1);
#endif
			
			if(strcmp(function, "write")==0) return concat(2, code1, c1);
		
			struct InterCodeNode* code2 = newInterCodeNode(ARG, first_arg, NULL, NULL, NULL);

			struct OperandNode* cur = arg_list->next->next;
			for(;cur != NULL; cur = cur->next) {
				code2 = concat(2,code2, cur->op);
			}
			struct InterCodeNode* c2 = newInterCodeNode(ASSIGNCALL, place, NULL, NULL, function);
			return concat(3, code1, code2, c2);
		}
	}
}

struct InterCodeNode* translate_Stmt(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_Stmt():%s\t%s\n", root->token, root->lexeme);
#endif
	switch(root->rule) {
		case Stmt__Exp_SEMI:{
			return translate_Exp(root->child, NULL);
		}
		case Stmt__Compst: {
			return translate_Compst(root->child);
		}
		case Stmt__RETURN_Exp_SEMI: {
			struct Node* exp = root->child->nextSibling;

			Operand t1 = new_temp();
			struct InterCodeNode* code1 = translate_Exp(exp, t1);
			struct InterCodeNode* code2 = newInterCodeNode(RETURNOP, t1, NULL, NULL ,NULL);
			return concat(2, code1, code2);
		}
		case Stmt__IF_LP_Exp_RP_Stmt: {
			struct Node* exp = root->child->nextSibling->nextSibling;
			struct Node* stmt1 = exp->nextSibling->nextSibling;

			Operand label1 = new_label();
			Operand label2 = new_label();
			struct InterCodeNode* code1 = translate_Cond(exp, label1, label2);
			struct InterCodeNode* code2 = translate_Stmt(stmt1);
			struct InterCodeNode* c1 = newInterCodeNode(LABELOP, label1, NULL, NULL, NULL);
			struct InterCodeNode* c2 = newInterCodeNode(LABELOP, label2, NULL, NULL, NULL);

			return concat(4, code1, c1, code2, c2);
		}
		case Stmt__IF_LP_Exp_RP_Stmt_ELSE_Stmt: {
			struct Node* exp = root->child->nextSibling->nextSibling;
			struct Node* stmt1 = exp->nextSibling->nextSibling;
			struct Node* stmt2 = stmt1->nextSibling->nextSibling;

			Operand label1 = new_label();
			Operand label2 = new_label();
			Operand label3 = new_label();

			struct InterCodeNode* code1 = translate_Cond(exp, label1, label2);
			struct InterCodeNode* code2 = translate_Stmt(stmt1);
			struct InterCodeNode* code3 = translate_Stmt(stmt2);

			struct InterCodeNode* c1 = newInterCodeNode(LABELOP, label1, NULL, NULL, NULL);
			struct InterCodeNode* c2 = newInterCodeNode(GOTO, label3, NULL, NULL, NULL);
			struct InterCodeNode* c3 = newInterCodeNode(LABELOP, label2, NULL, NULL, NULL);
			struct InterCodeNode* c4 = newInterCodeNode(LABELOP, label3, NULL, NULL, NULL);
#ifdef DEBUG
	printf("IFELSE!\n");
	printInterCodes(code1);
#endif

			return concat(7, code1, c1, code2, c2, c3, code3, c4);
		
		}
		case Stmt__WHILE_LP_Exp_RP_Stmt: {
			struct Node* exp = root->child->nextSibling->nextSibling;
			struct Node* stmt1 = exp->nextSibling->nextSibling;

			Operand label1 = new_label();
			Operand label2 = new_label();
			Operand label3 = new_label();

			struct InterCodeNode* code1 = translate_Cond(exp, label2, label3);
			struct InterCodeNode* code2 = translate_Stmt(stmt1);
			
			struct InterCodeNode* c1 = newInterCodeNode(LABELOP, label1, NULL, NULL, NULL);
			struct InterCodeNode* c2 = newInterCodeNode(LABELOP, label2, NULL, NULL, NULL);
			struct InterCodeNode* c3 = newInterCodeNode(GOTO, label1, NULL, NULL, NULL);
			struct InterCodeNode* c4 = newInterCodeNode(LABELOP, label3, NULL, NULL, NULL);
			return concat(6 ,c1, code1, c2, code2, c3, c4);
			}
	}
}

struct InterCodeNode* translate_Cond(struct Node* root, Operand label_true, Operand label_false) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_Cond():%s\t%s\n", root->token, root->lexeme);
#endif
	switch(root->rule) {
		case Exp__Exp_RELOP_Exp:{
			struct Node* exp1 = root->child;
			struct Node* relop = exp1->nextSibling;
			struct Node* exp2 = relop->nextSibling;

			Operand t1 = new_temp();
			Operand t2 = new_temp();
			struct InterCodeNode* code1 = translate_Exp(exp1, t1);

#ifdef DEBUG
	printf("....EXP_RELOP_EXP!\n");
	printInterCodes(code1);
#endif

			struct InterCodeNode* code2 = translate_Exp(exp2, t2);
			char* op = relop->lexeme;
			struct InterCodeNode* code3 = newInterCodeNode(IFOP, t1, t2, label_true, op);
			struct InterCodeNode* code4 = newInterCodeNode(GOTO, label_false, NULL, NULL ,NULL);
			return concat(4, code1, code2, code3, code4);
		}
		case Exp__NOT_Exp:{
			return translate_Cond(root->child->nextSibling, label_false, label_true);
		}
		case Exp__Exp_AND_Exp:{
			struct Node* exp1 = root->child;
			struct Node* exp2 = exp1->nextSibling->nextSibling;

			Operand label1 = new_label();
			struct InterCodeNode* code1 = translate_Cond(exp1, label1, label_false);
			struct InterCodeNode* code2 = translate_Cond(exp2, label_true, label_false);
			struct InterCodeNode* c = newInterCodeNode(LABELOP, label1, NULL, NULL, NULL);
			return concat(3, code1, c, code2);
		}
		case Exp__Exp_OR_Exp:{
			struct Node* exp1 = root->child;
			struct Node* exp2 = exp1->nextSibling->nextSibling;

			Operand label1 = new_label();
			struct InterCodeNode* code1 = translate_Cond(exp1, label_true, label1);
			struct InterCodeNode* code2 = translate_Cond(exp2, label_true, label_false);
			struct InterCodeNode* c = newInterCodeNode(LABELOP, label1, NULL, NULL, NULL);
			return concat(3, code1, c, code2);
		}
		default: {
					Operand t1 = new_temp();
					struct InterCodeNode* code1 = translate_Exp(root, t1);
					struct InterCodeNode* code2 = newInterCodeNode(IFOP, t1, constant0, label_true, "!=");
					struct InterCodeNode* code3 = newInterCodeNode(GOTO, label_false, NULL, NULL, NULL);

					return concat(3, code1, code2, code3);
				 }
	}
}

struct InterCodeNode* translate_Args(struct Node* root, struct OperandNode* arg_list) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_Args():%s\t%s\n", root->token, root->lexeme);
#endif
	switch(root->rule) {
		case Args__Exp: {
			struct Node* exp = root->child;

			Operand t1 = new_temp();
			struct InterCodeNode* code1 = translate_Exp(exp, t1);

			arg_list = arg_concat(t1, arg_list);
#ifdef DEBUG
	printf("Args__Exp\n");
	printInterCodes(code1);
#endif
			return code1;
		}
		case Args__Exp_COMMA_Args: {
			struct Node* exp = root->child;
			struct Node* args1 = exp->nextSibling->nextSibling;

			Operand t1 = new_temp();
			struct InterCodeNode* code1 = translate_Exp(exp, t1);
			arg_list = arg_concat(t1, arg_list);
			struct InterCodeNode* code2 = translate_Args(args1, arg_list);

			return concat(2, code1, code2);
		}
	}
}

struct InterCodeNode* translate_Compst(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_Compst():%s\t%s\n", root->token, root->lexeme);
#endif
	struct Node* cur = root->child->nextSibling;
	
	struct InterCodeNode* code = NULL;
	if(strcmp(cur->token, "DefList") == 0) {
		struct InterCodeNode* code1= translate_DefList(cur);
		code = concat(2, code, code1);
		cur = cur->nextSibling;
	}
	if(strcmp(cur->token, "StmtList") == 0) {
		struct InterCodeNode* code2 = translate_StmtList(cur);
		code = concat(2, code, code2);
	}

	return code;
}

struct InterCodeNode* translate_DefList(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_DefList():%s\t%s\n", root->token, root->lexeme);
#endif
	struct Node* def = root->child;
	struct Node* deflist = def->nextSibling;

	struct InterCodeNode* code1 = translate_Def(def);
	struct InterCodeNode* code2 = translate_DefList(deflist);

	return concat(2, code1, code2);
}

struct InterCodeNode* translate_StmtList(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_StmtList():%s\t%s\n", root->token, root->lexeme);
#endif
	struct Node* stmt = root->child;
	struct Node* stmtlist = stmt->nextSibling;

	struct InterCodeNode* code1 = translate_Stmt(stmt);
	struct InterCodeNode* code2 = translate_StmtList(stmtlist);

	return concat(2, code1, code2);
}

struct InterCodeNode* translate_Def(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_Def():%s\t%s\n", root->token, root->lexeme);
#endif
	struct Node* specifier = root->child;
	struct Node* declist = specifier->nextSibling;

	// specifier ........
	
	struct InterCodeNode* code2 = translate_DecList(declist);
	return code2;
}

struct InterCodeNode* translate_DecList(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_DecList():%s\t%s\n", root->token, root->lexeme);
#endif
	struct Node* dec = root->child;
	struct InterCodeNode* code = translate_Dec(dec);

	if(root->rule == DecList__Dec_COMMA_DecList) {
		struct Node* declist = dec->nextSibling->nextSibling;
		struct InterCodeNode* code2 = translate_DecList(root);
		code = concat(2, code, code2);
	}

	return code;
}

struct InterCodeNode* translate_Dec(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_Dec():%s\t%s\n", root->token, root->lexeme);
#endif
	struct Node* vardec = root->child;
	struct InterCodeNode* code = translate_VarDec(vardec);

	struct Symbol* sym = lookupVariable(vardec->lexeme);
	Operand place = newOperand(VARIABLE, sym->var_no);

	if(root->rule == Dec__VarDec_ASSIGNOP_Exp) {
		struct Node* exp = vardec->nextSibling->nextSibling;
		struct InterCodeNode* code2 = translate_Exp(exp, place);
		code = concat(2, code, code2);
	}

	return code;
}

struct InterCodeNode* translate_VarDec(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_VarDec():%s\t%s\n", root->token, root->lexeme);
#endif
	if(root->rule == VarDec__VarDec_LB_INT_RB) {
		printf("Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type.\n");
		return NULL;//!!!!!!!!!!!!!!
	}
	
	struct Node* id = root->child;
	root->lexeme = id->lexeme;
	return NULL;
}

struct InterCodeNode* translate_ExtDefList(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_ExtDefList():%s\t%s\n", root->token, root->lexeme);
#endif
	struct Node* extdef = root->child;
	struct Node* extdeflist = extdef->nextSibling;

	struct InterCodeNode* code1 = translate_ExtDef(extdef);
	struct InterCodeNode* code2 = translate_ExtDefList(extdeflist);

	return concat(2, code1, code2);
}

struct InterCodeNode* translate_ExtDef(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_ExtDef():%s\t%s\n", root->token, root->lexeme);
#endif
	switch(root->rule) {
		case ExtDef__Specifier_ExtDecList_SEMI: {
			struct Node* extdeclist = root->child->nextSibling;

			struct InterCodeNode* code = translate_ExtDecList(extdeclist);
			return code;
		}
		case ExtDef__Specifier_FunDec_Compst: {
			struct Node* fundec = root->child->nextSibling;
			struct Node* compst = fundec->nextSibling;

			struct InterCodeNode* code1 = translate_FunDec(fundec);
			struct InterCodeNode* code2 = translate_Compst(compst);

#ifdef DEBUG
	printf("ExtDef__Specifier_FunDec_Compst\n");
	printf("--code1:\n");
	printInterCodes(code1);
	printf("--code2:\n");
	printInterCodes(code2);
#endif
			return concat(2, code1, code2);
		}
		default: printf("3\n");return NULL;
	}
}

struct InterCodeNode* translate_ExtDecList(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_ExtDecList():%s\t%s\n", root->token, root->lexeme);
#endif

	struct Node* vardec = root->child;
	struct InterCodeNode* code = translate_VarDec(vardec);

	if(root->rule == ExtDecList__VarDec_COMMA_ExtDecList) {
		struct Node* extdeclist = vardec->nextSibling->nextSibling;
		struct InterCodeNode* code2 = translate_ExtDecList(extdeclist);

		return concat(2, code, code2);
	}
	return code;
}

struct InterCodeNode* translate_FunDec(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_FunDec():%s\t%s\n", root->token, root->lexeme);
#endif
	struct Node* id = root->child;
	char* function = id->lexeme;

	struct InterCodeNode* code1 = newInterCodeNode(FUNCTION, NULL, NULL, NULL, function);

	if(root->rule == FunDec__ID_LP_VarList_RP) {
		struct Node* varlist = id->nextSibling->nextSibling;

		struct InterCodeNode* code2 = translate_VarList(varlist);

		code1 = concat(2, code1, code2);
	}
	return code1;
}

struct InterCodeNode* translate_VarList(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_VarList():%s\t%s\n", root->token, root->lexeme);
#endif
	struct Node* paramdec = root->child;

	struct InterCodeNode* code = translate_ParamDec(paramdec);

	if(root->rule == VarList__ParamDec_COMMA_VarList) {
		struct Node* varlist = paramdec->nextSibling->nextSibling;
		struct InterCodeNode* code2 = translate_VarList(varlist);

		code = concat(2, code, code2);
	}

	return code;
}

struct InterCodeNode* translate_ParamDec(struct Node* root) {
	if(root == NULL)return NULL;
#ifdef DEBUG
	printf("enter translate_ParamDec():%s\t%s\n", root->token, root->lexeme);
#endif
	struct Node* specifier = root->child;
	struct Node* vardec = specifier->nextSibling;

	struct InterCodeNode* code1 = translate_VarDec(vardec);

	struct InterCodeNode* code2 = newInterCodeNode(PARAM, NULL, NULL, NULL, vardec->lexeme);
	return concat(2, code1, code2);
}

void generateIR(struct Node* root, char* filename) {
	constant0 = newOperand(CONSTANT, 0);
	constant1 = newOperand(CONSTANT, 1);

	icHead = translate_ExtDefList(root->child);

	outputIR2File(filename);
}
