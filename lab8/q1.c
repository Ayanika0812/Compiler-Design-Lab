#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    MAIN, INT, CHAR, ID, NUM, LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,
    SEMICOLON, COMMA, ASSIGN, IF, ELSE, REL_OP, ADD_OP, MUL_OP,
    END_OF_FILE, CHAR_LITERAL
} TokenType;


// Token structure
typedef struct Token {
    TokenType type;
    char* value;
    int row;
    int col;
} Token;

// Global variables for the tokenizer
char* buffer;
int bufferIndex = 0;
int row = 1;
int col = 1;

// Function prototypes
void getNextToken(void);
void match(TokenType type);
void initTokenizer(char* buf);

// Current token
Token currentToken;

// Function to get the next character from the buffer
char getNextChar() {
    if (bufferIndex >= strlen(buffer)) {
        return EOF;
    }
    char c = buffer[bufferIndex++];
    
    if (c == '\n') {
        row++;
        col = 1;
    } else {
        col++;
    }
    
    return c;
}

// Function to peek the next character without consuming it
char peekNextChar() {
    if (bufferIndex >= strlen(buffer)) {
        return EOF;
    }
    return buffer[bufferIndex];
}

// Function to get the next token
void getNextToken() {
    char c = getNextChar();
    while (isspace(c)) {
        c = getNextChar();
    }

    if (c == EOF) {
        currentToken.type = END_OF_FILE;
        return;
    }

    if (isdigit(c)) {
        currentToken.type = NUM;
        currentToken.value = malloc(10);
        currentToken.value[0] = c;
        int i = 1;
        while (isdigit(c = getNextChar())) {
            currentToken.value[i++] = c;
        }
        bufferIndex--; // Push back the last character
        currentToken.value[i] = '\0';
        return;
    }

    if (isalpha(c)) {
        currentToken.type = ID;
        currentToken.value = malloc(10);
        currentToken.value[0] = c;
        int i = 1;
        while (isalnum(c = getNextChar())) {
            currentToken.value[i++] = c;
        }
        bufferIndex--; // Push back the last character
        currentToken.value[i] = '\0';

        if (strcmp(currentToken.value, "main") == 0) {
            currentToken.type = MAIN;
        } else if (strcmp(currentToken.value, "int") == 0) {
            currentToken.type = INT;
        } else if (strcmp(currentToken.value, "char") == 0) {
            currentToken.type = CHAR;
        } else if (strcmp(currentToken.value, "if") == 0) {
            currentToken.type = IF;
        } else if (strcmp(currentToken.value, "else") == 0) {
            currentToken.type = ELSE;
        }
        return;
    }

    switch (c) {
        case '(':
            currentToken.type = LPAREN;
            break;
        case ')':
            currentToken.type = RPAREN;
            break;
        case '{':
            currentToken.type = LBRACE;
            break;
        case '}':
            currentToken.type = RBRACE;
            break;
        case '[':
            currentToken.type = LBRACKET;
            break;
        case ']':
            currentToken.type = RBRACKET;
            break;
        case ';':
            currentToken.type = SEMICOLON;
            break;
        case ',':
            currentToken.type = COMMA;
            break;
            case '>':
            if (peekNextChar() == '=') { // Handle >=
                currentToken.type = REL_OP;
                bufferIndex++; // Consume '='
            } else {
                currentToken.type = REL_OP; // Handle >
            }
            break;
    
        case '<':
            if (peekNextChar() == '=') { // Handle <=
                currentToken.type = REL_OP;
                bufferIndex++; // Consume '='
            } else {
                currentToken.type = REL_OP; // Handle <
            }
            break;
    
        case '=':
            if (peekNextChar() == '=') { // Handle ==
                currentToken.type = REL_OP;
                bufferIndex++; // Consume '='
            } else {
                currentToken.type = ASSIGN; // Handle =
            }
            break;
    
        case '!':
            if (peekNextChar() == '=') { // Handle !=
                currentToken.type = REL_OP;
                bufferIndex++; // Consume '='
            } else {
                printf("Error at line %d, column %d: Invalid character '!'\n", row, col);
                exit(1);
            }
            break;        

        // Handle single-quoted character literals
        case '\'':
            c = getNextChar(); // Get the character inside the quotes
            if (c == '\\') {   // Handle escape sequences like '\n'
                c = getNextChar();
                if (c == 'n' || c == 't' || c == 'r' || c == '\\' || c == '\'' || c == '\"') {
                    // Valid escape sequence
                    char escapedChar[3] = {'\\', c, '\0'};
                    currentToken.value = strdup(escapedChar);
                } else {
                    printf("Error at line %d, column %d: Invalid escape sequence\n", row, col);
                    exit(1);
                }
            } else if (c != '\'' && c != EOF) { // Handle regular characters
                char charLiteral[2] = {c, '\0'};
                currentToken.value = strdup(charLiteral);
            } else { // Invalid or incomplete character literal
                printf("Error at line %d, column %d: Invalid or incomplete character literal\n", row, col);
                exit(1);
            }

            if (getNextChar() != '\'') { // Ensure closing quote
                printf("Error at line %d, column %d: Missing closing single quote\n", row, col);
                exit(1);
            }

            currentToken.type = NUM; // Treat as a numeric value for simplicity
            return;

        default:
            printf("Invalid character: %c\n", c);
            exit(1);
    }
}


// Function to match a token type
void match(TokenType type) {
    if (currentToken.type != type) {
        printf("Error at line %d, column %d: Expected %d, but found %d\n", row, col, type, currentToken.type);
        printf("Token value: %s\n", currentToken.value);
        exit(1);
    }
    getNextToken();
}


