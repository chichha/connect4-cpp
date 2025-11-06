#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Board.h"

/**
 * Abstract base class for AI players
 * Defines the interface that all AI implementations must follow
 */
class AIPlayer {
public:
    virtual ~AIPlayer() = default;
    
    /**
     * Select the best move for the current board state
     * @param board The current game board
     * @return Column index (0-6) where the AI wants to place its piece
     */
    virtual int selectMove(const Board& board) = 0;
};

#endif // AIPLAYER_H
