/* Group: Flying Penguins
 * Bison parser for cmm language
 *
 */

%{
#include <stdio.h>
#include <stdlib.h>
%}

%union {
    struct ast *a;  /* Abstract syntax trees */
    double d;       /* float literals */
    int i;          /* int literals */
    char* str;      /* string literals */
    char* id;       /* identifiers */
    char op;        /* single character operators +, -, *, /, =, !... */
    char* s_op;     /* multiple character operators +=, -=, <=, >=... */
}

/* declare tokens */
%token <op> ASSIGNOP
%token <op> MULOP
%token <op> ADDOP
%token <op> UMINUS  /* unary minus, prevents using "+-<expresion>" */
%token <s_op> INCOP
%token <s_op> RELOP
%token <op> NOT
%token <s_op> OR
%token <s_op> AND
%token <d> FLT_LITERAL
%token <i> INT_LITERAL
%token <str> STR_LITERAL
%token <s_op> STREAMIN
%token <s_op> STREAMOUT
%token <id> ID
%token <id> FUNC_ID
/* reserved words */
%token CIN COUT ELSE ENDL FLOAT IF INT RETURN WHILE

/* Define L->R or R->L grouping and operator 
 * precedence. For justification, see:
 * http://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B#Operator_precedence
 */ 
%right ASSIGNOP INCOP
%left AND OR
%left RELOP
%left STREAMIN STREAMOUT
%left ADDOP
%left MULOP 
%nonassoc NOT UMINUS

/*
%type <a> expression factor term
*/

%%

program: /* nothing */
| variable_definitions function_definitions {
     /*
     printf('= %4.4g\n', eval($2));
     treefree($2);
     printf('> ');
     /**/
 }
;

function_definitions: 
    function_head block { 
        //$$ = newast('+', $1,$3); 
    }
    | function_definitions function_head block { 
        //$$ = newast('+', $1,$3); 
    }
 ;

identifier_list: ID
    | ID '[' INT_LITERAL ']' {}
    | identifier_list ',' ID {}
    | identifier_list ',' ID '[' INT_LITERAL ']' {}
;

variable_definitions: /*nothing*/
    | variable_definitions type identifier_list {}
;

type: INT {}
    | FLOAT {}
;

function_head: type ID arguments {}
;

arguments: '(' parameter_list ')' {}
;

parameter_list: /*nothing*/
    | parameters
;

parameters: type ID {}
    | type ID '[' ']' {}
    | parameters ',' type ID {}
    | parameters ',' type ID '[' ']' {}
;

block: '{' variable_definitions statements '}' {}
;

statements: /*nothing*/
    | statements statement {}
;

statement: expression ';' {}
    | compound_statement {}
    | RETURN expression {}
    | IF '(' bool_expression ')' statement ELSE statement {}
    | WHILE '(' bool_expression ')' statement {}
    | input_statement ';' {}
    | output_statement ';' {}
;

input_statement: CIN {}
    | input_statement STREAMIN variable {}
;

output_statement: COUT {}
    | output_statement STREAMOUT expression {}
    | output_statement STREAMOUT STR_LITERAL {}
    | output_statement STREAMOUT ENDL {}
;

compound_statement: '{' statements '}' {}
;


variable: ID {}
    | ID '[' expression ']' {}
;

expression_list: /* nothing */
    | expressions {}
;

expressions: expression {}
    | expressions ',' expression {}
;

expression: variable ASSIGNOP expression {}
    | variable INCOP expression {}
    | simple_expression {}
;

simple_expression: term {}
    | UMINUS term  {}   /* changed ADDOP to UMINUS to eliminate ambiguity */
    | simple_expression ADDOP term{}
;

term: factor {}
    | term MULOP factor {}
;
/* Needed to require id's that precede '(' to be function id's */
function: func_id '(' expression_list ')' {}
;

func_id: FUNC_ID {}
;

factor: variable {} /* replaced redundant rules for variable */
    | function {}
    | literal {}
    | '(' expression ')' {}
;

literal: INT_LITERAL {}
    | FLT_LITERAL {}
;

bool_expression: bool_term {}
    | bool_expression OR bool_term {}
;

bool_term: bool_factor {}
    | bool_term AND bool_factor {}
;

bool_factor: NOT bool_factor {}
    | '(' bool_expression ')' {}
    | simple_expression RELOP simple_expression {}
;

%%
