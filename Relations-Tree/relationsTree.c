#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INIT_DEG 0

typedef struct Node {
    void * data;
    struct Node * next;
} Node;

typedef struct Person {
    char * name;
    int age;
    struct Person * marriedTo;
    Node * children;
    struct Person * parent;
    int visited;
} Person;

typedef struct Persons {
    Node * head;
} Persons;


void printMenu(void);
int handleUserChoice(char, Persons *);
void exitProgram(Node * head);
Person * getPerson(Node * head, char *name);
void unVisitTree(Node * head);
void exitProgram(Node * head);
void incrementAge(Node * head, unsigned int amount);
void getIncrementNumber(Node * head);
void findCommonParent(Person *, char ***, unsigned int ,\
                      unsigned int , unsigned int *, Node * persons);
void getRelativeFromDegree(Persons * persons);
void getPersonsCount(Node * head);
Person * makePerson(Persons * persons, char * name, int age, Person * parent);
void addNewPerson(Persons * persons, Node ** head, Person * person);
void merryTwoPeps(Person * p1, Person * p2);
int validateMarriage(Person * p1, Person * p2);

/**
 * @brief Entry point for the program. 
 *
 * Initializes the persons list and runs a loop to display a menu,
 * capture user input, and delegate handling of that input to the 
 * appropriate function.
 *
 * @return 0 Always returns 0 on successful program termination.
 */
int main() {
   // User's menu selection
    char choice;

    // Initialize the persons list (empty at the start)
    Persons persons = { NULL };

    // Main program loop (runs until explicitly terminated in handleUserChoice)
    while (1) {
        // Display the program menu options
        printMenu();

        // Read user input (one character for choice)
        if (scanf("%c", &choice) != 1) {
            fprintf(stderr, "Error: invalid input.\n");
            break;  // Exit loop if input fails
        }

        // Clear the newline character left in the buffer
        scanf("%*c");

        // Handle the user's choice (may modify persons list or exit)
        handleUserChoice(choice, &persons);
    }

    return 0;
}

/**
 * @brief Recursively frees a linked list of nodes and associated person data.
 *
 * This function traverses the linked list starting from `head`, freeing
 * each node's data, including dynamically allocated strings and child lists.
 * It handles married persons carefully to avoid double-freeing children.
 *
 * @param head Pointer to the first node in the list.
 */
void freeList(Node * head) {

    Node *temp;  // Temporary pointer to hold the next node
    Person *p;   // Pointer to the person data stored in each node

    // Traverse the list
    while (head) {
        // Retrieve the person stored in the current node
        p = head->data;

        // Edge case: avoid freeing the children of the spouse twice
        if (p->marriedTo) {
            p->marriedTo->children = NULL;
        }

        // Free dynamically allocated components
        free(p->name);         // Free the person's name string
        freeList(p->children); // Recursively free the children list

        // Free the person structure itself
        free(p);

        // Save next node and free current node
        temp = head->next;
        free(head);

        // Advance to the next node
        head = temp;
    }
}

/**
 * @brief Recursively traverses a tree and resets the visited flag for each person.
 *
 * This function iterates through the tree nodes starting from `head`, setting
 * the `visited` variable of each `Person` to 0. If the person has a spouse
 * (`marriedTo`), their visited flag is also reset. The function recursively
 * processes both the person's children and the next sibling in the list.
 *
 * @param head Pointer to the root node of the tree/subtree to traverse.
 */
void unVisitTree(Node *head) {

    // Base case: if the current node is NULL, return immediately
    if (head == NULL) {
        return;
    }

    // Cast the node's data to a Person structure
    Person *person = (Person*)(head->data);

    // Reset the visited flag for this person
    person->visited = 0;

    // If the person has a spouse, reset their visited flag too
    if (person->marriedTo) {
        person->marriedTo->visited = 0;
    }
    
    // Recursively reset visited flags for the person's children
    unVisitTree(person->children);

    // Recursively reset visited flags for the next sibling in the list
    unVisitTree(head->next);
}


/**
 * @brief Frees all allocated memory for the persons list and terminates the program.
 *
 * This function ensures that all dynamically allocated memory for the list
 * (including persons and their children) is released before exiting.
 *
 * @param head Pointer to the first node of the list to be freed.
 */
