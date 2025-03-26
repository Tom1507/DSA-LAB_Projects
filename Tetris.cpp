#include <iostream>
#include <cstdlib>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

const int BOARD_MAX_WIDTH = 40;
const int BOARD_MAX_HEIGHT = 20;

using namespace std;

// Define the shapes
bool Shapes[4][4][4][4] = {
    // I
    {
        {{0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}},
        {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}},
        {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}
    },
    // L
    {
        {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}},
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}}
    },
    // N
    {
        {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
        {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
        {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}}
    },
    // Square
    {
        {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}}
    }
};

// Mac compatibility functions
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void setColor(int ForgC) {
    printf("\033[3%dm", ForgC % 8); // Basic 8 colors
}

int getch_mac() {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

bool kbhit_mac() {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ioctl(STDIN_FILENO, FIONREAD, &ch);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch != 0;
}

void printMargin(bool **board) {
    setColor(0);
    char block = 219;
    for (int i = 0; i <= BOARD_MAX_WIDTH; i++) {
        gotoxy(i, 0);
        cout << block;
        gotoxy(i, BOARD_MAX_HEIGHT + 1);
        cout << block;
        board[0][i] = true;
        board[BOARD_MAX_HEIGHT + 1][i] = true;
    }
    for (int j = 0; j <= BOARD_MAX_HEIGHT + 1; j++) {
        gotoxy(BOARD_MAX_WIDTH + 1, j);
        cout << block;
        board[j][BOARD_MAX_WIDTH + 1] = true;
    }
}

void printShape(bool a[4][4][4][4], int kind, int rotation, int x, int y, bool **board) {
    int startX = x;
    char block = 254;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (a[kind][rotation][i][j]) {
                gotoxy(x, y);
                cout << block;
                if (y >= 0 && y <= BOARD_MAX_HEIGHT && x >= 0 && x <= BOARD_MAX_WIDTH) {
                    board[y][x] = true;
                }
            }
            x++;
        }
        y++;
        x = startX;
    }
}

void deleteShape(bool a[4][4][4][4], int kind, int rotation, int x, int y, bool **board) {
    int startX = x;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (a[kind][rotation][i][j]) {
                gotoxy(x, y);
                cout << " ";
                if (y >= 0 && y <= BOARD_MAX_HEIGHT && x >= 0 && x <= BOARD_MAX_WIDTH) {
                    board[y][x] = false;
                }
            }
            x++;
        }
        y++;
        x = startX;
    }
}

void gameExit() {
    system("clear");
    gotoxy(10, 10);
    cout << "********** OUCH !!  **********";
    usleep(1500000); // microseconds
    system("clear");
    char sound = 7;
    setColor(0);
    for (int i = 0, j = 0; j < 20 && i < 40; i++, j++) {
        gotoxy(i, j);
        cout << "**********LOSER***********";
        gotoxy(i, j + 2);
        cout << "*******Game is Over*******";
        setColor(0);
        usleep(50000);
        system("clear");
    }
    gotoxy(20, 20);
    cout << "**********LOSER***********";
    gotoxy(20, 22);
    cout << "*******Game is Over*******";
    getchar();
    exit(1);
}

