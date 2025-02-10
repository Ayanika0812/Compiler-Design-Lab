#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int curr = 0;
char str[100];

// Function Declarations
void S();
void A();
void A_prime();
void B();
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

// S -> aAcBe
void S() {
    if (str[curr] == 'a') {
        curr++; // Consume 'a'
        A();    // Parse A
        if (str[curr] == 'c') {
            curr++; // Consume 'c'
            B();    // Parse B
            if (str[curr] == 'e') {
                curr++; // Consume 'e'
                return;
            } else {
                invalid();  // If 'e' is missing, it's invalid
            }
        } else {
            invalid();  // If 'c' is missing, it's invalid
        }
    } else {
        invalid();  // If 'a' is missing, it's invalid
    }
}

// A -> b A'
void A() {
    if (str[curr] == 'b') {
        curr++; // Consume 'b'
        A_prime(); // Parse A'
    } else {
        invalid();  // If 'b' is missing, it's invalid
    }
}

// A' -> b A' | 𝜖
void A_prime() {
    if (str[curr] == 'b') {
        curr++; // Consume 'b'
        A_prime(); // Recursive call for the next 'b'
    }
    // If no 'b' is found, A' → 𝜖 (empty), so we return without doing anything
}

// B -> d
void B() {
    if (str[curr] == 'd') {
        curr++; // Consume 'd'
        return;
    } else {
        invalid();  // If 'd' is missing, it's invalid
    }
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
./a.out
Enter String: abbcde
----------------SUCCESS!---------------
*/