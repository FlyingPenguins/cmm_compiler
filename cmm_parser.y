/* Companion source code for "flex & bison", published by O'Reilly
 * Media, ISBN 978-0-596-15597-1
 * Copyright (c) 2009, Taughannock Networks. All rights reserved.
 * See the README file for license conditions and contact info.
 * $Header: /home/johnl/flnb/code/RCS/fb3-1.y,v 2.1 2009/11/08 02:53:18 johnl Exp $
 */

/* calculator with AST */

%{
#  include <stdio.h>
#  include <stdlib.h>
%}

%union {
  struct ast *a;
  double d;
}

/* declare tokens */
%token ASSIGNOP
%token MULOP
%token ADDOP
%token INCOP
%token RELOP
%token NOT
%token OR
%token AND
%token FLT_LITERAL
%token INT_LITERAL
%token STR_LITERAL
%token STREAMIN
%token STREAMOUT
%token ID
/* reserved words */
%token CIN
%token COUT
%token ELSE
%token ENDL
%token FLOAT
%token IF
%token INT
%token RETURN
%token WHILE
%token 

%type <a> exp factor term

%%

program: /* nothing */
| variable_definitions function_definitions {
     /*
     printf("= %4.4g\n", eval($2));
     treefree($2);
     printf("> ");
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
    | varibale_definitions type identifier_list {}
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



%%
