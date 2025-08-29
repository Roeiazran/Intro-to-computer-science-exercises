#include "4inARow.h"

/**
 * @brief Convert a character representing a player to its integer equivalent
 * @param player The player as a character ('A', 'B', 'C', ...)
 * @return The player as an integer (1, 2, 3, ...)
 */
int getPlayerAsInt(char player) {
    // Subtract 'A' to get zero-based index, then add 1 to make it 1-based
    return player - 'A' + 1;
}

/**
 * @brief Convert an integer representing a player to its character equivalent
 * @param player The player as an integer
 * @return The player as a character
 */
char getPlayerAsChar(int player) {
    // Add 'A' to shift from 1-based index to ASCII character
    // Subtract 1 because 'A' corresponds to 1
    return player + 'A' - 1;
}

/**
 * @brief Initialize the game board
 * 
 * This function iterates over all rows and columns of the board
 * and sets each cell to EMPTY_POS (representing an empty space).
 * 
 * @param board The 2D board array to initialize
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 */
void initBoard(char board[ROWS][COLS], int rows, int columns) {
    
    // Loop over each row
    for (int i = 0; i < rows; i++) {
        // Loop over each column in the current row
        for (int j = 0; j < columns; j++) {
            // Set current cell to empty
            board[i][j] = EMPTY_POS;
        }
    }
}

/**
 * @brief Print the current state of the game board
 * 
 * This function iterates over rows and columns of the board
 * and prints the board with borders for better visualization.
 * 
 * @param board The 2D board array to print
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 */
void printBoard(char board[ROWS][COLS], int rows, int columns) {
    
    // Loop over each row including top and bottom borders
    for (int i = 0; i < rows + 2; i++) {
        // Loop over each column including side borders
        for (int j = 0; j < 2 * columns + 1; j++) {
            
            // Print top or bottom border
            if (i == 0 || i == rows + 1) {
                printf("~");
            
            // Print board cells with vertical separators
            } else if (i < rows + 1 && j < columns) {
                printf("|%c", board[i - 1][j]);
            
            // Print right edge of the board
            } else if (j == columns) {
                printf("|");
            }
        }
        // Move to next row
        printf("\n");
    }
}


/**
 * @brief Check if a given board state is valid
 * 
 * This function performs two main validations:
 * 1. Checks for invalid wins (e.g., multiple separate wins for the same player or
 *    wins with illegal placements).
 * 2. Checks for invalid moves or turn order violations.
 * 
 * The board is temporarily encoded to a string and decoded back to preserve state
 * during validation.
 * 
 * @param board The 2D board array
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param players Total number of players
 * @param connect Number of disks in a row required to win
 * @return 1 if the board is valid, 0 otherwise
 */
int isValidBoard(char board[ROWS][COLS], int rows, int columns, int players, int connect) {

    const int NO_WINS = 0;
    const int INVALID_WIN = -1;
    const int MAX_ALLOWED_WINS = 1;

    // Temporary string to save the board state for restoring later
    char str[2 * rows * columns + rows + 1];
    char *code = str;
    
    // Encode the board to a string
    encode(board, rows, columns, code);
    
    int validate = 1; // Flag to validate wins strictly
    int playerWins = 0; // Counter for total wins per player
    
    // Validate wins for each player
    for (int player = 1; player <= players; player++) {
        int isValidWin = checkForConnect(board, connect, rows, columns, validate, player);

        // If a valid win found, increment the win count
        if (isValidWin > NO_WINS) {
            playerWins++;
            // More than one win for the same player is invalid
            if (playerWins > MAX_ALLOWED_WINS) {
                return INVALID_BOARD;
            }
        }

        // If an invalid win is found, return immediately
        if (isValidWin == INVALID_WIN) {
            return INVALID_BOARD;
        }
    }

    // Restore the board from encoded string
    decode(code, board);
    
    // Validate legal moves and turn order
    int result = validatePlays(board, rows, columns, players);
    
    // Decode back again to restore board state
    decode(code, board);
    
    // Return 1 if board is valid, 0 otherwise
    return result;
}

/**
 * @brief Validate the board in terms of legal player turns and move placements
 * 
 * This function ensures the board follows the rules of turn order and proper
 * disk placement. It combines two validations:
 * 1. `validatePlayTimes`: Checks that players have played in the correct order
 *    and no player has taken too many turns.
 * 2. `validatePlayMoves`: Checks recursively that all disks are properly
 *    placed without floating disks.
 * 
 * @param board The 2D board array
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param players Total number of players in the game
 * @return 1 if the board is valid, 0 otherwise
 */
