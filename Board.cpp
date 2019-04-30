#include "Board.h"

#include <fstream>


Board::Board()
{
}

Board::Board(const char *b)
{
    setBoard(b);
}

Board::~Board()
{
}

bool Board::isValid()
{
    if (board == nullptr)
        return false;

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            for (int k = j + 1; k < 9; ++k) {

                //check the number in the row
                if (board[i][j].getNumber() == board[i][k].getNumber() && board[i][j].getNumber() != 0)
                    return false;

                //check the number in the column
                if (board[j][i].getNumber() == board[k][i].getNumber() && board[j][i].getNumber() != 0)
                    return false;

                //check the number in the square
                if (board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber() != 0
                        && board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber() ==
                        board[k % 3 + (i % 3) * 3][k / 3 + (i / 3) * 3].getNumber())
                    return false;
            }
        }
    }

    return true;
}

bool Board::isSolved()
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (this->board[i][j].getNumber() == 0)
                return false;
        }
    }
    return true;
}

bool Board::checkRow(int i, int j)
{
    for (int k = j + 1; k < 9; ++k) {
        if (this->board[i][j].getNumber() == this->board[i][k].getNumber())
            return false;
    }
    return true;
}

bool Board::checkColumn(int i, int j)
{
    for (int k = j + 1; k < 9; ++k) {
        if (this->board[i][j].getNumber() == this->board[k][j].getNumber())
            return false;
    }
    return true;
}

bool Board::checkSquare(int i, int j)
{
    for (int k = 0; k < 9; ++k) {
        if (this->board[i][j].getNumber() != 0
                && this->board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber()
                == this->board[k % 3 + (i % 3) * 3][k / 3 + (i / 3) * 3].getNumber()) {

            return false;
        }
    }
    return true;
}

void Board::step()
{
    //Board tmp = *this;
    bool step = false;
    while(!step){
        if(inclusive() || hiddenSingle())
            step = true;
        else
            break;
        nakedPair();
        if(isSolved())
            step = true;
    }
}

bool Board::inclusive()
{
    //get possibilities of each cells
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            //put number in cells where there only one possibility in list of possibilities
            if (this->board[i][j].getPossibilities().size() == 1) {
                int value = this->board[i][j].getPossibilities().front();
                this->board[i][j] = value;
                removePossibilities(i, j, &value);
                std::cout << "i : " << i << "; j : " << j << std::endl;
                return true;
            }
        }
    }
    return false;
}

bool Board::hiddenSingle()
{
    //check for each cells with multiple possibilities if the possibilities just appear once in row, column or square
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (this->board[i][j].getNumber() == 0) {
                int value = 0;
                //std::vector<int> possi = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                //this->board[i][j].setPossiblities(listOfPossibilities(possi, i, j));

                //if there are only one possibility, set the possibility in the cell
                if (rowSingle(i, j, &value) || columnSingle(i, j, &value) || blockSingle(i, j, &value)) {
                    this->board[i][j] = value;
                    removePossibilities(i, j, &value);
                    return true;
                }

            }
        }
    }
    return false;
}

std::vector<int> Board::listOfPossibilities(std::vector<int> &possible, int i, int j)
{
    //row
    for (int x = 0; x < 9; ++x) {
        if (this->board[i][x].getNumber() != 0)
            for (uint8_t z = 0; z < possible.size(); ++z) {
                //if the number is in row, remove this number to the cell's possibilities
                if (possible[z] == this->board[i][x].getNumber())
                    possible.erase(possible.begin() + z);
            }
    }
    //column
    for (int y = 0; y < 9; ++y) {
        if (this->board[y][j].getNumber() != 0) {
            for (uint8_t z = 0; z < possible.size(); ++z) {
                //if the number is in column, remove this number to the cell's possibilities
                if (possible[z] == this->board[y][j].getNumber())
                    possible.erase(possible.begin() + z);
            }
        }
    }

    int x, y;
    squareIndex(i, j, &x, &y);

    //square
    for (int k = x; k < (x + 3); ++k) {
        for (int p = y; p < (y + 3); ++p) {
            if (this->board[k][p].getNumber() != 0) {
                for (uint8_t z = 0; z < possible.size(); ++z) {
                    //if the number is in square, remove this number to the cell's possibilities
                    if (possible[z] == this->board[k][p].getNumber()) {
                        possible.erase(possible.begin() + z);
                    }
                }
            }
        }
    }
    return possible;
}