bool checkCollision(bool a[4][4][4][4], int kind, int rotation, int x, int y, bool **board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (a[kind][rotation][i][j]) {
                int boardX = x + j;
                int boardY = y + i;
                if (boardY > BOARD_MAX_HEIGHT || board[boardY][boardX]) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkFreeRight(bool a[4][4][4][4], int kind, int rotation, int x, int y, bool **board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (a[kind][rotation][i][j]) {
                int boardX = x + j + 1;
                int boardY = y + i;
                if (boardX > BOARD_MAX_WIDTH || board[boardY][boardX]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool checkFreeLeft(bool a[4][4][4][4], int kind, int rotation, int x, int y, bool **board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (a[kind][rotation][i][j]) {
                int boardX = x + j - 1;
                int boardY = y + i;
                if (boardX < 0 || board[boardY][boardX]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool checkFreeDown(bool a[4][4][4][4], int kind, int rotation, int x, int y, bool **board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (a[kind][rotation][i][j]) {
                int boardX = x + j;
                int boardY = y + i + 1;
                if (boardY > BOARD_MAX_HEIGHT || board[boardY][boardX]) {
                    return false;
                }
            }
        }
    }
    return true;
}

void gameHeadStart() {
    setColor(6);
    char sound = 7;
    char pointer = 175;
    gotoxy(20, 10);
    cout << " ---------->  WELCOME  <----------";
    usleep(2000000);
    system("clear");
    gotoxy(0, 0);
    cout << pointer << "  This is the Tetris game made by -> ATTA JIROFTY <- " << endl;
    usleep(1000000);
    gotoxy(0, 3);
    cout << pointer << "  This game is protected by ** Copyright ( C ) **" << endl;
    usleep(1000000);
    gotoxy(0, 6);
    cout << pointer << "  Please press a key to start the game." << endl;
    getchar();
    gotoxy(0, 8);
    cout << "************************GET READY************************" << endl;
    gotoxy(10, 10);
    cout << '1' << sound << endl;
    usleep(1000000);
    gotoxy(10, 12);
    cout << '2' << sound << endl;
    usleep(1000000);
    gotoxy(10, 14);
    cout << '3' << sound << endl;
    usleep(1000000);
    cout << sound << sound << sound;
    system("clear");
    gotoxy(20, 10);
    cout << "*************** -> GO GO GO <-   ***************";
    usleep(1000000);
}

void gameHelp() {
    char pointer = 175;
    char design = 178;
    gotoxy(BOARD_MAX_WIDTH + 3, 0);
    cout << pointer << "  Use 'w' key for up (rotate)";
    gotoxy(BOARD_MAX_WIDTH + 3, 1);
    cout << pointer << "  Use 's' key for down (faster drop)";
    gotoxy(BOARD_MAX_WIDTH + 3, 2);
    cout << pointer << "  Use 'a' key for left";
    gotoxy(BOARD_MAX_WIDTH + 3, 3);
    cout << pointer << "  Use 'd' key for right";
    gotoxy(BOARD_MAX_WIDTH + 3, 4);
    cout << pointer << "  Use 'enter' for Ending the game";
    gotoxy(BOARD_MAX_WIDTH + 3, 7);
    cout << design << "  ENJOY THE GAME  " << design;
    gotoxy(0, BOARD_MAX_HEIGHT + 3);
    cout << "Made in Yazd University - Computer Engineering Faculty ";
}

void levelPrint(int score) {
    gotoxy(BOARD_MAX_WIDTH + 3, 12);
    if (score < 200) {
        cout << "LEVEL 1      ";
    } else if (score < 500) {
        cout << "LEVEL 2      ";
    } else if (score < 800) {
        cout << "LEVEL 3      ";
    } else {
        cout << "LEVEL 4 BRAVO!!";
    }
}

void clearFilledLines(bool **board, int &score) {
    for (int i = 1; i <= BOARD_MAX_HEIGHT; i++) {
        bool filled = true;
        for (int j = 1; j <= BOARD_MAX_WIDTH; j++) {
            if (!board[i][j]) {
                filled = false;
                break;
            }
        }
        if (filled) {
            score += 100;
            for (int k = i; k > 1; k--) {
                for (int l = 1; l <= BOARD_MAX_WIDTH; l++) {
                    board[k][l] = board[k - 1][l];
                }
            }
            for (int l = 1; l <= BOARD_MAX_WIDTH; l++) {
                board[1][l] = false;
            }
            // Redraw the board
            for (int r = 1; r <= BOARD_MAX_HEIGHT; r++) {
                for (int c = 1; c <= BOARD_MAX_WIDTH; c++) {
                    gotoxy(c, r);
                    if (board[r][c]) {
                        cout << (char)254;
                    } else {
                        cout << " ";
                    }
                }
            }
        }
    }
}

int main() {
    char ch1 = 0;
    gameHeadStart();
    bool is_free = true;
    bool is_over = false;
    int score = 0;
    int sleep_time = 100000; // microseconds (0.1 seconds)
    int fast_forward_sleep = 10000; // microseconds (0.01 seconds)
    system("clear");
    bool **board = new bool *[BOARD_MAX_HEIGHT + 2];
    for (int i = 0; i <= BOARD_MAX_HEIGHT + 1; i++)
        board[i] = new bool[BOARD_MAX_WIDTH + 2];
    for (int i = 0; i <= BOARD_MAX_HEIGHT + 1; i++) {
        for (int j = 0; j <= BOARD_MAX_WIDTH + 1; j++)
            board[i][j] = false;
    }
    printMargin(board);
    gameHelp();
    system("clear && printf '\\e[35m'"); // Set color to magenta

    srand(time(0));

    while (true) {
        gotoxy(BOARD_MAX_WIDTH + 3, 10);
        cout << "SCORE :  " << score;
        if (score > 200)
            sleep_time = 70000;
        if (score > 500)
            sleep_time = 40000;
        if (score > 800)
            sleep_time = 30000;
        levelPrint(score);
    }
}
