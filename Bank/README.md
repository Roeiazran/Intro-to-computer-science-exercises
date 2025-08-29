# Bank Management System

## Overview
This C program simulates a simple **banking system** that allows users to:

- Create, delete, and update accounts.
- Deposit and withdraw money.
- Perform transfer instructions between accounts.
- View account details and related transactions.

It maintains **linked lists** for accounts and transactions, dynamically allocating memory as needed.

---

## Features
- Create accounts with unique account numbers.
- Deposit and withdraw money with validation.
- Execute multiple transactions atomically: all succeed or none.
- View transactions for a specific account.
- Dynamic memory management for account holder names and instructions.
- Graceful handling of invalid inputs.

---

## Running demonstration:

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