int validatePlays(char board[ROWS][COLS], int rows, int columns, int players) {
    
    // Determine the last player who played and validate turn order
    int lastPlayed = validatePlayTimes(board, rows, columns, players);
    
    // Invalid board if turn order is illegal
    if (!lastPlayed) {
        return INVALID_BOARD;
    }
    
    // Get total number of occupied positions on the board
    int numOfOccur = getNumOfOccurences(board, rows, columns, EMPTY_POS);
    
    // Recursively validate the layout of all moves
    int res = validatePlayMoves(board, rows, columns, lastPlayed, numOfOccur, players);
    
    // Return invalid if layout is incorrect
    if (!res) {
        return INVALID_BOARD;
    }

    // Board is valid if all checks pass
    return VALID_BOARD;
}

/**
 * @brief Find the column of a player in a given row
 * 
 * This function searches the specified row for a player's disk.
 * It also detects "floating disks" (disks with empty space below them),
 * which are invalid in games like Connect-style boards.
 * 
 * @param board The 2D board array
 * @param row The row to check
 * @param columns Total number of columns in the board
 * @param player Player number to search for
 * @param rows Total number of rows in the board (used for bounds checking)
 * @return Column index of the player if found, -1 if not found, -2 if a floating disk is detected
 */
int getPlayerCol(char board[ROWS][COLS], int row, int columns, int player, int rows) {
        
    const int NOT_FOUND = -1;
    const int FLOATING_DISK = -2;

    // Convert player number to character representation
    char playerAsChar = getPlayerAsChar(player);
    
    // Search the row for the player's disk
    for (int i = 0; i < columns; i++) {
        // Check for floating disk: non-empty position with empty space directly below
        if (board[row][i] != EMPTY_POS && row + 1 < rows && board[row + 1][i] == EMPTY_POS) {
            return FLOATING_DISK;
        }
        
        // If player's disk found, return its column
        if (board[row][i] == playerAsChar) {
            return i;
        }
    }
    
    // Player not found in the row
    return NOT_FOUND;
}


/**
 * @brief Recursively validate that all moves on the board are legal
 * 
 * This function ensures that each player's disk is placed correctly:
 * 1. Checks from the last played player down to the first player.
 * 2. Detects floating disks (disks with empty space directly below).
 * 3. Recursively removes validated disks to continue checking previous moves.
 * 
 * @param board The 2D board array
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param lastPlayed Last player who made a move
 * @param numOfOccur Total number of disk occurrences left to check
 * @param players Total number of players in the game
 * @return 1 if the board is valid, 0 otherwise
 */
int validatePlayMoves(char board[ROWS][COLS], int rows, int columns, int lastPlayed, int numOfOccur, int players) {
    
    const int FLOATING_DISK = -2;
    const int MIN_ROW = 0;

    // Base case: no remaining disks to validate
    if (numOfOccur == 0) {
        return VALID_BOARD;
    }
    
    // Iterate over each row to find the last played player's disk
    for (int i = 0; i < rows; i++) {
        int playerCol = getPlayerCol(board, i, columns, lastPlayed, rows);

        // Invalid if a floating disk is found
        if (playerCol == FLOATING_DISK) {
            return INVALID_BOARD;
        }
        
        // If a valid disk is found
        if (playerCol >= MIN_ROW) {
            
            // Move to the previous player (wrap around if needed)
            lastPlayed = lastPlayed - 1 == 0 ? players : (lastPlayed - 1);
            
            // Ensure no empty space above or it's the first row
            if ((i != 0 && board[i - 1][playerCol] == EMPTY_POS) || i == 0) {
                // Clear the current disk for recursive checking
                board[i][playerCol] = EMPTY_POS;
                break; // Stop scanning rows for this disk
            }
        }
        
        // If no player disk found in any row, board is invalid
        if (i == rows) {
            return INVALID_BOARD;
        }
    }

    // Recursively validate the remaining disks
    return validatePlayMoves(board, rows, columns, lastPlayed, numOfOccur - 1, players);
}


/**
 * @brief Helper function to validate the number of plays per player
 * 
 * This function checks for illegal moves on the board by ensuring:
 * 1. No player has played out of turn.
 * 2. No player has played more than one extra turn compared to others.
 * 
 * @param board The 2D board array
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param players Total number of players
 * @return The last player who played if the board is valid, 0 if the board is illegal
 */