void exitProgram(Node *head) {
    // Free the entire list and all associated person data recursively
    freeList(head);

    // Exit the program with a status code (1 indicates abnormal termination)
    exit(1);
}

/**
 * @brief Reads an input string of unlimited length from the user.
 *
 * This function prints a custom prompt to the user, then reads characters
 * one by one until a newline ('\n') is encountered. The string is dynamically
 * allocated and expanded as needed using `realloc`. Returns a pointer to the
 * resulting string.
 *
 * @param customString A custom prompt string to display to the user.
 * @param persons Pointer to the Persons structure (used to free memory on exit).
 * @return Pointer to the dynamically allocated string containing user input.
 */
char *getInfiniteString(char *customString, Persons *persons) {
    
    // Clear the input buffer from unwanted characters
    char userInput;
    printf("%s\n", customString);
    scanf("%c", &userInput);

    // Initialize index for the string
    int index = 0;

    // Allocate initial memory for the string (2 bytes for first char + '\0')
    char *userInputString = (char *)malloc(2 * sizeof(char));
    
    // If memory allocation fails, free list and exit
    if (userInputString == NULL) {
        exitProgram(persons->head);
    }

    // Read characters until newline is encountered
    while (userInput != '\n') {
        // Store the current character at the current index
        userInputString[index] = userInput;
    
        // Increment index for next character
        index++;

        // Reallocate memory to accommodate the next character + null terminator
        char *temp = (char*)realloc(userInputString, sizeof(char) * (index + 2));
        if (temp == NULL) {
            free(userInputString);      // Free previous memory before exiting
            exitProgram(persons->head);
        }

        // Assign reallocated memory back to the string pointer
        userInputString = temp;

        // Read the next character from input
        scanf("%c", &userInput);
    }

    // Null-terminate the string
    userInputString[index] = '\0';
    
    // Return the dynamically formed string
    return userInputString;
}


/**
 * @brief Reads an input string of unlimited length from the user.
 *
 * This function prints a custom prompt to the user, then reads characters
 * one by one until a newline ('\n') is encountered. The string is dynamically
 * allocated and expanded as needed using `realloc`. Returns a pointer to the
 * resulting string.
 *
 * @param customString A custom prompt string to display to the user.
 * @param persons Pointer to the Persons structure (used to free memory on exit).
 * @return Pointer to the dynamically allocated string containing user input.
 */
char *getInfiniteString(char *customString, Persons *persons) {
    
    // Clear the input buffer from unwanted characters
    char userInput;
    printf("%s\n", customString);
    scanf("%c", &userInput);

    // Initialize index for the string
    int index = 0;

    // Allocate initial memory for the string (2 bytes for first char + '\0')
    char *userInputString = (char *)malloc(2 * sizeof(char));
    
    // If memory allocation fails, free list and exit
    if (userInputString == NULL) {
        exitProgram(persons->head);
    }

    // Read characters until newline is encountered
    while (userInput != '\n') {
        // Store the current character at the current index
        userInputString[index] = userInput;
    
        // Increment index for next character
        index++;

        // Reallocate memory to accommodate the next character + null terminator
        char *temp = (char*)realloc(userInputString, sizeof(char) * (index + 2));
        if (temp == NULL) {
            free(userInputString);      // Free previous memory before exiting
            exitProgram(persons->head);
        }

        // Assign reallocated memory back to the string pointer
        userInputString = temp;

        // Read the next character from input
        scanf("%c", &userInput);
    }

    // Null-terminate the string
    userInputString[index] = '\0';
    
    // Return the dynamically formed string
    return userInputString;
}

/**
 * @brief Searches for a person in the tree by name.
 *
 * This function traverses the tree starting from `head`, looking for a person
 * whose `name` matches `nameCheck`. It uses the `visited` flag to avoid
 * revisiting the same person and recursively searches children and next siblings.
 *
 * @param head Pointer to the first node of the list/tree to search.
 * @param nameCheck The name of the person to find.
 * @return Pointer to the Person struct if found; otherwise, NULL.
 */
