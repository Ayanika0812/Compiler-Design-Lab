%{
#include <stdio.h>
extern FILE *yyin, *yyout; // Declare input and output file pointers
%}

%%
// Match "float" followed by a space, tab, or newline
float/[ \t\n] { 
    fprintf(yyout, "Token: KEYWORD_FLOAT (%s)\n", yytext); 
}

// Match "int" followed by a space, tab, or newline
int/[ \t\n] { 
    fprintf(yyout, "Token: KEYWORD_INT (%s)\n", yytext); 
}

// Match identifiers (variable names)
[a-zA-Z_][a-zA-Z0-9_]* {
    fprintf(yyout, "Token: IDENTIFIER (%s)\n", yytext);
}

// Match numbers
[0-9]+ {
    fprintf(yyout, "Token: NUMBER (%s)\n", yytext);
}

// Match operators (+, -, *, /)
[+\-*/] {
    fprintf(yyout, "Token: OPERATOR (%s)\n", yytext);
}

// Ignore spaces, tabs, and newlines
[ \t\n]+ ;

%%

int main(int argc, char **argv) {
    // Open input file if provided
    if (argc > 1) {
        yyin = fopen(argv[1], "r"); 
        if (!yyin) {
            perror("Error opening input file");
            return 1;
        }
    }

    // Open output file (optional)
    yyout = fopen("output.txt", "w");
    if (!yyout) {
        perror("Error opening output file");
        return 1;
    }

    // Start scanning
    yylex();

    // Close files
    if (yyin) fclose(yyin);
    if (yyout) fclose(yyout);

    return 0;
}

int yywrap() {
    return 1; // Stop scanning at EOF
}
