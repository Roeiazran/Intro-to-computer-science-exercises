#include "bank.h"
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
    Account * account = (Account*)malloc(sizeof(Account)); //allocate memory for the new account
    
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
 @brief Creates a single transaction.
 Allocates memory and initializes the transaction details.
 @param amount Transaction amount.
 @param from Source account number.
 @param to Destination account number.
 @param bank Pointer to the bank (freed on allocation failure).
 @return Pointer to the newly allocated Transaction structure.
 */
Transaction * makeTransaction(int amount, unsigned int from, unsigned int to, Bank ** bank) {
    Transaction * transaction = (Transaction*)malloc(sizeof(Transaction));

    if (transaction == NULL) { // free if allocation failed
        freeBank(bank);
    }

    // update transaction
    transaction->amount = amount;
    transaction->fromAccount = from;
    transaction->toAccount = to;
    return transaction;
}

/**
 @brief Adds a new transaction to the end of the transactions list.

 Recursively traverses to the end of the list and appends the transaction.
 
 @param bank Pointer to the bank (freed on allocation failure).
 @param transactions Pointer to the head of the transactions list.
 @param transaction Transaction to add.
 */
void addNewTransaction(Bank ** bank, Node ** transactions,  Transaction * transaction) {
    if (*transactions == NULL) { // Base case: reached the end of the list
  
        Node * temp = (Node*)malloc(sizeof(Node));  // allocate memory to the new transaction
        
        if (temp == NULL) { // free if allocation failed
            freeBank(bank);
        }
        
        temp->data = (Transaction*)transaction; // update node data

        temp->next = NULL; // set the tail next to nullptr
        *transactions = temp; // point last node to the new transaction
        return;
    }

    addNewTransaction(bank, &(*transactions)->next , transaction); // recursive call
}

/**
 @brief Validates a number converted from a string using strtol.
 Checks that the conversion consumed the entire string and that the number
 is positive.
 @param amount The integer value returned by strtol.
 @param endptr Pointer to the first invalid character after strtol conversion.
            Used to verify the entire string was a valid number.
 @return 1 if the input is a valid positive number, 0 otherwise.
*/
int isValidInformation(int amount, char * endptr) {
    if (*endptr != '\0' || amount <= 0) {
        return 0;   // invalid input
    }
    return 1;       // valid input
}

/**
 @brief Handles a withdrawal request from a user account.
 Validates the requested amount and deducts it from the account balance
 if sufficient funds are available.
 @param userAccount Pointer to the user's account node.
 @param amount Amount requested to withdraw.
 @param endptr Pointer returned by strtol for validating the input amount.
 @return 1 if the withdrawal is successful, 0 otherwise.
*/
int handleWithdraw(Node * userAccount, int amount, char * endptr) {
    // Validate the input amount and print an error if invalid
    if (!isValidInformation(amount, endptr)) {
        printf("Invalid amount");
        return 0;
    }

    // Check if the user has enough balance
    if (((Account*)userAccount->data)->balance < amount) {
        printf("Not enough money\n");
        return 0;
    }

    // Deduct the amount from the user's account balance
    ((Account*)userAccount->data)->balance -= amount;
    return 1;
}

/**
 @brief Handles a deposit action for a user account.
 Validates the input amount and adds it to the account balance.
 @param userAccount Pointer to the user's account node.
 @param amount Amount to deposit.
 @param endptr Pointer returned by strtol for validating the input amount.
 @return 1 if the deposit is successful, 0 otherwise.
*/
int handleDeposit(Node * userAccount, int amount, char * endptr) {
	// Validate the input amount and print an error if invalid
	if (!isValidInformation(amount, endptr)) {
		printf("Invalid amount\n");
		return 0;
	}

	// Add the amount to the user's account balance
	((Account*)userAccount->data)->balance += amount;
	return 1;
}

/**
 @brief Adds a new transaction to a given transactions list.
 Creates a Transaction structure and appends it to the list.
 @param from Source account number.
 @param to Destination account number.
 @param amount Transaction amount.
 @param bank Pointer to the bank (freed on allocation failure).
 @param transactions Pointer to the head of the transactions list.
*/
void addTransactionToList(unsigned int from, unsigned int to, int amount, Bank ** bank, Node ** transactions) {
	// Create a new transaction structure
	Transaction * transaction = makeTransaction(amount, from, to, bank);

	// Add the transaction to the transactions list
	addNewTransaction(bank, transactions, transaction);
}

