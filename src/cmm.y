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
%}

%union {
    struct ast *a;  /* Abstract syntax trees */
    double d;       /* float literals */
    int i;          /* int literals */
    char* str;      /* string literals */
    char* id;       /* identifiers */
    char* op;       /* operators +, -, *, /, =, +=, -=, <=, >=... */
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
%token <id> ID
/*%token <id> FUNC_ID*/
/* reserved words */
%token CIN COUT ELSE ENDL FLOAT IF INT RETURN WHILE

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

/*
%type <a> expression factor term
*/

%%

program: /* nothing */
| variable_definitions function_definitions {printf("Start program \n"); }
;

function_definitions:
    function_head block {printf("Start function \n"); }
    | function_definitions function_head block {printf("Start function \n");}
 ;

identifier_list: variable {printf("variable \n");}
    | identifier_list ',' variable {printf("ID list \n");}
;

variable_definitions: /*nothing*/
    | variable_definitions type identifier_list ';' {printf("Start variable defs \n");}
;

type: INT {printf("INT type \n");}
    | FLOAT {printf("FLOAT type \n");}
;

function_head: type ID arguments {printf("function head \n");}
;

arguments: '(' parameter_list ')' {printf("Args \n");}
;

parameter_list: /*nothing*/
    | parameters
;

parameters: type ID {printf("variable \n");}
    | type ID '[' ']' {printf("array \n");}
    | parameters ',' type ID {}
    | parameters ',' type ID '[' ']' {}
;

block: '{' variable_definitions statements '}' {printf("Block \n");}
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
    | input_statement STREAMIN variable {printf("Streamin \n");}
;

output_statement: COUT {printf("COUT \n");}
    | output_statement STREAMOUT expression {printf("Streamout \n");}
    | output_statement STREAMOUT STR_LITERAL {printf("Streamout %s\n", $3);}
    | output_statement STREAMOUT ENDL {printf("Streamout \n");}
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
/* Needed to require id's that precede '(' to be function id's 
function: func_id '(' expression_list ')' {}
;

func_id: FUNC_ID {}
;

*/
factor: literal {}
    | '(' expression ')' {}
    | ID '(' expression_list ')' {}
    | variable {}
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
