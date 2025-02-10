#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int curr = 0;
char str[100];

// Function Declarations
void S();
void U();
void V();
void W();
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

// S -> UVW
void S() {
    U();
    V();
    W();
}

// U -> (S) | aSb | d
void U() {
    if (str[curr] == '(') {
        curr++;
        S();  // Recursively parse S inside parentheses
        if (str[curr] == ')') {
            curr++;
            return;
        } else {
            invalid();  // If no closing parentheses, it's invalid
        }
    } else if (str[curr] == 'a') {
        curr++;
        S();  // Parse S
        if (str[curr] == 'b') {
            curr++;
            return;
        } else {
            invalid();  // If 'b' is missing, it's invalid
        }
    } else if (str[curr] == 'd') {
        curr++;
        return;  // Match the character 'd'
    } else {
        invalid();  // If none of the above options match, it's invalid
    }
}

// V -> aV | 𝜖
void V() {
    if (str[curr] == 'a') {
        curr++;
        V();  // Recursively parse V after 'a'
    }
    // If no 'a', V → 𝜖 (no operation), so we return.
}

// W -> cW | 𝜖
void W() {
    if (str[curr] == 'c') {
        curr++;
        W();  // Recursively parse W after 'c'
    }
    // If no 'c', W → 𝜖 (no operation), so we return.
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


/* cc q2.c 
student@oslab-02:~/220905128/lab6$ ./a.out
Enter String: (adb)
----------------SUCCESS!---------------
*/