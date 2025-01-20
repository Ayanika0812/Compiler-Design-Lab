#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct token {
    char token_name[100]; // Token name (the actual token string)
    unsigned int row, col; // Line and column numbers
    char type[50]; // Token type (keyword, operator, id, etc.), not printed
};

// Function to create a token and print it (without type)
void createToken(struct token *t, const char *token_name, unsigned int row, unsigned int col, const char *type) {
    strcpy(t->token_name, token_name);
    t->row = row;
    t->col = col;
    strcpy(t->type, type);  // Store the type (it will not be printed)
    printf("<%s, %u, %u>\n", t->token_name, t->row, t->col);  // Print token name, row, and col
}

// Function to handle white spaces, comments, and preprocessor directives
void processAndWriteToFile(FILE *fp, FILE *out_fp) {
    char c, prev_c = '\0';
    unsigned int row = 1, col = 1; // Initialize line and column numbers
    int in_multi_line_comment = 0;
    
    while ((c = fgetc(fp)) != EOF) {
        // Handle new line and skip unnecessary spaces
        if (c == '\n') {
            if (prev_c != '\n' && prev_c != '\0') { // Skip writing a new line for comments or preprocessor
                fprintf(out_fp, "\n");
                row++;
                col = 1;
            }
            prev_c = c;
            continue;
        }

        // Skip single-line comments
        if (c == '/' && prev_c == '/') {
            while (c != '\n' && c != EOF) {
                c = fgetc(fp);
            }
            continue;
        }

        // Skip multi-line comments
        if (in_multi_line_comment) {
            if (c == '*' && prev_c == '/') {
                c = fgetc(fp);
                if (c == '/') {
                    in_multi_line_comment = 0; // End of multi-line comment
                }
            }
            prev_c = c;
            continue;
        }

        // Detect multi-line comment start
        if (c == '/' && prev_c == '*') {
            in_multi_line_comment = 1;
            prev_c = c;
            continue;
        }

        // Skip preprocessor directives
        if (c == '#' && prev_c == '\n') {
            while (c != '\n' && c != EOF) {
                c = fgetc(fp);
            }
            continue;
        }

        // Write regular non-whitespace characters to the output file
        if (!isspace(c) || c == '\n') {
            fputc(c, out_fp);
            prev_c = c;
            col++;
        }
    }
}

// Function to identify arithmetic operators
void identifyArithmeticOperators(char c, unsigned int row, unsigned int col) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
        struct token t;
        createToken(&t, (char[]){c, '\0'}, row, col, "operator");
    }
}

// Function to identify relational operators
void identifyRelationalOperators(char *c, FILE *fp, unsigned int row, unsigned int col) {
    if (*c == '=' || *c == '<' || *c == '>' || *c == '!') {
        char nextChar = fgetc(fp);
        if (*c == '=' && (nextChar == '=')) {
            struct token t;
            createToken(&t, "==", row, col, "operator");
        } else if ((*c == '<' || *c == '>' || *c == '!') && (nextChar == '=')) {
            struct token t;
            createToken(&t, (char[]){*c, '=', '\0'}, row, col, "operator");
        } else {
            struct token t;
            createToken(&t, (char[]){*c, '\0'}, row, col, "operator");
            ungetc(nextChar, fp); // Put the character back
        }
    }
}

// Function to identify logical operators
void identifyLogicalOperators(char c, FILE *fp, unsigned int row, unsigned int col) {
    if (c == '&') {
        char nextChar = fgetc(fp);
        if (nextChar == '&') {
            struct token t;
            createToken(&t, "&&", row, col, "operator");
        } else {
            ungetc(nextChar, fp); // Put the character back
        }
    }
    else if (c == '|') {
        char nextChar = fgetc(fp);
        if (nextChar == '|') {
            struct token t;
            createToken(&t, "||", row, col, "operator");
        } else {
            ungetc(nextChar, fp); // Put the character back
        }
    }
    else if (c == '!') {
        struct token t;
        createToken(&t, "!", row, col, "operator");
    }
}

// Function to identify special symbols
void identifySpecialSymbols(char c, unsigned int row, unsigned int col) {
    if (c == '{' || c == '}' || c == ';' || c == ',' || c == '(' || c == ')' || c == '[' || c == ']' || c == '.') {
        struct token t;
        createToken(&t, (char[]){c, '\0'}, row, col, "special_symbol");
    }
}

// Function to identify keywords
int isKeyword(char *str) {
    char *keywords[] = {
        "int", "char", "if", "else", "while", "for", "return", "void", "break", "continue", "switch", "case", "default", "struct", "long", "float", "double"
    };
    for (int i = 0; i < 17; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;  // It's a keyword
        }
    }
    return 0;  // Not a keyword
}

void identifyKeywords(char *str, unsigned int row, unsigned int col) {
    if (isKeyword(str)) {
        struct token t;
        createToken(&t, str, row, col, "keyword");
    } else {
        struct token t;
        createToken(&t, "id", row, col, "identifier");  // Identifiers are labeled as "id"
    }
}

