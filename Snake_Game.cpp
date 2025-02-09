#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include<conio.h>
#include<windows.h>

#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif
using namespace std;

const int GRID_SIZE = 50;  

#ifdef _WIN32
bool kbhit(){
    return _kbhit();
}

bool getKey(){
    return _getch();
}

void sleep_ms(int milliseconds){
    Sleep(milliseconds);
}

#else
bool kbhit() {
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
        return true;
    }

    return false;
}

int getkey() {
    int key;
    key = getchar();
    return key;
}

void sleep_ms(int milliseconds){
    usleep(milliseconds*1000);
}
#endif

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    cout << "\033[H\033[J"; // ANSI CODE FOR CLEARING SCREEN
    #endif
}

class Snake {
public:
    vector<pair<int, int> > body;
    char direction;

    Snake() {
        body.emplace_back(GRID_SIZE / 2, GRID_SIZE / 2);
        body.emplace_back(GRID_SIZE / 2, GRID_SIZE / 2 - 1);
        body.emplace_back(GRID_SIZE / 2, GRID_SIZE / 2 - 2);
        direction = 'R';
    }

    void movement(bool size_increase) {
        pair<int, int> new_head = body[0];

        if (direction == 'U') new_head.first--;
        else if (direction == 'D') new_head.first++;
        else if (direction == 'R') new_head.second++;
        else if (direction == 'L') new_head.second--;

        body.insert(body.begin(), new_head);

        if (!size_increase) {
            body.pop_back();
        }
    }

    bool checkForCollision() {
        pair<int, int> head = body[0];
        if (head.first < 0 || head.first >= GRID_SIZE || head.second < 0 || head.second >= GRID_SIZE) {
            return true;
        }

        for (int i = 1; i < body.size(); i++) {
            if (body[i] == head) {
                return true;
            }
        }
        return false;
    }
};

class Game {
public:
    Snake snake;
    pair<int, int> food;
    int score;
    int high_score;
    bool game_over;

    Game() {
        score = 0;
        high_score = 0;
        game_over = false;
        generateFood();
    }

    void generateFood() {
        srand(time(0));
        bool pos = false;
        while (!pos) {
            food.first = rand() % GRID_SIZE;
            food.second = rand() % GRID_SIZE;
            pos = true;
            for (auto part : snake.body) {
                if (part == food) {
                    pos = false;
                    break;
                }
            }
        }
    }

    void board() {
        clearScreen();
        cout << "\033[40m"; // Set background to black
        for (int i = 0; i <= GRID_SIZE + 1; i++) {
            for (int j = 0; j <= GRID_SIZE + 1; j++) {
                if (i == 0 || i == GRID_SIZE + 1) {
                    cout << "-";
                } else if (j == 0 || j == GRID_SIZE + 1) {
                    cout << "|";
                } else {
                    bool isSnake = false;
                    for (auto part : snake.body) {
                        if (part.first == i - 1 && part.second == j - 1) {
                            cout << "\033[97mO\033[0m"; // White Snake
                            isSnake = true;
                            break;
                        }
                    }
                    if (!isSnake) {
                        if (food.first == i - 1 && food.second == j - 1) {
                            cout << "\033[97m*\033[0m"; // White food
                        } else {
                            cout << "\033[40m \033[0m"; // ensuring black background
                        }
                    }
                }
            }
            cout << endl;
        }
        cout << "\033[0m"; // Reset colors
        cout << "Score: " << score << "  High Score: " << high_score << endl;
    }

    void keyboardInput() {
        if (kbhit()) {
            char key = getkey();

            if ((key == 'w' || key == 'W') && snake.direction != 'D') snake.direction = 'U';
            else if ((key == 's' || key == 'S') && snake.direction != 'U') snake.direction = 'D';
            else if ((key == 'd' || key == 'D') && snake.direction != 'L') snake.direction = 'R';
            else if ((key == 'a' || key == 'A') && snake.direction != 'R') snake.direction = 'L';
        }
    }

    void updateGame() {
        keyboardInput();

        bool size_increase = (snake.body[0] == food);
        if (size_increase) {
            score += 10;
            high_score = max(score, high_score);
            generateFood();
        }

        snake.movement(size_increase);
        board();

        if (snake.checkForCollision()) {
            if (gameOver()) {
                restartGame();
            } else {
                game_over = true;
            }
        }

        sleep_ms(100); 
    }

    bool gameOver() {
        clearScreen();
        cout << " * * * * * * * * * * * * * * * " << endl << endl;
        cout << "- - - GAME OVER - - -" << endl << endl;
        cout << " * * * * * * * * * * * * * * * " << endl << endl;
        cout << "Final Score: " << score << "    High Score: " << high_score << endl << endl;
        cout << "Press Q to quit the game or R to restart the game..." << endl;

        while (true) {
            char choice = getkey();

            if (choice == 'r' || choice == 'R') {
                return true;
            } else if (choice == 'Q' || choice == 'q') {
                return false;
            }
        }
    }

    void restartGame() {
        snake = Snake();
        score = 0;
        game_over = false;
        generateFood();
    }
};

void startScreen() {
    cout << " - - - - Welcome to Snake Game - - - - " << endl << endl;
    cout << "The controls are: W = Up, S = Down, D = Right, A = Left" << endl;
    cout << "If the snake collides with the grid or itself, the game is over." << endl << endl;
    cout << "Press any key to continue...";
    getkey();
}

int main() {
    startScreen();
    Game game;

    while (true) {
        while (!game.game_over) {
            game.updateGame();
        }

        if (!game.gameOver()) {
            break;
        }

        game.restartGame();
    }

    return 0;
}