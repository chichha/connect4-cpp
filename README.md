# Connect 4 (Puissance 4) - C++

A classic Connect 4 game implemented in C++ with a graphical user interface using SDL2.

## Game Rules

Connect 4 is a two-player connection game where players take turns dropping colored pieces into a 7-column, 6-row vertically suspended grid. The objective is to be the first to form a horizontal, vertical, or diagonal line of four pieces.

- **Player 1**: X (Red pieces)
- **Player 2**: O (Yellow pieces)
- Players alternate turns
- First player to connect 4 pieces wins
- If the board fills up without a winner, it's a draw

## Prerequisites

- CMake 3.10 or higher
- C++ compiler with C++17 support
  - GCC 7+ (Linux)
  - MSVC 2017+ (Windows)
  - Clang 5+
- SDL2 development libraries
- SDL2_ttf development libraries

## Installing SDL2 Dependencies

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-ttf-dev
```

### Linux (Fedora/RHEL)

```bash
sudo dnf install SDL2-devel SDL2_ttf-devel
```

### macOS

```bash
brew install sdl2 sdl2_ttf
```

### Windows

Download and install SDL2 development libraries from:
- SDL2: https://www.libsdl.org/download-2.0.php
- SDL2_ttf: https://www.libsdl.org/projects/SDL_ttf/

## Building the Game

### Linux / macOS

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Run the game
./connect4
```

### Windows

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --config Release

# Run the game
.\Release\connect4.exe
```

## How to Play

1. Run the executable (`connect4` or `connect4.exe`)
2. **Game Mode Selection Screen**:
   - Choose "Player vs Player" for two-player mode
   - Choose "Player vs AI" to play against the computer
   - If playing against AI, select difficulty (Easy, Medium, or Hard)
   - For Hard mode, adjust the minimax search depth using the slider (1-8)
   - Click "Start Game" to begin
3. A window will open showing the Connect 4 board
4. Click on a column to drop your piece
5. The piece will fall to the lowest available position
6. Players alternate turns (AI moves automatically on its turn)
7. First player to connect 4 pieces horizontally, vertically, or diagonally wins!
8. Click "New Game" to restart with the same settings
9. Click "Back" to return to mode selection
10. Click "Quit" to exit the application

## Features

- **Graphical User Interface**: Beautiful SDL2-based GUI with visual feedback
- **Game Mode Selection**: Choose between Player vs Player or Player vs AI
- **AI Opponents**: Three difficulty levels:
  - **Easy**: Random AI that makes random valid moves
  - **Medium**: Minimax AI with depth 4 for strategic gameplay
  - **Hard**: Minimax AI with configurable depth (1-8) for advanced challenge
- **Mouse Controls**: Click-based column selection and menu navigation
- **Visual Feedback**: Column highlighting on hover, clear player turn indicator
- **Win Detection**: Automatic win/draw detection with visual display
- **Game Controls**: New Game, Back to Menu, and Quit buttons for easy game management

## Project Structure

```
connect4-cpp/
├── CMakeLists.txt       # CMake build configuration
├── README.md            # This file
├── include/             # Header files
│   ├── Board.h         # Board class declaration
│   ├── Game.h          # Game logic class declaration
│   ├── GameUI.h        # SDL2 UI class declaration
│   ├── AIPlayer.h      # AI player base interface
│   ├── RandomAI.h      # Random AI player (Easy difficulty)
│   └── MinimaxAI.h     # Minimax AI player (Medium/Hard difficulty)
├── src/                # Source files
│   ├── Board.cpp       # Board implementation
│   ├── Game.cpp        # Game logic implementation
│   ├── GameUI.cpp      # SDL2 UI implementation
│   ├── RandomAI.cpp    # Random AI implementation
│   ├── MinimaxAI.cpp   # Minimax AI implementation with alpha-beta pruning
│   └── main.cpp        # Entry point
├── build/              # Build directory (generated)
└── .github/
    └── workflows/
        └── build.yml   # CI/CD configuration
```

## CI/CD

This project uses GitHub Actions for continuous integration. On every push:
- Builds the project on Ubuntu and Windows
- Installs SDL2 dependencies automatically
- Uploads the compiled executable as an artifact

You can download pre-built executables from the Actions tab in the GitHub repository.

## License

This project is open source and available for educational purposes.
