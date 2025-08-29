# Connect-Style Board Game

## Overview
This is a terminal-based Connect-style board game implemented in C. Players take turns dropping their disks into columns, attempting to form a line of `CONNECT` disks (vertically, horizontally, or diagonally) to win. The game supports multiple players and includes validation for illegal moves and board states.

---

## Features
- Fully playable Connect-style game for 2 or more players.
- Dynamic board initialization with customizable rows and columns.
- Validates moves and ensures disks are placed according to game rules.
- Detects winners, ties, and ongoing game states.
- Prevents invalid board states such as multiple wins or floating disks.
- Supports saving and encoding the board state (64-base encoding used internally).
- **Board encoding and decoding**: the game internally encodes the board into a compact 64-base string for validation and state restoration.
---

For example the string ```'H /H /H /H /H /BACBBAD /'``` Encoded the following board:

~~~~~~~~~~~~~~~
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
|A|B|B|A| | | |
~~~~~~~~~~~~~~~

explanation: H - 7, B - 1, C - 2, and so we get 7 spaces in rows 1 - 5 and in row 6 we get 1 A 2 B's and another A and finally 3 spaces.

## Testing additional features

To test the additional features copy the following code into 4InARow.c file:

```
int main() {

    char board[ROWS][COLS];
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    makeMove(board, ROWS, COLS, NUM_PLAYERS, 'A', 3);
    makeMove(board, ROWS, COLS, NUM_PLAYERS, 'B', 3);
    makeMove(board, ROWS, COLS, NUM_PLAYERS, 'A', 0);
    undoMove(board, ROWS, COLS, 3);

    int status = getStatus(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
        
    if (status == 1) {
        printf("Game-over\n");
    } else if (status == 0) {
        printf("Tie\n");
    } else if (status == -1){
        printf("Game is in progress\n");
    } else {
        printf("Invalid return value from getStatus(...)\n");
    }
    char winner = getWinner(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
    if (winner == -1) {
        printf("No winner\n");
    } else {
        printf("Winner is '%c'\n", winner);
    }
    int valid = isValidBoard(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
    if (valid) {
        printf("Board is valid\n");
    } else {
        printf("Board is invalid\n");
    }
        char code[ROWS * COLS + 1];
        encode(board, ROWS, COLS, code);
        printf("Encoded board: '%s'\n", code);
        char newBoard[ROWS][COLS];
        decode(code, newBoard);
        printBoard(newBoard, ROWS, COLS);
}

```
