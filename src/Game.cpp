#include "Game.h"
#include <iostream>
#include <limits>

Game::Game() : currentPlayer('X'), gameOver(false), winner(' ') {}

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
        
        validMove = board.dropPiece(column, currentPlayer);
        
        if (!validMove) {
            std::cout << "Column is full or invalid! Try again.\n";
        }
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
}

bool Game::isGameOver() const {
    return gameOver;
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