Person *getPersonByName(Node *head, char *nameCheck) {
    
    // Base case: if no nodes, return NULL
    if (head == NULL) {
        return NULL;
    }

    // Get the person stored in the current node
    Person *person = (Person*)(head->data);
    Person *wantedPerson = NULL;
    
    // Skip this person if already visited
    if (person->visited) {
        return NULL;
    }

    // Mark the person as visited to avoid revisiting
    person->visited = 1;

    // Check if the current person's name matches the search
    if (!strcmp(person->name, nameCheck)) {
        return person;
    }

    // Recursively search the person's children
    if (!wantedPerson){
        wantedPerson = getPersonByName(person->children, nameCheck);
    }

    // Recursively search the next sibling if not found yet
    if (!wantedPerson) {
        wantedPerson = getPersonByName(head->next, nameCheck);
    }
    
    // Return the found person or NULL if not found
    return wantedPerson;
}

/**
 * @brief Creates a new person with the given details.
 *
 * Allocates memory for a new Person structure, initializes its fields,
 * and sets the parent pointer. Exits the program if memory allocation fails.
 *
 * @param persons Pointer to the Persons structure (used to free memory on exit).
 * @param name Name of the new person.
 * @param age Age of the new person.
 * @param parent Pointer to the parent Person (NULL if none).
 * @return Pointer to the newly created Person structure.
 */
Person *makePerson(Persons *persons, char *name, int age, Person *parent) {
    
    // Allocate memory for the new person
    Person *person = (Person*)malloc(sizeof(Person));

    // Check for allocation failure
    if (!person) {
        exitProgram(persons->head);
    }

    // Set person details
    person->name = name;
    person->age = age;
    person->visited = 0;
    person->parent = parent;

    // Return the newly created person
    return person;
}

/**
 * @brief Adds a new person to the persons list.
 *
 * Recursively traverses the list until an empty spot is found,
 * allocates a new Node, and stores the person in it. Exits the program
 * if memory allocation fails.
 *
 * @param persons Pointer to the Persons structure (used for freeing memory on exit).
 * @param head Double pointer to the head of the list/sublist.
 * @param person Pointer to the Person to add.
 */
void addNewPerson(Persons *persons, Node **head, Person *person) {

    // If the list/sublist is empty, insert the person here
    if (*head == NULL) {
        *head = (Node*)malloc(sizeof(Node));

        // Check for memory allocation failure
        if (!head) {
            exitProgram(persons->head);
        }

        // Set the new node's data and next pointer
        (*head)->data = person;
        (*head)->next = NULL;
        return;
    }

    // Recursively call with the next node in the list
    addNewPerson(persons, &(*head)->next, person);
}

/**
 * @brief Prompts the user and reads an unsigned integer input.
 *
 * Prints a custom message, reads an unsigned integer from standard input,
 * and clears the leftover newline character from the input buffer.
 *
 * @param customString Custom prompt to display to the user.
 * @return The unsigned integer entered by the user.
 */
unsigned int getNumberInput(char *customString) {
    unsigned int userInput;

    // Print the custom prompt
    printf("%s\n", customString);

    // Read unsigned integer from the user
    scanf("%u", &userInput);

    // Clear leftover newline character from the input buffer
    scanf("%*c");

    return userInput;
}

/**
 * @brief Adds a new family head to the persons list.
 *
 * Prompts the user for a name and age, checks if a person with that
 * name already exists in the list, and if not, creates and adds a new
 * person as the family head (parent is NULL). Frees memory if the name
 * is already taken.
 *
 * @param persons Pointer to the Persons list.
 */
void addFamilyHead(Persons *persons) {
    // Prompt the user to enter name and age
    char *name = getInfiniteString("Enter a name:", persons);
    unsigned int age = getNumberInput("Enter age:");

    // Check if the person already exists in the list
    Person *personPtr = getPerson(persons->head, name);

    if (!personPtr) {
        // If name is unique, create a new person and add to the list
        Person *person = makePerson(persons, name, age, NULL);
        addNewPerson(persons, &persons->head, person);
        return;
    }

    // Print error and free allocated name string
    printf("The name is already taken\n");
    free(name);
}


