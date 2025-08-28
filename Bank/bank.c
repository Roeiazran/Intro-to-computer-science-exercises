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

/**
 @brief Initializes the bank and runs the main program loop.
 Displays the menu, scans user input, and processes choices.
 @return 0 on successful execution.
 */
int main() {
    char choice;
    
    // initialize a bank instance
    Bank * bank = (Bank*)malloc(sizeof(Bank));

    // check for failed allocation
    if (bank == NULL) {
        freeBank(&bank);
    }

    // loop through, scanning the user choice for the menu
    while (1) {
        
        printMenu();
        scanf("%c", &choice);
        scanf("%*c");

        handleUserChoice(choice, &bank); // call the user choice handler
    }

    return 0;
}

/**
 @brief Prompt the user for his account number
 @return account number entered by the user
 */
unsigned int getAccountNumberInput(void) {

    unsigned int accountNumber;
    printf("Enter account number:\n"); // prompt the user to enter his account number
    scanf("%u", &accountNumber); // scan the user input

    scanf("%*c"); // clear the buffer from trailing '\n'
    return accountNumber;
}

/**
 @brief Prints "Account not found"
 */
void printAccountNotFound(void) {
    printf("Account not found\n");
}

/**
 @brief Frees all allocated memory used by the bank.
 Iterates over accounts and transactions, releasing their memory.
 @param bank Pointer to the bank structure.
 */
void freeBank(Bank ** bank) {

    Node *temp; // temporary pointer to avoid loosing access when freeing nodes
    
    freeListOfTransaction((*bank)->transactions); // call to deallocate the transactions list

    for (Node * account = (*bank)->accounts; account != NULL;) {
       
        free(((Account *)account->data)->accountHolder);  // free holder name
        free(account->data); // free the data
        temp = account->next;  // save the next node before freeing the current one
        free(account); // free the account
        account = temp; // advance account to the next account
    }

    free(*bank); // free the global bank instance
    exit(1);
}

/**
 @brief Gets account by number
 @param accounts bank account list
 @param accountNumber users account number
 @return account on successes and nullptr on failure
 */
Node * getAccountByNumber(Node * accountsHead, unsigned int accountNumber) {
    
    while (accountsHead != NULL) {

        // search the account in the system 
        if (((Account*)accountsHead->data)->accountNumber == accountNumber) {
            return accountsHead;
        }

        accountsHead = accountsHead->next;
    }

    return NULL;
}

/**
 @brief Reads an arbitrarily long string from user input.
 Dynamically allocates and resizes memory as needed.
 @param customString Message to display when prompting the user.
 @param bank Pointer to the bank (freed on allocation failure).
 @return Dynamically allocated string containing user input.
 */
char *getInfiniteString(char * customString, Bank ** bank) {
    
    char scannedChar;
    printf("%s\n", customString);
    scanf("%c", &scannedChar);

    int index = 0; // 
    char *str = (char *)malloc(2 * sizeof(char)), *temp; // allocate memory for the new string
    
    // free the bank on failed allocation
    if (str == NULL) {
        freeBank(bank);
    }

    while (scannedChar != '\n') {

        str[index] = scannedChar; // update the string
        index++;

        temp = (char*)realloc(str, sizeof(char) * (index + 2)); // reallocate the memory for the next input
        
        // free allocated memory on allocation failure
        if (temp == NULL) {
            free(str);
            freeBank(bank);
        }

        str = temp; // update allocated string
        scanf("%c", &scannedChar); // get the next user input
    }

    str[index] = '\0'; // set end of string
    return str;
}

/**
 @brief Adds a new account to the bank.
 Allocates a new node and inserts it at the head of the accounts list.
 @param accounts Pointer to the head of the accounts list.
 @param newAccount Account structure to add.
 @param bank Pointer to the bank (freed on allocation failure).
 */
void addNewAccount(Node ** accounts, Account * newAccount, Bank ** bank) {
    
    Node * account = (Node*)malloc(sizeof(Node)); // allocate memory
    
    //free and exit the program on allocation failure
    if (account == NULL) {
        freeBank(bank);
    }
    
    account->data = (Account*)newAccount; // update data to point to new account

    // make the newly created account the head of accounts list
    account->next = *accounts;
    *accounts = account;
}

