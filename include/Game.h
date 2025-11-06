#ifndef GAME_H
#define GAME_H

#include "Board.h"

class Game {
public:
    Game();
    
    // Game logic methods (UI-independent)
    bool makeMove(int column);
    void reset();
    char getCurrentPlayer() const;
    bool isGameOver() const;
    char getWinner() const;
    const Board& getBoard() const;
    
    // Console-specific methods (kept for backward compatibility)
    void start();
    void playTurn();
    void displayWinner() const;
    
private:
    Board board;
    char currentPlayer;
    bool gameOver;
    char winner;
    
    void switchPlayer();
    int getPlayerMove() const;
};

#endif // GAME_H
