#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

// Function to check if the character is part of a valid C identifier (for function names)
int is_valid_char_for_identifier(char c) {
    return isalnum(c) || c == '_';
}

// Function to extract function name, return type and number of arguments
void extract_function_details(const char *line) {
    char return_type[MAX_LINE_LENGTH];
    char function_name[MAX_LINE_LENGTH];
    char args[MAX_LINE_LENGTH];
    int i = 0, j = 0;

    // Skip initial whitespace
    while (isspace(line[i])) i++;

    // Extract return type
    while (isalnum(line[i]) || line[i] == '_') {
        return_type[j++] = line[i++];
    }
    return_type[j] = '\0';

    // Skip whitespace
    while (isspace(line[i])) i++;

    // Check for the function name (identifier)
    j = 0;
    while (is_valid_char_for_identifier(line[i])) {
        function_name[j++] = line[i++];
    }
    function_name[j] = '\0';

    // Skip whitespace and the opening parenthesis '('
    while (isspace(line[i])) i++;
    if (line[i] == '(') i++;

    // Extract arguments
    j = 0;
    int paren_count = 1;  // We already encountered the first '('
    while (line[i] != ')' && line[i] != '\0') {
        if (line[i] == '(') {
            paren_count++;
        } else if (line[i] == ')') {
            paren_count--;
        }
        args[j++] = line[i++];
        if (paren_count == 0) break;
    }
    args[j] = '\0';

    // Count arguments by splitting based on commas (simple approach)
    int num_args = 0;
    char *arg = strtok(args, ",");
    while (arg != NULL) {
        num_args++;
        arg = strtok(NULL, ",");
    }

    // Print the function details
    if (strlen(function_name) > 0) {
        printf("Function Name: %s\n", function_name);
        printf("Return Type: %s\n", return_type);
        printf("Number of Arguments: %d\n\n", num_args);
    }
}

// Function to process each line of the C file
void process_file(FILE *file) {
    char line[MAX_LINE_LENGTH];

    // Read each line
    while (fgets(line, sizeof(line), file)) {
        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '/' || line[0] == '\n') {
            continue;
        }

        // Check for function definition by looking for '(' and ')'
        // Ignore lines that are part of function calls
        if (strchr(line, '(') && strchr(line, ')') && !strchr(line, ';')) {
            extract_function_details(line);
        }
    }
}

int main() {
    char filename[MAX_LINE_LENGTH];
    
    // Ask the user for the file name
    printf("Enter the C file name to analyze: ");
    scanf("%s", filename);

    // Open the file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return 1;
    }

    // Process the file to extract function details
    process_file(file);

    // Close the file
    fclose(file);
    return 0;
}

/*

/a.out
Enter the C file name to analyze: adin.c
Function Name: add
Return Type: int
Number of Arguments: 2

Function Name: print_message
Return Type: void
Number of Arguments: 1

Function Name: no_return_type_function
Return Type: void
Number of Arguments: 0

Function Name: process_data
Return Type: void
Number of Arguments: 4

Function Name: main
Return Type: int
Number of Arguments: 0

*/


