#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE 10
#define ZERO_ACCOUNT 0

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct Account {
    unsigned int accountNumber;
    char *accountHolder;
    int balance;
} Account;

typedef struct Transaction {
    unsigned int fromAccount;
    unsigned int toAccount;
    int amount;
} Transaction;

typedef struct Bank {
    Node *accounts;
    Node *transactions;
} Bank;


void printMenu(void);
int handleUserChoice(char choice, Bank ** bank);
void printAccounts(Node * account);
void freeBank(Bank ** bank);
void printTransactions(Node * transaction);
void reverseList(Node ** list);
Account * makeAccount(unsigned int accountNumber, char * holderName, Bank ** bank);
Transaction * makeTransaction(int amount, unsigned int from, unsigned int to, Bank ** bank);
void addNewAccount(Node ** accounts, Account * newAccount, Bank ** bank);
void addNewTransaction(Bank ** bank, Node ** transactions,  Transaction * transaction);
void freeListOfTransaction(Node * transactions);