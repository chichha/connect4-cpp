#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "AIPlayer.h"
#include <memory>

enum class GameMode {
    PLAYER_VS_PLAYER,
    PLAYER_VS_AI,
    NETWORK_HOST,
    NETWORK_CLIENT
};

enum class AIDifficulty {
    EASY,    // Random AI
    MEDIUM,  // Minimax depth 4
    HARD     // Minimax depth 6
};

class Game {
public:
    Game();
    
    // Game configuration
    void setGameMode(GameMode mode);
    void setAIDifficulty(AIDifficulty difficulty);
    void setMinimaxDepth(int depth);
    GameMode getGameMode() const;
    bool isAITurn() const;
    
    // Game logic methods (UI-independent)
    bool makeMove(int column);
    void reset();
    char getCurrentPlayer() const;
    bool isGameOver() const;
    char getWinner() const;
    const Board& getBoard() const;
    
    // AI-specific methods
    int getAIMove();
    void makeAIMove();
    
    // Console-specific methods (kept for backward compatibility)
    void start();
    void playTurn();
    void displayWinner() const;
    
    // Network synchronization methods
    std::string serializeGameState() const;
    bool deserializeGameState(const std::string& stateStr);  // Note: Simplified version validates format only
    bool validateMove(int column, char player) const;
    
private:
    Board board;
    char currentPlayer;
    bool gameOver;
    char winner;
    
    // AI configuration
    GameMode gameMode;
    AIDifficulty aiDifficulty;
    int minimaxDepth;
    std::unique_ptr<AIPlayer> aiPlayer;
    char aiPlayerChar; // 'O' for Player 2 by default
    
    void switchPlayer();
    int getPlayerMove() const;
    void initializeAI();
};

#endif // GAME_H
