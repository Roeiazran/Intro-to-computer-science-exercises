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