%{
#include <stdio.h>

%}

%%

[0-9]+  { 
            int num = atoi(yytext); // Convert the matched text to an integer
            printf("%d in hexadecimal is: %X\n", num, num); // Print the number in hexadecimal
        }

.|\n    { ECHO; } // Echo all other characters (preserve them)

%%

int main() {
    printf("Enter the input (Ctrl+D to end input):\n");
    yylex(); // Process the input through the lexer
    return 0;
}

int yywrap() {
    return 1; // End of input
}



/*

student@oslab-02:~/220905128/lab5$ flex q5.l
student@oslab-02:~/220905128/lab5$ gcc lex.yy.c -o q5
student@oslab-02:~/220905128/lab5$ ./q5
Enter the input (Ctrl+D to end input):
25 100 255 500
25 in hexadecimal is: 19
 100 in hexadecimal is: 64
 255 in hexadecimal is: FF
 500 in hexadecimal is: 1F4


 */