int validatePlayTimes(char board[ROWS][COLS], int rows, int columns, int players) {
    
    const int MAX_DIFF_ALLOWED = 2;  // Maximum allowed difference in plays between players
    int max = 0;                     // Tracks the maximum moves by a single player
    int lastPlayedPlayer = 0;        // Tracks the last player who played

    // Check every player against the next
    for (int i = 0; i < players - 1; i++) {
        char player = 'A' + i; // Current player as character

        // Get the number of moves for current and next player
        int currCount = getNumOfOccurrences(board, rows, columns, player);
        int nextCount = getNumOfOccurrences(board, rows, columns, player + 1);

        // If next player played more than current, board is illegal
        if (currCount < nextCount) {
            return INVALID_BOARD;
        }

        // Update the last player who made a move
        if (currCount >= nextCount) {
            lastPlayedPlayer = i + 2; // +2 because player index starts at 0 and we compare with next
        }

        // Update maximum moves seen so far
        if (currCount > max) {
            max = currCount;
        }

        // If any player played too many extra moves, board is illegal
        if (max - currCount >= MAX_DIFF_ALLOWED || max - nextCount >= MAX_DIFF_ALLOWED) {
            return INVALID_BOARD;
        }
    }
    
    // Return the last player who played if board is valid
    return lastPlayedPlayer;
}


/**
 * @brief Get the number of occurrences of a player on the board, 
 *        or total occupied positions if no player is specified
 * 
 * If a player character is provided, the function counts only that
 * player's disks. If `player` is EMPTY_POS, it counts all occupied
 * positions on the board.
 * 
 * @param board The 2D board array
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param player Player character to count, or EMPTY_POS to count all disks
 * @return Count of occurrences
 */
int getNumOfOccurrences(char board[ROWS][COLS], int rows, int columns, char player) {
    
    int count = 0; // Initialize counter
    
    // Iterate over every cell in the board
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            
            // Count occurrences for the specified player
            if (player != EMPTY_POS) {
                if (board[i][j] == player) {
                    count++;
                }
            } 
            // If player not specified, count all non-empty cells
            else if (board[i][j] != EMPTY_POS) {
                count++;
            }
        }
    }
    
    return count; // Return the total count
}


/**
 * @brief Determine the winner on the board
 * 
 * This function iterates over all players and calls `checkForConnect`
 * to see if any player has achieved the required number of connected disks.
 * 
 * @param board The 2D board array
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param players Total number of players
 * @param connect Number of consecutive disks needed for a win
 * @return The winner as a character if found, otherwise -1
 */
char getWinner(char board[ROWS][COLS], int rows, int columns, int players, int connect) {
    
    const int NO_WINNER = -1;
    int validate = 0; // Assume board is valid, no need for additional validation
    
    // Check each player for a winning sequence
    for (int pl = 1; pl < players; pl++) {
        int winner = checkForConnect(board, connect, rows, columns, validate, pl);
        
        // If a winner is found, return the player's character
        if (winner) {
            return getPlayerAsChar(pl);
        }
    }
   
    // No winner found
    return NO_WINNER;
}


/**
 * @brief Check if a given player number is valid
 * 
 * A player is considered valid if their number is between 1 and the
 * total number of players allowed in the game.
 * 
 * @param players Total number of players allowed in the game
 * @param player Player number to validate
 * @return 1 if the player number is valid, 0 otherwise
 */
int isValidPlayer(int players, int player) {
    const int MIN_INVALID_PL = 0; // Minimum invalid player number
    
    // Return true if player number is greater than 0 and less than or equal to total players
    return player > MIN_INVALID_PL && player <= players;
}

/**
 * @brief Determine the current status of the game
 * 
 * This function first checks if there is a winner using `getWinner`.
 * If no winner is found, it checks if the board is full to determine a tie.
 * Otherwise, the game is still ongoing.
 * 
 * @param board The 2D board array
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param players Total number of players
 * @param connect Number of consecutive disks needed for a win
 * @return 1 if there is a winner, 0 if the board is full (tie), -1 if the game is ongoing
 */
int getStatus(char board[ROWS][COLS], int rows, int columns, int players, int connect) {
    
    const int GAME_WINNED = 1;
    const int WINNER_NOT_FOUND = -1;
    const int GAME_IS_ON = -1;
    const int TIE = 0;

    // Check for a winner
    char winner = getWinner(board, rows, columns, players, connect);
    
    if ((char) winner != WINNER_NOT_FOUND) {
        return GAME_WINNED; // A winner was found
    }

    // Check if the board is full for a tie
    int isFullBoard = checkForFullBoard(board, columns);
    if (isFullBoard) {
        return TIE; // The board is full, it's a tie
    }
    
    // No winner and the board is not full, game is still ongoing
    return GAME_IS_ON;
}


