#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

class Board {
public:
    static const int ROWS = 6;
    static const int COLS = 7;
    
    Board();
    
    bool dropPiece(int column, char player);
    bool checkWin(char player) const;
    bool isFull() const;
    void display() const;
    void reset();
    
private:
    std::vector<std::vector<char>> grid;
    
    bool checkDirection(int row, int col, int dRow, int dCol, char player) const;
    bool isValidColumn(int column) const;
    int getNextAvailableRow(int column) const;
};

#endif // BOARD_H
