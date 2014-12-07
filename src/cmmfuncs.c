/* Source code addapted for "flex & bison", published by O'Reilly
 * Media, ISBN 978-0-596-15597-1
 */
/*
 * helper functions for cmm.c
 */
#	include <stdio.h>
#	include <stdlib.h>
#	include <stdarg.h>
#	include <string.h>
#	include <math.h>
#	include "cmm.h"

/* symbol table definitions */
#define OAT_HASH_LEN (20)


struct _list_t_ {
    char *name;
    char *type;
    double value;
    struct ast *func;   /* statemnts for the function */
    struct symlist *syms; /* list of dummy variables */
    struct _list_t_ *next; /* linked list for collisions */
};

struct _hash_table_t_ {
    int size;
    symbol **table;
};

hash_table_t
*create_hash_table(int size)
{
    hash_table_t *new_table;

    if (size < 1) return NULL;

    /* Attempt to allocate memory for the table structure */
    if ((new_table = malloc(sizeof(hash_table_t))) == NULL) {
        return NULL;
    }

    /* Attempt to allocate memory for the table itself */
    if ((new_table->table = malloc(sizeof(symbol *) *size)) == NULL) {
        return NULL;
    }

    /* Initialize the elements of the table */
    for(int i = 0; i < size; i++) new_table->table[i] = NULL;

    /* Set the table's size */
    new_table->size = size;

    return new_table;
}

/* Jenkins' one-at-a-time hash */
unsigned
oat_hash(hash_table_t *hashtable, void *key, int len)
{

        unsigned char *p = key;
        unsigned h = 0;
        int i;

        for (i = 0; i < len; i++) {
                h += p[i];
                h += ( h << 10 );
                h ^= ( h >> 6 );
        }

        h += ( h << 3 );
        h ^= (h >> 11);
        h += (h << 15);

        /* TODO Collision detection? */
        return h % hashtable->size;
}

symbol
*lookup(hash_table_t *hashtable, char *str)
{
        symbol *list;
        unsigned int h = oat_hash(hashtable, str, OAT_HASH_LEN);

        /* Go to the correct list based on the hash value and see if str is in
         * the list. If it is, return a pointer to the list element.
         * If it isn't, the item isn't in the table, so return NULL.
         */
        for(list = hashtable->table[h]; list != NULL; list = list->next) {
                if (strcmp(str, list->name) == 0) return list;
        }

        return NULL;
}

int
add_id(hash_table_t *hashtable, char *name, char *type, double value)
{
        symbol *new_list;
        symbol *current_list;
        unsigned int h = oat_hash(hashtable, name, OAT_HASH_LEN);

        /* Attempt to allocate memory for list */
        if ((new_list = malloc(sizeof(symbol))) == NULL) return 1;

        /* Does item already exist? */
        current_list = lookup(hashtable, name);
        /* Item already exists, don't insert it again */
        if (current_list != NULL) return 2;
        /* Insert into list */
        new_list->name = strdup(name);
        new_list->type = strdup(type);
        new_list->value = value;
        new_list->next = hashtable->table[h];
        hashtable->table[h] = new_list;

        return 0;
}

void
free_table(hash_table_t *hashtable) {
        int i;
        symbol *list, *temp;

        if (hashtable == NULL) return;

        /* Free the memory for every item in the table, including the names,
         * types, and values themselves.
         */
        for(int i = 0; i < hashtable->size; i++) {
                list = hashtable->table[i];
                while(list != NULL) {
                        temp = list;
                        list = list->next;
                        free(temp->name);
                        free(temp->type);
                        //free(temp->value); 
                        free(temp);
                }
        }

        /* Free the table itself */
        free(hashtable->table);
        free(hashtable);
}

struct symlist *
newsymlist(symbol *sym, struct symlist *next)
{
  struct symlist *sl = malloc(sizeof(struct symlist));
  
  if(!sl) {
    yyerror("out of space");
    exit(0);
  }
  sl->sym = sym;
  sl->next = next;
  return sl;
}

void
symlistfree(struct symlist *sl)
{
  struct symlist *nsl;

  while(sl) {
    nsl = sl->next;
    free(sl);
    sl = nsl;
  }
}

/* end symbol table defintions */


/* ast type definitions */
struct ast *
newast(int nodetype, struct ast *l, struct ast *r)
{
  struct ast *a = malloc(sizeof(struct ast));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  a->l = l;
  a->r = r;
  return a;
}

struct ast *
newfloat(double d)
{
  struct floatval *a = malloc(sizeof(struct floatval));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'D';
  a->number = d;
  return (struct ast *)a;
}
struct ast *
newint(int k)
{
  struct intval *a = malloc(sizeof(struct intval));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'K';
  a->number = k;
  return (struct ast *)a;
}

