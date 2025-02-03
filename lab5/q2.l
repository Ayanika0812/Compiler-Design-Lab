%{
#include <stdio.h>

int words = 0;
int characters = 0;
int blanks = 0;
int lines = 0;
%}

%%

[ \t]            { blanks++; characters++; }  // Count blanks (spaces/tabs)
[a-zA-Z0-9]+     { words++; characters += yyleng; }  // Count words, also count characters
\n               { lines++; characters++; }  // Count lines (newline is a character)
.                { characters++; }  // Count all other characters

%%

int main() {
    printf("Enter text (Ctrl+D to end input):\n");
    yylex();  // Process the input through the lexer
    printf("\nWords: %d\n", words);
    printf("Characters: %d\n", characters);
    printf("Blanks (spaces/tabs): %d\n", blanks);
    printf("Lines: %d\n", lines);
    return 0;
}

int yywrap() {
    return 1;  // End of input
}
