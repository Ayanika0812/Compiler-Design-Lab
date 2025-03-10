#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

char *action[12][6] = {
    {"s5", "s6", "s7", "s4", "", "acc"},
    {"", "s6", "", "", "", "acc"},
    {"", "r2", "s7", "", "r2", "r2"},
    {"", "r4", "r4", "", "r4", "r4"},
    {"s5", "", "", "s4", "", ""},
    {"", "r6", "r6", "", "r6", "r6"},
    {"s5", "", "", "s4", "", ""},
    {"s5", "", "", "s4", "", ""},
    {"", "s6", "", "", "s11", ""},
    {"", "r1", "s7", "", "r1", "r1"},
    {"", "r3", "r3", "", "r3", "r3"},
    {"", "r5", "r5", "", "r5", "r5"}
};

int goTo[12][3] = {
    {1, 2, 3},
    {-1, -1, -1},
    {-1, -1, -1},
    {-1, -1, -1},
    {8, 2, 3},
    {-1, -1, -1},
    {-1, 9, 3},
    {-1, 10, 3},
    {-1, -1, -1},
    {-1, -1, -1},
    {-1, -1, -1},
    {-1, -1, -1}
};

int stack[MAX], top = -1;

void push(int val) {
    if (top < MAX - 1) stack[++top] = val;
}

int pop() {
    return (top >= 0) ? stack[top--] : -1;
}

int getState() {
    return stack[top];
}

void printStack() {
    for (int i = 0; i <= top; i++) {
        if (stack[i] < 10) printf("%d", stack[i]);
        else printf("%c", stack[i]);
    }
}

int symbolToIndex(char sym) {
    switch (sym) {
        case '+': return 1;
        case '*': return 2;
        case '(': return 3;
        case ')': return 4;
        case '$': return 5;
        case 'i': return 0;
        default: return -1;
    }
}

int nonTerminalToIndex(char sym) {
    switch (sym) {
        case 'E': return 0;
        case 'T': return 1;
        case 'F': return 2;
        default: return -1;
    }
}

void parse(char *input) {
    int ip = 0;
    char *act;
    push(0);
    printf("Stack\t\tInput\t\tAction\n");

    while (1) {
        printStack();
        printf("\t%s\t", input + ip);

        int state = getState();
        int symIndex = symbolToIndex(input[ip]);

        if (symIndex == -1) {
            printf("Error: Invalid symbol '%c'\n", input[ip]);
            return;
        }

        act = action[state][symIndex];

        if (strcmp(act, "acc") == 0) {
            printf("String accepted\n");
            return;
        } else if (act[0] == 's') {
            if (input[ip] == 'i' && input[ip + 1] == 'd') {
                printf("Shift id\n");
                push('i');
                ip += 2;
                push(atoi(act + 1));
            } else {
                printf("Shift %c\n", input[ip]);
                push(input[ip++]);
                push(atoi(act + 1));
            }
        } else if (act[0] == 'r') {
            int prod = atoi(act + 1);
            switch (prod) {
                case 1:
                    pop(); pop(); pop(); pop(); pop();
                    push('E');
                    push(goTo[getState()][0]);
                    printf("Reduce by E->E+T\n");
                    break;
                case 2:
                    pop(); pop();
                    push('E');
                    push(goTo[getState()][0]);
                    printf("Reduce by E->T\n");
                    break;
                case 3:
                    pop(); pop(); pop(); pop(); pop();
                    push('T');
                    push(goTo[getState()][1]);
                    printf("Reduce by T->T*F\n");
                    break;
                case 4:
                    pop(); pop();
                    push('T');
                    push(goTo[getState()][1]);
                    printf("Reduce by T->F\n");
                    break;
                case 5:
                    pop(); pop(); pop(); pop(); pop();
                    push('F');
                    push(goTo[getState()][2]);
                    printf("Reduce by F->(E)\n");
                    break;
                case 6:
                    pop(); pop();
                    push('F');
                    push(goTo[getState()][2]);
                    printf("Reduce by F->id\n");
                    break;
            }
        } else {
            printf("Error: Unexpected symbol '%c' at position %d\n", input[ip], ip + 1);
            return;
        }
    }
}

int main() {
    char input[MAX];
    printf("Enter the input string (end with $): ");
    scanf("%s", input);
    parse(input);
    return 0;
}
