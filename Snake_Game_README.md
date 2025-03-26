# 🐍🐍 Snake Game 🐍🐍

Welcome to the **Snake Game** built using C++! This is a fun and interactive text-based game where you control the snake and try to eat food while avoiding collisions 
with the boundary wall or the snake's body itself.

---

##  **How to Play**

- **Controls**:
  - **W** = Up
  - **S** = Down
  - **A** = Left
  - **D** = Right

- **Objective**:
  - The snake grows longer each time it eats food (`0`).
  - The game ends if the snake collides with the wall or its own body.
  - Score increases by 10 for each piece of food eaten.

---

## 🎮 **Features**:

- **Randomly generated food** that appears at random positions on the grid.
- **Snake growth** after consuming food.
- **Collision detection**: Prevents the snake from running into the walls or itself.
- **High Score** tracking.

---

## 💻 **How to Run the Game**

1. **Download and Install a C++ Compiler**:
   - Make sure you have a C++ compiler installed.You can use **GCC** for MacOS.

2. **Clone or Download the Repository**:
   - You can download the source code from this repository to your local machine and enjoy playing the game offline.

3. **Compile the Code**:
   - Open your terminal or command prompt.
   - Navigate to the folder where the file is located.
   - Run the following command:
     ```
     g++ -std=c++11 -Wall -o Snake_Game Snake_Game.cpp
     ```

4. **Run the Game**:
   - After compiling, run the following command to start the game:
     ```
     ./SnakeGame
     ```
---

##  **Game Instructions**

1. **Start the Game**:
   - Press any key to start the game after the initial prompt.

2. **Game Over Screen**:
   - The game will end if you hit the wall or your body.
   - You can **Press `R` to Restart** or **Press `Q` to Quit**.

3. **Scores**:
   - Your current score and high scores will be displayed at the top of the screen.

---

##  **Customization**

The game uses ANSI escape sequences to add colors to the terminal:

- **Snake Color**: White (`O` symbol)
- **Food Color**: White (`*` symbol)
- **Background Color**: Black
- This gives a **retro-type feeling**, a good old vibe of the game we used to play back in our childhood on our switch pad mobiles.
---


## **Contributors**

- **Rushil**
- **Vishwam**
- **Lakshya**
- **Dev**

## ⚙️ **Dependencies**

- **C++ Compiler** (GCC, MinGW)
- No external libraries are required for this game.

---

## 📝 **License**

This project is licensed under the MIT License.

---

## 🔧 **Troubleshooting**

- If you face any issues with **keyboard input**, ensure your terminal supports non-blocking input handling.

---

Happy Playing! 🐍🎮
