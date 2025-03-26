# Tetris Game

## Features
- **Classic Tetris Gameplay**: Rotate, move, and drop tetrominoes to form complete lines.
- **Multiple Tetromino Shapes**: Includes I, L, N, and Square shapes.
- **Collision Detection**: Ensures blocks fit within the board and stack properly.
- **Scoring System**: Earn points by completing lines.
- **Level Progression**: The game speeds up as you score higher.
- **Simple ASCII Graphics**: Runs entirely in the terminal.
- **Cross-Platform Compatibility**: Designed for UNIX-like systems (Linux/macOS).

## How to Play
1. Start the game by running the compiled executable.
2. Use the following controls to play:
   - `W` - Rotate the shape
   - `A` - Move left
   - `D` - Move right
   - `S` - Move down faster
   - `Enter` - End the game
3. Try to fit the tetrominoes to complete horizontal lines.
4. Each completed line increases your score.
5. As your score increases, the game speeds up.
6. The game ends when a new shape cannot be placed without collision.

## Requirements
- C++ compiler (G++ recommended)
- UNIX-like terminal (Linux/macOS)

## Compilation & Execution
To compile the game, run the following command in the terminal:
```sh
 g++ tetris.cpp -o tetris
```

Then, execute the game using:
```sh
 ./tetris
```

## Notes
- Windows users may need to modify terminal-specific functions (like `gotoxy` and `kbhit`).
- The game automatically increases speed as the score increases.
- The game ends when a shape cannot be placed without collision.

## Author
This project was created by **Atta Jirofty** as a simple terminal-based Tetris implementation.

## License
This project is protected by copyright. Use for educational purposes only.
