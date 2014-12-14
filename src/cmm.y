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
| variable_definitions function_definitions {printf("Program \n"); }
;

function_definitions:
    function_head block {printf("Function def \n"); }
    | function_definitions function_head block {printf("Function def \n");}
 ;

identifier_list: variable {printf("variable \n");}
    | identifier_list ',' variable {printf("ID list \n");}
;

variable_definitions: /*nothing*/
    | variable_definitions type identifier_list ';' {printf("variable defs \n");}
;

type: INT {printf("INT type \n");}
    | FLOAT {printf("FLOAT type \n");}
;

function_head: type ID arguments {printf("function head \n");}
;

arguments: '(' parameter_list ')' {printf("Args \n");}
;

parameter_list: /*nothing*/
    | parameters {printf("Param list \n");}
;

parameters: type ID {printf("variable \n");}
    | type ID '[' ']' {printf("array \n");}
    | parameters ',' type ID {printf("Params \n");}
    | parameters ',' type ID '[' ']' {printf("Params \n");}
;

block: '{' variable_definitions statements '}' {printf("Block \n");}
;

statements: /*nothing*/
    | statements statement {printf("Statements \n");}
;

statement: expression ';' {printf("Statement \n");}
    | compound_statement {printf("Compound Statement \n");}
    | RETURN expression ';' {printf("Return Statement \n");}
    | IF '(' bool_expression ')' statement ELSE statement {printf("IF Statement \n");}
    | WHILE '(' bool_expression ')' statement {printf("While Statement \n");}
    | input_statement ';' {printf("Input Statement \n");}
    | output_statement ';' {printf("Output Statement \n");}
;

input_statement: CIN {}
    | input_statement STREAMIN variable {printf("Streamin \n");}
;

output_statement: COUT {printf("COUT \n");}
    | output_statement STREAMOUT expression {printf("Streamout \n");}
    | output_statement STREAMOUT STR_LITERAL {printf("Streamout %s\n", $3);}
    | output_statement STREAMOUT ENDL {printf("Streamout \n");}
;

compound_statement: '{' statements '}' {printf("Compound Statement \n");}
;


variable: ID {printf("Variable ID \n");}
    | ID '[' expression ']' {printf("Variable Array \n");}
;

expression_list: /* nothing */
    | expressions {printf("Expressions \n");}
;

expressions: expression {printf("Expression \n");}
    | expressions ',' expression {printf("Multiple Expressions \n");}
;

expression: variable ASSIGNOP expression {printf("Assignment Expression \n");}
    | variable INCOP expression {printf(" Increment Expression \n");}
    | simple_expression {printf("Simple Expression \n");}
;

simple_expression: term {printf("Term \n");}
    | UMINUS term  {printf("Uminus \n");}   /* changed ADDOP to UMINUS to eliminate ambiguity */
    | simple_expression ADDOP term{printf("Addition \n");}
;

term: factor {printf("Factor \n");}
    | term MULOP factor {printf("Multiplication \n");}
;
/* Needed to require id's that precede '(' to be function id's 
function: func_id '(' expression_list ')' {}
;

func_id: FUNC_ID {}
;

*/
factor: literal {printf("Literal \n");}
    | '(' expression ')' {printf("(Expression) \n");}
    | ID '(' expression_list ')' {printf("ID ( exp_list) \n");}
    | variable {printf("Variable \n");}
;

literal: INT_LITERAL {printf(" INT_LITERAL \n");}
    | FLT_LITERAL {printf("FLT_LITERAL \n");}
;

bool_expression: bool_term {printf("bool_term \n");}
    | bool_expression OR bool_term {printf("bool_exp OR bool_term \n");}
;

bool_term: bool_factor {printf("bool_factor \n");}
    | bool_term AND bool_factor {printf("bool_term AND bool_factor \n");}
;

bool_factor: NOT bool_factor {printf("NOT bool_factor \n");}
    | '(' bool_expression ')' {printf("(bool_exp) \n");}
    | simple_expression RELOP simple_expression {printf("simple_exp RELOP simple_exp \n");}
;

%%