/**
 @brief Handles a deposit or withdrawal action for a user account.
 Prompts the user for an action (deposit or withdraw) and executes it,
 validating input and recording the transaction.
 @param bank Pointer to the bank structure.
*/
void withdrawOrDeposit(Bank ** bank) {
	const char withdraw[] = "withdraw";
	const char deposit[] = "deposit";

	// Get the account number and fetch the corresponding user account
	unsigned int accountNumber = getAccountNumberInput();
	Node * userAccount = getAccountByNumber((*bank)->accounts, accountNumber);

	// If no account was found, print an error and return
	if (userAccount == NULL) {
		printAccountNotFound();
		return;
	}

	// Get the user's choice: deposit or withdraw
	char *str = getInfiniteString("Would you like to deposit or withdraw money?", bank);
	int amount;
	char * amountStr;
	char * endptr;

	if (!strcmp(str, withdraw)) {
		// Get the withdrawal amount from the user
		amountStr = getInfiniteString("How much money would you like to withdraw?", bank);
		amount = (int)strtol(amountStr, &endptr, BASE);

		// Execute withdrawal if valid and record the transaction
		if (handleWithdraw(userAccount, amount, endptr)) {
			addTransactionToList(accountNumber, ZERO_ACCOUNT, amount, bank, &(*bank)->transactions);
			printf("Money withdrawn successfully; your new balance is %d\n",
			       ((Account*)userAccount->data)->balance);
		}

		free(amountStr);
		free(str);
		return;

	} else if (!strcmp(str, deposit)) {
		// Get the deposit amount from the user
		amountStr = getInfiniteString("How much money would you like to deposit?", bank);
		amount = (int)strtol(amountStr, &endptr, BASE);

		// Execute deposit if valid and record the transaction
		if (handleDeposit(userAccount, amount, endptr)) {
			addTransactionToList(ZERO_ACCOUNT, accountNumber, amount, bank, &(*bank)->transactions);
			printf("Money deposited successfully; your new balance is %d\n",
			       ((Account*)userAccount->data)->balance);
		}

		free(amountStr);
		free(str);
		return;
	}

	// Handle invalid action input
	free(str);
	printf("Invalid action\n");
}

/**
 @brief Parses a transaction instructions string and builds a linked list of transactions.
 Each transaction in the string should be in the format: from-to:amount, separated by commas.
 
 @param instructionsString The input string containing transaction instructions.
 @param bank Pointer to the bank structure (used to create transactions).
 @return Pointer to the head of the new transactions list, or NULL if the string is invalid.
*/
Node * getTransactionsFromString(char * instructionsString, Bank ** bank) {
	Node * transactions = NULL;  // Head of the transactions list
	char * tok = strtok(instructionsString, "-");  // Start tokenizing string by '-'
	char * endptr;

	while (tok != NULL) {
		// Parse 'from' account number
		unsigned int from = (unsigned int) strtol(tok, &endptr, BASE);

		tok = strtok(0, ":");  // Move to 'to' account part
		if (!isValidInformation(from, endptr) || tok == NULL)  {
			free(transactions);
			return NULL;
		}

		// Parse 'to' account number
		unsigned int to = (unsigned int) strtol(tok, &endptr, BASE);

		tok = strtok(0, ",");  // Move to amount part
		if (!isValidInformation(to, endptr) || tok == NULL)  {
			free(transactions);
			return NULL;
		}

		// Parse transaction amount
		int amount = (int) strtol(tok, &endptr, BASE);

		tok = strtok(0, "[^,]-");  // Move to the next transaction
		if (!isValidInformation(amount, endptr)) {
			free(transactions);
			return NULL;
		}

		// Validate edge case: 'from' and 'to' cannot be the same
		if (to == from) {
			free(transactions);
			return NULL;
		}

		// Add transaction to the list
		addTransactionToList(from, to, amount, bank, &transactions);

		// End of string check
		if (tok == NULL) {
			return transactions;
		}
	}

	// If parsing failed, free transactions list
	free(transactions);
	return NULL;
}

/**
 @brief Checks if a given character is a digit.

 @param ch Character to check.
 @return 1 if the character is a digit ('0'-'9'), 0 otherwise.
 */
int isPositivedigit(char ch) {
    return ch >= '0' && ch <= '9';
}

/**
 @brief Skips all positive digits in a string.

 Advances the string pointer while it points to digit characters ('0'-'9').

 @param str Address of the string pointer to advance.
 */
void skipDigits(char ** str) {
    while (isPositivedigit(**str)) {
        (*str)++;
    }
}

/**
 @brief Validates a transactions string.

 Recursively validates if the string is formatted as "from-to:amount" for each transaction, 
 with optional multiple transactions separated by commas, and containing only digits where expected.

 @param str The string to validate.
 @return 1 if the string is valid, 0 otherwise.
 */
