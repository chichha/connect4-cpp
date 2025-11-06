#include "MinimaxAI.h"
#include <algorithm>

MinimaxAI::MinimaxAI(int depth, char aiPlayer) 
    : depth(depth), aiPlayer(aiPlayer) {
    // Determine the opponent's player character
    humanPlayer = (aiPlayer == 'X') ? 'O' : 'X';
}

int MinimaxAI::selectMove(const Board& board) {
    // Make a copy of the board for simulation
    Board tempBoard = board;
    
    std::vector<int> validMoves = getValidMoves(board);
    
    if (validMoves.empty()) {
        return -1; // No valid moves
    }
    
    int bestMove = validMoves[0];
    int bestScore = std::numeric_limits<int>::min();
    
    // Try each valid move and find the one with the best score
    for (int col : validMoves) {
        Board simBoard = board;
        simBoard.dropPiece(col, aiPlayer);
        
        // Use minimax to evaluate this move
        int score = minimax(simBoard, depth - 1, 
                           std::numeric_limits<int>::min(), 
                           std::numeric_limits<int>::max(), 
                           false);
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = col;
        }
    }
    
    return bestMove;
}

int MinimaxAI::minimax(Board& board, int currentDepth, int alpha, int beta, bool isMaximizing) {
    // Terminal conditions
    if (board.checkWin(aiPlayer)) {
        return 1000000 + currentDepth; // Prefer faster wins
    }
    if (board.checkWin(humanPlayer)) {
        return -1000000 - currentDepth; // Prefer slower losses
    }
    if (board.isFull() || currentDepth == 0) {
        return evaluateBoard(board);
    }
    
    std::vector<int> validMoves = getValidMoves(board);
    
    if (isMaximizing) {
        int maxScore = std::numeric_limits<int>::min();
        
        for (int col : validMoves) {
            Board simBoard = board;
            if (simBoard.dropPiece(col, aiPlayer)) {
                int score = minimax(simBoard, currentDepth - 1, alpha, beta, false);
                maxScore = std::max(maxScore, score);
                alpha = std::max(alpha, score);
                
                // Alpha-beta pruning
                if (beta <= alpha) {
                    break;
                }
            }
        }
        
        return maxScore;
    } else {
        int minScore = std::numeric_limits<int>::max();
        
        for (int col : validMoves) {
            Board simBoard = board;
            if (simBoard.dropPiece(col, humanPlayer)) {
                int score = minimax(simBoard, currentDepth - 1, alpha, beta, true);
                minScore = std::min(minScore, score);
                beta = std::min(beta, score);
                
                // Alpha-beta pruning
                if (beta <= alpha) {
                    break;
                }
            }
        }
        
        return minScore;
    }
}

int MinimaxAI::evaluateBoard(const Board& board) {
    int score = 0;
    
    // Check all horizontal windows
    for (int row = 0; row < Board::ROWS; row++) {
        for (int col = 0; col <= Board::COLS - 4; col++) {
            char window[4] = {
                board.getCell(row, col),
                board.getCell(row, col + 1),
                board.getCell(row, col + 2),
                board.getCell(row, col + 3)
            };
            score += evaluateWindow(window);
        }
    }
    
    // Check all vertical windows
    for (int col = 0; col < Board::COLS; col++) {
        for (int row = 0; row <= Board::ROWS - 4; row++) {
            char window[4] = {
                board.getCell(row, col),
                board.getCell(row + 1, col),
                board.getCell(row + 2, col),
                board.getCell(row + 3, col)
            };
            score += evaluateWindow(window);
        }
    }
    
    // Check all diagonal windows (positive slope)
    for (int row = 0; row <= Board::ROWS - 4; row++) {
        for (int col = 0; col <= Board::COLS - 4; col++) {
            char window[4] = {
                board.getCell(row, col),
                board.getCell(row + 1, col + 1),
                board.getCell(row + 2, col + 2),
                board.getCell(row + 3, col + 3)
            };
            score += evaluateWindow(window);
        }
    }
    
    // Check all diagonal windows (negative slope)
    for (int row = 3; row < Board::ROWS; row++) {
        for (int col = 0; col <= Board::COLS - 4; col++) {
            char window[4] = {
                board.getCell(row, col),
                board.getCell(row - 1, col + 1),
                board.getCell(row - 2, col + 2),
                board.getCell(row - 3, col + 3)
            };
            score += evaluateWindow(window);
        }
    }
    
    // Center column preference
    int centerCol = Board::COLS / 2;
    int centerCount = 0;
    for (int row = 0; row < Board::ROWS; row++) {
        if (board.getCell(row, centerCol) == aiPlayer) {
            centerCount++;
        }
    }
    score += centerCount * 3;
    
    return score;
}

int MinimaxAI::evaluateWindow(const char window[4]) {
    int aiCount = countPieces(window, aiPlayer);
    int humanCount = countPieces(window, humanPlayer);
    bool empty = hasEmpty(window);
    
    // If window has pieces from both players, it's useless
    if (aiCount > 0 && humanCount > 0) {
        return 0;
    }
    
    // Score based on AI pieces in window
    if (aiCount == 4) {
        return 1000; // Four in a row (shouldn't happen if game checks win)
    } else if (aiCount == 3 && empty) {
        return 100; // Three with potential to make four
    } else if (aiCount == 2 && empty) {
        return 10; // Two with potential to make four
    }
    
    // Penalize opponent pieces in window
    if (humanCount == 3 && empty) {
        return -80; // Block opponent's three in a row
    } else if (humanCount == 2 && empty) {
        return -10;
    }
    
    return 0;
}

std::vector<int> MinimaxAI::getValidMoves(const Board& board) {
    std::vector<int> validMoves;
    
    for (int col = 0; col < Board::COLS; col++) {
        if (!board.isColumnFull(col)) {
            validMoves.push_back(col);
        }
    }
    
    return validMoves;
}

int MinimaxAI::countPieces(const char window[4], char player) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (window[i] == player) {
            count++;
        }
    }
    return count;
}

bool MinimaxAI::hasEmpty(const char window[4]) {
    for (int i = 0; i < 4; i++) {
        if (window[i] == ' ') {
            return true;
        }
    }
    return false;
}
