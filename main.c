#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define STR_LIMIT 100

typedef struct gameBoard {
    char **table;
    int row;
    int column;
} gameBoard;

void createGameBoard(gameBoard *board, int row, int column);
void clearGameBoard(gameBoard *board);
void printGameBoard(gameBoard board);
int isValidLocation(char *location, gameBoard board);
int isBoardFull(gameBoard board);
void placeStone(const char *location, char value, gameBoard *board);
int countWhiteSquares(gameBoard *board);
int countBlackSquares(gameBoard *board);
int isWhiteRemoveStone(char *location, gameBoard board);
int isBlackRemoveStone(char *location, gameBoard board);
int squareCheck(const char *location, gameBoard board);
void removeStone(const char *location, gameBoard *board);
int isValidWhiteMove(char *location, gameBoard board);
int isValidBlackMove(char *location, gameBoard board);
int isJumpOverStone(const char *location, gameBoard board);
int moveWhite(const char *location, gameBoard *board);
int moveBlack(const char *location, gameBoard *board);

int main() {
    while (1) {
        int row, column, operation;

        printf("\n0 - Exit\n1 - Play the game!\n\n");
        printf("Choose operation (0-1): ");
        scanf("%d", &operation);

        if (operation == 0) {
            printf("\nThanks for playing the game!\nExiting...\n");
            exit(0);
        }
        else if (operation != 1)
            printf("Invalid operation!\n");

        else {
            while (1) {
                printf("Enter the row count of the board: ");
                scanf("%d", &row);

                if (row >= 3 && row <= 7) {
                    column = row + 1;
                    break;
                }
                printf("Invalid row count!\n");
            }

            gameBoard newGameBoard;
            createGameBoard(&newGameBoard, row, column);

            while (!isBoardFull(newGameBoard)) {
                char *whiteLocation = (char *) malloc (sizeof (char) * STR_LIMIT);
                char *blackLocation = (char *) malloc (sizeof (char) * STR_LIMIT);

                printGameBoard(newGameBoard);

                do { // Player 1 (White)
                    printf("\nPlayer 1 (White) choose location: ");
                    scanf("%s", whiteLocation);
                } while (!isValidLocation(whiteLocation, newGameBoard));

                placeStone(whiteLocation, 'W', &newGameBoard);
                printGameBoard(newGameBoard);

                do { // Player 2 (Black)
                    printf("\nPlayer 2 (Black) choose location: ");
                    scanf("%s", blackLocation);
                } while (!isValidLocation(blackLocation, newGameBoard));

                placeStone(blackLocation, 'B', &newGameBoard);

                if (isBoardFull(newGameBoard)) {
                    free(whiteLocation);
                    free(blackLocation);
                }
            }

            printGameBoard(newGameBoard);

            int whiteSquares = countWhiteSquares(&newGameBoard), blackSquares = countBlackSquares(&newGameBoard);

            int whiteStones = (newGameBoard.row * newGameBoard.column) / 2;
            int blackStones = whiteStones;

            printf("\nBoard full! Now counting squares...\n\n");
            printf("White square(s): %d\n", whiteSquares);
            printf("Black square(s): %d\n\n", blackSquares);

            int firstBlackTurn = 0;

            if (blackSquares == 0) {
                firstBlackTurn = 1;

                if (whiteSquares == blackSquares)
                    whiteSquares++;
            }

            char *whiteRemoveStone = (char *) malloc (sizeof (char) * STR_LIMIT);
            char *blackRemoveStone = (char *) malloc (sizeof (char) * STR_LIMIT);

            while (whiteSquares != 0) { // Player 1 (White) removing stones
                printGameBoard(newGameBoard);

                do {
                    printf("\nPlayer 1 (White) choose for removing a Black stone (have %d left): ", whiteSquares);
                    scanf("%s", whiteRemoveStone);
                } while (!isWhiteRemoveStone(whiteRemoveStone, newGameBoard));

                removeStone(whiteRemoveStone, &newGameBoard);
                whiteSquares--;
                blackStones--;
            }

            while (blackSquares != 0) { // Player 2 (Black) removing stones
                printGameBoard(newGameBoard);

                do {
                    printf("\nPlayer 2 (Black) choose for removing a White stone (have %d left): ", blackSquares);
                    scanf("%s", blackRemoveStone);
                } while (!isBlackRemoveStone(blackRemoveStone, newGameBoard));

                removeStone(blackRemoveStone, &newGameBoard);
                blackSquares--;
                whiteStones--;
            }

            char *whiteMove = (char *) malloc (sizeof (char) * STR_LIMIT);
            char *blackMove = (char *) malloc (sizeof (char) * STR_LIMIT);

            while (whiteStones != 3 && blackStones != 3) {
                do {
                    if (firstBlackTurn)
                        break;
                    printGameBoard(newGameBoard);

                    printf("\nPlayer 1 (White) enter move: ");
                    scanf(" %[^\n]", whiteMove);

                } while (!isValidWhiteMove(whiteMove, newGameBoard));

                if (!firstBlackTurn) {
                    if (moveWhite(whiteMove, &newGameBoard) == 2) {
                        printf("Player 1 (White) got square!\n");
                        printGameBoard(newGameBoard);
                        do {
                            printf("\nPlayer 1 (White) choose for removing a Black stone: ");
                            scanf("%s", whiteRemoveStone);
                        } while (!isWhiteRemoveStone(whiteRemoveStone, newGameBoard));

                        removeStone(whiteRemoveStone, &newGameBoard);
                        blackStones--;
                    }
                }

                firstBlackTurn = 0;

                if (whiteStones == 3 || blackStones == 3)
                    break;

                do {
                    printGameBoard(newGameBoard);

                    printf("\nPlayer 2 (Black) enter move: ");
                    scanf(" %[^\n]", blackMove);
                } while (!isValidBlackMove(blackMove, newGameBoard));

                if (moveBlack(blackMove, &newGameBoard) == 2) {
                    printf("Player 2 (Black) got square!\n");
                    printGameBoard(newGameBoard);
                    do {
                        printf("\nPlayer 2 (Black) choose for removing a White stone: ");
                        scanf("%s", blackRemoveStone);
                    } while (!isBlackRemoveStone(blackRemoveStone, newGameBoard));

                    removeStone(blackRemoveStone, &newGameBoard);
                    whiteStones--;
                }
            }

            printGameBoard(newGameBoard);

            free(whiteRemoveStone);
            free(blackRemoveStone);

            if (blackStones == 3)
                printf("\n\nPLAYER 1 (WHITE) WON!\n");

            else
                printf("\n\nPLAYER 2 (BLACK) WON!\n");

            clearGameBoard(&newGameBoard);
        }
    }
}

