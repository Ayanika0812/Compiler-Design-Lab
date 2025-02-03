%{
#include <stdio.h>

int scanf_count = 0;
int printf_count = 0;
%}

%%

scanf    { printf("READ"); scanf_count++; }   // Replace scanf with READ
printf         { printf("WRITE"); printf_count++; } // Replace printf with WRITE

.|\n             { ECHO; }  // Echo all other characters (preserve them)

%%

int main() {
    printf("Enter the C code (Ctrl+D to end input):\n");
    yylex();  // Process the input through the lexer
    printf("\nNumber of scanf: %d\n", scanf_count);
    printf("Number of printf: %d\n", printf_count);
    return 0;
}

int yywrap() {
    return 1;  // End of input
}


/*


student@oslab-02:~/220905128/lab5$ flex q4.l
student@oslab-02:~/220905128/lab5$ gcc lex.yy.c -o q4
student@oslab-02:~/220905128/lab5$ ./q4
Enter the C code (Ctrl+D to end input):
#include<stdio.h>

int main() {
    int num;
    scanf("%d", &num);
    printf("You entered: %d\n", num);
    printf("\n");
    return 0;
}#include<stdio.h>

int main() {
    int num;
    READ("%d", &num);
    WRITE("You entered: %d\n", num);
    WRITE("\n");
    return 0;
}
Number of scanf: 1
Number of printf: 2


*/