/**
 * @brief Compares two strings using strcmp.
 *
 * @param name1 The first string.
 * @param name2 The second string.
 * @return Result of strcmp (0 if strings are equal, <0 or >0 otherwise).
 */
int compareNames(char *name1, char *name2) {
    return strcmp(name1, name2);
}


/**
 * @brief Checks if two persons are related through parent or spouse links.
 *
 * Recursively checks if p1 and p2 share a common parent, or if p1 is related
 * to p2's parent or spouse, and vice versa.
 *
 * @param p1 Pointer to the first person.
 * @param p2 Pointer to the second person.
 * @return 1 if the two persons are related, 0 otherwise.
 */
int isRelated(Person *p1, Person *p2) {

    int related = 0;

    // Exit condition: if either person or their parent is NULL
    if (p1 == NULL || p2 == NULL || p1->parent == NULL || p2->parent == NULL) {
        return 0;
    }
 
    // Check if the names of the parents are the same
    if (!compareNames(p1->parent->name, p2->parent->name)) {
        return 1;
    }

    // Recursively check relation through p2's parent
    related = isRelated(p1, p2->parent);

    if (related) {
        return 1;
    } else {
        // Check relation through p2's parent's spouse (marriedTo)
        related = isRelated(p1, p2->parent->marriedTo);
        if (related) {
            return 1;
        }
    }

    // Recursively check relation through p1's parent
    related = isRelated(p1->parent, p2);

    if (related) {
        return 1;
    } else {
        // Check relation through p1's parent's spouse (marriedTo)
        related = isRelated(p1->parent->marriedTo, p2);
    }
    
    return related;
}
/**
 * @brief Marries two persons by setting their marriedTo pointers.
 *
 * @param p1 Pointer to the first person.
 * @param p2 Pointer to the second person.
 */
void merryTwoPeps(Person *p1, Person *p2) {
    // Set married attribute for both persons
    p1->marriedTo = p2;
    p2->marriedTo = p1;

    // Print confirmation
    printf("%s and %s are now married\n", p1->name, p2->name);
}


/**
 * @brief Validates two persons before marriage.
 *
 * Checks if both persons exist, are old enough, are not related, and
 * are not already married.
 *
 * @param p1 Pointer to the first person.
 * @param p2 Pointer to the second person.
 * @return 1 if the marriage is valid, 0 otherwise.
 */
int validateMarriage(Person *p1, Person *p2) {
    const int MIN_AGE_TO_MARRY = 18;

    // Check if either person does not exist
    if (p1 == NULL || p2 == NULL) {
        printf("One of the persons does not exist\n");
        return 0;

    // Check age, relationship, and existing marriages
    } else if (p1->age < MIN_AGE_TO_MARRY || p2->age < MIN_AGE_TO_MARRY || 
               isRelated(p1, p2) || p1->marriedTo != NULL || p2->marriedTo != NULL) {
        printf("Invalid marriage\n");
        return 0;
    }

    // All checks passed
    return 1;
}


/**
 * @brief Prompts user for couple details and performs marriage if valid.
 *
 * Gets the names of the two persons wanting to marry, fetches their
 * corresponding Person structs, validates their marriage eligibility,
 * and marries them if valid.
 *
 * @param persons Pointer to the Persons list.
 */
void getDetailsToMarriage(Persons *persons) {
    // Prompt user for names
    char *p1Name = getInfiniteString("Enter the name of the first person:", persons);
    char *p2Name = getInfiniteString("Enter the name of the second person:", persons);

    // Fetch the actual persons from the list
    Person *p1 = getPerson(persons->head, p1Name);
    Person *p2 = getPerson(persons->head, p2Name);

    // Validate and marry if eligible
    if (validateMarriage(p1, p2)) {
        merryTwoPeps(p1, p2);
    }

    // Free dynamically allocated name strings
    free(p1Name);
    free(p2Name);
}


/**
 * @brief Frees a dynamically allocated array of strings.
 *
 * Iterates through each element of the array and frees the allocated string.
 *
 * @param string Array of dynamically allocated strings.
 * @param size Number of strings in the array.
 */
void freeStringsArray(char *string[], int size) {
    for (int i = 0; i < size; i++) {
        free(string[i]);
    }
}