/**
 * @brief Check if the board has no empty positions left
 * 
 * This function inspects the top row of the board. If any cell in the
 * top row is empty, the board is not full; otherwise, the board is full.
 * 
 * @param board The 2D board array
 * @param columns Number of columns in the board
 * @return 1 if the board is full, 0 if there is at least one empty cell
 */
int checkForFullBoard(char board[ROWS][COLS], int columns) {
    
    const int HAS_SPACE = 0;
    const int NO_SPACE = 1;

    // Iterate over each column in the top row
    for (int i = 0; i < columns; i++) {
        // If an empty position is found, board is not full
        if (board[0][i] == EMPTY_POS) {
            return HAS_SPACE;
        }
    }
    
    // No empty positions found, board is full
    return NO_SPACE;
}


/**
 * @brief Check if a player has won by connecting the required number of disks
 * 
 * This function iterates over the board, calling `getNumOfConnects` for each
 * disk of the given player. It tracks valid and invalid wins depending on the
 * `validate` flag. Returns the winner, no winner, or an invalid win indicator.
 * 
 * @param board The 2D board array
 * @param connect Number of consecutive disks needed for a win
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param validate Flag to determine whether to validate for possible invalid wins
 * @param player Player number to check
 * @return 1 for a valid win, 0 if no winner, -1 if invalid win detected
 */
int checkForConnect(char board[ROWS][COLS], int connect, int rows, int columns, int validate, int player) {

    const int INVALID_WIN = 1;
    const int INVALID_WIN_RETURN = -1;
    const int VALID_WIN = 1;
    const int ALLOWED_WINS = 1;
    const int NO_WINS = 0;
    
    int directions = 8;           // Number of directions to check (8: vertical, horizontal, diagonal)
    int arr[8] = {0};             // Array to store consecutive disks in each direction
    int wins = 0;                  // Count of valid wins found
    char playerAsChar = getPlayerAsChar(player); // Convert player number to character
    
    // Iterate over every cell in the board
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            
            // Only check cells that belong to the player
            if (board[row][col] == playerAsChar) {

                // Check for connects starting from this position
                int isWin = getNumOfConnects(board, connect - 1, rows, columns, arr, col, row,
                                             playerAsChar, directions, connect, validate);

                if (validate) {
                    // If validating for invalid wins
                    if (isWin == INVALID_WIN) {
                        return INVALID_WIN_RETURN; // Found invalid win
                    } else if (isWin > INVALID_WIN) {
                        wins++;
                        if (wins > ALLOWED_WINS) {
                            return INVALID_WIN_RETURN; // More than one win not allowed
                        }
                    }
                } else if (isWin > INVALID_WIN) {
                    // If not validating, any valid win is sufficient
                    return VALID_WIN;
                }

                // Reset the directions array before next iteration
                for (int i = 0; i < directions; i++) {
                    arr[i] = 0;
                }
            }
        }
    }
    
    // Return result based on the number of wins found
    if (wins) {
        return VALID_WIN;
    }

    return NO_WINS; // No winner found
}


/**
 * @brief Check if a disk at a given position is a valid "win disk"
 * 
 * A disk is considered a possible win disk if it is either:
 *   1. In the first row, or
 *   2. The first disk in its column (i.e., nothing above it)
 * 
 * If valid, the position is temporarily marked with a special character
 * to prevent counting it as part of multiple win sequences.
 * 
 * @param board The 2D board array
 * @param row Row index of the disk
 * @param col Column index of the disk
 * @return 1 if the disk is a valid win disk, 0 otherwise
 */
int isValidWin(char board[ROWS][COLS], int row, int col) {
    
    const int VALID_WIN_DISK = 1;
    const char POSSIBLE_WIN = '+'; // Temporary marker for possible win
    const int FIRST_ROW = 0;
    
    // Check if the disk is in the first row or has nothing above it
    if (row == FIRST_ROW || board[row - 1][col] == EMPTY_POS) {
        
        // Temporarily mark the position to prevent double counting
        board[row][col] = POSSIBLE_WIN;
        
        // This is a valid win disk
        return VALID_WIN_DISK;
    }
    
    // Not a valid win disk
    return !VALID_WIN_DISK;
}

/**
 * @brief Check if a given position on the board contains a specific player
 *        and increment a counter array at a specified index if true.
 * 
 * This function ensures the position is within board boundaries, checks
 * for the presence of the player at that position, and increments the
 * counter array if both conditions are met.
 * 
 * @param board The 2D board array
 * @param row Row index to check
 * @param col Column index to check
 * @param player The player character to check for
 * @param countArr Array storing counts for different positions or directions
 * @param pos Index in countArr to increment if player is present
 * @param rows Total number of rows in the board
 * @param columns Total number of columns in the board
 * @return 1 if the operation was successful (player found and counter incremented), 0 otherwise
 */
