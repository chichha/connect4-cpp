#ifndef RANDOMAI_H
#define RANDOMAI_H

#include "AIPlayer.h"
#include <random>

/**
 * Random AI player that selects moves randomly
 * This serves as the "Easy" difficulty option
 */
class RandomAI : public AIPlayer {
public:
    RandomAI();
    ~RandomAI() override = default;
    
    /**
     * Selects a random valid move from all available columns
     * @param board The current game board
     * @return A randomly selected valid column index
     */
    int selectMove(const Board& board) override;
    
private:
    std::random_device rd;
    std::mt19937 gen;
};

#endif // RANDOMAI_H
