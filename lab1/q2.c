#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *sourceFile, *destFile;
    char sourceFilename[100], destFilename[100];
    long fileSize;
    int ch;

    // Step 1: Get the source and destination filenames from the user
    printf("Enter the source filename: ");
    scanf("%s", sourceFilename);
    
    printf("Enter the destination filename: ");
    scanf("%s", destFilename);

    // Step 2: Open the source file in read mode
    sourceFile = fopen(sourceFilename, "r");
    if (sourceFile == NULL) {
        printf("Error: Could not open source file %s\n", sourceFilename);
        exit(1); // Exit if the source file can't be opened
    }

    // Step 3: Find the size of the source file
    fseek(sourceFile, 0, SEEK_END);  // Move file pointer to the end
    fileSize = ftell(sourceFile);    // Get the current position (size of file)
    printf("Size of the source file: %ld bytes\n", fileSize);

    // Step 4: Open the destination file in write mode
    destFile = fopen(destFilename, "w");
    if (destFile == NULL) {
        printf("Error: Could not open destination file %s\n", destFilename);
        fclose(sourceFile);
        exit(1); // Exit if the destination file can't be opened
    }

    // Step 5: Read the source file from the end and write to the destination file
    for (long i = fileSize - 1; i >= 0; i--) {
        fseek(sourceFile, i, SEEK_SET);  // Move file pointer to the desired position
        ch = fgetc(sourceFile);          // Read the character
        fputc(ch, destFile);             // Write the character to the destination file
    }

    printf("File contents reversed and stored in %s\n", destFilename);

    // Step 6: Read and display the contents of the destination file
    printf("\nContents of the destination file (%s):\n", destFilename);
    
    // Open the destination file in read mode to display its contents
    fclose(destFile);  // Close the destination file before reopening it for reading
    destFile = fopen(destFilename, "r");  // Open for reading
    
    if (destFile == NULL) {
        printf("Error: Could not open destination file %s for reading.\n", destFilename);
        fclose(sourceFile);
        exit(1);
    }
    
    // Display the contents of the destination file
    while ((ch = fgetc(destFile)) != EOF) {
        putchar(ch);  // Print each character to the console
    }
    
    // Step 7: Close both files
    fclose(sourceFile);
    fclose(destFile);

    return 0;
}

/*
cc q2.c
student@oslab-02:~/220905128/lab1$ ./a.out
Enter the source filename: srcfile.txt
Enter the destination filename: dstfile.txt
Size of the source file: 24 bytes
File contents reversed and stored in dstfile.txt

Contents of the destination file (dstfile.txt):

321 elif CRS si sihT iH
*/
