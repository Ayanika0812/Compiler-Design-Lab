%{
#include <stdio.h>
#include <stdlib.h>
%}
%token INT FLOAT CHAR ID SEMICOLON

%%
stmt : declaration SEMICOLON { printf("Valid Declaration\n"); exit(0); }
     ;
declaration : type ID
            ;
type : INT
     | FLOAT
     | CHAR
     ;
%%
int yyerror(char *msg) {
    printf("Invalid Declaration\n");
    exit(1);
}
int main() {
    printf("Enter the declaration:\n");
    yyparse();
}
