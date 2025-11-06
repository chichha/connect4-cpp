#include "Board.h"
#include <iostream>

Board::Board() {
    grid.resize(ROWS, std::vector<char>(COLS, ' '));
}

bool Board::dropPiece(int column, char player) {
    if (!isValidColumn(column)) {
        return false;
    }
    
    int row = getNextAvailableRow(column);
    if (row == -1) {
        return false;
    }
    
    grid[row][column] = player;
    return true;
}

bool Board::checkWin(char player) const {
    // Check horizontal
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col <= COLS - 4; col++) {
            if (checkDirection(row, col, 0, 1, player)) {
                return true;
            }
        }
    }
    
    // Check vertical
    for (int row = 0; row <= ROWS - 4; row++) {
        for (int col = 0; col < COLS; col++) {
            if (checkDirection(row, col, 1, 0, player)) {
                return true;
            }
        }
    }
    
    // Check diagonal (down-right)
    for (int row = 0; row <= ROWS - 4; row++) {
        for (int col = 0; col <= COLS - 4; col++) {
            if (checkDirection(row, col, 1, 1, player)) {
                return true;
            }
        }
    }
    
    // Check diagonal (down-left)
    for (int row = 0; row <= ROWS - 4; row++) {
        for (int col = 3; col < COLS; col++) {
            if (checkDirection(row, col, 1, -1, player)) {
                return true;
            }
        }
    }
    
    return false;
}

bool Board::isFull() const {
    for (int col = 0; col < COLS; col++) {
        if (grid[0][col] == ' ') {
            return false;
        }
    }
    return true;
}

void Board::display() const {
    std::cout << "\n  ";
    for (int col = 0; col < COLS; col++) {
        std::cout << col + 1 << " ";
    }
    std::cout << "\n";
    
    for (int row = 0; row < ROWS; row++) {
        std::cout << "| ";
        for (int col = 0; col < COLS; col++) {
            std::cout << grid[row][col] << " ";
        }
        std::cout << "|\n";
    }
    
    std::cout << "+";
    for (int col = 0; col < COLS; col++) {
        std::cout << "--";
    }
    std::cout << "+\n";
}

void Board::reset() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            grid[row][col] = ' ';
        }
    }
}

char Board::getCell(int row, int col) const {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        return grid[row][col];
    }
    return ' ';
}

bool Board::isColumnFull(int column) const {
    if (!isValidColumn(column)) {
        return true;
    }
    return getNextAvailableRow(column) == -1;
}

bool Board::checkDirection(int row, int col, int dRow, int dCol, char player) const {
    for (int i = 0; i < 4; i++) {
        int r = row + i * dRow;
        int c = col + i * dCol;
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS || grid[r][c] != player) {
            return false;
        }
    }
    return true;
}

bool Board::isValidColumn(int column) const {
    return column >= 0 && column < COLS;
}

int Board::getNextAvailableRow(int column) const {
    for (int row = ROWS - 1; row >= 0; row--) {
        if (grid[row][column] == ' ') {
            return row;
        }
    }
    return -1;
}