/**
 * @brief Fetches a person by name and resets all visited flags in the tree.
 *
 * Wrapper around getPersonByName that also calls unVisitTree to reset
 * the visited flags after the search.
 *
 * @param head Pointer to the head of the list/tree.
 * @param name Name of the person to fetch.
 * @return Pointer to the Person struct if found; NULL otherwise.
 */
Person *getPerson(Node *head, char *name) {
    Person *p = getPersonByName(head, name);

    // Reset visited flags for future searches
    unVisitTree(head);

    return p;
}
/**
 * @brief Validates the parents and offspring details before adding a new child.
 *
 * Checks that both parents exist, are married to each other, and that the
 * child's name is not already taken.
 *
 * @param p1 Pointer to the first parent.
 * @param p2 Pointer to the second parent.
 * @param child Pointer to the child (NULL if the name is not taken).
 * @return 1 if the offspring details are valid, 0 otherwise.
 */
int validateOffSpringParents(Person *p1, Person *p2, Person *child) {
    // Check if either parent does not exist
    if (p1 == NULL || p2 == NULL) {
        printf("One of the parents does not exist\n");
        return 0;

    // Check if the parents are not married to each other
    } else if (!p1->marriedTo || strcmp(p1->marriedTo->name, p2->name)) {
        printf("The parents are not married\n");
        return 0;

    // Check if the child name is already taken
    } else if (child != NULL) {
        printf("The name is already taken\n");
        return 0;
    }

    return 1;
}


/**
 * @brief Adds a new offspring to a married couple.
 *
 * Prompts the user for the parents' names and the child's name, validates
 * the details, creates the new child, adds it to the parents' children list,
 * and ensures the married couple share the same children list.
 *
 * @param persons Pointer to the Persons list.
 */
void addOffSpring(Persons *persons) {
    char *strings[3];
    const int ARRAY_SIZE = 3;

    // Get names from user
    strings[0] = getInfiniteString("Enter the name of the first parent:", persons);
    strings[1] = getInfiniteString("Enter the name of the second parent:", persons);
    strings[2] = getInfiniteString("Enter offspring's name:", persons);

    // Check if persons list is NULL
    if (persons == NULL) {
        printf("One of the parents does not exist\n");
        return;
    }

    // Fetch parent and child pointers
    Person *p1 = getPerson(persons->head, strings[0]);
    Person *p2 = getPerson(persons->head, strings[1]);
    Person *child = getPerson(persons->head, strings[2]);

    // Validate offspring and parents
    if (!validateOffSpringParents(p1, p2, child)) {
        freeStringsArray(strings, ARRAY_SIZE);
        return;
    }

    // Create new child and add to parent's children list
    Person *newPerson = makePerson(persons, strings[2], 0, p1);
    addNewPerson(persons, &p1->children, newPerson);

    // Ensure both parents share the same children list
    p1->marriedTo->children = p1->children;

    // Free the first two allocated strings (parent names)
    freeStringsArray(strings, ARRAY_SIZE - 1);

    printf("%s was born\n", newPerson->name);
}


/**
 * @brief Prints the persons tree starting from a given person.
 *
 * Recursively traverses the tree, printing each person and their spouse
 * indented according to generation level (tabCounter). Uses the visited
 * flag to avoid printing the same person twice.
 *
 * @param head Pointer to the first node of the list/tree.
 * @param tabCounter Number of tabs for indentation to format hierarchy.
 */
void printPersons(Node *head, int tabCounter) {

    // Base case: if the node is NULL, return
    if (head == NULL) {
        return;
    }

    // Get the current person
    Person *person = (Person*)(head->data);

    // Skip if already printed
    if (person->visited) {
        return;
    }

    // Print tabs for formatting according to generation level
    for (int i = 0; i < tabCounter; i++) {
        printf("\t");
    }

    // Mark the person as visited
    person->visited = 1;

    // Print the person
    printf("%s (%d)", person->name, person->age);

    // If married, print spouse and mark as visited
    if (person->marriedTo) {
        printf(" - %s (%d)\n", person->marriedTo->name, person->marriedTo->age);
        person->marriedTo->visited = 1;
    } else {
        printf("\n");
    }

    // Recursively print children with increased indentation
    printPersons(person->children, tabCounter + 1);

    // Recursively print next sibling at same indentation
    printPersons(head->next, tabCounter);
}
/**
 * @brief Prints the family tree starting from a given person.
 *
 * Prompts the user for a person's name, temporarily adjusts the list
 * to point to that person as the head, prints the family tree using
 * printPersons, and then restores the original list structure.
 *
 * @param persons Pointer to the Persons list.
 */
