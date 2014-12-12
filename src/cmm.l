/* Lexer for cmm compiler
 * Group: Flying Penguins
 * Members: Zoe Fultz, Bryan Garza, 
 * Brandon Halpin, Joshua Titley, Francis Tweedy
 * Adapted from source code in "flex & bison", published by O'Reilly
 * 
 */
/* recognize tokens for the CMM compiler */

%option noyywrap nodefault yylineno
%x C_COMMENT
%{
# include "cmm.h"
# include "cmm.tab.h"
#define YYSTYPE int
extern YYSTYPE yyltype;
int DEBUG = 1;
%}

%%


"="     {if(DEBUG) printf("%d ASSIGNOP \n", yylineno); yylval.op = yytext; return ASSIGNOP; /* Assignment operator */}

"*" |
"/" |
"%"     {if(DEBUG) printf("%d MULOP \n", yylineno); yylval.op = yytext; return MULOP; /* Multiplication operators */}

"+" |
"-"     {if(DEBUG) printf("%d ADDOP \n", yylineno); yylval.op = yytext; return ADDOP; /* Addition operators */}

"!"     {if(DEBUG) printf("%d NOT \n", yylineno); yylval.op = yytext; return NOT; /* Logical operators */}
"||"    {if(DEBUG) printf("%d OR \n", yylineno); yylval.op = yytext; return OR; }
"&&"    {if(DEBUG) printf("%d AND \n", yylineno); yylval.op = yytext; return AND; }

"," |
";" |
"(" |
")" |
"[" |
"]" |
"{" |
"}"    {if(DEBUG) printf("%d Single char \n", yylineno); return yytext[0]; /* Single char literal tokens */}


">"  |
"<"  |
"<>" |
"==" |
">=" |
"<=" {if(DEBUG) printf("%d RELOP \n", yylineno); yylval.op = yytext; return RELOP;  /* Relational operators */}

">>" {if(DEBUG) printf("%d Streamin \n", yylineno); return STREAMIN;}
"<<" {if(DEBUG) printf("%d Streaout \n", yylineno);return STREAMOUT;}
[\"]((("*"[^/])?)|[^*])*[\"] {if(DEBUG) printf("%d STR_LITERAL \n", yylineno); return STR_LITERAL;}

"cin"   {if(DEBUG) printf("%d CIN \n", yylineno); return CIN;  /* keywords */}
"cout"  {if(DEBUG) printf("%d COUT \n", yylineno); return COUT; }
"else"  {if(DEBUG) printf("%d ELSE \n", yylineno); return ELSE; }
"ENDL"  {if(DEBUG) printf("%d ENDL \n", yylineno); return ENDL; }
"if"    {if(DEBUG) printf("%d IF \n", yylineno); return IF; }
"int"   {if(DEBUG) printf("%d INT \n", yylineno); return INT; }
"return" {if(DEBUG) printf("%d RETURN \n", yylineno); return RETURN; }
"while" {if(DEBUG) printf("%d WHILE \n", yylineno); return WHILE; }

 
[a-zA-Z][a-zA-Z0-9]*  {if(DEBUG) printf("%d ID %s \n", yylineno, yytext); /*yylval.str = lookup(yytext);*/ return ID; /* variable/function IDs */}

[0-9]+"."[0-9]* |
"."[0-9]+ {if(DEBUG) printf("%d FLT_LITERAL \n", yylineno); yylval.d = atof(yytext); return FLT_LITERAL; }

[0-9]+ {if(DEBUG) printf("%d INT_LITERAL \n", yylineno); yylval.i = atoi(yytext); return INT_LITERAL; }

^"#"[ \t]*include[ \t]*[\"<].*[^\">] {if(DEBUG) printf("%d INCLUDE \n", yylineno);/* ignore includes*/}
"/*"((("*"[^/])?)|[^*])*"*/" {if(DEBUG) printf("%d Block Comment \n", yylineno);}

 
"//".*\n    {if(DEBUG) printf("%d Inline Comment \n", yylineno);}
[ \t]       {if(DEBUG) printf("%d Whitespace \n", yylineno);}
\\\n    {printf("%d c> ", yylineno); /* ignore line continuation */}
[ \r\n]    {if(DEBUG) printf("%d Newline \n", yylineno);}

.	{ yyerror("Mystery character %c\n", *yytext); }

%%

