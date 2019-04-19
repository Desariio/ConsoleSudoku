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
                if (board[i][j].getNumber() == board[i][k].getNumber() && board[i][j].getNumber() != 0) {
                    return false;
                }
                //check the number in the column
                if (board[j][i].getNumber() == board[k][i].getNumber() && board[j][i].getNumber() != 0) {
                    return false;
                }
                //check the number in the square
                if (board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber() != 0
                    && board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber() ==
                       board[k % 3 + (i % 3) * 3][k / 3 + (i / 3) * 3].getNumber() )
                    return false;
            }
            /*if(board[i][j].getNumber() != 0)
                if ( !checkSquare(i, j) )
                    return false;*/
        }
    }

    return true;
}

bool Board::isSolved()
{
    return false;
}

bool Board::checkRow( int i, int j)
{
    for (int k = j + 1; k < 9; ++k) {
        if (this->board[i][j].getNumber() == this->board[i][k].getNumber()) {
            return false;
        }

    }
    return true;
}

bool Board::checkColumn(int i, int j)
{
    for (int k = j + 1; k < 9; ++k) {
        if (this->board[i][j].getNumber() == this->board[k][j].getNumber()) {
            return false;
        }

    }
    return true;
}

bool Board::checkSquare(int i, int j)
{
    for (int k =0; k < 9; ++k) {
        if (this->board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber()
            == this->board[k % 3 + (i % 3) * 3][k / 3 + (i / 3) * 3].getNumber()) {
            std::cout << i << " " << j << " " << k <<
                this->board[k % 3 + (i % 3) * 3][k / 3 + (i / 3) * 3].getNumber() <<	std::endl;
            return false;
        }

    }
    return true;
}

void Board::step()
{
    //get possibilities of each cells (just row and column for the moment)
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            //set the list of possibilities for each cells are equals to 0
            if (this->board[i][j].getNumber() == 0) {
                std::vector<int> possi = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

                this->board[i][j].setPossiblities( listOfPossibilities(possi, i, j));
            }
            //put number in cells where there only one possibility in list of possibilities
            if(this->board[i][j].getPossibilities().size() == 1){
                this->board[i][j] = this->board[i][j].getPossibilities().front();
            }

        }
    }
std::cout << " num: " << this->board[0][2].getPossibilities() << std::endl;
    //show possibilities of cells
    /*for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if(this->board[i][j].getNumber() == 0)
                std::cout << this->board[i][j].getPossibilities() << std::endl;
        }
    }*/


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
    return **board;
}

std::vector<int> Board::listOfPossibilities(std::vector<int> &possible, int i,int j)
{
    //row
    for (int x = 0; x < 9; ++x) {
        if(this->board[i][x].getNumber() != 0 )
            for (uint8_t z = 0; z < possible.size(); ++z) {
                if (possible[z] == this->board[i][x].getNumber())
                    possible.erase(possible.begin() + z);
            }

    }
    //column
    for (int y = 0; y < 9; ++y) {
        if (this->board[y][j].getNumber() != 0) {
            for (uint8_t z = 0; z < possible.size(); ++z) {
                if (possible[z] == this->board[y][j].getNumber())
                    possible.erase(possible.begin() + z);
            }
        }
    }
    // std::cout << "i : " << i << ", j : " << j << std::endl;
    //std::cout << "x : " << j % 3 + (i % 3) * 3 << ", y : " << j / 3 + (i / 3) * 3 << std::endl;

    int x, y;
    if( i < 3) x = 0;
    else if(i < 6) x = 3;
    else x = 6;
    if(j < 3) y = 0;
    else if(j < 6) y = 3;
    else y = 6;

    //square
    for(int k = x; k < (x + 3); ++k){
        for(int p = y; p < (y + 3); ++p){
            if(this->board[k][p].getNumber() != 0){
                for(uint8_t z = 0; z < possible.size(); ++z){
                    if(possible[z] == this->board[k][p].getNumber()){
                         possible.erase(possible.begin() + z);
                    }
                }
            }
        }
    }
    /*for(int k = 0; k < 9; ++k){
        if (this->board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber() != 0
            && this->board[k % 3 + (i % 3) * 3][k / 3 + (i / 3) * 3].getNumber() != 0)
            std::cout << j % 3 + (i % 3) * 3 << "  " << j / 3 + (i / 3) * 3 << std::endl;
    }*/

    /*for (int x = 0; x < 9; ++x){
        for (int y = 0; y < 9; ++y) {
            if (this->board[i][x / 3 + (i / 3) * 3].getNumber() != 0
                && this->board[y % 3 + (i % 3) * 3][j].getNumber() != 0)
                possible.push_back(this->board[x][y].getNumber());
        }
    }*/
    return possible;
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