struct ast *
newcmp(int cmptype, struct ast *l, struct ast *r)
{
  struct ast *a = malloc(sizeof(struct ast));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = '0' + cmptype;
  a->l = l;
  a->r = r;
  return a;
}

struct ast *
newfunc(int functype, struct ast *l)
{
  struct fncall *a = malloc(sizeof(struct fncall));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'F';
  a->l = l;
  a->functype = functype;
  return (struct ast *)a;
}

struct ast *
newcall(symbol *s, struct ast *l)
{
  struct ufncall *a = malloc(sizeof(struct ufncall));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'C';
  a->l = l;
  a->s = s;
  return (struct ast *)a;
}

struct ast *
newref(symbol *s)
{
  struct symref *a = malloc(sizeof(struct symref));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'N';
  a->s = s;
  return (struct ast *)a;
}

struct ast *
newasgn(symbol *s, struct ast *v)
{
  struct symasgn *a = malloc(sizeof(struct symasgn));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = '=';
  a->s = s;
  a->v = v;
  return (struct ast *)a;
}

struct ast *
newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *el)
{
  struct flow *a = malloc(sizeof(struct flow));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  a->cond = cond;
  a->tl = tl;
  a->el = el;
  return (struct ast *)a;
}



/* define a function */
void
dodef(symbol *name, struct symlist *syms, struct ast *func)
{
  if(name->syms) symlistfree(name->syms);
  if(name->func) treefree(name->func);
  name->syms = syms;
  name->func = func;
}

static double callbuiltin(struct fncall *);
static double calluser(struct ufncall *);

double
eval(struct ast *a)
{
  double v;

  if(!a) {
    yyerror("internal error, null eval");
    return 0.0;
  }

  switch(a->nodetype) {
    /* float/double */
  case 'D': v = ((struct floatval *)a)->number; break;

    /* int */
  case 'K': v = ((struct intval *)a)->number; break;

    /* name reference */
  case 'N': v = ((struct symref *)a)->s->value; break;

    /* assignment */
  case '=': v = ((struct symasgn *)a)->s->value =
      eval(((struct symasgn *)a)->v); break;

    /* expressions */
  case '+': v = eval(a->l) + eval(a->r); break;
  case '-': v = eval(a->l) - eval(a->r); break;
  case '*': v = eval(a->l) * eval(a->r); break;
  case '/': v = eval(a->l) / eval(a->r); break;
  case '|': v = fabs(eval(a->l)); break;
  case 'M': v = -eval(a->l); break;

    /* comparisons */
  case '1': v = (eval(a->l) > eval(a->r))? 1 : 0; break;
  case '2': v = (eval(a->l) < eval(a->r))? 1 : 0; break;
  case '3': v = (eval(a->l) != eval(a->r))? 1 : 0; break;
  case '4': v = (eval(a->l) == eval(a->r))? 1 : 0; break;
  case '5': v = (eval(a->l) >= eval(a->r))? 1 : 0; break;
  case '6': v = (eval(a->l) <= eval(a->r))? 1 : 0; break;

  /* control flow */
  /* null if/else/do expressions allowed in the grammar, so check for them */
  case 'I': 
    if( eval( ((struct flow *)a)->cond) != 0) {
      if( ((struct flow *)a)->tl) {
	v = eval( ((struct flow *)a)->tl);
      } else
	v = 0.0;		/* a default value */
    } else {
      if( ((struct flow *)a)->el) {
        v = eval(((struct flow *)a)->el);
      } else
	v = 0.0;		/* a default value */
    }
    break;

  case 'W':
    v = 0.0;		/* a default value */
    
    if( ((struct flow *)a)->tl) {
      while( eval(((struct flow *)a)->cond) != 0)
	v = eval(((struct flow *)a)->tl);
    }
    break;			/* last value is value */
	              
  case 'L': eval(a->l); v = eval(a->r); break;

  case 'F': v = callbuiltin((struct fncall *)a); break;

  case 'C': v = calluser((struct ufncall *)a); break;

  default: printf("internal error: bad node %c\n", a->nodetype);
  }
  return v;
}

static double
callbuiltin(struct fncall *f)
{
/* TODO remove or replace this function
  enum bifs functype = f->functype;
  double v = eval(f->l);

 switch(functype) {

 case B_sqrt:
   return sqrt(v);
 case B_exp:
   return exp(v);
 case B_log:
   return log(v);
 case B_print:
   printf("= %4.4g\n", v);
   return v;

 default:
   yyerror("Unknown built-in function %d", functype);
   return 0.0;
 }
/**/
 return 0.0; // default return
}

