%{
#include <stdio.h>
#include <stdlib.h>
%}

%token IF ELSE ID NUMBER RELOP

%%
input : stmt { printf("Valid decision statement\n"); exit(0); }
      ;

stmt : IF '(' condition ')' stmt ELSE stmt 
     | IF '(' condition ')' stmt
     | '{' stmt_list '}' 
     | ID '=' expr
     ;

condition : ID RELOP ID;

expr : ID
     | NUMBER;

stmt_list : stmt stmt_list
          | /* empty */;
%%
int yyerror(char *msg) {
    printf("Invalid statement\n");
    exit(0);
}

int main() {
    printf("Enter a decision-making statement:\n");
    yyparse();
    return 0;
}
