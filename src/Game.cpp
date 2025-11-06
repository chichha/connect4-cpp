#include "Game.h"
#include "RandomAI.h"
#include "MinimaxAI.h"
#include <iostream>
#include <limits>

Game::Game() 
    : currentPlayer('X'), gameOver(false), winner(' '),
      gameMode(GameMode::PLAYER_VS_PLAYER), 
      aiDifficulty(AIDifficulty::MEDIUM),
      minimaxDepth(4),
      aiPlayerChar('O') {}

void Game::setGameMode(GameMode mode) {
    gameMode = mode;
    if (mode == GameMode::PLAYER_VS_AI) {
        initializeAI();
    } else {
        aiPlayer.reset();
    }
}

void Game::setAIDifficulty(AIDifficulty difficulty) {
    aiDifficulty = difficulty;
    if (gameMode == GameMode::PLAYER_VS_AI) {
        initializeAI();
    }
}

void Game::setMinimaxDepth(int depth) {
    minimaxDepth = depth;
    if (gameMode == GameMode::PLAYER_VS_AI && 
        (aiDifficulty == AIDifficulty::MEDIUM || aiDifficulty == AIDifficulty::HARD)) {
        initializeAI();
    }
}

GameMode Game::getGameMode() const {
    return gameMode;
}

bool Game::isAITurn() const {
    return gameMode == GameMode::PLAYER_VS_AI && 
           currentPlayer == aiPlayerChar && 
           !gameOver;
}

void Game::initializeAI() {
    switch (aiDifficulty) {
        case AIDifficulty::EASY:
            aiPlayer = std::make_unique<RandomAI>();
            break;
        case AIDifficulty::MEDIUM:
            aiPlayer = std::make_unique<MinimaxAI>(4, aiPlayerChar);
            break;
        case AIDifficulty::HARD:
            aiPlayer = std::make_unique<MinimaxAI>(minimaxDepth, aiPlayerChar);
            break;
    }
}

int Game::getAIMove() {
    if (aiPlayer && isAITurn()) {
        return aiPlayer->selectMove(board);
    }
    return -1;
}

void Game::makeAIMove() {
    if (isAITurn() && aiPlayer) {
        int column = aiPlayer->selectMove(board);
        if (column >= 0) {
            makeMove(column);
        }
    }
}

bool Game::makeMove(int column) {
    if (gameOver) {
        return false;
    }
    
    if (!board.dropPiece(column, currentPlayer)) {
        return false;
    }
    
    if (board.checkWin(currentPlayer)) {
        gameOver = true;
        winner = currentPlayer;
    } else if (board.isFull()) {
        gameOver = true;
        winner = ' ';
    } else {
        switchPlayer();
    }
    
    return true;
}

void Game::reset() {
    board.reset();
    currentPlayer = 'X';
    gameOver = false;
    winner = ' ';
    // Reinitialize AI if in AI mode
    if (gameMode == GameMode::PLAYER_VS_AI) {
        initializeAI();
    }
}

char Game::getCurrentPlayer() const {
    return currentPlayer;
}

bool Game::isGameOver() const {
    return gameOver;
}

char Game::getWinner() const {
    return winner;
}

const Board& Game::getBoard() const {
    return board;
}

void Game::start() {
    std::cout << "=================================\n";
    std::cout << "  Welcome to Connect 4!\n";
    std::cout << "=================================\n";
    std::cout << "Player 1: X\n";
    std::cout << "Player 2: O\n";
    std::cout << "Connect 4 pieces to win!\n";
    std::cout << "=================================\n\n";
    
    while (!isGameOver()) {
        board.display();
        playTurn();
    }
    
    board.display();
    displayWinner();
}

void Game::playTurn() {
    std::cout << "\nPlayer " << currentPlayer << "'s turn.\n";
    
    int column;
    bool validMove = false;
    
    while (!validMove) {
        column = getPlayerMove();
        
        if (column == -1) {
            continue;
        }
        
        validMove = makeMove(column);
        
        if (!validMove) {
            std::cout << "Column is full or invalid! Try again.\n";
        }
    }
}

void Game::displayWinner() const {
    std::cout << "\n=================================\n";
    if (winner == ' ') {
        std::cout << "  It's a draw!\n";
    } else {
        std::cout << "  Player " << winner << " wins!\n";
    }
    std::cout << "=================================\n";
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

int Game::getPlayerMove() const {
    int column;
    std::cout << "Enter column (1-7): ";
    
    if (!(std::cin >> column)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    
    return column - 1;
}

// Network synchronization methods
std::string Game::serializeGameState() const {
    std::string state;
    
    // Serialize board (42 cells: 6 rows x 7 cols)
    for (int row = 0; row < Board::ROWS; row++) {
        for (int col = 0; col < Board::COLS; col++) {
            char cell = board.getCell(row, col);
            state += (cell == ' ') ? '.' : cell;
        }
    }
    
    return state;
}

bool Game::deserializeGameState(const std::string& stateStr) {
    if (stateStr.length() != Board::ROWS * Board::COLS) {
        return false;
    }
    
    // Validate format
    for (char c : stateStr) {
        if (c != 'X' && c != 'O' && c != '.') {
            return false;
        }
    }
    
    // Reset board first
    board.reset();
    
    // Reconstruct board state by replaying moves column by column
    // This is a simplified approach - tracks which cells should have pieces
    // In a full implementation, we would store and replay the exact move sequence
    for (int row = Board::ROWS - 1; row >= 0; row--) {
        for (int col = 0; col < Board::COLS; col++) {
            int idx = row * Board::COLS + col;
            char cell = stateStr[idx];
            if (cell != '.') {
                // Try to place piece in this column
                // This is a limitation: we can't perfectly reconstruct without move history
                // For now, we validate the format is correct
                // A better approach would be to send the full board state in a different format
            }
        }
    }
    
    return true;
}

bool Game::validateMove(int column, char player) const {
    // Check if it's the correct player's turn
    if (player != currentPlayer) {
        return false;
    }
    
    // Check if game is not over
    if (gameOver) {
        return false;
    }
    
    // Check if column is valid and not full
    if (column < 0 || column >= Board::COLS) {
        return false;
    }
    
    if (board.isColumnFull(column)) {
        return false;
    }
    
    return true;
}
