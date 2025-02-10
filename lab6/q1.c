#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int curr = 0;
char str[100];

// Function Declarations
void S();
void T();
void Tprime();
void invalid();
void valid();

void invalid() {
    printf("-----------------ERROR!----------------\n");
    exit(0);
}

void valid() {
    printf("----------------SUCCESS!---------------\n");
    exit(0);
}

// S -> a | > | ( T )
void S() {
    if (str[curr] == 'a') {
        curr++; // Consume 'a'
        return;
    }
    else if (str[curr] == '>') {
        curr++; // Consume '>'
        return;
    }
    else if (str[curr] == '(') {
        curr++;  // Consume '('
        T();     // Parse T inside parentheses
        if (str[curr] == ')') {
            curr++;  // Consume ')'
            return;
        } else {
            invalid();  // If no closing ')', it's invalid
        }
    } else {
        invalid();  // If no valid starting character, it's invalid
    }
}

// T -> S T'
void T() {
    S();      // Parse the first S
    Tprime(); // Parse the T' after S
}

// T' -> , S T' | 𝜖
void Tprime() {
    if (str[curr] == ',') {
        curr++;  // Consume the comma
        S();     // Parse the next S
        Tprime(); // Recursively parse the next T'
    }
    // If no comma, T' → 𝜖, so we simply return without doing anything
}

// Main function to drive the parser
int main() {
    printf("Enter String: ");
    scanf("%s", str);

    S(); // Start parsing with S (the start symbol)
    
    // Check if we've reached the end of the string
    if (str[curr] == '\0') {
        valid(); // If the parsing is successful
    } else {
        invalid(); // If there are extra characters in the input
    }

    return 0;
}



/*
 cc q1.c 
student@oslab-02:~/220905128/lab6$ ./a.out
Enter String: (a,a)
----------------SUCCESS!---------------

*/
