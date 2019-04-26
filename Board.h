#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
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
    void inclusive();
    void hiddenSingle();

    Cells setBoard(const char *b);
    std::vector<int> listOfPossibilities(std::vector<int> &possible, int i, int j);

    bool columnSingle(int i, int j, int *value);
    bool rowSingle(int i, int j, int *value);
    bool blockSingle(int i, int j, int *value);
    bool setSingle(std::vector<int> const& allColumnPossibilities, std::vector<int> const& cellPossibilities, int *value);

    friend std::ostream & operator<<(std::ostream & flux, Board const &board);
    friend bool operator==(Board const& tmp, Board const& board);
    friend bool operator!=(Board const& tmp, Board const& board);

private:
    Cells board[9][9];

    bool isEqual(Board const& board) const;
};

#endif // BOARD_H
