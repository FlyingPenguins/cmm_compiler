/* Lexer for cmm compiler
 * Group: Flying Penguins
 * Members: Zoe Fultz, Bryan Garza, 
 * Brandon Halpin, Joshua Titley, Francis Tweedy
 * Adapted from source code in "flex & bison", published by O'Reilly
 * 
 */
/* recognize tokens for the CMM compiler */

%option noyywrap nodefault yylineno
%x COMMENT
%{
# include "cmm.h"
# include "cmm.tab.h"
%}

%%
"="     { yylval.op = yytext; return ASSIGNOP; /* Assignment operator */}

"*" |
"/" |
"%"     { yylval.op = yytext; return MULOP; /* Multiplication operators */}

"+" |
"-"     { yylval.op = yytext; return ADDOP; /* Addition operators */}

"!"     { yylval.op = yytext; return NOT; /* Logical operators */}
"||"    { yylval.op = yytext; return OR; }
"&&"    { yylval.op = yytext; return AND; }

"," |
";" |
"(" |
")" |
"[" |
"]" |
"{" |
"}"    { return yytext[0]; /* Single char literal tokens */}


">"  |
"<"  |
"<>" |
"==" |
">=" |
"<=" { yylval.op = yytext; return RELOP;  /* Relational operators */}


"cin"   { return CIN;  /* keywords */}
"cout"  { return COUT; }
"else"  { return ELSE; }
"ENDL"  { return ENDL; }
"if"    { return IF; }
"int"   { return INT; }
"return" { return RETURN; }
"while" { return WHILE; }

 
[a-zA-Z][a-zA-Z0-9]*  { yylval.str = lookup(yytext); return ID; /* variable/function IDs */}

[0-9]+"."[0-9]* |
"."[0-9]+ { yylval.d = atof(yytext); return FLT_LITERAL; }

[0-9]+ {yylval.i = atoi(yytext); return INT_LITERAL; }

^"#"[ \t]*include[ \t]*[\"<] /*ignore includes */
 
"/*"           { BEGIN(COMMENT) ; /* comments */}
<COMMENT>"*/"  { BEGIN(INITIAL); }
<COMMENT>([^*]|\en)+|.

 
"//".*\n
[ \t]   
\\\n    {printf("c> "); /* ignore line continuation */} 
"\n"    { return EOL; }

.	{ yyerror("Mystery character %c\n", *yytext); }

%%