// Recursive Descent Parser functions
void program();
void declarations();
void identifier_list();
void statement_list();
void statement();
void assign_stat();
void expn();
void eprime();
void simple_expn();
void seprime();
void term();
void tprime();
void factor();
void decision_stat();
void dprime();

void program() {
    if (currentToken.type != MAIN) {
        printf("Error at line %d, column %d: Expected 'main'\n", row, col);
        exit(1);
    }
    match(MAIN);
    match(LPAREN);
    match(RPAREN);
    match(LBRACE);
    
    declarations();
    statement_list();
    
    if (currentToken.type != RBRACE) {
        printf("Error at line %d, column %d: Expected '}'\n", row, col);
        exit(1);
    }
    match(RBRACE);
    
    printf("Parsing successful!\n");
}

void declarations() {
    while (currentToken.type == INT || currentToken.type == CHAR) {
        if (currentToken.type == INT) {
            match(INT);
        } else {
            match(CHAR);
        }
        
        identifier_list();
        
        if (currentToken.type != SEMICOLON) {
            printf("Error at line %d, column %d: Expected ';'\n", row, col);
            exit(1);
        }
        match(SEMICOLON);
    }
}

void identifier_list() {
    if (currentToken.type != ID) {
        printf("Error at line %d, column %d: Expected identifier\n", row, col);
        exit(1);
    }
    match(ID);
    
    if (currentToken.type == LBRACKET) { // Handle array declaration
        match(LBRACKET);
        
        if (currentToken.type == NUM) { // Array size
            match(NUM);
        } else {
            printf("Error at line %d, column %d: Expected array size\n", row, col);
            exit(1);
        }
        
        match(RBRACKET);
    }
    
    while (currentToken.type == COMMA) {
        match(COMMA);
        
        if (currentToken.type != ID) {
            printf("Error at line %d, column %d: Expected identifier\n", row, col);
            exit(1);
        }
        match(ID);
        
        if (currentToken.type == LBRACKET) { // Handle array declaration
            match(LBRACKET);
            
            if (currentToken.type == NUM) { // Array size
                match(NUM);
            } else {
                printf("Error at line %d, column %d: Expected array size\n", row, col);
                exit(1);
            }
            
            match(RBRACKET);
        }
    }
}


void statement_list() {
    while (currentToken.type == ID || currentToken.type == IF) {
        statement();
    }
}

void statement() {
    if (currentToken.type == ID) {
        assign_stat();
        
        if (currentToken.type != SEMICOLON) {
            printf("Error at line %d, column %d: Expected ';'\n", row, col);
            exit(1);
        }
        match(SEMICOLON);
    } else if (currentToken.type == IF) {
        decision_stat();
    }
}


void assign_stat() {
    if (currentToken.type == ID) {
        match(ID);
        
        if (currentToken.type == LBRACKET) { // Handle array indexing
            match(LBRACKET);
            
            if (currentToken.type == NUM) { // Array index
                match(NUM);
            } else {
                printf("Error at line %d, column %d: Expected array index\n", row, col);
                exit(1);
            }
            
            match(RBRACKET);
        }
        
        if (currentToken.type != ASSIGN) {
            printf("Error at line %d, column %d: Expected '='\n", row, col);
            exit(1);
        }
        match(ASSIGN);
        
        expn();
    } else {
        printf("Error at line %d, column %d: Expected identifier\n", row, col);
        exit(1);
    }
}


void expn() {
    simple_expn();
    
    if (currentToken.type == REL_OP) {
        eprime();
    }
}

void eprime() {
    match(REL_OP);
    simple_expn();
}

void simple_expn() {
    term();
    
    while (currentToken.type == ADD_OP) {
        seprime();
    }
}

void seprime() {
    match(ADD_OP);
    term();
}

void term() {
    factor();
    
    while (currentToken.type == MUL_OP) {
        tprime();
    }
}

void tprime() {
    match(MUL_OP);
    factor();
}

void factor() {
    if (currentToken.type == ID) {
        match(ID);
        
        if (currentToken.type == LBRACKET) { // Handle array indexing
            match(LBRACKET);
            
            expn();
            
            match(RBRACKET);
        }
    } else if (currentToken.type == NUM || currentToken.type == CHAR_LITERAL) { 
        match(currentToken.type); 
    } else {
        printf("Error at line %d, column %d: Expected identifier or number\n", row, col);
        exit(1);
    }
}



void decision_stat() {
    match(IF);
    match(LPAREN);
    expn();
    match(RPAREN);
    match(LBRACE);
    
    statement_list();
    
    if (currentToken.type != RBRACE) {
        printf("Error at line %d, column %d: Expected '}'\n", row, col);
        exit(1);
    }
    match(RBRACE);
    
    dprime();
}

void dprime() {
    if (currentToken.type == ELSE) {
        match(ELSE);
        match(LBRACE);
        
        statement_list();
        
        if (currentToken.type != RBRACE) {
            printf("Error at line %d, column %d: Expected '}'\n", row, col);
            exit(1);
        }
        match(RBRACE);
    }
}

// Function to initialize the tokenizer with the buffer
void initTokenizer(char* buf) {
    buffer = buf;
    bufferIndex = 0;
    row = 1;
    col = 1;
    getNextToken(); // Get the first token
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }

    // Read the entire file into a buffer
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char* inputBuffer = malloc(fileSize + 1);
    if (!inputBuffer) {
        printf("Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    size_t bytesRead = fread(inputBuffer, 1, fileSize, file);
    inputBuffer[bytesRead] = '\0'; // Null-terminate the buffer

    fclose(file);

    // Initialize tokenizer with the buffer
    initTokenizer(inputBuffer);
    program();

    free(inputBuffer);

    return 0;
}
