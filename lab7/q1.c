/*
Program → main () { declarations statement-list }
Declarations → data-type identifier-list; declarations | ε
data-type → int | char
identifier-list → id | id, identifier-list | id[number], identifier-list | id[number]
statement_list → statement statement_list | ε
statement → assign-stat; | decision_stat
assign_stat → id = expn
expn → simple-expn eprime
eprime → relop simple-expn | ε
simple-expn → term seprime
seprime → addop term seprime | ε
term → factor tprime
tprime → mulop factor tprime | ε
factor → id | num
decision-stat → if (expn) { statement_list } dprime
dprime → else { statement_list } | ε
relop → == | != | <= | >= | > | <
addop → + | -
mulop → * | / | %
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token structure
typedef struct {
    char token[100];
    int row, col;
} Token;

char input[10000];  // Store file contents
int pos = 0, row = 1, col = 1;  // Track position
Token currentToken;

// Function declarations
Token getNextToken();
void program();
void declarations();
void data_type();
void identifier_list();
void assign_stat();
void match(char *expected);
void advance();
void readFile(const char *filename);

// ** Read file into input buffer **
void readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    // Read file content into `input` buffer
    fread(input, 1, sizeof(input), file);
    fclose(file);
}

// ** Lexical Analyzer (Tokenizer) **
Token getNextToken() {
    Token tok;
    tok.row = row;
    tok.col = col;

    // Skip spaces, newlines, tabs, and comments
    while (input[pos] == ' ' || input[pos] == '\n' || input[pos] == '\t' || input[pos] == '/') {
        if (input[pos] == '\n') {
            row++;
            col = 1;
        } else if (input[pos] == '\t') {
            col += 4;  // You can adjust the tab width here (usually 4 spaces)
        } else if (input[pos] == '/') {
            if (input[pos + 1] == '/') {  // Single-line comment
                while (input[pos] != '\n' && input[pos] != '\0') pos++;
            } else if (input[pos + 1] == '*') {  // Multi-line comment
                pos += 2;
                while (input[pos] != '\0' && !(input[pos] == '*' && input[pos + 1] == '/')) {
                    if (input[pos] == '\n') row++;
                    pos++;
                }
                if (input[pos] == '*') pos += 2;  // Skip closing */
            }
        }
        else {
            col++;  // Increment column for other characters
        }
        pos++;
    }

    // Keywords and Identifiers
    if (isalpha(input[pos])) {
        int start = pos;
        while (isalnum(input[pos])) pos++;
        strncpy(tok.token, &input[start], pos - start);
        tok.token[pos - start] = '\0';
        return tok;
    }

    // Numbers
    if (isdigit(input[pos])) {
        int start = pos;
        while (isdigit(input[pos])) pos++;
        strncpy(tok.token, &input[start], pos - start);
        tok.token[pos - start] = '\0';
        return tok;
    }

    // Special characters
    if (input[pos] != '\0') {
        tok.token[0] = input[pos++];
        tok.token[1] = '\0';
    } else {
        strcpy(tok.token, "EOF");
    }

    return tok;
}

// Fetch next token
void advance() {
    currentToken = getNextToken();
}

// Match token
void match(char *expected) {
    if (strcmp(currentToken.token, expected) == 0) {
        advance();
    } else {
        printf("Error: Expected '%s' at row %d, col %d, found '%s'\n",
               expected, currentToken.row, currentToken.col, currentToken.token);
        exit(1);  // Stop execution on error
    }
}

// ** Recursive Descent Parsing Functions **
void program() {
    match("main");
    match("(");
    match(")");
    match("{");
    declarations();
    assign_stat();
    match("}");

    // Check if there are unexpected tokens after `}`
    if (strcmp(currentToken.token, "EOF") != 0) {
        printf("Error: Unexpected token '%s' at row %d, col %d\n",
               currentToken.token, currentToken.row, currentToken.col);
        exit(1);
    }

    printf("Parsing Successful!\n");
}

void declarations() {
    if (strcmp(currentToken.token, "int") == 0 || strcmp(currentToken.token, "char") == 0) {
        data_type();
        identifier_list();
        match(";");
        declarations(); // Recursive call for multiple declarations
    }
}

void data_type() {
    if (strcmp(currentToken.token, "int") == 0 || strcmp(currentToken.token, "char") == 0) {
        advance();
    } else {
        printf("Error: Expected 'int' or 'char' at row %d, col %d\n", currentToken.row, currentToken.col);
        exit(1);
    }
}

void identifier_list() {
    if (isalpha(currentToken.token[0])) {
        advance();
        if (strcmp(currentToken.token, ",") == 0) {
            advance();
            identifier_list();
        }
    } else {
        printf("Error: Expected identifier at row %d, col %d\n", currentToken.row, currentToken.col);
        exit(1);
    }
}

void assign_stat() {
    // Step 1: Expect identifier
    if (strcmp(currentToken.token, "EOF") == 0) {
        printf("Error: Unexpected end of file. Expected identifier at row %d, col %d\n",
               currentToken.row, currentToken.col);
        exit(1);
    }
    if (!isalpha(currentToken.token[0])) {  
        printf("Error: Expected identifier at row %d, col %d, found '%s'\n",
               currentToken.row, currentToken.col, currentToken.token);
        exit(1);
    }
    advance();  // Move to next token after identifier

    // Step 2: Expect '='
    if (strcmp(currentToken.token, "EOF") == 0) {
        printf("Error: Unexpected end of file. Expected '=' at row %d, col %d\n",
               currentToken.row, currentToken.col);
        exit(1);
    }
    if (strcmp(currentToken.token, "=") != 0) {
        printf("Error: Expected '=' at row %d, col %d, found '%s'\n",
               currentToken.row, currentToken.col, currentToken.token);
        exit(1);
    }
    advance();  // Move to next token after '='

    // Step 3: Expect identifier (id) or number (num)
    if (strcmp(currentToken.token, "EOF") == 0) {
        printf("Error: Unexpected end of file. Expected identifier or number at row %d, col %d\n",
               currentToken.row, currentToken.col);
        exit(1);
    }
    if (!isalpha(currentToken.token[0]) && !isdigit(currentToken.token[0])) {
        printf("Error: Expected identifier or number at row %d, col %d, found '%s'\n",
               currentToken.row, currentToken.col, currentToken.token);
        exit(1);
    }
    advance();  // Move to next token after id/num

    // Step 4: Expect semicolon `;`
    if (strcmp(currentToken.token, "EOF") == 0) {
        printf("Error: Unexpected end of file. Expected ';' at row %d, col %d\n",
               currentToken.row, currentToken.col);
        exit(1);
    }
    if (strcmp(currentToken.token, ";") != 0) {
        printf("Error: Expected ';' at row %d, col %d, found '%s'\n",
               currentToken.row, currentToken.col, currentToken.token);
        exit(1);
    }
    advance();  // Move to next token after semicolon
}

// ** Main function to run the parser **
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    readFile(argv[1]); // Read file contents into buffer
    advance(); // Get first token
    program(); // Start parsing

    return 0;
}



/*

 gcc q1.c -o q1
student@oslab-02:~/220905128/lab7$ ./q1 test1.c
Parsing Successful!
student@oslab-02:~/220905128/lab7$ ./q1 err1.c
Error: Expected ';' at row 2, col 6, found '='
student@oslab-02:~/220905128/lab7$ ./q1 err2.c
Error: Unexpected end of file. Expected identifier at row 1, col 1


*/