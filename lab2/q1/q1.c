#include <stdio.h>

int main() {
    FILE *inputFile, *outputFile;
    char inputFileName[100], outputFileName[100];
    int currentChar, previousChar = 0;
    printf("Enter the name of the input file: ");
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
    while ((currentChar = getc(inputFile)) != EOF) {
        if (currentChar == ' ' || currentChar == '\t') {
            if (previousChar != ' ') {
                putc(' ', outputFile); 
                previousChar = ' ';
            }
        } else {
            putc(currentChar, outputFile);
            previousChar = currentChar;
        }
    }

    // Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("Processing complete. Output written to %s.\n", outputFileName);
    return 0;
}

/* cc q1.c
student@oslab-02:~/220905128/lab2/q1$ ./a.out
Enter the name of the input file: q1input.txt
Enter the name of the output file: q1out.txt
Processing complete. Output written to q1out.txt.
*/
