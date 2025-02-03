%{
#include <stdio.h>
#include <ctype.h>  // For the tolower() function
%}

%%

"//".*                  { ECHO; }  // Skip single-line comments
"/*"[^*]*"*"+[^/]*"/"   { ECHO; }  // Skip multi-line comments

[a-zA-Z] {
    if (isupper(yytext[0])) {  // Check if the character is uppercase
        yytext[0] = tolower(yytext[0]);  // Convert to lowercase
    }
    ECHO;  // Output the modified or unchanged character
}

.|\n { ECHO; }  // Echo any other characters (spaces, punctuation, etc.)

%%

int main() {
    printf("Enter the C code (Ctrl+D to end input):\n");
    yylex();  // Process the input through the lexer
    return 0;
}

int yywrap() {
    return 1;  // End of input
}



/*
 flex q6.l
student@oslab-02:~/220905128/lab5$ gcc lex.yy.c -o q6
student@oslab-02:~/220905128/lab5$ ./q6
Enter the C code (Ctrl+D to end input):
#include <stdio.h>

int main() {
    // This is a comment
    int x = 10;
    printf("HELLO WORLD\n");  // ANOTHER COMMENT
    /* This is a multi-line
       comment */
    return 0;
}#include <stdio.h>

int main() {
    // This is a comment
    int x = 10;
    printf("hello world\n");  // ANOTHER COMMENT
    /* This is a multi-line
       comment */
    return 0;
}student@oslab
*/