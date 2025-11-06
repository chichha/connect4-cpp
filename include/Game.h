#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <string>

class Game {
public:
    Game();
    
    void start();
    void playTurn();
    bool isGameOver() const;
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
