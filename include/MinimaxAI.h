#ifndef MINIMAXAI_H
#define MINIMAXAI_H

#include "AIPlayer.h"
#include <vector>
#include <limits>

/**
 * Minimax AI player with alpha-beta pruning
 * Provides "Medium" and "Hard" difficulty options based on search depth
 */
class MinimaxAI : public AIPlayer {
public:
    /**
     * Constructor
     * @param depth Search depth for minimax algorithm (1-8 recommended)
     * @param aiPlayer Character representing the AI player ('X' or 'O')
     */
    MinimaxAI(int depth = 4, char aiPlayer = 'O');
    ~MinimaxAI() override = default;
    
    /**
     * Selects the best move using minimax algorithm with alpha-beta pruning
     * @param board The current game board
     * @return Column index with the best evaluated move
     */
    int selectMove(const Board& board) override;
    
private:
    int depth;
    char aiPlayer;
    char humanPlayer;
    
    /**
     * Core minimax algorithm with alpha-beta pruning
     * @param board The game board (will be modified during search)
     * @param currentDepth Current depth in the search tree
     * @param alpha Best value for maximizer
     * @param beta Best value for minimizer
     * @param isMaximizing True if maximizing player's turn
     * @return Evaluated score for this position
     */
    int minimax(Board& board, int currentDepth, int alpha, int beta, bool isMaximizing);
    
    /**
     * Evaluates the board position using heuristics
     * @param board The game board to evaluate
     * @return Score for the position (positive favors AI, negative favors opponent)
     */
    int evaluateBoard(const Board& board);
    
    /**
     * Evaluates a window of 4 cells
     * @param window Array of 4 cells to evaluate
     * @return Score for this window
     */
    int evaluateWindow(const char window[4]);
    
    /**
     * Gets list of valid column indices (non-full columns)
     * @param board The game board
     * @return Vector of valid column indices
     */
    std::vector<int> getValidMoves(const Board& board);
    
    /**
     * Counts pieces of a specific player in a window
     * @param window Array of 4 cells
     * @param player Player character to count
     * @return Number of player's pieces in window
     */
    int countPieces(const char window[4], char player);
    
    /**
     * Checks if a window has empty cells
     * @param window Array of 4 cells
     * @return True if window contains at least one empty cell
     */
    bool hasEmpty(const char window[4]);
};

#endif // MINIMAXAI_H
