#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TABLE_SIZE 100

// Define a simple structure for the hash table entry
typedef struct Node {
    char *verb;
    struct Node *next;
} Node;

// Create the hash table
Node* hashTable[TABLE_SIZE];

// Simple hash function (based on the sum of ASCII values of the characters)
int hash(char *str) {
    int hashValue = 0;
    while (*str) {
        hashValue += *str++;
    }
    return hashValue % TABLE_SIZE;
}

// Function to insert a verb into the hash table
void insert(char *str) {
    int index = hash(str);  // Get the hash value for the string
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->verb = strdup(str);  // Duplicate the string into the node
    newNode->next = NULL;

    // If the hash table index is empty, insert the node
    if (hashTable[index] == NULL) {
        hashTable[index] = newNode;
    } else {
        // Collision handling: Add the node to the linked list at that index
        Node *temp = hashTable[index];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Function to search for a verb in the hash table
int search(char *key) {
    int index = hash(key);  // Get the hash value for the string
    Node *temp = hashTable[index];

    // Traverse the linked list at that index to find the verb
    while (temp != NULL) {
        if (strcmp(temp->verb, key) == 0) {
            return index;  // Found the verb, return the hash value (index)
        }
        temp = temp->next;
    }

    // Verb not found
    return -1;
}

// Function to tokenize input sentence and identify verbs
void identifyAndInsertVerbs(char *sentence) {
    // A very simple list of verbs for demonstration
    char *knownVerbs[] = {"is", "run", "eat", "play", "have", "be", "go", "do", "make"};
    int numVerbs = sizeof(knownVerbs) / sizeof(knownVerbs[0]);

    // Tokenize the sentence using space as a delimiter
    char *token = strtok(sentence, " ");
    while (token != NULL) {
        // Check if the token is a verb in the known list
        for (int i = 0; i < numVerbs; i++) {
            if (strcasecmp(token, knownVerbs[i]) == 0) {
                if (search(token) == -1) {
                    // Verb not found in the table, insert it
                    insert(token);
                    printf("Inserted verb: %s\n", token);
                } else {
                    printf("Verb already present: %s\n", token);
                }
            }
        }
        token = strtok(NULL, " ");  // Move to the next word
    }
}

int main() {
    char sentence[256];

    // Initialize the hash table (set all entries to NULL)
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    // Read the input sentence
    printf("Enter a sentence: ");
    fgets(sentence, sizeof(sentence), stdin);
    sentence[strcspn(sentence, "\n")] = '\0';  // Remove the newline character from input

    // Identify and insert verbs from the sentence
    identifyAndInsertVerbs(sentence);

    // Optionally, you can search for a specific verb
    char searchVerb[50];
    printf("Enter a verb to search: ");
    scanf("%s", searchVerb);
    if (search(searchVerb) != -1) {
        printf("Verb '%s' found in the hash table.\n", searchVerb);
    } else {
        printf("Verb '%s' not found in the hash table.\n", searchVerb);
    }

    return 0;
}

/*
./a.out
Enter a sentence: I will run to the park and play with Carla
Inserted verb: run
Inserted verb: play
Enter a verb to search: play
Verb 'play' found in the hash table.
*/
