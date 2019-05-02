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

    void step();

    friend std::ostream & operator<<(std::ostream & flux, Board const &board);
    friend bool operator==(Board const& tmp, Board const& board);
    friend bool operator!=(Board const& tmp, Board const& board);


private:
    Cells board[9][9];

    bool inclusive();
    bool hiddenSingle();
    void nakedPair();
    void nakedTriple();
    void hiddenTriple();

    Cells setBoard(const char *b);
    std::vector<int> listOfPossibilities(std::vector<int> &possibilities, int i, int j);

    bool rowSingle(int i, int j, int *value);
    bool columnSingle(int i, int j, int *value);
    bool blockSingle(int i, int j, int *value);

    bool rowHiddenPair(int i, int j, int *value);

    void removePossibilities(int i, int j, int *value);
    void removeValue(std::vector<int> &possibilities, int i, int j, int *value);
    void removeRowPairPossibilities(std::vector<int> &possibilities, int i, int j, int x);
    void removeColumnPairPossibilities(std::vector<int> &possibilities, int i, int j, int x);
    void removeSquarePairPossibilities(std::vector<int> &possibilities, int i, int j, int v, int w);

    void removeRowNakedTriple(std::vector<int> &possibilities, int i, int j, int k);
    void removeColumnNakedTriple(std::vector<int> &possibilities, int i, int j, int k);
    void removeSquareNakedTriple(std::vector<int> &possibilities, int i, int j, int k, int p);

    bool setHiddenSingle(std::vector<int> const& allPossibilities, std::vector<int> const& cellPossibilities, int *value);
    void rowNakedPair(int i, int j);
    void columnNakedPair(int i, int j);
    void squareNakedPair(int i, int j);

    void rowNakedTriple(int i, int j);
    void columnNakedTriple(int i, int j);
    void squareNakedTriple(int i, int j);

    void squareIndex(int i, int j, int *x, int *y);

    bool isEqual(Board const& board) const;
};

#endif // BOARD_H

