/**
 * @file 4InARow.h
 * @brief Header file for 4InARow.c
 */
#define ROWS 6
#define COLS 7
#define NUM_PLAYERS 2
#define CONNECT 4
#define EMPTY_POS ' '
#define INVALID_BOARD 0
#define VALID_BOARD 1

int getBottomEmptyPos(char board[ROWS][COLS],int rows, int column);
int validatePositions (int col, int row, int columns, int rows, int connect,int action);
int getNumOfConnects(char board[ROWS][COLS], int connect, int rows, int columns, int countArr[], int col, int row,char\
                     player, int directions,int originalConnect, int toValidate);
int checkForConnect(char board[ROWS][COLS], int connect, int rows, int columns,int validate,int player);
int checkForFullBoard(char board[ROWS][COLS], int columns);
int isValidPlayer (int players, int player);
int getNumOfOccurences(char board[ROWS][COLS], int rows, int columns, char player);
int validatePlayTimes(char board[ROWS][COLS], int rows, int columns, int players);
int validatePlayMoves(char board[ROWS][COLS], int rows, int columns, int lastPlayed, int numOfOccur, int players);
int validatePlays(char board[ROWS][COLS], int rows, int columns, int players);
int get64BaseAsInteger(char input);

void initBoard(char board[ROWS][COLS], int rows, int columns);
void printBoard(char board[ROWS][COLS], int rows, int columns);
int makeMove(char board[ROWS][COLS], int rows, int columns, int players, char player, int column);
int undoMove(char board[ROWS][COLS], int rows, int columns, int column);
int getStatus(char board[ROWS][COLS], int rows, int columns, int players, int connect);
char getWinner(char board[ROWS][COLS], int rows, int columns, int players, int connect);
int isValidBoard(char board[ROWS][COLS], int rows, int columns, int players, int connect);
void encode(const char board[ROWS][COLS], int rows, int cols, char *code);
void decode(const char *code, char board[ROWS][COLS]);