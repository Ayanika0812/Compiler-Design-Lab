#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 30

struct node {
    char lexeme[100];  // Variable name or string literal
    char type[20];     // Variable type (e.g., int, float, string_literal, function)
    int size;          // Size in bytes (used for variables)
    struct node* next; // Pointer to the next entry in case of collisions (for open hashing)
};

// Global symbol table
struct node* symbol_table[TABLE_SIZE];

// Hash function to map variable names to indices in the symbol table
int hash(char* str) {
    int hash_value = 0;
    while (*str) {
        hash_value = (hash_value * 31 + *str) % TABLE_SIZE;
        str++;
    }
    return hash_value;
}

// Function to initialize the symbol table
void initialize_symbol_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        symbol_table[i] = NULL;
    }
}

// Function to search the symbol table for a variable name
struct node* search_in_symbol_table(char* lexeme) {
    int index = hash(lexeme);
    struct node* entry = symbol_table[index];
    while (entry) {
        if (strcmp(entry->lexeme, lexeme) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

// Function to insert a new entry into the symbol table
void insert_into_symbol_table(char* lexeme, char* type, int size) {
    if (search_in_symbol_table(lexeme)) {
        return;  // Do not insert if the entry already exists
    }

    struct node* new_entry = (struct node*)malloc(sizeof(struct node));
    strcpy(new_entry->lexeme, lexeme);
    strcpy(new_entry->type, type);
    new_entry->size = size;
    new_entry->next = NULL;

    int index = hash(lexeme);
    if (!symbol_table[index]) {
        symbol_table[index] = new_entry;
    } else {
        struct node* temp = symbol_table[index];
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_entry;
    }
}

// Function to display the symbol table
void display_symbol_table() {
    printf("Symbol Table:\n");
    printf("------------------------------------------------------\n");
    printf("LexemeName\tType\tSize\n");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        struct node* entry = symbol_table[i];
        while (entry) {
            printf("%s\t\t%s\t%d\n", entry->lexeme, entry->type, entry->size);
            entry = entry->next;
        }
    }
}

// Function to check if a string is a keyword
int is_keyword(const char* str) {
    const char* keywords[] = {
        "int", "float", "char", "double", "if", "else", "while", "for", "return", "void", "main", "break",
        "continue", "switch", "case", "default", "do", "sizeof", "struct", "typedef", NULL
    };

    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to remove preprocessor directives (i.e., #include, #define)
void remove_preprocessor_directives(const char* input_file, const char* output_file) {
    FILE* fa = fopen(input_file, "r");
    FILE* fb = fopen(output_file, "w");

    if (!fa || !fb) {
        printf("Error opening files\n");
        exit(1);
    }

    int ch;
    while ((ch = fgetc(fa)) != EOF) {
        // Skip lines starting with '#'
        if (ch == '#') {
            while ((ch = fgetc(fa)) != EOF && ch != '\n') {}
        } else {
            fputc(ch, fb);
        }
    }

    fclose(fa);
    fclose(fb);
}

// Function to remove comments (single-line and multi-line)
void remove_comments(const char* input_file, const char* output_file) {
    FILE* fa = fopen(input_file, "r");
    FILE* fb = fopen(output_file, "w");

    if (!fa || !fb) {
        printf("Error opening files\n");
        exit(1);
    }

    int ch, prev_ch = -1;
    int in_comment = 0;
    while ((ch = fgetc(fa)) != EOF) {
        // Handling single-line comment (//)
        if (ch == '/' && prev_ch == '/') {
            in_comment = 1;
            while ((ch = fgetc(fa)) != EOF && ch != '\n') {}
        }
        // Handling multi-line comment (/* */)
        if (ch == '*' && prev_ch == '/') {
            in_comment = 1;
            while ((ch = fgetc(fa)) != EOF && !(prev_ch == '*' && ch == '/')) {
                prev_ch = ch;
            }
            if (ch == EOF) break; // If EOF reached, break out
            ch = fgetc(fa); // Skip past the closing '/'
            prev_ch = -1;
        }

        if (!in_comment) {
            fputc(ch, fb);
        }

        if (ch == '\n') {
            in_comment = 0;
        }

        prev_ch = ch;
    }

    fclose(fa);
    fclose(fb);
}

// Function to remove extra whitespace (spaces, tabs, newlines)
void remove_whitespace(const char* input_file, const char* output_file) {
    FILE* fa = fopen(input_file, "r");
    FILE* fb = fopen(output_file, "w");

    if (!fa || !fb) {
        printf("Error opening files\n");
        exit(1);
    }

    int ch, prev_ch = -1;
    while ((ch = fgetc(fa)) != EOF) {
        if (isspace(ch)) {
            if (prev_ch != ' ' && prev_ch != '\n' && prev_ch != '\t') {
                fputc(' ', fb); // Replace all whitespaces with a single space
            }
        } else {
            fputc(ch, fb);
        }
        prev_ch = ch;
    }

    fclose(fa);
    fclose(fb);
}

// Function to identify tokens and fill the symbol table
void identify_tokens(const char* input_file) {
    FILE* fp = fopen(input_file, "r");
    if (fp == NULL) {
        printf("Cannot open file\n");
        exit(0);
    }

    int row = 1, col = 1;
    char c, buffer[100];
    int buffer_index = 0;
    char type[10] = "";

    while ((c = fgetc(fp)) != EOF) {
        // Handle newlines
        if (c == '\n') {
            row++;
            col = 1;
            continue;
        }

        // Handle whitespace
        if (isspace(c)) {
            col++;
            continue;
        }

        // Handle string literals
        if (c == '"') {
            buffer[buffer_index++] = c;
            col++;
            while ((c = fgetc(fp)) != EOF && c != '"') {
                buffer[buffer_index++] = c;
                col++;
            }
            buffer[buffer_index++] = c;  // Include the closing "
            col++;
            buffer[buffer_index] = '\0';

            insert_into_symbol_table(buffer, "string_literal", strlen(buffer) + 1);  // Store string literal
            buffer_index = 0;
            continue;
        }

        // Handle identifiers and keywords
        if (isalpha(c) || c == '_') {
            buffer[buffer_index++] = c;
            col++;
            while (isalnum((c = fgetc(fp))) || c == '_') {
                buffer[buffer_index++] = c;
                col++;
            }
            buffer[buffer_index] = '\0';

            // Check if it's a function like printf or main
            if (strcmp(buffer, "main") == 0) {
                insert_into_symbol_table(buffer, "function", 0);  // Treat 'main' as a function
            } else if (strcmp(buffer, "printf") == 0) {
                insert_into_symbol_table(buffer, "function", 0);  // Treat 'printf' as a function
            }
            // Check for types (e.g., int, float, char)
            else if (strcmp(buffer, "int") == 0 || strcmp(buffer, "float") == 0 || strcmp(buffer, "char") == 0) {
                // Store the type for future variables
                strcpy(type, buffer);
            } else {
                // Handle variables based on the type
                if (strcmp(type, "int") == 0) {
                    insert_into_symbol_table(buffer, "int", 4);
                } else if (strcmp(type, "float") == 0) {
                    insert_into_symbol_table(buffer, "float", 4);
                } else if (strcmp(type, "char") == 0) {
                    insert_into_symbol_table(buffer, "char", 1);
                }
            }

            buffer_index = 0;
            ungetc(c, fp);
            continue;
        }
    }

    fclose(fp);
}

int main() {
    initialize_symbol_table();

    // Step 1: Remove preprocessor directives and comments
    remove_preprocessor_directives("q1in.c", "preprocessed.c");
    remove_comments("preprocessed.c", "no_comments.c");

    // Step 2: Remove extra whitespace
    remove_whitespace("no_comments.c", "cleaned.c");

    // Step 3: Identify tokens and update the symbol table
    identify_tokens("cleaned.c");

    // Step 4: Display the symbol table
    display_symbol_table();

    return 0;
}
