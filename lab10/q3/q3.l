%{
#include "q3.tab.h"
%}

%%
[0-9]+     { yylval = atoi(yytext); return NUMBER; }
[+\-*/()]  { return yytext[0]; }
\n         { return '\n'; }
[ \t]      { /* Ignore whitespaces */ }
.          { printf("Invalid character: %s\n", yytext); exit(1); }
%%

int yywrap() {
    return 1;
}

/*

yytext	Holds the matched string (e.g., "42", "+")
yylval	Stores the numerical value for Bison (e.g., 42 for NUMBER)
atoi(yytext)	Converts a string ("42") into an integer (42)
yytext[0]	Returns the first character of yytext (e.g., '+')

*/
