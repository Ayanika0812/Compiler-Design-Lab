#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char filename[100], ch;
    int lineCount = 0, charCount = 0;

    // Step 1: Get the file name from the user
    printf("Enter the filename to open: ");
    scanf("%s", filename);

    // Step 2: Open the file for reading
    file = fopen(filename, "r");
    
    // Check if the file exists
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);  // Exit the program if the file can't be opened
    }

    // Step 3: Read the file character by character
    while ((ch = fgetc(file)) != EOF) {
        charCount++;  // Increment character count for each character
        if (ch == '\n') {  // Increment line count when a newline is found
            lineCount++;
        }
    }

    // If there was no newline at the end of the file, increment the line count once more
    if (charCount > 0 && ch != '\n') {
        lineCount++;
    }

    // Step 4: Display the result
    printf("Total number of lines: %d\n", lineCount);
    printf("Total number of characters: %d\n", charCount);

    // Step 5: Close the file
    fclose(file);

    return 0;
}


/* 
cc q1.c
student@oslab-02:~/220905128/lab1$ ./a.out
Enter the filename to open: file.txt
Total number of lines: 2
Total number of characters: 12
*/

