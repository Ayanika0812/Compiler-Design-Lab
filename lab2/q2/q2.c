#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool is_preprocessor_directive(const char *line) {
    // Skip leading spaces or tabs
    while (*line == ' ' || *line == '\t') {
        line++;
    }

    // A preprocessor directive starts with '#'
    return *line == '#';
}

int main() {
    char input_file_name[256], output_file_name[256];
    
    // Ask the user for the input and output file names
    printf("Enter the input file name: ");
    scanf("%s", input_file_name);

    printf("Enter the output file name: ");
    scanf("%s", output_file_name);

    FILE *input_file = fopen(input_file_name, "r");
    FILE *output_file = fopen(output_file_name, "w");

    if (!input_file || !output_file) {
        printf("Error opening file.\n");
        return 1;
    }

    char line[1024];

    // Read the input file line by line
    while (fgets(line, sizeof(line), input_file)) {
        // If the line is a preprocessor directive, skip it
        if (!is_preprocessor_directive(line)) {
            fputs(line, output_file);
        }
    }

    fclose(input_file);
    fclose(output_file);

    printf("Preprocessor directives removed and output written to %s\n", output_file_name);
    return 0;
}
/*cc q2.c
student@oslab-02:~/220905128/lab2/q2$ ./a.out
Enter the input file name: q2in.c
Enter the output file name: q2out.c
Preprocessor directives removed and output written to q2out.c
*/
