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
   - Choose "Player vs Player" for two-player mode on same computer
   - Choose "Player vs AI" to play against the computer
   - Choose "Host Game" to start a LAN multiplayer game as server
   - Choose "Join Game" to connect to another player's hosted game
3. **For LAN Multiplayer**:
   - **Host**: Click "Host Game" - your IP address will be displayed. Share this with your opponent.
   - **Join**: Click "Join Game", then "Connect" to join using default IP (127.0.0.1 for same machine)
   - Once both players are connected, the game starts automatically
4. **For AI Mode**: Select difficulty (Easy, Medium, or Hard)
   - For Hard mode, adjust the minimax search depth using the slider (1-8)
   - Click "Start Game" to begin
5. A window will open showing the Connect 4 board
6. Click on a column to drop your piece
7. The piece will fall to the lowest available position
8. Players alternate turns (AI moves automatically on its turn)
9. First player to connect 4 pieces horizontally, vertically, or diagonally wins!
10. Click "New Game" to restart with the same settings
11. Click "Back" to return to mode selection
12. Click "Quit" to exit the application

## Features

- **Graphical User Interface**: Beautiful SDL2-based GUI with visual feedback
- **Game Mode Selection**: Choose between Player vs Player, Player vs AI, or LAN Multiplayer
- **LAN Multiplayer**: Play over local network with TCP/IP connection
  - **Host Game**: Start a server and wait for opponent to connect
  - **Join Game**: Connect to a hosted game using server IP address
  - Real-time game state synchronization
  - Cross-platform network support (Windows, Linux, macOS)
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
│   ├── MinimaxAI.h     # Minimax AI player (Medium/Hard difficulty)
│   ├── NetworkManager.h      # Network socket manager (TCP)
│   ├── NetworkProtocol.h     # Network message protocol
│   ├── GameServer.h          # LAN multiplayer server
│   └── GameClient.h          # LAN multiplayer client
├── src/                # Source files
│   ├── Board.cpp       # Board implementation
│   ├── Game.cpp        # Game logic implementation
│   ├── GameUI.cpp      # SDL2 UI implementation
│   ├── RandomAI.cpp    # Random AI implementation
│   ├── MinimaxAI.cpp   # Minimax AI implementation with alpha-beta pruning
│   ├── NetworkManager.cpp    # Cross-platform networking
│   ├── NetworkProtocol.cpp   # Message serialization
│   ├── GameServer.cpp        # Server implementation
│   ├── GameClient.cpp        # Client implementation
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

## LAN Multiplayer Guide

### Network Requirements
- Both players must be on the same local network (LAN)
- Default port: 4444 (ensure this port is not blocked by firewall)
- For testing on same computer, use IP: 127.0.0.1 (localhost)

### How to Find Your IP Address

**Windows:**
```cmd
ipconfig
```
Look for "IPv4 Address" under your active network adapter.

**Linux/macOS:**
```bash
ifconfig
```
Or:
```bash
ip addr show
```
Look for your local network IP (usually starts with 192.168.x.x or 10.x.x.x).

### Firewall Configuration
If connection fails, you may need to allow the game through your firewall:

**Windows Firewall:**
1. Control Panel → System and Security → Windows Defender Firewall
2. Click "Allow an app through Windows Firewall"
3. Click "Change settings" → "Allow another app"
4. Browse to connect4.exe and add it

**Linux (ufw):**
```bash
sudo ufw allow 4444/tcp
```

**macOS:**
System Preferences → Security & Privacy → Firewall Options → Add connect4 to allowed apps

### Troubleshooting
- **Connection refused**: Ensure the host has started a game and the IP address is correct
- **Connection timeout**: Check firewall settings on both machines
- **Port already in use**: Another instance might be running. Close it and try again
- **Game disconnected**: Network interruption occurred. Both players should return to menu and reconnect