void createGameBoard(gameBoard *board, int row, int column) {
    char **table = (char **) malloc (sizeof (char *) * row);
    for (int i = 0; i < row; i++)
        table[i] = (char *) malloc (sizeof (char) * column);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
            table[i][j] = ' ';
    }

    board->table = table;
    board->row = row;
    board->column = column;
}

void clearGameBoard(gameBoard *board) {
    int row = board->row;

    for (int i = 0; i < row; i++)
        free(board->table[i]);

    free(board->table);
    board->row = 0;
    board->column = 0;
}

void printGameBoard(gameBoard board) {
    printf("\n");

    char ch = 'A';
    int row = board.row, column = board.column;

    for (int i = 0; i < column; i++)
        printf("  %c ", ch++);
    printf("\n");

    for (int i = 0; i < row; i++) {
        printf("%d ", i + 1);
        printf("%c", board.table[i][0]);

        for (int j = 1; j < column; j++)
            printf("---%c", board.table[i][j]);
        printf(" %d \n ", i + 1);

        for (int j = 1; j < column && i != row; j++)
            printf(" |  ");

        if (i != row) printf(" |\n");
    }

    ch = 'A';
    for (int i = 0; i < column; i++)
        printf("  %c ", ch++);

    printf("\n");
}

int isValidLocation(char *location, gameBoard board) {
    if (strlen(location) != 2) {
        printf("Invalid location code!\n");
        return 0;
    }

    location[1] = toupper(location[1]);

    if (!isdigit(location[0]) || !(location[0] >= 49 && location[0] < (49 + board.row))) {
        printf("Invalid location code!\n");
        return 0;
    }

    if (!isalpha(location[1]) || !(location[1] >= 65 && location[1] < (65 + board.column))) {
        printf("Invalid location code!\n");
        return 0;
    }

    int lineRow = location[0] - 49;
    int lineColumn = location[1] - 65;

    if (board.table[lineRow][lineColumn] != ' ') {
        printf("Location is not empty!\n");
        return 0;
    }

    return 1;
}

