#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100
struct token {
    char token_name[MAX_TOKEN_LENGTH]; 
    unsigned int row, col;              
    char type[50];                    
};
void createToken(struct token *t, const char *token_name, unsigned int row, unsigned int col, const char *type) {
    strcpy(t->token_name, token_name);
    t->row = row;
    t->col = col;
    strcpy(t->type, type);
    printf("<%s, %u, %u, %s>\n", t->token_name, t->row, t->col, t->type);
}
int isKeyword(const char *str) {
    char *keywords[] = {
        "int", "char", "if", "else", "while", "for", "return", "void", "break", "continue", 
        "switch", "case", "default", "struct", "long", "float", "double"
    };
    for (int i = 0; i < 17; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;  
        }
    }
    return 0; 
}
int isIdentifier(const char *str) {
    if (isalpha(str[0]) || str[0] == '_') {
        for (int i = 1; i < strlen(str); i++) {
            if (!isalnum(str[i]) && str[i] != '_') {
                return 0; 
            }
        }
        return 1; 
    }
    return 0; 
}
int isNumeric(const char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i]) && str[i] != '.') {
            return 0;
        }
    }
    return 1;
}
int isStringLiteral(const char *str) {
    return str[0] == '"' && str[strlen(str) - 1] == '"';
}
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
void skipMultiLineComment(FILE *fp, char *c, unsigned int *row, unsigned int *col) {
    while (*c != EOF) {
        if (*c == '*') {
            char nextChar = fgetc(fp);
            if (nextChar == '/') {
                break; 
            }
            ungetc(nextChar, fp);
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
int getNextToken(FILE *fp, struct token *t, unsigned int *row, unsigned int *col, FILE *outFile) {
    char c = fgetc(fp);
    if (c == EOF) {
        return 0;
    }

    c = skipWhitespace(fp, &c, row, col);

    // Skip comments
    if (c == '/') {
        char nextChar = fgetc(fp);
        if (nextChar == '/') {
            skipSingleLineComment(fp, &c, row, col);
            return getNextToken(fp, t, row, col, outFile); 
        }
        if (nextChar == '*') {
            skipMultiLineComment(fp, &c, row, col);
            return getNextToken(fp, t, row, col, outFile);
        }
        ungetc(nextChar, fp);
    }
    if (c == '#') {
        skipPreprocessorDirective(fp, &c, row, col);
        return getNextToken(fp, t, row, col, outFile);
    }
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
        fprintf(outFile, "<%s, %u, %u, %s>\n", t->token_name, t->row, t->col, t->type);
        (*col) += i;
        return 1;
    }
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
        fprintf(outFile, "<%s, %u, %u, %s>\n", t->token_name, t->row, t->col, t->type);
        (*col) += i;
        return 1;
    }
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
        fprintf(outFile, "<%s, %u, %u, %s>\n", t->token_name, t->row, t->col, t->type);
        (*col) += i;
        return 1;
    }
    if (ispunct(c)) {
        char str[2] = {c, '\0'};
        strcpy(t->token_name, str);
        t->row = *row;
        t->col = *col;
        strcpy(t->type, "special_symbol");
        fprintf(outFile, "<%s, %u, %u, %s>\n", t->token_name, t->row, t->col, t->type);
        (*col)++;
        return 1;
    }

    return 0;
}

int main() {
    FILE *fp = fopen("q2in.c", "r");
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    FILE *outFile = fopen("output.diff", "w");
    if (outFile == NULL) {
        printf("Cannot open output file.\n");
        fclose(fp);
        return 1;
    }

    struct token t;
    unsigned int row = 1, col = 1;
    while (getNextToken(fp, &t, &row, &col, outFile)) {
        // Process tokens
    }

    fclose(fp);
    fclose(outFile);
    return 0;
}
