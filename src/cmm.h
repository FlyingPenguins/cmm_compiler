/* Adapted from "flex & bison", published by O'Reilly
 */
/*
 * Declarations for cmm compiler
 */

/* list of symbols, for an argument list */
#include "symtable.h"
struct symlist {
  symbol *sym;
  struct symlist *next;
};

struct symlist *newsymlist(symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

/* node types
 *  + - * / |
 *  K Integer
 *  D Double/Float
 *  0-7 -> CMP Comparison Operator //TODO Figure out what to do with comparisons
 *  M unary minus
 *  L statement list
 *  I IF statement
 *  W WHILE statement
 *  N symbol ref
 *  = assignment
 *  S list of symbols
 *  F built in function call
 *  C user function call
 *  INC Increment operator +=, -=
 *  LOG Logical Operator
 *  IO Stream In/Out ">>" "<<"
 */ 

enum bifs {			/* built-in functions */
  B_sqrt = 1,
  B_exp,
  B_log,
  B_print
};

/* nodes in the Abstract Syntax Tree */
/* all have common initial nodetype */

struct ast {
  int nodetype;
  struct ast *l;
  struct ast *r;
};

struct fncall {			/* built-in function */
  int nodetype;			/* type F */
  struct ast *l;
  enum bifs functype;
};

struct ufncall {		/* user function */
  int nodetype;			/* type C */
  struct ast *l;		/* list of arguments */
  symbol *s;
};

struct flow {
  int nodetype;			/* type I or W */
  struct ast *cond;		/* condition */
  struct ast *tl;		/* then or do list */
  struct ast *el;		/* optional else list */
};

struct floatval {
  int nodetype;			/* type K */
  double number;
};

struct intval {
  int nodetype;         /* TODO Create type id */
  int number;
};

struct symref {
  int nodetype;			/* type N */
  symbol *s;
};

struct symasgn {
  int nodetype;			/* type = */
  symbol *s;
  struct ast *v;		/* value */
};

/* build an AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(symbol *s, struct ast *l);
struct ast *newref(symbol *s);
struct ast *newasgn(symbol *s, struct ast *v);
struct ast *newfloat(double d);
struct ast *newint(int d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

/* define a function */
void dodef(symbol *name, struct symlist *syms, struct ast *stmts);

/* evaluate an AST */
double eval(struct ast *);

/* delete and free an AST */
void treefree(struct ast *);

/* interface to the lexer */
extern int yylineno; /* from lexer */
void yyerror(char *s, ...);

extern int debug;
void dumpast(struct ast *a, int level);
