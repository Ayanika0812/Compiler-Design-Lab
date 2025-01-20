#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

// Token structure
struct token {
    char token_name[MAX_TOKEN_LENGTH];  // Token value
    unsigned int row, col;              // Row and column number
    char type[50];                      // Type of token (keyword, operator, id, etc.)
};

// Function to create a token
void createToken(struct token *t, const char *token_name, unsigned int row, unsigned int col, const char *type) {
    strcpy(t->token_name, token_name);
    t->row = row;
    t->col = col;
    strcpy(t->type, type);  // Store the type
    printf("<%s, %u, %u, %s>\n", t->token_name, t->row, t->col, t->type);  // Print token
}

// Check if a string is a keyword
int isKeyword(const char *str) {
    char *keywords[] = {
        "int", "char", "if", "else", "while", "for", "return", "void", "break", "continue", 
        "switch", "case", "default", "struct", "long", "float", "double"
    };
    for (int i = 0; i < 17; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;  // It's a keyword
        }
    }
    return 0;  // Not a keyword
}

// Check if a string is a valid identifier
int isIdentifier(const char *str) {
    if (isalpha(str[0]) || str[0] == '_') {
        for (int i = 1; i < strlen(str); i++) {
            if (!isalnum(str[i]) && str[i] != '_') {
                return 0;  // Invalid identifier
            }
        }
        return 1;  // Valid identifier
    }
    return 0;  // Not an identifier
}

// Check if a string is a numerical constant
int isNumeric(const char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i]) && str[i] != '.') {
            return 0;  // Not a number
        }
    }
    return 1;  // It's a number
}

// Check if a string is a valid string literal
int isStringLiteral(const char *str) {
    return str[0] == '"' && str[strlen(str) - 1] == '"';
}

// Skip whitespace characters and return the next non-whitespace character
int skipWhitespace(FILE *fp, char *c, unsigned int *row, unsigned int *col) {
    while (isspace(*c)) {
        if (*c == '\n') {
            (*row)++;
            *col = 1;
        } else {
            (*col)++;
        }
        *c = fgetc(fp);
    }
    return *c;
}

// Function to handle multi-line comments (/* ... */)
void skipMultiLineComment(FILE *fp, char *c, unsigned int *row, unsigned int *col) {
    while (*c != EOF) {
        if (*c == '*') {
            char nextChar = fgetc(fp); // Get the next character
            if (nextChar == '/') {
                break; // End of comment
            }
            ungetc(nextChar, fp); // If not '/', put the character back
        }
        if (*c == '\n') {
            (*row)++;
            *col = 1;
        } else {
            (*col)++;
        }
        *c = fgetc(fp);
    }
}

// Function to handle single-line comments (//)
void skipSingleLineComment(FILE *fp, char *c, unsigned int *row, unsigned int *col) {
    while (*c != '\n' && *c != EOF) {
        if (*c == '\n') {
            (*row)++;
            *col = 1;
        } else {
            (*col)++;
        }
        *c = fgetc(fp);
    }
}

// Function to handle preprocessor directives (e.g., #define, #include)
void skipPreprocessorDirective(FILE *fp, char *c, unsigned int *row, unsigned int *col) {
    while (*c != '\n' && *c != EOF) {
        if (*c == '\n') {
            (*row)++;
            *col = 1;
        } else {
            (*col)++;
        }
        *c = fgetc(fp);
    }
}

// Function to get the next token from the input file
int getNextToken(FILE *fp, struct token *t, unsigned int *row, unsigned int *col) {
    char c = fgetc(fp);
    if (c == EOF) {
        return 0;  // End of file
    }

    c = skipWhitespace(fp, &c, row, col);

    // Skip comments
    if (c == '/') {
        char nextChar = fgetc(fp);
        if (nextChar == '/') {
            skipSingleLineComment(fp, &c, row, col);
            return getNextToken(fp, t, row, col);  // Recursively call to find next token
        }
        if (nextChar == '*') {
            skipMultiLineComment(fp, &c, row, col);
            return getNextToken(fp, t, row, col);  // Recursively call to find next token
        }
        ungetc(nextChar, fp);  // Put back the second '/'
    }

    // Skip preprocessor directives
    if (c == '#') {
        skipPreprocessorDirective(fp, &c, row, col);
        return getNextToken(fp, t, row, col);  // Recursively call to find next token
    }

    // Handle string literals
    if (c == '"') {
        char str[MAX_TOKEN_LENGTH];
        int i = 0;
        str[i++] = c;
        c = fgetc(fp);
        while (c != EOF && c != '"') {
            str[i++] = c;
            c = fgetc(fp);
        }
        str[i++] = c;
        str[i] = '\0';
        strcpy(t->token_name, str);
        t->row = *row;
        t->col = *col;
        strcpy(t->type, "string_literal");
        printf("<%s, %u, %u, %s>\n", t->token_name, t->row, t->col, t->type);
        (*col) += i;
        return 1;
    }

    // Handle identifiers, keywords, and numerical constants
    if (isalpha(c) || c == '_') {
        char str[MAX_TOKEN_LENGTH];
        int i = 0;
        str[i++] = c;
        c = fgetc(fp);
        while (isalnum(c) || c == '_') {
            str[i++] = c;
            c = fgetc(fp);
        }
        str[i] = '\0';
        ungetc(c, fp);

        if (isKeyword(str)) {
            strcpy(t->token_name, str);
            t->row = *row;
            t->col = *col;
            strcpy(t->type, "keyword");
        } else {
            strcpy(t->token_name, str);
            t->row = *row;
            t->col = *col;
            strcpy(t->type, "identifier");
        }
        printf("<%s, %u, %u, %s>\n", t->token_name, t->row, t->col, t->type);
        (*col) += i;
        return 1;
    }

    // Handle numerical constants
    if (isdigit(c)) {
        char str[MAX_TOKEN_LENGTH];
        int i = 0;
        str[i++] = c;
        c = fgetc(fp);
        while (isdigit(c) || c == '.') {
            str[i++] = c;
            c = fgetc(fp);
        }
        str[i] = '\0';
        ungetc(c, fp);

        strcpy(t->token_name, str);
        t->row = *row;
        t->col = *col;
        strcpy(t->type, "numerical_constant");
        printf("<%s, %u, %u, %s>\n", t->token_name, t->row, t->col, t->type);
        (*col) += i;
        return 1;
    }

    // Handle special symbols (operators, punctuation)
    if (ispunct(c)) {
        char str[2] = {c, '\0'};
        strcpy(t->token_name, str);
        t->row = *row;
        t->col = *col;
        strcpy(t->type, "special_symbol");
        printf("<%s, %u, %u, %s>\n", t->token_name, t->row, t->col, t->type);
        (*col)++;
        return 1;
    }

    return 0;  // Invalid token or EOF
}

int main() {
    FILE *fp = fopen("q2in.c", "r");  // Open a C file for lexing
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    struct token t;
    unsigned int row = 1, col = 1;
    while (getNextToken(fp, &t, &row, &col)) {
        // Tokens are printed in getNextToken()
    }

    fclose(fp);
    return 0;
}