/**
 @brief Creates a new account structure with the given details.
 @param accountNumber New account number.
 @param holderName Name of the account holder.
 @param bank Pointer to the bank (freed on allocation failure).
 @return Pointer to the newly allocated Account structure.
 */
Account * makeAccount(unsigned int accountNumber, char * holderName, Bank ** bank) {

    //allocate memory for the new account
    Account * account = (Account*)malloc(sizeof(Account));
    
    //free the bank if allocation failed
    if (account == NULL) {
        freeBank(bank);
    }
    
    // fill the account with data from input
    account->accountNumber = accountNumber;
    account->accountHolder = holderName;
    account->balance = 0;
    return account;
}

/**
 @brief Get the details (account number and the holders name) from the user and creates new account for him
 Checks that account number does not exists in the system.
 @param bank bank
 */
void getAccountDetails(Bank ** bank) {

    unsigned int accountNumber = getAccountNumberInput(); // get the account number from the user
    
    //check if account if the account already exists
    if (accountNumber == ZERO_ACCOUNT || getAccountByNumber((*bank)->accounts, accountNumber)) {
        printf("Account number already exists\n");
        return;
    }

    char * holderName = getInfiniteString("Enter account holder:", bank);  // get from the user its name

    Account * account = makeAccount(accountNumber, holderName, bank); // make the account

    addNewAccount(&((*bank)->accounts), account, bank); // add the account to the accounts list
    printf("Account created successfully\n");
}

/**
 @brief Deallocate an account node memory
 @param account Pointer to the node needs to be freed
 */
void freeSingleAccount(Node ** account) {
    free(((Account*)(*account)->data)->accountHolder);
    free((Account*)(*account)->data);
    free(*account);
}

/**
 @brief Deletes an account from the bank. (use of previous pointer will simplify this code)
 Prompts the user for an account number and removes it if found, and deletes the node from the list.
 @param accountsHead Pointer to the head of the accounts list.
 */
void deleteAccount(Node ** accountsHead) {
    unsigned int userAccNum = getAccountNumberInput(); // get the account number from the user
    Node * curr = *accountsHead, *head = *accountsHead, *temp;
  
    while (curr != NULL) {

        // account was found in the head of accounts list
        if (((Account*)curr->data)->accountNumber == userAccNum) {
            
            temp = curr->next; // save the next node before deallocation
            freeSingleAccount(&curr); // free the node
            *accountsHead = temp; // reposition the head of the list

            printf("Account deleted successfully\n");
            return;
        }

        // otherwise, adjust pointers to bypass the deleted node
        if (((Account*)curr->next) != NULL && ((Account*)curr->next->data)->accountNumber == userAccNum) {

            temp = curr->next; // save the next node before deallocation
            curr->next = curr->next->next; // relink the current node next
            freeSingleAccount(&temp); // free the account
            *accountsHead = head; // bring the pointer back to the beginning of the list
    
            printf("Account deleted successfully\n");
            return;
        }

        curr = curr->next; // advance curr
        (*accountsHead) = (*accountsHead)->next; // advance 
    }

    *accountsHead = head;
    printAccountNotFound();
}

/**
 @brief Updates the account holder’s name.
 Validates the account exists, then prompts for and assigns a new name.
 @param bank Pointer to the bank.
 */
void updateAccount(Bank ** bank) {

    unsigned int accountNumber = getAccountNumberInput(); // get the account number from the user
    Node * accounts = (*bank)->accounts, * account = getAccountByNumber(accounts, accountNumber);

    if (account != NULL) {

        // get the new account holder name from the user
        char * holderName = getInfiniteString("Enter account holder:", bank);
        free(((Account*)account->data)->accountHolder); // deallocate old name
        ((Account*)account->data)->accountHolder = holderName; // assign the new name
        return;
    }
    printAccountNotFound();
}

/**
 @brief function to make a single transaction
 the function get the transaction detalis and allocating new transaction
 @param amount transaction
 @param from transaction
 @param to transaction
 @param bank the main bank
 @return single Transaction sturcture
 */
