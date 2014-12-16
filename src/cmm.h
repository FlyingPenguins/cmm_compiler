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
int add_id(hash_table_t *hashtable, char *name, int type, double value);
void free_table(hash_table_t *hashtable);




enum type {			/* variable types */
    t_int = 1,
    t_double,
    t_array_double,
    t_array_int,
    t_func
};

/* node types */
enum node_t {
    ADD = 1,// '+'
    SUB,    // '-'
    MUL,    // '*'
    DIV,    // '/'
    MOD,    // %
    K,      // int
    D,      // double or float
    GT,     // >
    LT,     // <
    GTEQ,   // >=
    LTEQ,   // <=
    NOTEQ,  // !=
    EQ,     // ==
    UMIN, // (-)var
    LIST,   // statement list
    I,      // If
    W,      // While
    ASG,    // '=' assignment
    SYM,    // symbol reference
    CALL,   // function call
    INC,    // increment/decrement '--' '++'
    L_NOT,    // not
    L_AND,    // and
    L_OR,     // or
    IN,     // input 'cin >>'
    OUT     // output 'cout <<'
};

/* nodes in the Abstract Syntax Tree */
/* all have common initial nodetype */

struct ast {
  int nodetype;
  struct ast *l;
  struct ast *r;
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
  int type;
};

struct symasgn {
  int nodetype;			/* type = */
  symbol *s;
  struct ast *v;		/* value */
};

struct symin {
	int nodetype;		/*type IN */
	symbol *s;
};

struct symout {
	int nodetype;		/*type OUT */
	struct ast *a;
	char* str;
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
struct ast *newin(symbol *s);
struct ast *newout(struct ast *a, char* str);

/* define a function */
void dodef(symbol *name, struct symlist *syms, struct ast *stmts);

/* evaluate an AST */
double eval(struct ast *);

/* delete and free an AST */
void treefree(struct ast *);

/* interface to the lexer */
extern int yylineno; /* from lexer */
extern int yyparse(void); /* from bison */
void yyerror(char *s, int level, ...);

extern int debug;
void dumpast(struct ast *a, int level);

