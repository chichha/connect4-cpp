#include "RandomAI.h"
#include <vector>

RandomAI::RandomAI() : gen(rd()) {}

int RandomAI::selectMove(const Board& board) {
    // Build list of valid columns (not full)
    std::vector<int> validMoves;
    
    for (int col = 0; col < Board::COLS; col++) {
        if (!board.isColumnFull(col)) {
            validMoves.push_back(col);
        }
    }
    
    // If no valid moves (board is full), return -1
    // This shouldn't happen in normal gameplay as game should end before board fills
    if (validMoves.empty()) {
        return -1;
    }
    
    // Select a random move from valid moves
    std::uniform_int_distribution<> dis(0, validMoves.size() - 1);
    int randomIndex = dis(gen);
    
    return validMoves[randomIndex];
}