Transaction * makeTransaction(int amount, unsigned int from, unsigned int to, Bank ** bank) {
    Transaction * transaction = (Transaction*)malloc(sizeof(Transaction));
    //check for allocation faliure and frees the bank
    if (transaction == NULL) {
        freeBank(bank);
    }
    //assing transaction details and return it
    transaction->amount = amount;
    transaction->fromAccount = from;
    transaction->toAccount = to;
    return transaction;
}
/**
 @brief function to add new transaction to the end of transactions list
 the function recursively advance transactions head to the end and add new transaction to it
 @param bank the main bank
 @param transactions of the bank
 @param transaction structure to add to transactions
 */
void addNewTransaction(Bank ** bank, Node ** transactions,  Transaction * transaction) {
    
    //exit condition
    if (*transactions == NULL) {
        //allocate memory for a new transaction
        Node * temp = (Node*)malloc(sizeof(Node));
        
        //check if allocation failed
        if (temp == NULL) {
            //free all memory
            freeBank(bank);
        }
        
        //initialize data to the new transaction address that was send to the function
        temp->data = (Transaction*)transaction;
        
        //point last transaction to NULL
        temp->next = NULL;
        *transactions = temp;
        return;
    }
    //call itself next
    addNewTransaction(bank, &(*transactions)->next , transaction);
}

/**
 @brief function to check strtol output
 the function will get strtol endptr and validate the amount
 @param amount inserted by the user
 @param endptr strtol endptr
 @return 1 if the amount is 0 or positive and its a valid number
 */
int isValidInformation(int amount, char * endptr) {
    //check if endptr point to char or the amount if negative and return
    if (*endptr != '\0' || amount <= 0) {
        return 0;
    }
    return 1;
}

/**
 @brief function to handle user withdraw actions
 the function will validate the user inserted data and withdraw the money from its account
 @param userAccount the user account
 @param amount the user inserted amount
 @param endptr strtol endptr to validate the amount
 @return 1 if the money withdraned  successfully and 0 otherwise
 */
int handleWithdraw(Node * userAccount, int amount, char * endptr) {
    //validate the user amount and print error
    if (!isValidInformation(amount, endptr)) {
        printf("Invalid amount");
        return 0;
    }
    //check for user balance
    if (((Account*)userAccount->data)->balance < amount) {
        printf("Not enough money\n");
        return 0;
    }
    //updating user account balace and return
    ((Account*)userAccount->data)->balance -= amount;
    return 1;
}

/**
 @brief function to handle deposit action from the user
 the function will validate the user information and deposit money to its account
 @param userAccount the user account
 @param amount transaction
 @param endptr the endptr of strtol to validate the amount
 @return 1 if operation succeeded 0 otherwise
 */
int handleDeposit(Node * userAccount, int amount, char * endptr) {
    //checks for input validity and prints
    if (!isValidInformation(amount, endptr)) {
        printf("Invalid amount\n");
        return 0;
    }
    //updating the user account and returns success
    ((Account*)userAccount->data)->balance += amount;
    return 1;
}
/**
 @brief function to add transaction to a gievn transactions list
 the function works as a middleman making adding transaction to the list more approachable
 @param from  transcation
 @param to transactions
 @param amount transaction
 @param bank the main bank
 @param transactions list of transactions
 */
void addTransactionToList(unsigned int from, unsigned int to, int amount, Bank ** bank,\
                          Node ** transactions) {
    //makes single transaction and adding it to transactions
    Transaction * transaction = makeTransaction(amount, from, to, bank);
    addNewTransaction(bank, transactions, transaction);
}

/**
 @brief function to make deposit or withdraw
 the function will handle each case calling the appropriate function to deal with the action
 @param bank bank on the main
 */