// Function to identify numerical constants
void identifyNumericalConstants(char *str, unsigned int row, unsigned int col) {
    if (isdigit(str[0]) || (str[0] == '.' && isdigit(str[1]))) {
        struct token t;
        createToken(&t, str, row, col, "numerical_constant");
    }
}

// Function to identify string literals
void identifyStringLiterals(char *str, unsigned int row, unsigned int col) {
    if (str[0] == '"' && str[strlen(str) - 1] == '"') {
        struct token t;
        createToken(&t, str, row, col, "string_literal");
    }
}

// Function to identify identifiers
void identifyIdentifiers(char *str, unsigned int row, unsigned int col) {
    if (isalpha(str[0]) || str[0] == '_') {
        for (int i = 1; i < strlen(str); i++) {
            if (!isalnum(str[i]) && str[i] != '_') {
                return; // Invalid identifier
            }
        }
        if (isKeyword(str)) {
            struct token t;
            createToken(&t, str, row, col, "keyword");
        } else {
            struct token t;
            createToken(&t, str, row, col, "identifier");  // Valid identifier
        }
    }
}
int main() {
    char c, buffer[100];
    unsigned int row = 1, col = 1; // Initialize line and column numbers
    FILE *fp = fopen("q1in.c", "r");

    if (fp == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }

    while ((c = fgetc(fp)) != EOF) {
        // Skip whitespace (space, tab, newline)
        if (isspace(c)) {
            if (c == '\n') {
                row++;
                col = 1;
            }
            continue; // Skip the white space character
        }

        // Skip single-line comments (//)
        if (c == '/' && (c = fgetc(fp)) == '/') {
            while ((c = fgetc(fp)) != EOF && c != '\n') {
                // Skip until newline
            }
            row++; // Increment row for the new line after the comment
            col = 1; // Reset column for the new line
            continue;
        }

        // Skip multi-line comments (/*...*/)
        if (c == '/' && (c = fgetc(fp)) == '*') {
            while ((c = fgetc(fp)) != EOF) {
                if (c == '*' && (c = fgetc(fp)) == '/') {
                    break; // End of multi-line comment
                }
            }
            col = 1; // Reset column after skipping comment
            continue;
        }

        // Skip preprocessor directives (lines starting with #)
        if (c == '#') {
            while ((c = fgetc(fp)) != EOF && c != '\n') {
                // Skip the entire line for preprocessor directives
            }
            row++; // Increment row for the new line after the directive
            col = 1; // Reset column for the new line
            continue;
        }

        // Handle string literals
        if (c == '"') {
            int j = 0;
            while ((c = fgetc(fp)) != EOF && c != '"') {
                buffer[j++] = c;
            }
            buffer[j] = '"';  // Include the closing quote
            buffer[j + 1] = '\0';
            identifyStringLiterals(buffer, row, col);
        }
        // Handle keywords and identifiers
        else if (isalpha(c) || c == '_') {  // Start of a potential identifier or keyword
            int i = 0;
            buffer[i] = c;
            i++;
            while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_')) {
                buffer[i++] = c;
            }
            buffer[i] = '\0';  // Null-terminate the string
            ungetc(c, fp);  // Put the character back

            identifyKeywords(buffer, row, col);  // Check for keyword or identifier
        }
        // Handle numerical constants
        else if (isdigit(c)) {  // Start of a potential number
            int i = 0;
            buffer[i] = c;
            i++;
            while ((c = fgetc(fp)) != EOF && (isdigit(c) || c == '.')) {
                buffer[i++] = c;
            }
            buffer[i] = '\0';
            ungetc(c, fp);  // Put the character back

            identifyNumericalConstants(buffer, row, col);
        }
        // Handle special symbols (operators and other symbols)
        else {
            // Check for arithmetic, relational, and logical operators
            identifyArithmeticOperators(c, row, col);
            identifyRelationalOperators(&c, fp, row, col);
            identifyLogicalOperators(c, fp, row, col);

            // Handle special symbols
            identifySpecialSymbols(c, row, col);
        }

        col++; // Increment column number
    }

    fclose(fp);
    return 0;
}

/*
cc q1.c
student@oslab-02:~/220905128/lab3$ ./a.out
<int, 1, 1>
<id, 1, 2>
<(, 1, 3>
<), 1, 4>
<{, 2, 1>
<int, 3, 1>
<id, 3, 2>
<,, 3, 3>
<id, 3, 4>
<,, 3, 5>
<id, 3, 6>
<;, 3, 7>
<id, 4, 1>
<=, 4, 2>
<1, 4, 3>
<;, 4, 4>
<id, 5, 1>
<=, 5, 2>
<1, 5, 3>
<;, 5, 4>
<id, 6, 1>
<=, 6, 2>
<id, 6, 3>
<+, 6, 4>
<id, 6, 5>
<;, 6, 6>
<}, 7, 1>
*/