void printFamily(Persons *persons) {

    const int TAB_COUNTER = 0;

    // Prompt for the name of the person to print
    char *name = getInfiniteString("Enter the name of the person:", persons);

    // Fetch the person from the list
    Person *p = getPerson(persons->head, name);

    // If the person does not exist, print error
    if (!p) {
        printf("The person does not exist\n");
        free(name);
        return;
    }

    // Temporarily store current head data and next pointer
    Node *tempHeadData = persons->head->data;
    Node *tempNext = persons->head->next;

    // Set head to point to the selected person
    persons->head->data = p;
    persons->head->next = NULL;

    // Print the person's family tree and reset visited flags
    printPersons(persons->head, TAB_COUNTER);
    unVisitTree(persons->head);

    // Restore original list structure
    persons->head->data = tempHeadData;
    persons->head->next = tempNext;

    // Free the dynamically allocated name string
    free(name);
}


/**
 * @brief Increments the age of every person in the tree by a given amount.
 *
 * Traverses the tree recursively, incrementing each person's age once
 * using the visited flag to avoid duplicates.
 *
 * @param head Pointer to the head of the list/tree.
 * @param amount Number of years to increment.
 */
void incrementAge(Node *head, unsigned int amount) {

    if (head == NULL) {
        return;
    }

    Person *p = head->data;

    // Skip already visited persons
    if (p->visited) {
        return;
    }

    // Increment age and mark as visited
    p->age += amount;
    p->visited = 1;

    // Recursively increment children and next sibling
    incrementAge(p->children, amount);
    incrementAge(head->next, amount);
}


/**
 * @brief Prompts the user to enter a number of years and increments all persons' ages.
 *
 * Calls incrementAge on the head of the list and resets visited flags afterward.
 *
 * @param head Pointer to the head of the persons list/tree.
 */
void getIncrementNumber(Node *head) {

    unsigned int amount;

    // Prompt the user for number of years
    printf("Enter number of years:\n");
    scanf("%u", &amount);
    scanf("%*c");

    // Increment ages and reset visited flags
    incrementAge(head, amount);
    unVisitTree(head);
}


/**
 * @brief Adds a name pointer to a dynamically allocated array of string pointers.
 *
 * Reallocates the array to accommodate the new element and increments the array size.
 * Exits the program if memory allocation fails.
 *
 * @param name Pointer to the name to add.
 * @param arr Pointer to the array of string pointers.
 * @param sizeArr Pointer to the current size of the array.
 * @param persons Pointer to the persons list (used for freeing memory on exit).
 */
void addNameToArray(char *name, char ***arr, unsigned int *sizeArr, Node *persons) {

    char ***tempArr = arr;

    // Reallocate array to add space for the new name
    (*arr) = (char**)realloc(*arr, (*sizeArr + 1) * sizeof(char*));

    // Check for allocation failure
    if (!*arr) {
        free(tempArr);
        exitProgram(persons);
    }

    // Add the new name to the array and increment size
    (*arr)[*sizeArr] = name;
    (*sizeArr)++;
}
/**
 * @brief Adds names of persons at a specific degree level to a dynamic array.
 *
 * Traverses the tree starting from a given parent node and recursively goes down
 * the tree until the specified degree is reached. Adds all persons at that degree
 * level to the array of names.
 *
 * @param person Pointer to the current node in the tree.
 * @param deg The target degree level to reach.
 * @param currDeg The current depth during recursion.
 * @param sizeArr Pointer to the size of the array (incremented as names are added).
 * @param arr Pointer to the array of names.
 * @param persons Pointer to the persons list (used for memory management in addNameToArray).
 */
