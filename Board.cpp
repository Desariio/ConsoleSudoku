#include "Board.h"
#include <fstream>
#include <iostream>

Board::Board(){

}

Board::Board(const char *b){
    setBoard(b);
}

Board::~Board(){

}

bool Board::isValid(){
    if(board == nullptr)
        return false;

    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            for(int k = j + 1; k < 9; ++k){
                //Check if there are two same number in the same row
                if(board[i][j].getNumber() == board[i][k].getNumber()
                   && board[i][j].getNumber() != 0){

                    return false;
                }

                //Check if there are two same number in the same column
                if(board[j][i].getNumber() == board[k][i].getNumber()
                  && board[j][i].getNumber() != 0){

                    return false;
                }

                //Check if there are twoe same number in the same square 3 * 3
                if( board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber() != 0
                    && board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber() ==
                    board[k % 3 + (i % 3) * 3][k / 3 + (i / 3) * 3].getNumber() )

                    return false;
            }
        }
    }
    return true;
}

bool Board::isSolved(){

}

void Board::step(){

    std::vector<int> possi = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    /*for(int i = 0; i < 9; ++i){
        for (int j = 0; j < 9; ++j){
            if(board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber() != 0)
                possi.erase(possi.begin() + board[i][j].getNumber() - 1);

            if(board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3].getNumber() == 0)
                board[j % 3 + (i % 3) * 3][j / 3 + (i / 3) * 3] = 1;
        }
    }*/
    if(board[0][2].getNumber() == 0)
        board[0][2] = possi.front();
}

Cells Board::setBoard(const char *b)
{
    std::ifstream file(b);
    if(file){
        for(int i =0; i < 9; ++i){
            for(int j = 0; j < 9; ++j){
                int line;
                file >> line;
                board[i][j] = line;
            }
        }
    }else{
        std::cout << "File not found" << std::endl;
    }
    return **board;
}

bool Board::isEqual(Board const& board)const{
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            if(this->board == board.board)
                return true;
    return false;
}

std::ostream& operator<<(std::ostream& flux, Board &board){
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            flux << board.board[i][j].getNumber() << " ";
        }
        std::cout << '\n';
    }
    return flux;
}

bool operator==(Board const& tmp, Board const& board){
    return tmp.isEqual(board);
}

bool operator!=(Board const& tmp, Board const& board){
    return !(tmp == board);
}