int isBoardFull(gameBoard board) {
    for (int i = 0; i < board.row; i++) {
        for (int j = 0; j < board.column; j++) {
            if (board.table[i][j] == ' ')
                return 0;
        }
    }
    return 1;
}

void placeStone(const char *location, char value, gameBoard *board) {
    int lineRow = location[0] - 49;
    int lineColumn= location[1] - 65;

    board->table[lineRow][lineColumn] = value;
}

int countWhiteSquares(gameBoard *board) {
    int count = 0;
    for (int i = 0; i < board->row - 1; i++) {
        for (int j = 0; j < board->column; j++) {
            if (board->table[i][j] == board->table[i][j + 1] &&
                board->table[i][j + 1] == board->table[i + 1][j + 1] &&
                board->table[i + 1][j + 1] == board->table[i + 1][j] &&
                board->table[i][j] == 'W') count++;
        }
    }
    return count;
}

int countBlackSquares(gameBoard *board) {
    int count = 0;
    for (int i = 0; i < board->row - 1; i++) {
        for (int j = 0; j < board->column; j++) {
            if (board->table[i][j] == board->table[i][j + 1] &&
                board->table[i][j + 1] == board->table[i + 1][j + 1] &&
                board->table[i + 1][j + 1] == board->table[i + 1][j] &&
                board->table[i][j] == 'B') count++;
        }
    }
    return count;
}

int isWhiteRemoveStone(char *location, gameBoard board) {
    if (strlen(location) != 2) {
        printf("Invalid location code!\n");
        return 0;
    }

    location[1] = toupper(location[1]);

    if (!isdigit(location[0]) || !(location[0] >= 49 && location[0] < (49 + board.row))) {
        printf("Invalid location code!\n");
        return 0;
    }

    if (!isalpha(location[1]) || !(location[1] >= 65 && location[1] < (65 + board.column))) {
        printf("Invalid location code!\n");
        return 0;
    }

    int lineRow = location[0] - 49;
    int lineColumn = location[1] - 65;

    if (board.table[lineRow][lineColumn] == ' ') {
        printf("Location is empty!\n");
        return 0;
    }

    if (board.table[lineRow][lineColumn] == 'W') {
        printf("You cannot remove your own stone!\n");
        return 0;
    }

    if (squareCheck(location, board)) {
        printf("You cannot remove a stone in square!\n");
        return 0;
    }

    return 1;
}

int isBlackRemoveStone(char *location, gameBoard board) {
    if (strlen(location) != 2) {
        printf("Invalid location code!\n");
        return 0;
    }

    location[1] = toupper(location[1]);

    if (!isdigit(location[0]) || !(location[0] >= 49 && location[0] < (49 + board.row))) {
        printf("Invalid location code!\n");
        return 0;
    }

    if (!isalpha(location[1]) || !(location[1] >= 65 && location[1] < (65 + board.column))) {
        printf("Invalid location code!\n");
        return 0;
    }

    int lineRow = location[0] - 49;
    int lineColumn = location[1] - 65;

    if (board.table[lineRow][lineColumn] == ' ') {
        printf("Location is empty!\n");
        return 0;
    }

    if (board.table[lineRow][lineColumn] == 'B') {
        printf("You cannot remove your own stone!\n");
        return 0;
    }

    if (squareCheck(location, board)) {
        printf("You cannot remove a stone in square!\n");
        return 0;
    }

    return 1;
}

