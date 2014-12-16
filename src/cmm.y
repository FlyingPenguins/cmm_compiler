/* Bison parser for cmm language compiler
 * Group: Flying Penguins
 * Members: Zoe Fultz, Bryan Garza, 
 * Brandon Halpin, Joshua Titley, Francis Tweedy
 * 
 *
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include "cmm.h"
extern int DEBUG;
extern hash_table_t* hash_table;
%}

%union {
    struct ast *a;  /* Abstract syntax trees */
    double d;       /* float literals */
    int i;          /* int literals */
    char* str;      /* string literals */
    char* id;       /* identifiers */
    char* op;       /* operators +, -, *, /, =, +=, -=, <=, >=... */
    struct symlist *sl;
    struct symbol *s;
}

/* declare tokens */
%token <op> ASSIGNOP
%token <op> MULOP
%token <op> ADDOP
%token <op> UMINUS  /* unary minus, prevents using "+-<expresion>" */
%token <op> INCOP
%token <op> RELOP
%token <op> NOT
%token <op> OR
%token <op> AND
%token <d> FLT_LITERAL
%token <i> INT_LITERAL
%token <str> STR_LITERAL
%token <op> STREAMIN
%token <op> STREAMOUT
%token <op> CIN
%token <op> COUT
%token <id> ID
/*%token <id> FUNC_ID*/
/* reserved words */
%token ELSE ENDL FLOAT IF INT RETURN WHILE

/* Define L->R or R->L grouping and operator
 * precedence. For justification, see:
 * http://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B#Operator_precedence
 */
%right ASSIGNOP INCOP
%left AND OR ID
%left RELOP
%left STREAMIN STREAMOUT
%left ADDOP
%left MULOP
%nonassoc NOT UMINUS


%type <a> function_definitions expression statements statement output_statement compound_statement input_statement arguments identifier_list bool_expression bool_factor bool_term factor term literal simple_expression expressions expression_list 
%type <sl> parameter_list
%type <s> variable

%%

program: /* nothing */
| variable_definitions function_definitions {
    printf(" Building Program \n");
    if(DEBUG) dumpast($2, 0); 
    eval($2);
    treefree($2);
    }
;

function_definitions:
    function_head block {
        printf("Function def \n"); 
        }
    | function_definitions function_head block {
        printf("Function def \n");
        }
 ;

identifier_list: variable {
        printf("variable \n");
        $$ = $1;}
    | identifier_list ',' variable {
        printf("ID list \n");}
;

variable_definitions: /*nothing*/
    | variable_definitions type identifier_list ';' {printf("variable defs \n");}
;

type: INT {printf("INT type \n");}
    | FLOAT {printf("FLOAT type \n");}
;

function_head: type ID arguments {printf("function head \n");}
;

arguments: '(' parameter_list ')' {
        printf("Args \n");
        $$ = $2;}
;

parameter_list: /*nothing*/ {$$ = NULL;}
    | parameters {printf("Param list \n");}
;

parameters: type ID {
        printf("variable \n");
        }
    | type ID '[' ']' {printf("array \n");}
    | parameters ',' type ID {printf("Params \n");}
    | parameters ',' type ID '[' ']' {printf("Params \n");}
;

block: '{' variable_definitions statements '}' {
        printf("Block \n");}
;

statements: /*nothing*/     {$$ = NULL;}
    | statements statement {
        printf("Statements \n");
        if ($2 == NULL)
            $$ = $1;
        else
	        $$ = newast('L', $1, $2);}
;

statement: expression ';' {
        printf("Statement \n");
        $$ = $1;}
    | compound_statement {
        printf("Compound Statement \n");
        $$ = $1;}
    | RETURN expression ';' {
        printf("Return Statement \n");
        }
    | IF '(' bool_expression ')' statement ELSE statement {
        printf("IF Statement \n");
        $$ = newflow(I, $3, $5, $7);}
    | WHILE '(' bool_expression ')' statement {
        printf("While Statement \n");
        $$ = newflow(W, $3, $5, NULL);}
    | input_statement ';' {
        printf("Input Statement \n");
        $$ = $1;}
    | output_statement ';' {
        printf("Output Statement \n");
        $$ = $1;}
