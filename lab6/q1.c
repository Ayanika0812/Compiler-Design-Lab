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

// S -> a | > | ( T )
void S() {
    if (str[curr] == 'a') {
        curr++;
        return;
    }
    else if (str[curr] == '>') {
        curr++;
        return;
    }
    else if (str[curr] == '(') {
        curr++;
        T();  // Parsing T inside parentheses
        if (str[curr] == ')') {
            curr++;
            return;
        } else {
            invalid();
        }
    } else {
        invalid();
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
        curr++;   // Consume the comma
        S();      // Parse the next S
        Tprime(); // Recursively parse the next T'
    }
    // If no comma, T' → 𝜖, so we simply return without doing anything
}

// Main function to drive the parser
int main() {
    printf("Enter String: ");
    scanf("%s", str);

    T(); // Start parsing with T
    
    // Check if we've reached the end of the string
    if (str[curr] == '\0') {
        valid();
    } else {
        invalid();
    }

    return 0;
}



/*

 ./a.out
Enter String: (a,a)
----------------SUCCESS!---------------
*/