void withdrawOrDeposit(Bank ** bank) {
    //init consts
    const char withdraw[] = "withdraw";
    const char deposit[] = "deposit";
    
    //gets the account number from user and the user account
    unsigned int accountNumber = getAccountNumberInput();
    Node * userAccount = getAccountByNumber((*bank)->accounts, accountNumber);
    
    //if no user was found print
    if (userAccount == NULL) {
        printAccountNotFound();
        return;
    }
    
    //gets the user choice
    char *str = getInfiniteString("Would you like to deposit or withdraw money?", bank);
    //init transactions detalis variables
    int amount;
    char * amountStr;
    char * endptr;
    //checks for the users different choices
    if (!strcmp(str, withdraw)) {
        //get the amount and run strtol on it
        amountStr = getInfiniteString("How much money would you like to withdraw?", bank);
        amount = (int)strtol(amountStr, &endptr, BASE);
        
        //handles withdraw and recording the transaction if details are currect
        if (handleWithdraw(userAccount, amount, endptr)) {
            //make transaction from user account to account number 0 and print
            addTransactionToList(accountNumber, ZERO_ACCOUNT, amount, bank, &(*bank)->transactions);
            printf("Money withdrawn successfully; your new balance is %d\n",\
                   ((Account*)userAccount->data)->balance);
        }
        //free allocated memory
        free(amountStr);
        free(str);
        return;
        //check for deposit
    } else if (!strcmp(str, deposit)) {
        //get the amount from the user and run strtol on it
        amountStr = getInfiniteString("How much money would you like to deposit?", bank);
        amount = (int)strtol(amountStr, &endptr, BASE);
        
        //handles deposit and recording the transaction if details are currect
        if (handleDeposit(userAccount, amount, endptr)) {
            //make transaction from account number 0 to user account and print
            addTransactionToList(ZERO_ACCOUNT, accountNumber, amount, bank, &(*bank)->transactions);
            printf("Money deposited successfully; your new balance is %d\n",\
                   ((Account*)userAccount->data)->balance);
        }
        //free allocated memory
        free(amountStr);
        free(str);
        return;
    }
    
    //handles invalid transactions by free allocatoed memory and printing
    free(str);
    printf("Invalid action\n");
    return;
}

/**
 @brief function to get make transactions list from given transactions string
 @param instructionsString  the instructions string
 @param bank the main bank
 @return pointer to the head of the new list
 */
Node * getTransactionsFromString(char * instructionsString, Bank ** bank) {
    
    //initialize transcations variable
    Node * transactions = NULL;
    //initialize tok and gets the first of the string
    char * tok = strtok(instructionsString, "-");
    //initialize endptr to strtol
    char * endptr;
    while (tok != NULL) {
        //gets the from part and run strtol on it
        unsigned int from = (unsigned int) strtol(tok, &endptr, BASE);
        //get the next part
        tok = strtok(0, ":");
        //validate the part and check for unexpected end of string
        if (!isValidInformation(from, endptr) || tok == NULL)  {
            //frees transcations
            free(transactions);
            return NULL;
        }
        //gets the from part and run strtol on it
        unsigned int to = (unsigned int) strtol(tok, &endptr, BASE);
        
        //get the next part
        tok = strtok(0, ",");
        
        //validate the part and check for unexpected end of string
        if (!isValidInformation(to, endptr) || tok == NULL)  {
            free(transactions);
            return NULL;
        }
        
        //gets the from part and run strtol on it
        int amount = (int) strtol(tok, &endptr, BASE);
        
        //skip all commas and jump next
        tok = strtok(0, "[^,]-");
        //validate the part
        if (!isValidInformation(amount, endptr)) {
            free(transactions);
            return NULL;
        }
        
        //edge case
        if (to == from) {
            free(transactions);
            return NULL;
        }
    
        //adds the transaction
        addTransactionToList(from, to, amount, bank, &transactions);
        
        //check for end of string
        if (tok == NULL) {
            return transactions;
        }
    }
    
    //free transactions if the string wasn't valid
    free(transactions);
    return NULL;
}

/**
 @brief function to check if a given character is a digit
 @param ch char to check
 @return 1 if its a digit 0 otherwise
 */
int isPositivedigit(char ch) {
    return ch <= '9' && ch >= '0';
}


/**
 @brief function to stil all positive digits from string
 the function will advance the pointer to the string
 @param str address of the string to advance
 */
void skipDigits(char ** str) {
    //advance string all while ite value is a positive number
    while (isPositivedigit(**str)) {
        (*str)++;
    }
}

/**
 @brief function to validate transactions string
 the function will recursively validate if the string in formated as from-to:amount with no characters that isn't digit
 @param str the string to validate
 @return 1 if string is valid 0 otherwise
 */
