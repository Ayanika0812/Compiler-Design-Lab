#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Function Prototypes
int isArithmeticOperator(char c);
int isRelationalOperator(char c, char next);
int isLogicalOperator(char c, char next);
int isSpecialSymbol(char c);
int isKeyword(char *str);
int isIdentifier(char *str);
int isNumeric(char *str);
int isStringLiteral(char *str);

void identifyTokens(char *input, FILE *outputFile) {
    int i = 0;
    int row = 1;   // Start at row 1
    int col = 1;   // Start at column 1
    while (input[i] != '\0') {
        // Check for new line and adjust row and column
        if (input[i] == '\n') {
            row++;  // Move to the next row
            col = 1;  // Reset column to 1
            i++;
            continue;
        }

        // Check for space and just skip it
        if (isspace(input[i])) {
            i++;  // Skip spaces
            col++;  // Increment column for each space
            continue;
        }

        // Check for Arithmetic Operators
        if (isArithmeticOperator(input[i])) {
            fprintf(outputFile, "<%c,%d,%d>\n", input[i], row, col);
            i++;
            col++;
        }
        
        // Check for Relational or Logical Operators
        else if (input[i] == '=' || input[i] == '<' || input[i] == '>' || input[i] == '!' || input[i] == '&' || input[i] == '|') {
            if (isRelationalOperator(input[i], input[i+1])) {
                fprintf(outputFile, "<%c%c,%d,%d>\n", input[i], input[i+1], row, col);
                i += 2;
                col += 2;
            } else if (isLogicalOperator(input[i], input[i+1])) {
                fprintf(outputFile, "<%c%c,%d,%d>\n", input[i], input[i+1], row, col);
                i += 2;
                col += 2;
            } else {
                fprintf(outputFile, "<%c,%d,%d>\n", input[i], row, col);
                i++;
                col++;
            }
        }
        
        // Check for Special Symbols
        else if (isSpecialSymbol(input[i])) {
            fprintf(outputFile, "<%c,%d,%d>\n", input[i], row, col);
            i++;
            col++;
        }
        
        // Check for String Literals
        else if (input[i] == '"') {
            int start = i;
            i++;
            while (input[i] != '"' && input[i] != '\0') i++;
            if (input[i] == '"') {
                char str[100];
                strncpy(str, input + start, i - start + 1);
                str[i - start + 1] = '\0';
                fprintf(outputFile, "<%s,%d,%d>\n", str, row, col);
                i++;
                col += (i - start + 1);
            }
        }
        
        // Check for Keywords or Identifiers
        else if (isalpha(input[i]) || input[i] == '_') {
            int start = i;
            while (isalnum(input[i]) || input[i] == '_') i++;
            char str[100];
            strncpy(str, input + start, i - start);
            str[i - start] = '\0';

            if (isKeyword(str)) {
                fprintf(outputFile, "<%s,%d,%d>\n", str, row, col);
            } else if (isIdentifier(str)) {
                // Instead of printing the actual identifier, print "id"
                fprintf(outputFile, "<id,%d,%d>\n", row, col);
            }
            col += (i - start);
        }
        
        // Check for Numeric Constants
        else if (isdigit(input[i])) {
            int start = i;
            while (isdigit(input[i])) i++;
            char num[100];
            strncpy(num, input + start, i - start);
            num[i - start] = '\0';
            fprintf(outputFile, "<%s,%d,%d>\n", num, row, col);
            col += (i - start);
        } else {
            i++;
            col++;  // Increment column for any unrecognized character
        }
    }
}

// Function Definitions

int isArithmeticOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

int isRelationalOperator(char c, char next) {
    if (c == '=') {
        return (next == '=');
    } else if (c == '<' || c == '>' || c == '!') {
        return (next == '=' || next == '\0');
    }
    return 0;
}

int isLogicalOperator(char c, char next) {
    if (c == '&' || c == '|') {
        return (next == c);  // Logical AND (&&) or OR (||)
    }
    return 0;
}

int isSpecialSymbol(char c) {
    return (c == '{' || c == '}' || c == '[' || c == ']' || c == '(' || c == ')'
            || c == ';' || c == ',' || c == '.' || c == ':' || c == '#');
}

int isKeyword(char *str) {
    const char *keywords[] = {
        "int", "float", "char", "if", "else", "while", "return", "void", "for", "break", "continue", "switch", "case", "default"
    };
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isIdentifier(char *str) {
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

int isNumeric(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int isStringLiteral(char *str) {
    return (str[0] == '"' && str[strlen(str) - 1] == '"');
}

int main() {
    // Sample input
    char input[] = "int x = 10;\nfloat y = 20.5;\nif (x == y) {\ny = x;\n}";

    // Open output file in write mode
    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Call the identifyTokens function and write to the file
    identifyTokens(input, outputFile);

    // Close the output file
    fclose(outputFile);

    printf("Output written to 'output.txt'.\n");

    return 0;
}