;

input_statement: CIN {$$ = newast(IN, NULL, NULL);}
    | input_statement STREAMIN variable {
        printf("Streamin \n");
        $$ = newin($3);}
;

output_statement: COUT {//TODO define new ast type?
        printf("COUT \n");
        $$ = newast(OUT, NULL, NULL);
        }
    | output_statement STREAMOUT expression {
        printf("Streamout \n");
        $$ = newout($3, NULL);}
    | output_statement STREAMOUT STR_LITERAL {
        printf("Streamout\n");
        $$ = newout(NULL, $3);}
    | output_statement STREAMOUT ENDL {
        printf("Streamout \n");
        $$ = newout(NULL, "\n");}
;

compound_statement: '{' statements '}' {
        printf("Compound Statement \n");
        $$ = $2;}
;


variable: ID {
        printf("Variable ID \n");
        $$ = newref(lookup(hash_table, $1));}
    | ID '[' expression ']' {
        printf("Variable Array \n");
        $$ = newref(lookup(hash_table, $1));}
;

expression_list: /* nothing */  {$$ = NULL;}
    | expressions {
        printf("Expressions \n");
        $$ = $1;}
;

expressions: expression {
        printf("Expression \n");
        $$ = $1;}
    | expressions ',' expression {
        printf("Multiple Expressions \n");
        if ($3 == NULL)
	        $$ = $1;  
        else
			$$ = newast('L', $1, $3);}
;

expression: variable ASSIGNOP expression {
        printf("Assignment Expression \n");
        $$ = newasgn($1, $3);}
    | variable INCOP expression {
        printf(" Increment Expression \n");
        $$ = newast($2, $1, $3);}
    | simple_expression {
        printf("Simple Expression \n");
        $$ = $1;}
;

simple_expression: term {printf("Term \n");}
    | UMINUS term  {/* changed ADDOP to UMINUS*/
        printf("Uminus \n");
        $$ = newast(UMIN, $2, NULL)}   
    | simple_expression ADDOP term{
        printf("Addition \n");
        $$ = newast($2, $1, $3);}
;

term: factor {
        printf("Factor \n");
        $$ = $1;}
    | term MULOP factor {
        printf("Multiplication \n");
        $$ = newast($2, $1, $3);}
;
/* Needed to require id's that precede '(' to be function id's 
function: func_id '(' expression_list ')' {}
;

func_id: FUNC_ID {}
;

*/
factor: literal {
        printf("Literal \n");
        $$ = $1;}
    | '(' expression ')' {
        printf("(Expression) \n");
        $$ = $2;}
    | ID '(' expression_list ')' {
        printf("ID ( exp_list) \n");
        $$ = newcall($1, $3);}
    | variable {
        printf("Variable \n");
        $$ = $1;}
;

literal: INT_LITERAL {
        printf(" INT_LITERAL \n");
        $$ = newint($1);}
    | FLT_LITERAL {
        printf("FLT_LITERAL \n");
        $$ = newfloat($1);}
;

bool_expression: bool_term {
        printf("bool_term \n");
        $$ = $1;}
    | bool_expression OR bool_term {
        printf("bool_exp OR bool_term \n");
        $$ = newcmp($2, $1, $3);}
;

bool_term: bool_factor {
        printf("bool_factor \n");
        $$ = $1;}
    | bool_term AND bool_factor {
        printf("bool_term AND bool_factor \n");
        $$ = newcmp($2, $1, $3);}
;

bool_factor: NOT bool_factor {
        printf("NOT bool_factor \n"); 
        $$ = newast(L_NOT, $2, NULL);}
    | '(' bool_expression ')' {
        printf("(bool_exp) \n"); $$ = $2;}
    | simple_expression RELOP simple_expression {
        printf("simple_exp RELOP simple_exp \n");
        $$ = newcmp($2, $1, $3);}
;

%%
