#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_KEYWORDS 32

const char *keywords[MAX_KEYWORDS] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof",
    "static", "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};

int isKeyword(const char *word) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1; // It's a keyword
        }
    }
    return 0;
}
void toUpperCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

int main() {
    FILE *inputFile, *outputFile;
    char inputFileName[100], outputFileName[100];
    char word[256];
    int ch, index = 0;

    printf("Enter the name of the input C file: ");
    scanf("%s", inputFileName);

    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Cannot open file %s.\n", inputFileName);
        return 1;
    }
    printf("Enter the name of the output file: ");
    scanf("%s", outputFileName);
    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("Cannot open file %s.\n", outputFileName);
        fclose(inputFile);
        return 1;
    }
    while ((ch = getc(inputFile)) != EOF) {
        if (isalnum(ch) || ch == '_') {
            word[index++] = ch;
        } else {
            if (index > 0) {
                // End of a word
                word[index] = '\0';
                if (isKeyword(word)) {
                    toUpperCase(word);
                }
                fputs(word, outputFile);
                index = 0;
            }
            putc(ch, outputFile);
        }
    }
    if (index > 0) {
        word[index] = '\0';
        if (isKeyword(word)) {
            toUpperCase(word);
        }
        fputs(word, outputFile);
    }
    fclose(inputFile);
    fclose(outputFile);

    printf("Processing complete. Keywords have been converted to uppercase in %s.\n", outputFileName);
    return 0;
}