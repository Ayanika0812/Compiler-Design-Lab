%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
%}
%token NUMBER
%left '+' '-'
%left '*' '/'
%left '^'
%left 'n'
%%
input : input line 
      | /* empty */
      ;
line  : '\n' 
      | exp '\n' { printf("Result = %d\n", $1); }
      ;
exp   : NUMBER              { $$ = $1; }
      | exp exp '+'         { $$ = $1 + $2; }
      | exp exp '-'         { $$ = $1 - $2; }
      | exp exp '*'         { $$ = $1 * $2; }
      | exp exp '/'         { 
                                if ($2 == 0) {
                                    printf("Error: Division by zero\n");
                                    exit(1);
                                } else {
                                    $$ = $1 / $2;
                                }
                            }
      | exp exp '^'         { $$ = pow($1, $2); }
      | exp 'n'             { $$ = -$1; }
      ;

%%
int yyerror(char *msg) {
    printf("Invalid expression\n");
    exit(1);
}
int main() {
    printf("Enter a postfix expression:\n");
    yyparse();
    return 0;
}


