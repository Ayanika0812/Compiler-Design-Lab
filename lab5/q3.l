%{
#include <stdio.h>

int positive_integers = 0;
int negative_integers = 0;
int positive_floats = 0;
int negative_floats = 0;
%}

%%

[0-9]+               { positive_integers++; }               // Positive integers
-[0-9]+              { negative_integers++; }               // Negative integers
[0-9]+\.[0-9]+       { positive_floats++; }                 // Positive floating-point numbers
-[0-9]+\.[0-9]+      { negative_floats++; }                 // Negative floating-point numbers

.|\n                  {} // Ignore other characters (spaces, punctuation, etc.)

%%

int main() {
    printf("Enter the input (Ctrl+D to end):\n");
    yylex();  // Process the input through the lexer
    printf("\nPositive integers: %d\n", positive_integers);
    printf("Negative integers: %d\n", negative_integers);
    printf("Positive floating point numbers: %d\n", positive_floats);
    printf("Negative floating point numbers: %d\n", negative_floats);
    return 0;
}

int yywrap() {
    return 1;  // End of input
}


/*
student@oslab-02:~/220905128/lab5$ flex q3.l
student@oslab-02:~/220905128/lab5$ gcc lex.yy.c -o q3
student@oslab-02:~/220905128/lab5$ ./q3
Enter the input (Ctrl+D to end):
123
-123
0
23.00
-56.00
34

Positive integers: 3
Negative integers: 1
Positive floating point numbers: 1
Negative floating point numbers: 1*/