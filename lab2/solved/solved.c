//Program to remove single and multiline comments from a given ‘C’ file.
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fa, *fb;
    int ca, cb;

    // Open input C file for reading
    fa = fopen("solvedinput.c", "r");
    if (fa == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }

    // Open output file for writing
    fb = fopen("solvedout.c", "w");

    ca = getc(fa);
    while (ca != EOF) {
        // Handle spaces
        if (ca == ' ') {
            putc(ca, fb);
            while (ca == ' ') {
                ca = getc(fa);
            }
        }

        // Handle comments
        if (ca == '/') {
            cb = getc(fa);
            
            // Single-line comment
            if (cb == '/') {
                while (ca != '\n') {
                    ca = getc(fa);
                }
            }
            // Multi-line comment
            else if (cb == '*') {
                do {
                    while (ca != '*') {
                        ca = getc(fa);
                    }
                    ca = getc(fa);
                } while (ca != '/');
            }
            // Not a comment, write normally
            else {
                putc(ca, fb);
                putc(cb, fb);
            }
        } else {
            putc(ca, fb);
        }
        
        ca = getc(fa);
    }

    // Close the files
    fclose(fa);
    fclose(fb);
    
    return 0;
}
