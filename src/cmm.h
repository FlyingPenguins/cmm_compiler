/* Adapted from "flex & bison", published by O'Reilly
 */
/*
 * Declarations for cmm compiler
 */

/* types for symbol table */
typedef struct _list_t_ symbol;

typedef struct _hash_table_t_ hash_table_t;

struct symlist {
  symbol *sym;
  struct symlist *next;
};

struct symlist *newsymlist(symbol *sym, struct symlist *next);
/* functions for symbol table */
void symlistfree(struct symlist *sl);
hash_table_t *create_hash_table(int size);
unsigned oat_hash(hash_table_t *hashtable, void *key, int len);
symbol *lookup_string(hash_table_t *hashtable, char *str);
int add_id(hash_table_t *hashtable, char *name, char *type, double value);
void free_table(hash_table_t *hashtable);

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
  char* nodetype;
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
struct ast *newast(char* nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(char* cmptype, struct ast *l, struct ast *r);
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
extern int yyparse(void); /* from bison */
void yyerror(char *s, ...);

extern int debug;
void dumpast(struct ast *a, int level);

