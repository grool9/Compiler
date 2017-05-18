#include "common.h"

static struct InterCode* no_rule(struct Node* root);
static struct InterCode* exp__int(struct Node* root);
static struct InterCode* program__extdeflist(struct Node* root);
static struct InterCode* extdeflist__extdef_extdeflist(struct Node* root);
static struct InterCode* extdef__specifier_extdeclist_semi(struct Node* root);
static struct InterCode* extdef__specifier_semi(struct Node* root);
static struct InterCode* extdef__specifier_fundec_compst(struct Node* root);
static struct InterCode* extdeclist__vardec(struct Node* root);
static struct InterCode* extdeclist__vardec_comma_extdeclist(struct Node* root);
static struct InterCode* specifier__type(struct Node* root);
static struct InterCode* specifier__structspecifier(struct Node* root);
static struct InterCode* structspecifier__struct_opttag_lc_deflist_rc(struct Node* root);
static struct InterCode* structspecifier__struct_tag(struct Node* root);
static struct InterCode* opttag__id(struct Node* root);
static struct InterCode* tag__id(struct Node* root);
static struct InterCode* vardec__id(struct Node* root);
static struct InterCode* vardec__vardec_lb_int_rb(struct Node* root) ;
static struct InterCode* fundec__id_lp_varlist_rp(struct Node* root);
static struct InterCode* fundec__id_lp_rp(struct Node* root);
static struct InterCode* varlist__paramdec_comma_varlist(struct Node* root);
static struct InterCode* varlist__paramdec(struct Node* root);
static struct InterCode* paramdec__specifier_vardec(struct Node* root);
static struct InterCode* compst__lc_deflist_stmtlist_rc(struct Node* root);
static struct InterCode* stmtlist__stmt_stmtlist(struct Node* root);
static struct InterCode* stmt__exp_semi(struct Node* root);
static struct InterCode* stmt__compst(struct Node* root);
static struct InterCode* stmt__return_exp_semi(struct Node* root);
static struct InterCode* stmt__if_lp_exp_rp_stmt(struct Node* root);
static struct InterCode* stmt__if_lp_exp_rp_stmt_else_stmt(struct Node* root);
static struct InterCode* stmt__while_lp_exp_rp_stmt(struct Node* root);
static struct InterCode* deflist__def_deflist(struct Node* root);
static struct InterCode* def__specifier_declist_semi(struct Node* root);
static struct InterCode* declist__dec(struct Node* root);
static struct InterCode* declist__dec_comma_declist(struct Node* root);
static struct InterCode* dec__vardec(struct Node* root);
static struct InterCode* dec__vardec_assignop_exp(struct Node* root);
static struct InterCode* exp__exp_assignop_exp(struct Node* root);
static struct InterCode* exp__exp_and_exp(struct Node* root);
static struct InterCode* exp__exp_or_exp(struct Node* root);
static struct InterCode* exp__exp_relop_exp(struct Node* root);
static struct InterCode* exp__exp_plus_exp(struct Node* root);
static struct InterCode* exp__exp_minus_exp(struct Node* root);
static struct InterCode* exp__exp_star_exp(struct Node* root);
static struct InterCode* exp__exp_div_exp(struct Node* root);
static struct InterCode* exp__lp_exp_rp(struct Node* root);
static struct InterCode* exp__minus_exp(struct Node* root);
static struct InterCode* exp__not_exp(struct Node* root);
static struct InterCode* exp__id_lp_args_rp(struct Node* root);
static struct InterCode* exp__id_lp_rp(struct Node* root);
static struct InterCode* exp__exp_lb_exp_rb(struct Node* root);
static struct InterCode* exp__exp_dot_id(struct Node* root);
static struct InterCode* exp__id(struct Node* root);
static struct InterCode* exp__int(struct Node* root);
static struct InterCode* exp__float(struct Node* root);
static struct InterCode* args__exp_comma_args(struct Node* root);
static struct InterCode* args__exp(struct Node* root);

typedef struct InterCode* (*Function)(struct Node*);

