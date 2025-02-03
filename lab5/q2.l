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



/*
student@oslab-02:~/220905128/lab5$ flex q2.l
student@oslab-02:~/220905128/lab5$ gcc lex.yy.c -o q2
student@oslab-02:~/220905128/lab5$ ./q2
Enter text (Ctrl+D to end input):
Hello
This is Lab5
CD lab

Words: 6
Characters: 26
Blanks (spaces/tabs): 3
Lines: 3
*/