void addPersonsToArray(Node *person, unsigned int deg, unsigned int currDeg,
                       unsigned int *sizeArr, char ***arr, Node *persons) {
    
    // Base case: no person at this node
    if (person == NULL) {
        return;
    }

    // Stop recursion if we exceeded the target degree
    if (currDeg > deg) {
        return;
    }

    // Get the current person
    Person *p = person->data;

    // If the current degree matches the target, add the person's name
    if (deg == currDeg) {
        addNameToArray(p->name, arr, sizeArr, persons);
    } else {
        // Recursively explore children to reach the target degree
        addPersonsToArray(p->children, deg, currDeg + 1, sizeArr, arr, persons);
    }

    // Explore the next sibling at the same level
    addPersonsToArray(person->next, deg, currDeg, sizeArr, arr, persons);
}


/**
 * @brief Finds a common ancestor at a specified degree and collects descendants' names.
 *
 * Recursively traverses upward in the family tree from the start person,
 * moving toward left and right parents until the specified degree is reached.
 * Then calls addPersonsToArray to collect names of descendants at the given degree.
 *
 * @param person Pointer to the starting person node.
 * @param arr Pointer to the array to fill with names of relatives.
 * @param deg The target degree distance from the starting node.
 * @param currDeg The current recursion depth (used internally).
 * @param sizeArr Pointer to the size of the array (incremented as names are added).
 * @param persons Pointer to the persons list (used for memory management).
 */
void findCommonParent(Person *person, char ***arr, unsigned int deg,
                      unsigned int currDeg, unsigned int *sizeArr, Node *persons) {

    // If current depth matches the target degree, collect descendant names
    if (currDeg == deg) {
        int addDeg = deg - 1;  // Adjust degree for child traversal
        addPersonsToArray(person->children, addDeg, INIT_DEG, sizeArr, arr, persons);
        return;
    }

    // Recursively traverse up to the parent and parent's spouse
    if (person->parent) {
        findCommonParent(person->parent, arr, deg, currDeg + 1, sizeArr, persons);
        findCommonParent(person->parent->marriedTo, arr, deg, currDeg + 1, sizeArr, persons);
    } else {
        // If the degree is too large and parent is NULL, adjust degree and stay at current node
        findCommonParent(person, arr, deg - 1, currDeg, sizeArr, persons);
    }
}


/**
 * @brief Prints a sorted array of names, skipping duplicates.
 *
 * Iterates through the array of names, printing each one, and avoids printing
 * consecutive duplicate names.
 *
 * @param arr Array of string pointers to print.
 * @param size Number of elements in the array.
 */
void printArr(char **arr, unsigned int size) {

    // Print first element if array is not empty
    if (size) {
        printf("%s\n", arr[0]);
    }

    // Print remaining elements while skipping consecutive duplicates
    for (unsigned int i = 1; i < size; i++) {
        if (!strcmp(arr[i], arr[i - 1])) {
            continue;
        }
        printf("%s\n", arr[i]);
    }
}
/**
 * @brief Swaps two elements in a string array.
 *
 * @param arr Array of string pointers.
 * @param i Index of the first element.
 * @param j Index of the second element.
 */
void swap(char **arr, int i, int j) {
    char *temp = *(arr + i);
    *(arr + i) = *(arr + j);
    *(arr + j) = temp;
}


/**
 * @brief Sorts an array of strings using bubble sort.
 *
 * Compares strings using compareNames function and stops early if the array
 * is already sorted.
 *
 * @param arr Array of string pointers to sort.
 * @param size Number of elements in the array.
 */
void bubbleSortArr(char **arr, unsigned int size) {
    
    int isSwapped = 0; // Flag to check if any swaps occurred

    for (unsigned int i = 0; i < size - 1; i++) {
        for (unsigned int j = 0; j < size - 1 - i; j++) {
            // Compare adjacent names
            if (compareNames(arr[j], arr[j + 1]) > 0) {
                swap(arr, j, j + 1); // Swap if out of order
                isSwapped = 1;
            }
        }
        // If no swaps occurred, the array is sorted
        if (!isSwapped) {
            return;
        }
        isSwapped = 0; // Reset for the next pass
    }
}