bool Board::rowSingle(int i, int j, int *value)
{
    std::vector<int> allPossible;
    //get all possibilities in the row for cells in i, j position
    for (int z = 0; z < 9; ++z) {
        for (uint8_t x = 0; x < this->board[i][z].getPossibilities().size(); ++x) {
            allPossible.push_back(this->board[i][z].getPossibilities()[x]);
        }
    }

    return setHiddenSingle(allPossible, this->board[i][j].getPossibilities(), value);
}

bool Board::columnSingle(int i, int j, int *value)
{
    std::vector<int> allPossible;
    //get all possibilities in the column for cells in i, j position
    for (int z = 0; z < 9; ++z) {
        for (uint8_t x = 0; x < this->board[z][j].getPossibilities().size(); ++x) {
            allPossible.push_back(this->board[z][j].getPossibilities()[x]);
        }
    }

    return setHiddenSingle(allPossible, this->board[i][j].getPossibilities(), value);
}

bool Board::blockSingle(int i, int j, int * value)
{
    std::vector<int> allPossible;
    int x, y;
    squareIndex(i, j, &x, &y);

    //get all possibilities in the row for cells in i, j position
    for (int k = x; k < (x + 3); ++k) {
        for (int p = y; p < (y + 3); ++p) {
            for (uint8_t z = 0; z < this->board[k][p].getPossibilities().size(); ++z) {
                allPossible.push_back(this->board[k][p].getPossibilities()[z]);
            }
        }
    }

    return setHiddenSingle(allPossible, this->board[i][j].getPossibilities(), value);

}

void Board::removeValue(std::vector<int>& possibilities, int i, int j, int *value)
{
    for (uint8_t z = 0; z < possibilities.size(); ++z) {
        if (possibilities[z] == *value) {
            possibilities.erase(possibilities.begin() + z);
            this->board[i][j].setPossiblities(possibilities);
        }
    }
}

void Board::removeRowPairPossibilities(std::vector<int>& possibilities, int i, int j, int x)
{
    for (uint8_t z = 0; z < possibilities.size(); ++z) {
        if (this->board[i][j].getPossibilities()[0] == possibilities[z]) {
            possibilities.erase(possibilities.begin() + z);
            this->board[i][x].setPossiblities(possibilities);
        }

        if (this->board[i][j].getPossibilities()[1] == possibilities[z]) {
            possibilities.erase(possibilities.begin() + z);
            this->board[i][x].setPossiblities(possibilities);
        }
    }
}

void Board::removeColumnPairPossibilities(std::vector<int>& possibilities, int i, int j, int x)
{
    for (uint8_t z = 0; z < possibilities.size(); ++z) {
        if (this->board[i][j].getPossibilities()[0] == possibilities[z]) {
            possibilities.erase(possibilities.begin() + z);
            this->board[x][j].setPossiblities(possibilities);
        }

        if (this->board[i][j].getPossibilities()[1] == possibilities[z]) {
            possibilities.erase(possibilities.begin() + z);
            this->board[x][j].setPossiblities(possibilities);
        }
    }
}

void Board::removeSquarePairPossibilities(std::vector<int>& squareVector, int i, int j, int v, int w)
{
    for(uint8_t z = 0; z < squareVector.size(); ++z){
        if(this->board[i][j].getPossibilities()[0] == squareVector[z]){
            squareVector.erase(squareVector.begin() + z);
            this->board[v][w].setPossiblities(squareVector);
        }
        if(this->board[i][j].getPossibilities()[1] == squareVector[z]){
            squareVector.erase(squareVector.begin() + z);
            this->board[v][w].setPossiblities(squareVector);
        }
    }
}

void Board::removePossibilities(int i, int j, int *value)
{
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            if (x == i && y != j) {
                std::vector<int> tmp = this->board[i][y].getPossibilities();
                removeValue(tmp, i, y, value);
            }
            if (y == j && x != i) {
                std::vector<int> tmp = this->board[x][j].getPossibilities();
                removeValue(tmp, x, j, value);
            }
            if (x != i && y != j) {
                int x, y;
                squareIndex(i, j, &x, &y);

                //get all possibilities in the row for cells in i, j position
                for (int k = x; k < (x + 3); ++k) {
                    for (int p = y; p < (y + 3); ++p) {
                        std::vector<int> tmp = this->board[k][p].getPossibilities();
                        removeValue(tmp, k, p, value);
                    }
                }
            }
        }
    }
}