int validateTransactionString(char * str){
    //checks for non digit in the begining
    if (!isPositivedigit(*str)) {
        return 0;
    }
    //skips all digits
    skipDigits(&str);
    //checks for the cheracter '-'
    if (!*str || *str != '-') {
        return 0;
    }
    //increment
    str++;
    //skips all digits
    skipDigits(&str);
    //checks for the cheracter ':'
    if (!*str || *str != ':') {
        return 0;
    }
    //increment
    str++;
    //skips all digits
    skipDigits(&str);
    //checks for end of string
    if (!*str) {
        return 1;
    }
    //check for multiple transactions
    if (*str != ',') {
        return 0;
    }
    //if multiple transactions skip the commas
    while (*str == ',') {
        str++;
        //if no transaction after the comma return error
        if (!*str) {
            return 0;
        }
    }
    //if end of string return
    if (!*str) {
        return 1;
    }
    //calls
    return validateTransactionString(str);
}

/**
 @brief function to execute all transactions from given string
 the function will run recursivly executing every commend and on faliure reveser the made transactions
 @param accounts bank accounts list
 @param transactios transactions list to execute
 @return 1 if execution completed 0 otherwise
 */
int executeTransferInstructions(Node * accounts, Node * transactios) {

    //checks for exit condition - no transactions
    if (transactios == NULL) {
        return 1;
    }
    //init transcations details
    unsigned int from = ((Transaction*)transactios->data)->fromAccount;
    unsigned int to = ((Transaction*)transactios->data)->toAccount;
    int amount = ((Transaction*)transactios->data)->amount;
    //init the accounts invloved in the transcation
    Node * fromAccount = getAccountByNumber(accounts, from);
    Node * toAccount = getAccountByNumber(accounts, to);
    //checks for account that does not exists
    if (fromAccount == NULL || toAccount == NULL) {
        return 0;
    }
    //check for balance less than transaction amount
    if (((Account*)fromAccount->data)->balance < amount) {
        return 0;
    }
    //update the account
    ((Account*)fromAccount->data)->balance -= amount;
    ((Account*)toAccount->data)->balance += amount;
    
    //calls the function with next
    int opSucceed = executeTransferInstructions(accounts, transactios->next);

    //checks for operations faliure and undo every operation made so far
    if (!opSucceed) {
        ((Account*)toAccount->data)->balance -= amount;
        ((Account*)fromAccount->data)->balance += amount;
        return 0;
    }
    return 1;
}

/**
 @brief function to concate to lists
 @param list1 the list to cancate to
 @param list2 the list to concate
 */
void concateTwoList(Node ** list1, Node ** list2) {

    //store the head of the list to return to it later
    Node * head = *list1;

    //if the list is empty assign it and return
    if ((*list1) == NULL) {
        (*list1) = (*list2);
        return;
    }

    //advane list1 to its end
    while ((*list1)->next != NULL) {
        (*list1) = (*list1)->next;
    }
    //assing its next to point to list2
    (*list1)->next = *list2;
    //restore its head
    (*list1) = head;
}


/**
 @brief function to free list of transactions
 the function will free the date from the transaction and also free the transactions list
 @param transactions to be freed
 */
void freeListOfTransaction(Node * transactions) {
    
    Node * temp;
    //runs for every transaction
    for (Node * transaction = transactions; transaction != NULL;) {
        //free the data
        free(transaction->data);
        //save transaction next to not lost access to it
        temp = transaction->next;
        //free the transaction itself
        free(transaction);
        //point to next transaction
        transaction = temp;
    }
}


/**
 @brief function to exec multyple transactions
 the function stored seperate transactions list and execute them and conact them to the bank
 @param bank the bank on the main
 @param insructionsString the instructions inserted by the user
 */
void makeInstructionsList(Bank ** bank, char * insructionsString) {
    //initialize transactions from the string
    Node * transcations = getTransactionsFromString(insructionsString, bank);
    
    //intializee operation flag to print invalid instructions
    int operationResult = 0;
    //runs on every transaction
    if (transcations != NULL) {
        //tryies execute the transactions and store the result of the opeartiion
        operationResult = executeTransferInstructions(((*bank)->accounts), transcations);
        //checks for operation faliure and concate and print on success
        if (operationResult) {
            concateTwoList(&((*bank)->transactions), &transcations);
            printf("Instructions executed successfully\n");
        }
    }
    //free and prints if the operation falied
    if (!operationResult) {
        freeListOfTransaction(transcations);
        printf("Invalid instructions\n");
        return;
    }
    
}
/**
 @brief function to get instruction string frmo the user validate it partially and making the transactions
 @param bank bank on the main
 */