int storeCount(char board[ROWS][COLS], int row, int col, char player, int countArr[], int pos,
               int rows, int columns) {
    
    const int OP_SUCCESS = 1;

    // Check if the position is within board boundaries
    if (row >= 0 && row < rows && col >= 0 && col < columns) {
        // Check if the specified player is at this position
        if (board[row][col] == player) {
            // Increment the counter at the given index
            countArr[pos]++;
            return OP_SUCCESS; // Operation successful
        }
    }

    // Position invalid or player not found
    return !OP_SUCCESS;
}

/**
 * @brief Validate if a player's move creates a valid win on the board
 * 
 * This function checks in all directions (vertical, horizontal, diagonal)
 * using the results from `getNumOfConnects` (stored in arr[]) to see if 
 * any sequence qualifies as a win. A sequence is considered valid if at 
 * least one disk in it has no disk above it (checked using isValidWin).
 * 
 * @param board The 2D board array
 * @param arr Array of counts in 8 directions (bottom, top, right, left, 
 *            bottom-right, bottom-left, top-right, top-left)
 * @param originalConnect The required number of connected disks for a win
 * @param connect Current offset to check along each direction
 * @param row Row index of the last move
 * @param col Column index of the last move
 * @return 1 if a valid win disk is found, 0 otherwise
 */
int validateWin(char board[ROWS][COLS], int arr[], int originalConnect, int connect, int row, int col) {
    
    const int VALID_WIN = 1;
    int res = 0;

    // Check downward
    if (arr[0] == originalConnect) {
        res += isValidWin(board, row + connect, col);
    }
    // Check upward
    if (arr[1] == originalConnect) {
        res += isValidWin(board, row - connect, col);
    }
    // Check right
    if (arr[2] == originalConnect) {
        res += isValidWin(board, row, col + connect);
    }
    // Check left
    if (arr[3] == originalConnect) {
        res += isValidWin(board, row, col - connect);
    }
    // Check bottom-right diagonal
    if (arr[4] == originalConnect) {
        res += isValidWin(board, row + connect, col + connect);
    }
    // Check bottom-left diagonal
    if (arr[5] == originalConnect) {
        res += isValidWin(board, row + connect, col - connect);
    }
    // Check top-right diagonal
    if (arr[6] == originalConnect) {
        res += isValidWin(board, row - connect, col + connect);
    }
    // Check top-left diagonal
    if (arr[7] == originalConnect) {
        res += isValidWin(board, row - connect, col - connect);
    }
    
    // Return 1 if any valid win disk was found
    if (res > 0) {
        return VALID_WIN;
    }
    
    // No valid win found
    return !VALID_WIN;
}

/**
 * @brief Recursively checks a position for connected disks in all directions
 * 
 * This function inspects all eight directions around a given position and counts
 * consecutive disks for the same player. It can also validate the sequence to ensure
 * that at least one disk in the sequence is a "possible win disk" (i.e., has no disk
 * above it). The recursion decrements `connect` with each call to explore all positions
 * along the sequence.
 * 
 * @param board The 2D board array
 * @param connect Current offset along the direction to check
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param countArr Array storing the count of disks in each of the 8 directions
 * @param col Column index of the starting position
 * @param row Row index of the starting position
 * @param player Player character to check for
 * @param size Size of countArr (typically 8 for 8 directions)
 * @param originalConnect The required number of connected disks for a win
 * @param toValidate Flag indicating whether to validate the sequence as a possible win
 * @return Number > 1 if there was a valid win, 0 if no wins, 1 if an invalid win
 */
