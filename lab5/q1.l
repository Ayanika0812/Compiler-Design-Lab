%{
#include <stdio.h>

int vowels = 0;
int consonants = 0;
%}

%%

[aeiouAEIOU]  { vowels++; }        // Count vowels
[b-df-hj-np-tv-zB-DF-HJ-NP-TV-Z] { consonants++; }  // Count consonants

.|\n            {}  // Ignore other characters (e.g., spaces, punctuation, etc.)

%%

int main() {
    printf("Enter a string: ");
    yylex();  // Process the input through the lexer
    printf("Vowels: %d\n", vowels);
    printf("Consonants: %d\n", consonants);
    return 0;
}

int yywrap() {
    return 1;  // End of input
}







/*
flex q1.l
student@oslab-02:~/220905128/lab5$ gcc lex.yy.c -o output
student@oslab-02:~/220905128/lab5$ ./output
Enter a string: Hello World
Vowels: 3
Consonants: 7
student@oslab-02:~/220905128/lab5$ ./output
Enter a string: Apple
Vowels: 2
Consonants: 3
*/