# Connect 4 (Puissance 4) - C++

A classic Connect 4 game implemented in C++ that runs in the console.

## Game Rules

Connect 4 is a two-player connection game where players take turns dropping colored pieces into a 7-column, 6-row vertically suspended grid. The objective is to be the first to form a horizontal, vertical, or diagonal line of four pieces.

- **Player 1**: X
- **Player 2**: O
- Players alternate turns
- First player to connect 4 pieces wins
- If the board fills up without a winner, it's a draw

## Prerequisites

- CMake 3.10 or higher
- C++ compiler with C++11 support
  - GCC 4.8+ (Linux)
  - MSVC 2015+ (Windows)
  - Clang 3.4+

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
2. The game displays a 6x7 grid
3. Players take turns entering a column number (1-7)
4. The piece drops to the lowest available position in that column
5. First player to connect 4 pieces horizontally, vertically, or diagonally wins!

## Project Structure

```
connect4-cpp/
├── CMakeLists.txt       # CMake build configuration
├── README.md            # This file
├── include/             # Header files
│   ├── Board.h         # Board class declaration
│   └── Game.h          # Game class declaration
├── src/                # Source files
│   ├── Board.cpp       # Board implementation
│   ├── Game.cpp        # Game implementation
│   └── main.cpp        # Entry point
├── build/              # Build directory (generated)
└── .github/
    └── workflows/
        └── build.yml   # CI/CD configuration
```

## CI/CD

This project uses GitHub Actions for continuous integration. On every push:
- Builds the project on Ubuntu and Windows
- Uploads the compiled executable as an artifact

You can download pre-built executables from the Actions tab in the GitHub repository.

## License

This project is open source and available for educational purposes.