int getNumOfConnects(char board[ROWS][COLS], int connect, int rows, int columns,
                     int countArr[], int col, int row, char player, int size,
                     int originalConnect, int toValidate) {

    const int NO_WINS = 0;

    // Exit condition: when connect reaches -1, check if any direction has a complete sequence
    if (connect == -1) {
        for (int i = 0; i < size; i++) {
            if (countArr[i] == originalConnect) {
                return originalConnect + 1; // Found a complete sequence
            }
        }
        return NO_WINS; // No wins found
    }

    int exitFlag = 0;

    // Check all eight directions for a matching disk at the current offset
    exitFlag += storeCount(board, row + connect, col, player, countArr, 0, rows, columns); // Down
    exitFlag += storeCount(board, row - connect, col, player, countArr, 1, rows, columns); // Up
    exitFlag += storeCount(board, row, col + connect, player, countArr, 2, rows, columns); // Right
    exitFlag += storeCount(board, row, col - connect, player, countArr, 3, rows, columns); // Left
    exitFlag += storeCount(board, row + connect, col + connect, player, countArr, 4, rows, columns); // Bottom-Right
    exitFlag += storeCount(board, row + connect, col - connect, player, countArr, 5, rows, columns); // Bottom-Left
    exitFlag += storeCount(board, row - connect, col - connect, player, countArr, 6, rows, columns); // Top-Left
    exitFlag += storeCount(board, row - connect, col + connect, player, countArr, 7, rows, columns); // Top-Right

    // If no matching disks found in any direction, return NO_WINS
    if (!exitFlag) {
        return NO_WINS;
    }

    // Recursive call with connect - 1 to move closer to the end of the sequence
    int result = getNumOfConnects(board, connect - 1, rows, columns, countArr, col,
                                  row, player, size, originalConnect, toValidate);

    // If validating and still within the original connect length
    if (connect < originalConnect && result && toValidate) {
        int validate = validateWin(board, countArr, originalConnect, connect, row, col);

        // Decrement result if it's not a possible win disk
        if (!validate) {
            return result - 1;
        } else {
            return result; // Valid win disk, keep result
        }
    }

    return result; // Return result of recursion
}

/**
 * @brief Undo the last move made in a specific column
 * 
 * This function checks if the column is valid and not empty,
 * then removes the last placed marker from that column.
 * 
 * @param board The 2D board array
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param column Column index to undo the move
 * @return 1 if the undo was successful, 0 otherwise
 */
int undoMove(char board[ROWS][COLS], int rows, int columns, int column) {
    
    const int UNDO_SUCCESS = 1;
    const int MIN_COL = 0;

    // Validate the column index
    if (column < MIN_COL || column >= columns) {
        return !UNDO_SUCCESS; // Invalid column
    }
    
    // Find the lowest empty row in the selected column
    int emptyRow = getBottomEmptyPos(board, rows, column);
    
    // Check if the column is completely empty
    if (emptyRow == rows - 1) {
        return !UNDO_SUCCESS; // Nothing to undo
    }
    
    // Remove the last placed marker
    board[emptyRow + 1][column] = EMPTY_POS;

    return UNDO_SUCCESS; // Undo successful
}


/**
 * @brief Make a move on the board for a given player
 * 
 * This function validates the selected column, checks the player,
 * and ensures the column is not full before placing the player's marker.
 * 
 * @param board The 2D board array
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param players Total number of players
 * @param player Character representing the player
 * @param column Column index where the player wants to place the marker
 * @return 1 if the move was successful, 0 otherwise
 */
int makeMove(char board[ROWS][COLS], int rows, int columns, int players, char player, int column) {
    
    const int MOVE_SUCCESS = 1;
    const int MIN_ROW_COL = 0;

    // Validate the column index
    if (column < MIN_ROW_COL || column >= columns) {
        return !MOVE_SUCCESS; // Invalid column
    }
    
    // Convert player character to integer representation
    int integerPlayer = getPlayerAsInt(player);
    
    // Validate the player number
    if (!isValidPlayer(players, integerPlayer)) {
        return !MOVE_SUCCESS; // Invalid player
    }

    // Find the lowest empty row in the selected column
    int emptyRow = getBottomEmptyPos(board, rows, column);
    
    // Check if the column is full
    if (emptyRow < MIN_ROW_COL) {
        return !MOVE_SUCCESS; // Column full
    }
    
    // Place the player's marker in the board
    board[emptyRow][column] = player;
    
    return MOVE_SUCCESS; // Move successful
}

/**
 * @brief Get the lowest empty position in a given column
 * 
 * This function searches from the bottom row to the top row in the
 * specified column to find the first empty cell.
 * 
 * @param board The 2D board array
 * @param rows Number of rows in the board
 * @param column The column index to check
 * @return Row index of the bottom-most empty cell, or -1 if the column is full
 */
int getBottomEmptyPos(char board[ROWS][COLS], int rows, int column) {
    
    const int POS_NOT_FOUND = -1;

    // Iterate from the bottom row to the top row
    for (int row = rows - 1; row >= 0 ; row--) {
        // Check if the current cell is empty
        if (board[row][column] == EMPTY_POS) {
            return row; // Return the row index if empty
        }
    }

    // No empty cell found in the column
    return POS_NOT_FOUND;
}