int validateTransactionString(char * str) {
    if (!isPositivedigit(*str)) {
        return 0;
    }
    
    skipDigits(&str);
    
    if (!*str || *str != '-') {
        return 0;
    }
    str++;
    
    skipDigits(&str);
    
    if (!*str || *str != ':') {
        return 0;
    }
    str++;
    
    skipDigits(&str);
    
    if (!*str) {
        return 1;
    }
    
    if (*str != ',') {
        return 0;
    }
    
    while (*str == ',') {
        str++;
        if (!*str) {
            return 0;
        }
    }
    
    if (!*str) {
        return 1;
    }
    
    return validateTransactionString(str);
}

/**
 @brief Executes all transactions from a given list.

 Recursively executes each transaction in order. If any transaction fails 
 (due to missing accounts or insufficient balance), all previous operations are reversed.

 @param accounts The list of bank accounts.
 @param transactions The list of transactions to execute.
 @return 1 if all transactions were executed successfully, 0 otherwise.
 */
int executeTransferInstructions(Node * accounts, Node * transactions) {

    // Base case: no more transactions
    if (transactions == NULL) {
        return 1;
    }

    // Extract transaction details
    unsigned int from = ((Transaction*)transactions->data)->fromAccount;
    unsigned int to = ((Transaction*)transactions->data)->toAccount;
    int amount = ((Transaction*)transactions->data)->amount;

    // Get the accounts involved
    Node * fromAccount = getAccountByNumber(accounts, from);
    Node * toAccount = getAccountByNumber(accounts, to);

    // Fail if any account does not exist
    if (fromAccount == NULL || toAccount == NULL) {
        return 0;
    }

    // Fail if fromAccount has insufficient balance
    if (((Account*)fromAccount->data)->balance < amount) {
        return 0;
    }

    // Perform the transaction
    ((Account*)fromAccount->data)->balance -= amount;
    ((Account*)toAccount->data)->balance += amount;

    // Recurse to execute remaining transactions
    int opSucceed = executeTransferInstructions(accounts, transactions->next);

    // Undo transaction if subsequent operations failed
    if (!opSucceed) {
        ((Account*)toAccount->data)->balance -= amount;
        ((Account*)fromAccount->data)->balance += amount;
        return 0;
    }

    return 1;
}

/**
 @brief Concatenates two linked lists.

 Appends all nodes of list2 to the end of list1. If list1 is empty, it is set to list2.

 @param list1 Pointer to the first list (modified to include list2).
 @param list2 Pointer to the second list to append.
 */
void concatenateTwoList(Node ** list1, Node ** list2) {

    // Store the head of list1 to restore it later
    Node * head = *list1;

    // If list1 is empty, assign it to list2 and return
    if ((*list1) == NULL) {
        (*list1) = (*list2);
        return;
    }

    // Advance list1 to its last node
    while ((*list1)->next != NULL) {
        (*list1) = (*list1)->next;
    }

    // Link the last node of list1 to list2
    (*list1)->next = *list2;

    // Restore the head of list1
    (*list1) = head;
}

/**
 @brief Frees a list of transactions.

 Frees the memory of each transaction’s data and the nodes themselves.

 @param transactions Pointer to the head of the transaction list.
 */
void freeListOfTransaction(Node * transactions) {

    Node * temp;
    // Iterate through all transactions
    for (Node * transaction = transactions; transaction != NULL;) {
        // Free transaction data
        free(transaction->data);

        // Save pointer to next node
        temp = transaction->next;

        // Free the node itself
        free(transaction);

        // Move to the next transaction
        transaction = temp;
    }
}

/**
 @brief Executes multiple transactions from a string of instructions.

 Converts a user-provided instructions string into a transaction list,
 executes the transactions, and appends them to the bank's transaction list
 if execution is successful.

 @param bank Pointer to the bank structure.
 @param instructionsString The user-provided instructions string.
 */
void makeInstructionsList(Bank ** bank, char * instructionsString) {
    // Initialize transactions list from the string
    Node * transactions = getTransactionsFromString(instructionsString, bank);
    
    // Initialize operation flag to detect invalid instructions
    int operationResult = 0;

    // If transactions were parsed successfully
    if (transactions != NULL) {
        // Attempt to execute the transactions and store the result
        operationResult = executeTransferInstructions((*bank)->accounts, transactions);

        // On success, append executed transactions to the bank's list
        if (operationResult) {
            concatenateTwoList(&((*bank)->transactions), &transactions);
            printf("Instructions executed successfully\n");
        }
    }

    // On failure, free the transaction list and print an error
    if (!operationResult) {
        freeListOfTransaction(transactions);
        printf("Invalid instructions\n");
        return;
    }
}
/**
 @brief Reads a transaction instruction string from the user, validates it, and executes the transactions.

 Prompts the user for instructions, performs basic structure validation (from-to:amount),
 and creates the transactions in the bank if valid.

 @param bank Pointer to the bank structure.
 */
