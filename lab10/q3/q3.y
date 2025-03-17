%{
#include <stdio.h>
#include <stdlib.h>
%}

%token NUMBER
%left '+' '-'
%left '*' '/'
%%

input : expr '\n' { printf("Result = %d\n", $1); exit(0); }
      ;

expr  : expr '+' expr { $$ = $1 + $3; }
      | expr '-' expr { $$ = $1 - $3; }
      | expr '*' expr { $$ = $1 * $3; }
      | expr '/' expr { 
            if ($3 == 0) {
                printf("Error: Division by zero\n");
                exit(1);
            } else {
                $$ = $1 / $3;
            }
        }
      | '(' expr ')' { $$ = $2; }
      | NUMBER { $$ = $1; }
      ;

%%

int yyerror(char *msg) {
    printf("Invalid expression\n");
    exit(1);
}

int main() {
    printf("Enter an arithmetic expression:\n");
    yyparse();
    return 0;
}