int squareCheck(const char *location, gameBoard board) {
    int lineRow = location[0] - 49;
    int lineColumn = location[1] - 65;

    if (lineRow < 1 && lineColumn < 1) {
        if (board.table[lineRow][lineColumn] == board.table[lineRow][lineColumn + 1] &&
             board.table[lineRow][lineColumn + 1] == board.table[lineRow + 1][lineColumn + 1] &&
             board.table[lineRow + 1][lineColumn + 1] && board.table[lineRow + 1][lineColumn])
                return 1;
    }
    else if (lineRow < 1 && lineColumn >= board.column - 1) {
        if (board.table[lineRow][lineColumn] == board.table[lineRow + 1][lineColumn] &&
            board.table[lineRow + 1][lineColumn] == board.table[lineRow + 1][lineColumn - 1] &&
            board.table[lineRow + 1][lineColumn - 1] == board.table[lineRow][lineColumn - 1])
                return 1;
    }
    else if (lineRow >= board.row - 1 && lineColumn >= board.column - 1) {
        if (board.table[lineRow][lineColumn] == board.table[lineRow][lineColumn - 1] &&
            board.table[lineRow][lineColumn - 1] == board.table[lineRow - 1][lineColumn - 1] &&
            board.table[lineRow - 1][lineColumn - 1] == board.table[lineRow - 1][lineColumn])
                return 1;
    }
    else if (lineRow >= board.row - 1 && lineColumn < 1) {
        if (board.table[lineRow][lineColumn] == board.table[lineRow - 1][lineColumn] &&
            board.table[lineRow - 1][lineColumn] == board.table[lineRow - 1][lineColumn + 1] &&
            board.table[lineRow - 1][lineColumn + 1] == board.table[lineRow][lineColumn + 1])
                return 1;
    }
    else if (lineRow < 1) {
        if  ((board.table[lineRow][lineColumn] == board.table[lineRow][lineColumn + 1] &&
              board.table[lineRow][lineColumn + 1] == board.table[lineRow + 1][lineColumn + 1] &&
              board.table[lineRow + 1][lineColumn + 1] && board.table[lineRow + 1][lineColumn])
            ||
            ((board.table[lineRow][lineColumn] == board.table[lineRow + 1][lineColumn] &&
              board.table[lineRow + 1][lineColumn] == board.table[lineRow + 1][lineColumn - 1] &&
              board.table[lineRow + 1][lineColumn - 1] == board.table[lineRow][lineColumn - 1])))
                return 1;
    }
    else if (lineRow >= board.row - 1) {
        if  ((board.table[lineRow][lineColumn] == board.table[lineRow][lineColumn - 1] &&
              board.table[lineRow][lineColumn - 1] == board.table[lineRow - 1][lineColumn - 1] &&
              board.table[lineRow - 1][lineColumn - 1] == board.table[lineRow - 1][lineColumn])
            ||
            ((board.table[lineRow][lineColumn] == board.table[lineRow - 1][lineColumn] &&
              board.table[lineRow - 1][lineColumn] == board.table[lineRow - 1][lineColumn + 1] &&
              board.table[lineRow - 1][lineColumn + 1] == board.table[lineRow][lineColumn + 1])))
                return 1;
    }
    else if (lineColumn < 1) {
        if ((board.table[lineRow][lineColumn] == board.table[lineRow][lineColumn + 1] &&
            board.table[lineRow][lineColumn + 1] == board.table[lineRow + 1][lineColumn + 1] &&
            board.table[lineRow + 1][lineColumn + 1] && board.table[lineRow + 1][lineColumn])
           ||
           ((board.table[lineRow][lineColumn] == board.table[lineRow - 1][lineColumn] &&
            board.table[lineRow - 1][lineColumn] == board.table[lineRow - 1][lineColumn + 1] &&
            board.table[lineRow - 1][lineColumn + 1] == board.table[lineRow][lineColumn + 1])))
                return 1;
    }
    else if (lineColumn >= board.column - 1) {
        if ((board.table[lineRow][lineColumn] == board.table[lineRow + 1][lineColumn] &&
            board.table[lineRow + 1][lineColumn] == board.table[lineRow + 1][lineColumn - 1] &&
            board.table[lineRow + 1][lineColumn - 1] == board.table[lineRow][lineColumn - 1])
           ||
           ((board.table[lineRow][lineColumn] == board.table[lineRow][lineColumn - 1] &&
             board.table[lineRow][lineColumn - 1] == board.table[lineRow - 1][lineColumn - 1] &&
             board.table[lineRow - 1][lineColumn - 1] == board.table[lineRow - 1][lineColumn])))
                return 1;
    }
    else {
        if ((board.table[lineRow][lineColumn] == board.table[lineRow][lineColumn + 1] &&
            board.table[lineRow][lineColumn + 1] == board.table[lineRow + 1][lineColumn + 1] &&
            board.table[lineRow + 1][lineColumn + 1] && board.table[lineRow + 1][lineColumn])
           ||
           (board.table[lineRow][lineColumn] == board.table[lineRow - 1][lineColumn] &&
            board.table[lineRow - 1][lineColumn] == board.table[lineRow - 1][lineColumn + 1] &&
            board.table[lineRow - 1][lineColumn + 1] == board.table[lineRow][lineColumn + 1])
           ||
           (board.table[lineRow][lineColumn] == board.table[lineRow][lineColumn - 1] &&
            board.table[lineRow][lineColumn - 1] == board.table[lineRow - 1][lineColumn - 1] &&
            board.table[lineRow - 1][lineColumn - 1] == board.table[lineRow - 1][lineColumn])
           ||
           (board.table[lineRow][lineColumn] == board.table[lineRow + 1][lineColumn] &&
            board.table[lineRow + 1][lineColumn] == board.table[lineRow + 1][lineColumn - 1] &&
            board.table[lineRow + 1][lineColumn - 1] == board.table[lineRow][lineColumn - 1]))
                return 1;
        }

    return 0;
}