void getInstructionsString(Bank ** bank) {
    // Read the instructions string from the user
    char * str = getInfiniteString("Enter instructions:", bank);
    
    // Validate the string for correct transaction structure
    if (!validateTransactionString(str)) {
        free(str); // Free memory on error
        printf("Invalid instructions\n");
        return;
    }

    // Create and execute transactions, then free the string
    makeInstructionsList(bank, str);
    free(str);
}


/**
 @brief Prints all transactions related to a specific user account.

 Iterates through the bank's transaction list and prints each transaction involving the account.
 Prints "No transactions" if the account has no transactions.

 @param transactions Pointer to the head of the transaction list.
 @param accountNum Account number of the user.
 */
void printUserTransactions(Node * transactions, unsigned int accountNum) {
    unsigned int from;
    unsigned int to;
    int amount;
    int hasTransactions = 0; // Flag to detect if the user has any transactions

    // Iterate over all transactions
    while (transactions != NULL) {
        from = ((Transaction *)transactions->data)->fromAccount;
        to = ((Transaction *)transactions->data)->toAccount;
        amount = ((Transaction *)transactions->data)->amount;

        // Check if the transaction involves the user
        if (from == accountNum || to == accountNum) {
            if (!hasTransactions) {
                printf("Transactions:\n");
                hasTransactions = 1;
            }

            // Determine transaction type and print accordingly
            if (from == ZERO_ACCOUNT) {
                printf("Deposited %d\n", amount);
            } else if (to == ZERO_ACCOUNT) {
                printf("Withdrew %d\n", amount);
            } else if (to == accountNum) {
                printf("%d from %u\n", amount, from);
            } else {
                printf("%d to %u\n", amount, to);
            }
        }

        // Move to the next transaction
        transactions = transactions->next;
    }

    // If no transactions found, notify the user
    if (!hasTransactions) {
        printf("No transactions\n");
    }
}
/**
 @brief Prints details of a specific account.

 Prompts the user for an account number and prints its details, including the balance and related transactions.

 @param bank Pointer to the main bank structure.
 */
void viewAccount(Bank ** bank) {
    // Get the user account number and fetch the account
    unsigned int accountNum = getAccountNumberInput();
    Node * account = getAccountByNumber((*bank)->accounts, accountNum);

    // Check if account exists
    if (account == NULL) {
        printAccountNotFound();
        return;
    }

    // Print account details
    unsigned int accountNumber = ((Account*)account->data)->accountNumber;
    printf("Account #%u (%s)\nBalance: %d\n", accountNumber,
           ((Account*)account->data)->accountHolder, ((Account*)account->data)->balance);

    // Print transactions related to this account
    Node * transactions = (*bank)->transactions;
    printUserTransactions(transactions, accountNumber);
}


/**
 @brief Handles a user's menu choice.

 Executes the appropriate bank operation based on the user's input choice.

 @param choice Character entered by the user representing the menu choice.
 @param bank Pointer to the main bank structure.
 @return 0 if the user chooses to exit, 1 otherwise.
 */
int handleUserChoice(char choice, Bank ** bank) {
    switch (choice) {
        case '0':
            freeBank(bank); // Exit and free the bank
            break;
        case '1':
            getAccountDetails(bank); // Create new account
            break;
        case '2':
            deleteAccount(&((*bank)->accounts)); // Delete an account
            break;
        case '3':
            updateAccount(bank); // Update account details
            break;
        case '4':
            withdrawOrDeposit(bank); // Handle deposit/withdraw
            break;
        case '5':
            getInstructionsString(bank); // Execute transfer instructions
            break;
        case '6':
            viewAccount(bank); // View specific account
            break;
        default:
            printf("Invalid option\n");
    }

    return 1; // Continue main loop
}

/**
 @brief Prints the main menu options.
 */
void printMenu() {
    printf("Choose an option:\n"
           "0. Exit\n"
           "1. Create account\n"
           "2. Delete account\n"
           "3. Update account\n"
           "4. Deposit / Withdraw money\n"
           "5. Transferring\n"
           "6. View account\n");
}
