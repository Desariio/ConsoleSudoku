#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include "Cells.h"

class Board: public Cells{
    public:
        Board();
        Board(const char *b);
        ~Board();

        bool isValid();
        bool isSolved();
        void step();

        Cells setBoard(const char *b);

        friend std::ostream& operator<<(std::ostream& flux, Board &board);
        friend bool operator==(Board const& tmp, Board const& board);
        friend bool operator!=(Board const& tmp, Board const& board);
    private:
        Cells board[9][9];

        bool isEqual(Board const& board)const;
};

#endif // BOARD_H
