%{
#include "q2.tab.h"
%}

%%
if      return IF;
else    return ELSE;
[0-9]+  return NUMBER;
[a-zA-Z][a-zA-Z0-9]* return ID;
[<>]=?|==|!= return RELOP;
"("     return '(';
")"     return ')';
"{"     return '{';
"}"     return '}';
"="     return '=';
\n      ;
[ \t]   ;
.       return yytext[0];
%%

int yywrap() {
    return 1;
}