void removeStone(const char *location, gameBoard *board) {
    int lineRow = location[0] - 49;
    int lineColumn = location[1] - 65;

    board->table[lineRow][lineColumn] = ' ';
}

int isValidWhiteMove(char *location, gameBoard board) {
    if (strlen(location) != 5) {
        printf("Invalid white move code!\n");
        return 0;
    }

    if (location[2] != ' ') {
        printf("Invalid white move code!\n");
        return 0;
    }

    location[1] = toupper(location[1]);

    if (!isdigit(location[0]) || !(location[0] >= 49 && location[0] < (49 + board.row))) {
        printf("Invalid current location code!\n");
        return 0;
    }

    if (!isalpha(location[1]) || !(location[1] >= 65 && location[1] < (65 + board.column))) {
        printf("Invalid current location code!\n");
        return 0;
    }

    int currentLineRow = location[0] - 49;
    int currentLineColumn = location[1] - 65;

    if (board.table[currentLineRow][currentLineColumn] == ' ') {
        printf("Current location is empty!\n");
        return 0;
    }

    if (board.table[currentLineRow][currentLineColumn] == 'B') {
        printf("You cannot play other player's stone!\n");
        return 0;
    }

    location[4] = toupper(location[4]);

    if (!isdigit(location[3]) || !(location[3] >= 49 && location[3] < (49 + board.row))) {
        printf("Invalid next location code!\n");
        return 0;
    }

    if (!isalpha(location[4]) || !(location[4] >= 65 && location[4] < (65 + board.column))) {
        printf("Invalid next location code!\n");
        return 0;
    }

    if (location[1] != location[4] && location[0] != location[3]) {
        printf("Invalid white move!\n");
        return 0;
    }

    if (location[1] == location[4] && location[0] == location[3]) {
        printf("Current and next location are same!\n");
        return 0;
    }

    char *nextLocation = (char*) malloc (sizeof(char) * 3);
    nextLocation[3] = location[3];
    nextLocation[4] = location[4];

    int nextLineRow = location[3] - 49;
    int nextLineColumn = location[4] - 65;

    if (board.table[nextLineRow][nextLineColumn] != ' ') {
        printf("Next location is not empty!\n");
        return 0;
    }

    if (isJumpOverStone(location, board)) {
        printf("Your stone cannot jump over an another stone!\n");
        return 0;
    }

    return 1;
}

