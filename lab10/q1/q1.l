%{
#include "q1.tab.h"
%}

%%
int     { return INT; }
float   { return FLOAT; }
char    { return CHAR; }
[a-zA-Z][a-zA-Z0-9_]* { return ID; }
";"     { return SEMICOLON; }
[ \t]   ;
\n      { return 0; }
.       { return yytext[0]; }
%%

int yywrap() {
    return 1;
}
