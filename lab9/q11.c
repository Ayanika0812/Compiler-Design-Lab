#include <stdio.h>

#include <stdlib.h>

#include <string.h>
 
#define MAX_STACK_SIZE 100

#define NUM_TERMINALS 6

#define NUM_RULES 6

#define MAX_INPUT_SIZE 100
 
// Stack to store states

int stack[MAX_STACK_SIZE];

int top = -1;
 
// Input string and input pointer

const char *input;

int input_index = 0;
 
// Terminal symbols

char terminals[] = {'i', '+', '*', '(', ')', '$'};

char non_terminals[] = {'S', 'E', 'T', 'F'};
 
// Rule lengths (number of symbols for each production)

int rule_length[] = {1, 3, 3, 1, 3, 1};  
 
// Action Table for Shift, Reduce, or Error (simplified for illustration)

int action_table[5][5] = {

    {1, 0, 0, 2, 0},  // state 0

    {0, -1, -1, 2, 3},  // state 1

    {0, 0, -1, -1, 3},  // state 2

    {4, 0, 5, 2, 3},   // state 3

    {0, 0, 0, 0, 0}    // state 4

};
 
// GOTO Table for Non-Terminals

int goto_table[5][4] = {

    {1, 0, 0, 2},  // state 0

    {0, 0, 0, 0},  // state 1

    {0, 0, 0, 0},  // state 2

    {0, 0, 0, 0},  // state 3

    {0, 0, 0, 0}   // state 4

};
 
// Function to push a state onto the stack

void push(int state) {

    if (top >= MAX_STACK_SIZE - 1) {

        printf("Stack Overflow\n");

        exit(1);

    }

    stack[++top] = state;

}
 
// Function to pop a state from the stack

int pop() {

    if (top == -1) {

        printf("Stack Underflow\n");

        exit(1);

    }

    return stack[top--];

}
 
// Function to check the action table

int action(int state, char symbol) {

    int symbol_index = -1;

    for (int i = 0; i < NUM_TERMINALS; i++) {

        if (terminals[i] == symbol) {

            symbol_index = i;

            break;

        }

    }

    return action_table[state][symbol_index];

}
 
// Function to shift and move to the next state

void shift(int state) {

    push(state);

    input_index++;

}
 
// Function to reduce based on production rule

void reduce(int production) {

    int len = rule_length[production - 1];

    for (int i = 0; i < len; i++) {

        pop();

    }

    int goto_state = goto_table[stack[top]][production];

    push(goto_state);

}
 
// Function to handle tokenization

int next_token() {

    if (input[input_index] == '\0') return -1;  // End of string

    return input[input_index++];

}
 
// Function to perform the shift-reduce parsing

int parse() {

    push(0); // Starting state

    char symbol;
 
    while ((symbol = next_token()) != '$') {
 
        int current_state = stack[top];

        int action_code = action(current_state, symbol);
 
        if (action_code > 0) {

            shift(action_code);

        } else if (action_code < 0) {

            reduce(-action_code);

        } else {

            return 1;

        }

    }

    printf("Parsing successful!\n");

    return 0;

}
 
int main() {

    char buffer[MAX_INPUT_SIZE];
    printf("Enter the input string (use 'id' as identifiers and end with '$'): ");
    fgets(buffer, MAX_INPUT_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcat(buffer, "$");
    input = buffer;
    if (parse() == 0) {
        printf("Parsing completed.\n");
    } else {
        printf("Parsing failed.\n");
    }
    return 0;

}

 