int isValidBlackMove(char *location, gameBoard board) {
    if (strlen(location) != 5) {
        printf("Invalid white move code!\n");
        return 0;
    }

    if (location[2] != ' ') {
        printf("Invalid white move code!\n");
        return 0;
    }

    location[1] = toupper(location[1]);

    if (!isdigit(location[0]) || !(location[0] >= 49 && location[0] < (49 + board.row))) {
        printf("Invalid current location code!\n");
        return 0;
    }

    if (!isalpha(location[1]) || !(location[1] >= 65 && location[1] < (65 + board.column))) {
        printf("Invalid current location code!\n");
        return 0;
    }

    int currentLineRow = location[0] - 49;
    int currentLineColumn = location[1] - 65;

    if (board.table[currentLineRow][currentLineColumn] == ' ') {
        printf("Current location is empty!\n");
        return 0;
    }

    if (board.table[currentLineRow][currentLineColumn] == 'W') {
        printf("You cannot play other player's stone!\n");
        return 0;
    }

    location[4] = toupper(location[4]);

    if (!isdigit(location[3]) || !(location[3] >= 49 && location[3] < (49 + board.row))) {
        printf("Invalid next location code!\n");
        return 0;
    }

    if (!isalpha(location[4]) || !(location[4] >= 65 && location[4] < (65 + board.column))) {
        printf("Invalid next location code!\n");
        return 0;
    }

    if (location[1] != location[4] && location[0] != location[3]) {
        printf("Invalid white move!\n");
        return 0;
    }

    if (location[1] == location[4] && location[0] == location[3]) {
        printf("Current and next location are same!\n");
        return 0;
    }

    int nextLineRow = location[3] - 49;
    int nextLineColumn = location[4] - 65;

    if (board.table[nextLineRow][nextLineColumn] != ' ') {
        printf("Next location is not empty!\n");
        return 0;
    }

    if (isJumpOverStone(location, board)) {
        printf("Your stone cannot jump over an another stone!\n");
        return 0;
    }

    return 1;
}

int isJumpOverStone(const char *location, gameBoard board) {
    int row = location[0] - 49, column = location[1] - 65, moveCount;

    if (location[0] == location[3]) {
        moveCount = location[4] - location[1];

        if (moveCount < 0) {
            for (int i = 1; i < (-moveCount); i++) {
                if (board.table[row][column - i] != ' ')
                    return 1;
            }
        }
        else {
            for (int i = 1; i < moveCount; i++) {
                if (board.table[row][column + i] != ' ')
                    return 1;
            }
        }
    }
    else {
        moveCount = location[3] - location[0];

        if (moveCount < 0) {
            for (int i = 1; i < (-moveCount); i++) {
                if (board.table[row - i][column] != ' ')
                    return 1;
            }
        }
        else {
            for (int i = 1; i < moveCount; i++) {
                if (board.table[row + i][column] != ' ')
                    return 1;
            }
        }
    }

    return 0;
}

int moveWhite(const char *location, gameBoard *board) {
    char *currentLocation = (char*) malloc (sizeof(char) * 3);
    char *nextLocation = (char*) malloc (sizeof(char) * 3);

    currentLocation[0] = location[0];
    currentLocation[1] = location[1];
    currentLocation[2] = '\0';

    nextLocation[0] = location[3];
    nextLocation[1] = location[4];
    nextLocation[2] = '\0';

    removeStone(currentLocation, board);
    placeStone(nextLocation, 'W', board);

    if (squareCheck(nextLocation, *board))
        return 2;

    return 1;
}

int moveBlack(const char *location, gameBoard *board) {
    char *currentLocation = (char*) malloc (sizeof(char) * 3);
    char *nextLocation = (char*) malloc (sizeof(char) * 3);

    currentLocation[0] = location[0];
    currentLocation[1] = location[1];
    currentLocation[2] = '\0';

    nextLocation[0] = location[3];
    nextLocation[1] = location[4];
    nextLocation[2] = '\0';

    removeStone(currentLocation, board);
    placeStone(nextLocation, 'B', board);

    if (squareCheck(nextLocation, *board))
        return 2;

    return 1;
}