Function functionList[Args__Exp+1] = {
	no_rule,
	program__extdeflist, 
	extdeflist__extdef_extdeflist, 
	extdef__specifier_extdeclist_semi, 
	extdef__specifier_semi, 
	extdef__specifier_fundec_compst, 
	extdeclist__vardec, 
	extdeclist__vardec_comma_extdeclist, 
	specifier__type, 
	specifier__structspecifier, 
	structspecifier__struct_opttag_lc_deflist_rc, 
	structspecifier__struct_tag, 
	opttag__id, 
	tag__id, 
	vardec__id, 
	vardec__vardec_lb_int_rb, 
	fundec__id_lp_varlist_rp, 
	fundec__id_lp_rp, 
	varlist__paramdec_comma_varlist, 
	varlist__paramdec, 
	paramdec__specifier_vardec, 
	compst__lc_deflist_stmtlist_rc, 
	stmtlist__stmt_stmtlist, 
	stmt__exp_semi, 
	stmt__compst, 
	stmt__return_exp_semi, 
	stmt__if_lp_exp_rp_stmt, 
	stmt__if_lp_exp_rp_stmt_else_stmt, 
	stmt__while_lp_exp_rp_stmt, 
	deflist__def_deflist, 
	def__specifier_declist_semi, 
	declist__dec, 
	declist__dec_comma_declist, 
	dec__vardec, 
	dec__vardec_assignop_exp, 
	exp__exp_assignop_exp, 
	exp__exp_and_exp, 
	exp__exp_or_exp, 
	exp__exp_relop_exp,
	exp__exp_plus_exp,
	exp__exp_minus_exp,
	exp__exp_star_exp,
	exp__exp_div_exp,
	exp__lp_exp_rp,
	exp__minus_exp,
	exp__not_exp,
	exp__id_lp_args_rp,
	exp__id_lp_rp,
	exp__exp_lb_exp_rb,
	exp__exp_dot_id,
	exp__id,
	exp__int,
	exp__float,
	args__exp_comma_args,
	args__exp
};

#define translate(x)  (((x) == NULL) ? NULL : functionList[(x)->rule](x))

void generateIR(struct Node* root, char* filename) {
	translate(root);

	// file...
}

static struct InterCode* no_rule(struct Node* root) { // depth traversal
	struct Node* child = root->child;
	while(child != NULL) {
		translate(child);
		child = child->nextSibling;
	}
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* program__extdeflist(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* extdeflist__extdef_extdeflist(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* extdef__specifier_extdeclist_semi(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* extdef__specifier_semi(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
	
static struct InterCode* extdef__specifier_fundec_compst(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* extdeclist__vardec(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* extdeclist__vardec_comma_extdeclist(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* specifier__type(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}	
 
static struct InterCode* specifier__structspecifier(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* structspecifier__struct_opttag_lc_deflist_rc(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

//
static struct InterCode* structspecifier__struct_tag(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* opttag__id(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* tag__id(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* vardec__id(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* vardec__vardec_lb_int_rb(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* fundec__id_lp_varlist_rp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* fundec__id_lp_rp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* varlist__paramdec_comma_varlist(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}	
	

static struct InterCode* varlist__paramdec(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* paramdec__specifier_vardec(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* compst__lc_deflist_stmtlist_rc(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* stmtlist__stmt_stmtlist(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}

static struct InterCode* stmt__exp_semi(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* stmt__compst(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* stmt__return_exp_semi(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* stmt__if_lp_exp_rp_stmt(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
	
//	
static struct InterCode* stmt__if_lp_exp_rp_stmt_else_stmt(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* stmt__while_lp_exp_rp_stmt(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* deflist__def_deflist(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* def__specifier_declist_semi(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
	

static struct InterCode* declist__dec(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* declist__dec_comma_declist(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* dec__vardec(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* dec__vardec_assignop_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
} 
	
//
static struct InterCode* exp__exp_assignop_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__exp_and_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__exp_or_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__exp_relop_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__exp_plus_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__exp_minus_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
	
static struct InterCode* exp__exp_star_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}	
static struct InterCode* exp__exp_div_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__lp_exp_rp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__minus_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__not_exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
//
static struct InterCode* exp__id_lp_args_rp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__id_lp_rp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__exp_lb_exp_rb(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__exp_dot_id(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__id(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__int(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* exp__float(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* args__exp_comma_args(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
static struct InterCode* args__exp(struct Node* root) {
	//int value = root->child->lexeme;
	struct InterCode* tmp;
	return tmp;
}