/**
 * @brief Convert an integer (0-63) into its corresponding Base64 character
 * 
 * Maps integers to Base64 characters according to the standard:
 *   0-25   -> 'A'-'Z'
 *   26-51  -> 'a'-'z'
 *   52-61  -> '0'-'9'
 *   62     -> '+'
 *   63     -> '/'
 * 
 * @param input Integer to convert (expected 0-63)
 * @return Corresponding Base64 character, or -1 if input is invalid
 */
char getIntegerAs64Base(int input) {
    
    const int NOT_SUCCESS = -1;        // Return value for invalid input
    const int A_64_BASE = 0;           // Start of uppercase letters
    const int LETTERS_COUNT = 26;      // Number of letters in alphabet
    const int SMALL_A_64_BASE = 26;    // Start of lowercase letters
    const int ZERO_64_BASE = 52;       // Start of digits
    const int NUMBER_COUNT = 10;        // Number of digits (0-9)
    const int PLUS_64BASE = 62;        // Base64 value for '+'

    // Map input to uppercase letter
    if (input >= A_64_BASE && input <= A_64_BASE + LETTERS_COUNT - 1) {
        return 'A' + (input - A_64_BASE);
    }

    // Map input to lowercase letter
    if (input >= SMALL_A_64_BASE && input <= SMALL_A_64_BASE + LETTERS_COUNT - 1) {
        return 'a' + (input - SMALL_A_64_BASE);
    }

    // Map input to digit
    if (input >= ZERO_64_BASE && input <= ZERO_64_BASE + NUMBER_COUNT - 1) {
        return '0' + (input - ZERO_64_BASE);
    }

    // Map input to special Base64 characters
    if (input == PLUS_64BASE) {
        return '+';
    } else {
        return '/';
    }

    // Return error if input was not valid (this line is technically unreachable)
    return NOT_SUCCESS;
}

/**
 * @brief Convert a Base64 character to its corresponding integer value
 * 
 * This function maps Base64 characters (A-Z, a-z, 0-9, +, /) to
 * integer values from 0 to 63.
 * 
 * @param input The Base64 character to convert
 * @return The integer representation of the Base64 character
 */
int get64BaseAsInteger(char input) {
    
    // Initialize constants
    const char PLUS = '+';          // '+' corresponds to 62
    const int SMALL_A_BASE_DIFF = 26; // Offset for lowercase letters
    const int ZERO_BASE_DIFF = 52;    // Offset for digits
    const int PLUS_64_BASE = 62;      // Base64 value for '+'
    const int SLASH_64_BASE = 63;     // Base64 value for '/'

    // Check if the character is an uppercase letter (A-Z)
    if (input >= 'A' && input <= 'Z') {
        return input - 'A'; // Map 'A'-'Z' to 0-25

    // Check if the character is a lowercase letter (a-z)
    } else if (input >= 'a' && input <= 'z') {
        return input - ('a' - SMALL_A_BASE_DIFF); // Map 'a'-'z' to 26-51

    // Check if the character is a digit (0-9)
    } else if (input >= '0' && input <= '9') {
        return input - ('0' - ZERO_BASE_DIFF); // Map '0'-'9' to 52-61

    // Check if the character is '+'
    } else if (input == PLUS) {
        return PLUS_64_BASE;

    // Otherwise, treat it as '/'
    } 
    return SLASH_64_BASE;
}

/**
 * @brief Insert a Base64 character and its occurrence into a string
 * 
 * This function uses pointer arithmetic to append two characters:
 *   1. The number of occurrences converted to a Base64 character
 *   2. The character itself
 * 
 * @param code Pointer to the current end of the string
 * @param numOfOccurrences Number of occurrences of a given character
 * @param theChar The character to insert
 * @return Pointer to the position after the newly inserted characters
 */
char * insertBase64CharIntoString(char *code, char numOfOccurrences, char theChar) {
    
    // Convert occurrence count to Base64 and store at current position
    *code = getIntegerAs64Base(numOfOccurrences);
    
    // Move pointer to next position
    code++;
    
    // Store the actual character at the new position
    *code = theChar;
    
    // Move pointer past the newly stored character
    code++;
    
    // Return the pointer to the updated end of the string
    return code;
}

/**
 * @brief Decode a compressed board string into the 2D board array
 * 
 * This function reads a string encoded with Base64 counts and characters,
 * and populates the board accordingly. It stops decoding when it reaches
 * the end of the string.
 * 
 * @param code Pointer to the encoded board string
 * @param board 2D array representing the game board
 */