/**
 * @brief Retrieves and prints all relatives at a given degree from a person.
 *
 * Prompts the user for a name and degree, finds the person, allocates an array
 * of names, calls findCommonParent to populate the array, sorts it, and prints it.
 *
 * @param persons Pointer to the persons list.
 */
void getRelativeFromDegree(Persons *persons) {

    // Prompt for name and degree
    char *name = getInfiniteString("Enter the name of the person:", persons);
    unsigned int degree = getNumberInput("Enter degree:");

    // Fetch the person
    Person *p = getPerson(persons->head, name);

    if (!p) {
        printf("The person does not exist\n");
        free(name);
        return;
    }

    printf("Cousins:\n");

    if (degree == 0) {
        printf("%s\n", p->name);
    } else {
        unsigned int size = 0;
        char **namesArr = (char **)malloc(sizeof(char*)); // Allocate array for names

        // Populate the names array
        findCommonParent(p, &namesArr, degree, INIT_DEG, &size, persons->head);

        // Sort and print the array if names were found
        if (*namesArr) {
            bubbleSortArr(namesArr, size);
            printArr(namesArr, size);
        } else {
            printf("%s\n", p->name);
        }

        free(namesArr); // Free the array
    }

    free(name); // Free the name string
}


/**
 * @brief Recursively counts the number of persons in the list/tree.
 *
 * Uses visited flag to avoid counting the same person multiple times.
 * Also counts married partners once.
 *
 * @param head Pointer to the head node of the list/tree.
 * @param count Pointer to the counter variable to increment.
 */
void getNumberOfNodes(Node *head, unsigned int *count) {

    if (head == NULL) {
        return;
    }

    Person *p = head->data;

    if (!p->visited) {
        (*count)++;
    }

    p->visited = 1; // Mark as visited

    // Count spouse if exists and not visited
    if (p->marriedTo) {
        if (!p->marriedTo->visited) {
            (*count)++;
        }
        p->marriedTo->visited = 1;
    }

    // Recursively count children and next siblings
    getNumberOfNodes(p->children, count);
    getNumberOfNodes(head->next, count);
}


/**
 * @brief Prints the total number of persons in the list/tree.
 *
 * Initializes a counter, calls getNumberOfNodes, resets visited flags,
 * and prints the count with proper grammar.
 *
 * @param head Pointer to the head of the list/tree.
 */
void getPersonsCount(Node *head) {

    unsigned int count = 0;

    getNumberOfNodes(head, &count);
    unVisitTree(head); // Reset visited flags

    if (count == 1) {
        printf("There is one person\n");
    } else {
        printf("There are %u people\n", count);
    }
}

/**
 * @brief Handles the user's menu choice and executes the corresponding action.
 *
 * Receives a character input from the user, checks it against possible menu options,
 * and calls the appropriate function from the persons list. Exits the program
 * if the choice is '0'.
 *
 * @param choice Character representing the user's menu selection.
 * @param persons Pointer to the persons list.
 * @return 0 if the user wants to exit, 1 otherwise to continue the main loop.
 */
int handleUserChoice(char choice, Persons *persons) {

    // Evaluate the user choice
    switch (choice) {
        case '0': // Exit the program
            exitProgram(persons->head);
            break;
        case '1': // Add a new family head
            addFamilyHead(persons);
            break;
        case '2': // Marry two people
            getDetailsToMarriage(persons);
            break;
        case '3': // Add a new offspring
            addOffSpring(persons);
            break;
        case '4': // Print family top-down
            printFamily(persons);
            break;
        case '5': // Increment years for everyone
            getIncrementNumber(persons->head);
            break;
        case '6': // Count total people
            getPersonsCount(persons->head);
            break;
        case '7': // Print relatives at a given degree
            getRelativeFromDegree(persons);
            break;
        default: // Handle invalid input
            printf("Invalid option\n");
    }
    
    // Continue the main loop
    return 1;
}


/**
 * @brief Prints the program menu for user interaction.
 *
 * Lists all available options with corresponding numbers for user selection.
 */
void printMenu() {
    printf("Choose an option:\n\
0. Exit\n\
1. Add a head\n\
2. Marry two people\n\
3. New offspring\n\
4. Print family top-down\n\
5. Years pass\n\
6. Count people\n\
7. Print cousins\n");
}
