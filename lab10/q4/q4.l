%{
#include "q4.tab.h"
%}

%%
[0-9]+     { yylval = atoi(yytext); return NUMBER; }
[+\-*/^n]  { return yytext[0]; }
\n         { return '\n'; }
[ \t]      { /* Ignore whitespaces */ }
.          { printf("Invalid character: %s\n", yytext); exit(1); }
%%

int yywrap() {
    return 1;
}