void decode(const char *code, char board[ROWS][COLS]) {
    const char END_OF_LINE = '/'; // Marker for end of a row
    int row = 0;
    int col = 0;

    // Process each character until the end of the string
    while (*code != '\0') {
        
        // Get the number of times to insert the next character
        int cols = get64BaseAsInteger(*code);
        
        // Move pointer to the character to insert
        code++;
        char currChar = *code;

        // Move pointer past the character
        code++;

        // Fill the board with the current character 'cols' times
        for (int i = col; cols > 0; cols--) {
            // Store current column to update 'col' after loop
            col = i;
            board[row][i++] = currChar;
        }

        // Move to the next column
        col++;

        // Check if we've reached the end of the row
        if (*code == END_OF_LINE) {
            row++;   // Move to the next row
            col = 0; // Reset column to 0
            code++;  // Move past the end-of-line marker
        }
    }
}

/**
 * @brief Encode a 2D board into a compressed string representation
 * 
 * This function converts sequences of the same character into a Base64
 * count followed by the character itself, row by row. Each row ends with
 * a special end-of-row marker ('/'), and the string ends with '\0'.
 * 
 * @param board The 2D board array to encode
 * @param rows Number of rows in the board
 * @param columns Number of columns in the board
 * @param code Pointer to the string buffer to store the encoded board
 */
void encode(const char board[ROWS][COLS], int rows, int columns, char *code) {
    
    const char END_OF_ROW = '/';     // Marker for end of a row
    const char END_OF_STRING = '\0'; // Null terminator for the string
    const int RIGHT_BOUNDARY = columns - 2; // Prevent out-of-bounds access
    int counter = 1; // Count consecutive occurrences of a character

    // Iterate through every row of the board
    for (int row = 0; row < rows; row++) {
        // Iterate through every column of the current row
        for (int col = 0; col < columns; col++) {
            
            // Ensure we do not access out-of-bounds for next column
            if (col < RIGHT_BOUNDARY) {
                // Check if current character is the same as the next
                if (board[row][col] == board[row][col + 1]) {
                    counter++; // Increment counter for consecutive characters
                } else {
                    // Insert the sequence into the encoded string
                    code = insertBase64CharIntoString(code, counter, board[row][col]);
                    counter = 1; // Reset counter
                }
            } else {
                // Handle the last columns to avoid array overflow
                if (board[row][col] == board[row][col + 1]) {
                    counter++;
                    code = insertBase64CharIntoString(code, counter, board[row][col]);
                } else {
                    // Store current and next character separately
                    code = insertBase64CharIntoString(code, counter, board[row][col]);
                    code = insertBase64CharIntoString(code, 1, board[row][col + 1]);
                }
                counter = 1; // Reset counter
                break; // End of row processing
            }
        }

        // Append end-of-row marker
        *code = END_OF_ROW;
        code++;
    }

    // Terminate the string
    *code = END_OF_STRING;
}


/**
 * @brief Main game loop that handles turns, player input, and determines game status
 * 
 * This function initializes the board, repeatedly prompts players to make moves,
 * updates and prints the board, and checks for a winner or tie after each turn.
 */
void run() {

    // Initialize the board
    char board[ROWS][COLS];
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);

    // Game state variables
    char winner = -1;       // Stores winner character, -1 if no winner yet
    char status = -1;       // Game status: 1 = winner, 0 = tie, -1 = ongoing
    int turn = 1;           // 1 for player A's turn, 0 for player B
    int col;                // Column input by the player
    char pl = 'A';          // Current player character

    // Main loop: runs until a winner or a full board
    while (winner == -1 && status == -1) {
        // Prompt current player to select a column
        printf("Enter a column: ");
        scanf("%d", &col);

        // Determine which player's turn it is
        pl = turn ? 'A' : 'B';

        // Attempt to make the move
        if (!makeMove(board, ROWS, COLS, NUM_PLAYERS, pl, col)) {
            printf("Invalid column\n");  // Invalid input, retry
            continue;
        } else {
            turn = !turn;               // Switch turn
            printBoard(board, ROWS, COLS); // Display updated board
        }

        // Check for a winner and overall game status
        winner = getWinner(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
        status = getStatus(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
    }

    // Game finished: print outcome
    if (status == 1) {
        printf("Game over\n");
    } else if (status == 0) {
        printf("Tie\n");
    } 

    // Print winner if found
    if (winner != -1) {
        printf("The winner is %c\n", winner);
    }
}

/**
 * @brief Main function
 * 
 * This is the entry point of the program. It starts the game by calling
 * the run() function which handles the game loop.
 */
int main() {
    run(); // Start the Connect-style game
    return 0; // Exit the program successfully
}