Cells Board::setBoard(const char *b)
{
    std::ifstream file(b);
    if (file) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                int line;
                file >> line;
                board[i][j] = line;
            }
        }
    }
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            //set the list of possibilities for each cells are equals to 0
            if (this->board[i][j].getNumber() == 0) {
                std::vector<int> possi = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                this->board[i][j].setPossiblities(listOfPossibilities(possi, i, j));
            }
        }
    }
    return **board;
}

bool Board::setHiddenSingle(std::vector<int> const& allPossibilities, std::vector<int> const& cellPossibilities, int *value)
{
    //check if the number is present only once in the allPossiblities
    int occurence = 0;

    std::map<int, int> res;
    std::map<int, int>::iterator it;
    for (uint8_t x = 0; x < cellPossibilities.size(); ++x) {
        for (uint8_t z = 0; z < allPossibilities.size(); ++z) {
            if (allPossibilities[z] == cellPossibilities[x]) {
                ++occurence;
            }
        }
        res.insert(std::pair<int, int>(occurence, cellPossibilities[x]));
        occurence = 0;
    }
    //if there is only one occurence of one number in cellPossibilities
    //return the number in value and true
    it = res.find(1);
    if (it != res.end()) {
        *value = it->second;
        return true;
    }

    return false;
}

void Board::rowNakedPair( int i, int j)
{
    for (int u = 0; u < 9; ++u) {
        if (u != j) {
            if (this->board[i][j].getPossibilities() == this->board[i][u].getPossibilities() && this->board[i][j].getPossibilities().size() == 2) {
                for (int x = 0; x < 9; ++x) {
                    if (x != j && x != u) {
                        std::vector<int> tmp = this->board[i][x].getPossibilities();
                        removeRowPairPossibilities(tmp, i, j, x);
                    }
                }
            }
        }
    }
   // inclusive();
}

void Board::columnNakedPair( int i, int j)
{
    for (int u = 0; u < 9; ++u) {
        if (u != i) {
            if (this->board[i][j].getPossibilities() == this->board[u][j].getPossibilities() && this->board[i][j].getPossibilities().size() == 2) {
                for (int x = 0; x < 9; ++x) {
                    if (x != i && x != u) {
                        std::vector<int> tmp = this->board[x][j].getPossibilities();
                        removeColumnPairPossibilities(tmp, i, j, x);
                    }
                }
            }
        }
    }
    //inclusive();
}

void Board::squareNakedPair(int i, int j)
{
    int l, y;
    squareIndex(i, j, &l, &y);

    for(int k = l; k < (l + 3); ++k){
        for(int p = y; p < (y + 3); ++p){
            if(i != k && p != j){
                if(this->board[i][j].getPossibilities() == this->board[k][p].getPossibilities() && this->board[i][j].getPossibilities().size() == 2){
                    //std::cout << "i : "<< i << ", j :" << j << std::endl;
                    for(int v = l; v < (l+3); ++v){
                        for(int w = y; w < (y+3); ++w){
                             //std::cout << this->board[v][w].getPossibilities() << " v : "<< v << ", w :" << w << std::endl;
                            if( ( (v != i && w != j) || (v == i && w != j) || (v != i && w == j) )
                                && ( (v != k && w != p) || (v == k && w != p) || (v != k && w == p) )){

                                std::vector<int> squareVector = this->board[v][w].getPossibilities();
                                removeSquarePairPossibilities(squareVector, i, j, v, w);
                            }
                        }
                    }
                }
            }

        }
    }
    //inclusive();
}

void Board::nakedPair()
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (this->board[i][j].getNumber() == 0) {
                rowNakedPair(i, j);
                columnNakedPair(i, j);
                squareNakedPair(i, j);
            }
        }
    }

}

void Board::squareIndex(int i, int j, int *x, int *y)
{
    if (i < 3) *x = 0;
    else if (i < 6) *x = 3;
    else *x = 6;
    if (j < 3) *y = 0;
    else if (j < 6) *y = 3;
    else *y = 6;
}

bool Board::isEqual(Board const& board) const
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (this->board[i][j].getNumber() != board.board[i][j].getNumber())
                return false;
        }
    }
    return true;
}


std::ostream & operator<<(std::ostream & flux, Board const &board)
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            flux << board.board[i][j].getNumber() << " ";
        }
        flux << '\n';
    }
    return flux;
}

bool operator==(Board const& tmp, Board const& board)
{
    return board.isEqual(tmp);
}

bool operator!=(Board const& tmp, Board const& board)
{
    return !(tmp == board);
}