void getInstructionsString(Bank ** bank) {
    //initialize the instruction string from the user
    char * str = getInfiniteString("Enter instructions:", bank);
    
    //validating the string for its structure i.e from-to:amount
    if (!validateTransactionString(str)) {
        //frees str and prints error
        free(str);
        printf("Invalid instructions\n");
        return;
    }
    //makes the transaction and frees str
    makeInstructionsList(bank, str);
    free(str);
}


/**
 @brief function to print the user transactions
 @param transactions the transactions made on the bank
 @param accountNum user account number
 */
void printUserTransactions(Node * transactions, unsigned int accountNum) {
    //initialize
    unsigned int from;
    unsigned int to;
    int amount;
    //initialize flag to print "no transactions"
    int hasTransactions = 0;
    //runs on ecery transaction on bank
    while (transactions != NULL) {
        //assign the variable with the transaction detalis
        from = ((Transaction *)transactions->data)->fromAccount;
        to = ((Transaction *)transactions->data)->toAccount;
        amount = ((Transaction *)transactions->data)->amount;
        
        //checks if the user account wes part of the transaction
        if (from == accountNum || to == accountNum) {
            //prints transcation line once
            if (transactions && !hasTransactions) {
                printf("Transactions:\n");
                hasTransactions = 1;
            }
            //check for the nature of the transactions, from 0 - deposit
            if (from == ZERO_ACCOUNT) {
                printf("Deposited %d\n", amount);
                //to 0 - withdraw
            } else if (to == ZERO_ACCOUNT) {
                printf("Withdrew %d\n", amount);
                //deposited to the account from another account
            } else if (to == accountNum){
                printf("%d from %u\n",amount, from);
                //user deposited to another account
            } else {
                printf("%d to %u\n", amount, to);
            }
        }
        //advancing transactions to next transaction
        transactions = transactions->next;
    }
    //check for no transaction for the user and prints
    if (!hasTransactions) {
        printf("No transactions\n");
    }
}

/**
 @brief function that prints one account detalis
 the function will get the account number from user and print its detalis
 @param bank bank stores in the maim
 */
void viewAccount(Bank ** bank) {
    //get the user account number and fetch the user account
    unsigned int accountNum = getAccountNumberInput();
    Node * account = getAccountByNumber((*bank)->accounts, accountNum);
    
    //check for account not found and prints error msg
    if (account == NULL) {
        printAccountNotFound();
        return;
    }
    
    //initialize account number for better visibility and print the user account details
    unsigned int accountNumber = ((Account*)account->data)->accountNumber;
    printf("Account #%u (%s)\nBalance: %d\n", accountNumber,\
           ((Account*)account->data)->accountHolder, ((Account*)account->data)->balance);
   
    //initialize transaction from the bank and calls prints it
    Node * transactions = (*bank)->transactions;
    printUserTransactions(transactions, accountNumber);
}

/**
 @brief function to handle the user input for the menu choice
 @param choice the user entered choice
 @param bank  bank stores in the main
 @return 0 in case the player want to exit 1 if not
 */
int handleUserChoice(char choice, Bank ** bank) {

    //check for every possible choice case
    switch (choice) {
        case '0':
            freeBank(bank);
            break;
        case '1':
            getAccountDetails(bank);
            break;
        case '2':
            deleteAccount(&((*bank)->accounts));
            break;
        case '3':
            updateAccount(bank);
            break;
        case '4':
            withdrawOrDeposit(bank);
            break;
        case '5':
            getInstructionsString(bank);
            break;
        case '6':
            viewAccount(bank);
            break;
        default:
            printf("Invalid option\n");
    }
    
    //return 1 to keep the main's loop
    return 1;
}

/**
 @brief function to print the menu
 */
void printMenu() {
    printf("Choose an option:\n\
0. Exit\n\
1. Create account\n\
2. Delete account\n\
3. Update account\n\
4. Deposit / Withdraw money\n\
5. Transferring\n\
6. View account\n");
}


