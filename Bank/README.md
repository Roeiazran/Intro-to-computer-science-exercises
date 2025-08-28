# Bank

This program simulates a bank system, with a focus on memory allocation and leaks through the use of linked lists and structures.
The program supports the following features:
1. Create a new account:
Given an account number, if it does not already exist in the bank, the program will create an account with the specified account holder’s name.
2. Delete an existing account:
Given an account number of an existing account, the program will delete that account.
3. Update an existing account:
Given an account number of an existing account, the program will update the account holder’s name.
4. Withdraw and deposit to an account:
5. Transfer funds (executing multiple transactions):
Example: ```30-25:40, 20-60:100``` represents transferring 40 from account 30 to 25 and 100 from account 20 to 60. This feature includes a mechanism that guarantees all transfers will be executed only if all are valid.
6. View an account:
Prints the account owner’s details, including all past transactions associated with the account.
--- 

To run the programm simply execute:

```
gcc bank.c
```
and then
```
./a.out
```

--

Running demonstration:

```

Welcome to the bank!
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
1
Enter account number:
100
Enter account holder:
Albert Einstein ֿAccount created successfully
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
1
Enter account number:
200
Enter account holder:
Audrey Hepburn ֿAccount created successfully
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
4
Enter account number:
100
Would you like to deposit or withdraw money?
deposit
How much money would you like to deposit?
1000
Money deposited successfully; your new balance is 1000
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
4
Enter account number:
100
Would you like to deposit or withdraw money?
idk
Invalid action
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
4
Enter account number:
200
Would you like to deposit or withdraw money?
withdraw
How much money would you like to withdraw?
50
Not enough money
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
5
Enter instructions:
100-200:300,200-100:50
Instructions executed successfully
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
5
Enter instructions:
30-45:50
Invalid instruction
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
6
Enter account number:
300
Account not found
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
6
Enter account number:
100
Account #100 (Albert Einstein)
Balance: 750
Transactions:
Deposited 1000
300 to 200
50 from 200
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
6
Enter account number:
200
Account #200 (Audrey Hepburn)
Balance: 250
Transactions:
300 from 100
50 to 100
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
0

```

 

