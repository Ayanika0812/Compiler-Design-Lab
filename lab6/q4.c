#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int curr = 0;
char str[100];

// Function Declarations
void S();
void L();
void L_prime();
void invalid();
void valid();

// Function to print error and exit
void invalid() {
    printf("-----------------ERROR!----------------\n");
    exit(0);
}

// Function to print success and exit
void valid() {
    printf("----------------SUCCESS!---------------\n");
    exit(0);
}

// S -> (L) | a
void S() {
    if (str[curr] == '(') {
        curr++;  // Consume '('
        L();     // Parse L
        if (str[curr] == ')') {
            curr++;  // Consume ')'
            return;
        } else {
            invalid();  // If no closing ')', it's invalid
        }
    } else if (str[curr] == 'a') {
        curr++;  // Consume 'a'
        return;  // S -> a, so we return here
    } else {
        invalid();  // If neither '(' nor 'a' is found, it's invalid
    }
}

// L -> S L'
void L() {
    S();    // Parse S
    L_prime();  // Parse L'
}

// L' -> , S L' | 𝜖
void L_prime() {
    if (str[curr] == ',') {
        curr++;  // Consume ','
        S();     // Parse S
        L_prime();  // Recursively parse the rest
    }
    // If no ',' is found, L' → 𝜖 (empty), so we return without doing anything
}

// Main function to drive the parser
int main() {
    printf("Enter String: ");
    scanf("%s", str);

    S();  // Start parsing with S
    
    // Check if we've reached the end of the string
    if (str[curr] == '\0') {
        valid();
    } else {
        invalid();
    }

    return 0;
}


/*
tudent@oslab-02:~/220905128/lab6$ ./a.out
Enter String: (a,a)
----------------SUCCESS!---------------
student@oslab-02:~/220905128/lab6$ ./a.out
Enter String: a
----------------SUCCESS!---------------
student@oslab-02:~/220905128/lab6$ ./a.out
Enter String: ((,a
-----------------ERROR!----------------
student@oslab-02:~/220905128/lab6$ ./a.out
Enter String: ((,a)
*/