#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

// Function to read a line from the file
int read_line(FILE *file, char *buffer) {
    if (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        return 1; // Line read successfully
    }
    return 0; // No more lines to read
}

int main() {
    FILE *file1, *file2, *result;
    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    char file1_name[100], file2_name[100], result_name[100];

    // Take input from user for filenames
    printf("Enter the first filename: ");
    scanf("%s", file1_name);

    printf("Enter the second filename: ");
    scanf("%s", file2_name);

    printf("Enter the result filename: ");
    scanf("%s", result_name);

    // Open the first file for reading
    file1 = fopen(file1_name, "r");
    if (file1 == NULL) {
        perror("Error opening file1");
        return 1;
    }

    // Open the second file for reading
    file2 = fopen(file2_name, "r");
    if (file2 == NULL) {
        perror("Error opening file2");
        fclose(file1);
        return 1;
    }

    // Open the result file for writing
    result = fopen(result_name, "w");
    if (result == NULL) {
        perror("Error opening result file");
        fclose(file1);
        fclose(file2);
        return 1;
    }

    // Read and write lines alternatively
    while (1) {
        // Read a line from the first file, if available
        if (read_line(file1, line1)) {
            fputs(line1, result);  // Write to result file
        }

        // Read a line from the second file, if available
        if (read_line(file2, line2)) {
            fputs(line2, result);  // Write to result file
        }

        // If both files have no more lines, break the loop
        if (feof(file1) && feof(file2)) {
            break;
        }
    }

    printf("Lines merged successfully and stored in %s\n", result_name);

    // Close all files
    fclose(file1);
    fclose(file2);
    fclose(result);

    return 0;
}


/*cc q32.c
student@oslab-02:~/220905128/lab1/q3$ ./a.out
Enter the first filename: file1.txt
Enter the second filename: file2.txt
Enter the result filename: res.txt
Lines merged successfully and stored in res.txt
student@oslab-02:~/220905128/lab1/q3$ cat res.txt
This is file1.txt
This is file2.txt

*/