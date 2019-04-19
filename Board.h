#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include "Cells.h"

class Board : public Cells {
public:
    Board();
    Board(const char *b);
    ~Board();

    bool isValid();
    bool isSolved();

    bool checkRow( int i, int j);
    bool checkColumn(int i, int j);
    bool checkSquare(int i, int j);

    void step();
    Cells setBoard(const char *b);
    std::vector<int> listOfPossibilities(std::vector<int> &possible, int i, int j);

    friend std::ostream & operator<<(std::ostream & flux, Board const &board);
    friend bool operator==(Board const& tmp, Board const& board);
    friend bool operator!=(Board const& tmp, Board const& board);
private:
    Cells board[9][9];

    bool isEqual(Board const& board) const;
};

#endif // BOARD_H
