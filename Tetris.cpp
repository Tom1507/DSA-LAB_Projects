#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <ncurses.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

const int WIDTH = 12;  
const int HEIGHT = 24; 


int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

class Tetris {
private:
    const vector<vector<vector<int>>> SHAPES = {
        {{1,1,1,1}},           // I-piece
        {{1,1},{1,1}},         // O-piece
        {{1,1,1},{0,1,0}},     // T-piece
        {{1,1,1},{1,0,0}},     // L-piece
        {{1,1,1},{0,0,1}},     // J-piece
        {{1,1,0},{0,1,1}},     // Z-piece
        {{0,1,1},{1,1,0}}      // S-piece
    };

    vector<vector<int>> board;
    vector<vector<int>> currentPiece;
    int currentX = 0, currentY = 0;
    int score = 0;
    random_device rd;
    mt19937 gen;

public:
    Tetris() : board(HEIGHT, vector<int>(WIDTH, 0)), gen(rd()) {
        newPiece();
    }

    bool newPiece() {
        uniform_int_distribution<> dis(0, SHAPES.size() - 1);
        currentPiece = SHAPES[dis(gen)];
        
        currentX = WIDTH / 2 - currentPiece[0].size() / 2;
        currentY = 0;

        return !checkCollision();
    }

    bool checkCollision() {
        for (size_t y = 0; y < currentPiece.size(); ++y) {
            for (size_t x = 0; x < currentPiece[y].size(); ++x) {
                if (currentPiece[y][x]) {
                    int boardX = currentX + x;
                    int boardY = currentY + y;

                    if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT) 
                        return true;

                    if (boardY >= 0 && board[boardY][boardX]) 
                        return true;
                }
            }
        }
        return false;
    }

    void rotatePiece() {
        auto rotated = currentPiece;
        //reverse(rotated.begin(), rotated.end());
        
        auto temp = vector<vector<int>>(
            rotated[0].size(), 
            vector<int>(rotated.size())
        );
        
        for (size_t i = 0; i < rotated.size(); ++i) {
            for (size_t j = 0; j < rotated[0].size(); ++j) {
                temp[j][rotated.size() - 1 - i] = rotated[i][j];
            }
        }
        
        auto oldPiece = currentPiece;
        currentPiece = temp;

        if (checkCollision()) 
            currentPiece = oldPiece;
    }

    bool movePiece(int dx, int dy) {
        currentX += dx;
        currentY += dy;

        if (checkCollision()) {
            currentX -= dx;
            currentY -= dy;

            if (dy > 0) {
                lockPiece();
                clearLines();
                return newPiece();
            }
            return false;
        }
        return true;
    }

    void lockPiece() {
        for (size_t y = 0; y < currentPiece.size(); ++y) {
            for (size_t x = 0; x < currentPiece[y].size(); ++x) {
                if (currentPiece[y][x] && currentY + y >= 0) {
                    board[currentY + y][currentX + x] = 1;
                }
            }
        }
    }

    void clearLines() {
        for (int y = HEIGHT - 1; y >= 0; --y) {
            if (all_of(board[y].begin(), board[y].end(), [](int cell) { return cell; })) {
                board.erase(board.begin() + y);
                board.insert(board.begin(), vector<int>(WIDTH, 0));
                score += 100;
                y++;
            }
        }
    }

    void draw(WINDOW* win) {
        wclear(win);

        // Draw board border
        box(win, 0, 0);

        // Draw board
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (board[y][x]) {
                    mvwaddch(win, y+1, x+1, '#');
                }
            }
        }

        // Draw current piece
        for (size_t y = 0; y < currentPiece.size(); ++y) {
            for (size_t x = 0; x < currentPiece[y].size(); ++x) {
                if (currentPiece[y][x]) {
                    int boardX = currentX + x;
                    int boardY = currentY + y;
                    if (boardY >= 0) {
                        mvwaddch(win, boardY+1, boardX+1, '#');
                    }
                }
            }
        }

        // Draw score
        mvwprintw(win, 0, 1, "Score: %d", score);
        
        wrefresh(win);
    }

    int getScore() const { return score; }
};

int main() {
    // Initialize ncurses
    initscr(); // start ncurses mode
    cbreak();  // proceed without waiting for enter key
    noecho();  // prevent input from appearing
    keypad(stdscr, TRUE);  // standard inputs like wasd and arrow keys 
    curs_set(0);  // do not blink the cursor
    
    // Create game window
    WINDOW* gameWin = newwin(HEIGHT + 2, WIDTH + 2, 2, 10);  // print boarder of game
    keypad(gameWin, TRUE);  
    nodelay(gameWin, TRUE);  // do not wait for input
    
    // Seed random number generator
    srand(time(NULL));  // seed random number generator
    
    Tetris game;
    
    int ch;
    bool gameOver = false;
    int dropCounter = 0;
    
    while (!gameOver) {
        // Draw the game
        game.draw(gameWin);
        
        // Handle input
        if (kbhit()) {
            ch = getchar();
            switch (ch) {
                case 'q':
                    gameOver = true;
                    break;
                case 'a':
                    game.movePiece(-1, 0);
                    break;
                case 'd':
                    game.movePiece(1, 0);
                    break;
                case ' ':
                    game.movePiece(0, 1);
                    break;
                case 'r':
                    game.rotatePiece();
                    break;
            }
        }
        
        // Automatic move down
        dropCounter++;
        if (dropCounter >= 20) {
            game.movePiece(0, 1);
            dropCounter = 0;
        }
        
        // Control game speed
        if(game.getScore() < 200 && game.getScore() >= 0){
        napms(16);
        }
        else if(game.getScore() >= 200 && game.getScore() < 500){
            napms(12);
        }
        else{
            napms(8);
        }
    }

    // Clean up ncurses
    endwin();
    
    // Display final score
    cout << "Game Over! Final Score: " << game.getScore() << endl;

    return 0;
}