static double
calluser(struct ufncall *f)
{
  symbol *fn = f->s;	/* function name */
  struct symlist *sl;		/* dummy arguments */
  struct ast *args = f->l;	/* actual arguments */
  double *oldval, *newval;	/* saved arg values */
  double v;
  int nargs;
  int i;

  if(!fn->func) {
    yyerror("call to undefined function", fn->name);
    return 0;
  }

  /* count the arguments */
  sl = fn->syms;
  for(nargs = 0; sl; sl = sl->next)
    nargs++;

  /* prepare to save them */
  oldval = (double *)malloc(nargs * sizeof(double));
  newval = (double *)malloc(nargs * sizeof(double));
  if(!oldval || !newval) {
    yyerror("Out of space in %s", fn->name); return 0.0;
  }
  
  /* evaluate the arguments */
  for(i = 0; i < nargs; i++) {
    if(!args) {
      yyerror("too few args in call to %s", fn->name);
      free(oldval); free(newval);
      return 0;
    }

    if(args->nodetype == 'L') {	/* if this is a list node */
      newval[i] = eval(args->l);
      args = args->r;
    } else {			/* if it's the end of the list */
      newval[i] = eval(args);
      args = NULL;
    }
  }
		     
  /* save old values of dummies, assign new ones */
  sl = fn->syms;
  for(i = 0; i < nargs; i++) {
    symbol *s = sl->sym;

    oldval[i] = s->value;
    s->value = newval[i];
    sl = sl->next;
  }

  free(newval);

  /* evaluate the function */
  v = eval(fn->func);

  /* put the dummies back */
  sl = fn->syms;
  for(i = 0; i < nargs; i++) {
    symbol *s = sl->sym;

    s->value = oldval[i];
    sl = sl->next;
  }

  free(oldval);
  return v;
}


void
treefree(struct ast *a)
{
  switch(a->nodetype) {

    /* two subtrees */
  case '+':
  case '-':
  case '*':
  case '/':
  case '1':  case '2':  case '3':  case '4':  case '5':  case '6':
  case 'L':
    treefree(a->r);

    /* one subtree */
  case '|':
  case 'M': case 'C': case 'F':
    treefree(a->l);

    /* no subtree */
  case 'D': case 'K': case 'N':
    break;

  case '=':
    free( ((struct symasgn *)a)->v);
    break;

  case 'I': case 'W':
    free( ((struct flow *)a)->cond);
    if( ((struct flow *)a)->tl) free( ((struct flow *)a)->tl);
    if( ((struct flow *)a)->el) free( ((struct flow *)a)->el);
    break;

  default: printf("internal error: free bad node %c\n", a->nodetype);
  }	  
  
  free(a); /* always free the node itself */

}

void
yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int
main()
{
  printf("> "); 
  return yyparse();
}

/* debugging: dump out an AST */
int debug = 0;
void
dumpast(struct ast *a, int level)
{

  printf("%*s", 2*level, "");	/* indent to this level */
  level++;

  if(!a) {
    printf("NULL\n");
    return;
  }

  switch(a->nodetype) {
    /* float/double */
  case 'D': printf("number %4.4g\n", ((struct floatval *)a)->number); break;

    /* int */
  case 'K': printf("number %4.4d\n", ((struct intval *)a)->number); break;

    /* name reference */
  case 'N': printf("ref %s\n", ((struct symref *)a)->s->name); break;

    /* assignment */
  case '=': printf("= %s\n", ((struct symref *)a)->s->name);
    dumpast( ((struct symasgn *)a)->v, level); return;

    /* expressions */
  case '+': case '-': case '*': case '/': case 'L':
  case '1': case '2': case '3':
  case '4': case '5': case '6': 
    printf("binop %c\n", a->nodetype);
    dumpast(a->l, level);
    dumpast(a->r, level);
    return;

  case '|': case 'M': 
    printf("unop %c\n", a->nodetype);
    dumpast(a->l, level);
    return;

  case 'I': case 'W':
    printf("flow %c\n", a->nodetype);
    dumpast( ((struct flow *)a)->cond, level);
    if( ((struct flow *)a)->tl)
      dumpast( ((struct flow *)a)->tl, level);
    if( ((struct flow *)a)->el)
      dumpast( ((struct flow *)a)->el, level);
    return;
	              
  case 'F':
    printf("builtin %d\n", ((struct fncall *)a)->functype);
    dumpast(a->l, level);
    return;

  case 'C': printf("call %s\n", ((struct ufncall *)a)->s->name);
    dumpast(a->l, level);
    return;

  default: printf("bad %c\n", a->nodetype);
    return;
  }
}
