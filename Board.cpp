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

void Board::step()
{
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            std::vector<int> tmp = this->board[i][j].getPossibilities();
            std::cout << "[ ";
            for(uint8_t z = 0; z < tmp.size(); ++z)
                std::cout << tmp[z] ;
            std::cout << " ]";

        }
        std::cout << '\n';
    }

    bool insert = false;
    while(!insert){

        nakedPair();
        nakedTriple();
        //hiddenPair();
        if(pointingPair() )
            break;

        if(inclusive() || hiddenSingle())
            break;

        if(isSolved())
            insert = true;
        else
            break;
    }
    std::cout << "BOARD 0 1 : " << this->board[0][1].getPossibilities() << std::endl;
/*
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(i == 7 && j == 6){
                int l, c;
                squareIndex(i, j, &l, &c);
                for(int k = l; k < (l + 3); ++k){
                    for(int p = c; p < (c + 3); ++p){
                        std::vector<int> tmp = this->board[k][p].getPossibilities();
                        if(k != i ){
                            for(uint8_t z = 0; z < this->board[i][j].getPossibilities().size(); ++z){
                                for(uint8_t x = 0; x < tmp.size(); ++x){
                                    if(tmp[x] == this->board[i][j].getPossibilities()[z]){
                                        int val = this->board[k][p].getPossibilities()[z];
                                        std::cout << "VAL :" << val << ", k : " << k << std::endl;
                                        tmp.erase(tmp.begin() + x);
                                        this->board[k][p].setPossiblities(tmp);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(i == 0 && j == 8 || i == 2 && j == 7 || i == 2 && j == 8){
                std::vector<int> tmp = this->board[i][j].getPossibilities();
                for(uint8_t z = 0; z < tmp.size(); ++z){
                    if(tmp[z] == 8){
                        tmp.erase(tmp.begin() + z);
                        this->board[i][j].setPossiblities(tmp);
                    }
                    if(tmp[z] == 3){
                        tmp.erase(tmp.begin() + z);
                        this->board[i][j].setPossiblities(tmp);
                    }
                }
            }
            if(i == 6 && j == 7){
                std::vector<int> tmp = this->board[i][j].getPossibilities();
                for(uint8_t z = 0; z < tmp.size(); ++z){
                    if(tmp[z] == 1){
                        tmp.erase(tmp.begin() + z);
                        this->board[i][j].setPossiblities(tmp);
                    }
                }
            }
            if(i == 8 && j == 6){
                std::vector<int> tmp = this->board[i][j].getPossibilities();
                for(uint8_t z = 0; z < tmp.size(); ++z){
                    if(tmp[z] == 7){
                        tmp.erase(tmp.begin() + z);
                        this->board[i][j].setPossiblities(tmp);
                    }
                }
            }
            if(i == 8 && j == 0){
                std::vector<int> tmp = this->board[i][j].getPossibilities();
                for(uint8_t z = 0; z < tmp.size(); ++z){
                    if(tmp[z] == 8){
                        tmp.erase(tmp.begin() + z);
                        this->board[i][j].setPossiblities(tmp);
                    }
                }
            }
        }
    }*/

     std::cout << '\n';
   /*  for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            std::vector<int> tmp = this->board[i][j].getPossibilities();
            std::cout << "[ ";
            for(uint8_t z = 0; z < tmp.size(); ++z)
                std::cout << tmp[z] ;
            std::cout << " ]";

        }
        std::cout << '\n';
    }*/

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
                std::cout << "Inclusive \ni : " << i << "; j : " << j << std::endl;
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
                    std::cout << "Hidden Single \ni : " << i << "; j : " << j << std::endl;
                    return true;
                }

            }
        }
    }
    return false;
}


void Board::hiddenPair()
{
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(this->board[i][j].getNumber() == 0){
                int value = 0;
                rowHiddenPair(i, j, &value);
            }
        }
    }
}

void Board::rowHiddenPair(int i, int j, int *value)
{
    int occurence = 0;
    std::vector<int> allPossibilities;
    std::map<int, int> res;
    std::map<int, int>::iterator it;
    *value = 0;
    for(int c = 0; c < 9; ++c){
        for(uint8_t z = 0; z < this->board[i][c].getPossibilities().size(); ++z){
            allPossibilities.push_back(this->board[i][c].getPossibilities()[z]);
        }
    }

    for (uint8_t x = 0; x < this->board[i][j].getPossibilities().size(); ++x) {
        for (uint8_t z = 0; z < allPossibilities.size(); ++z) {
            if (allPossibilities[z] == this->board[i][j].getPossibilities()[x]) {
                ++occurence;
            }
        }
        res.insert(std::pair<int, int>(occurence, this->board[i][j].getPossibilities()[x]));
        occurence = 0;

    }

    it = res.find(2);
    std::vector<int> pair;
    if (it != res.end()) {
        //if(i == 6){
            //*value = it->second;
            //++nbVal;
            //std::cout << "VAL : " << *value << ", i : " << i << ", j : " << j << ", nbVal : " << nbVal << std::endl;
            pair.push_back(it->second);

        //}
    }
//std::cout << "PAIR : (" << pair[0] << ", "<< pair[1] << ") , i : " << i << ", j : " << j << std::endl;
    for(int p = 0; p < 9; ++p){
        for(uint8_t z = 0; z < this->board[i][p].getPossibilities().size(); ++z){
            for(uint8_t x = 0; x < pair.size(); ++x){
                if(this->board[i][p].getPossibilities()[z] == pair[x]){
                    //std::cout << "PAIR : " << pair << std::endl;
                }
            }
        }
    }
}

/*
void Board::hiddenTriple()
{
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(this->board[i][j].getNumber() == 0 ){
                int value;
                if(rowTriple(i, j) || columnTriple(i, j) || blockTriple(i, j)){

                }
            }
        }
    }
}*/

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



void Board::removeRowNakedTriple(std::vector<int> &possibilities, int i, int j, int k)
{
    for(uint8_t z = 0; z < possibilities.size(); ++z){
        if(this->board[i][j].getPossibilities()[0] == possibilities[z]){
            possibilities.erase(possibilities.begin() + z);
            this->board[i][k].setPossiblities(possibilities);
        }
        if(this->board[i][j].getPossibilities()[1] == possibilities[z]){
            possibilities.erase(possibilities.begin() + z);
            this->board[i][k].setPossiblities(possibilities);
        }
        if(this->board[i][j].getPossibilities()[2] == possibilities[z]){
            possibilities.erase(possibilities.begin() + z);
            this->board[i][k].setPossiblities(possibilities);
        }
    }
}

void Board::removeColumnNakedTriple(std::vector<int> &possibilities, int i, int j, int k)
{
    for(uint8_t z = 0; z < possibilities.size(); ++z){
        if(this->board[i][j].getPossibilities()[0] == possibilities[z]){
            possibilities.erase(possibilities.begin() + z);
            this->board[k][j].setPossiblities(possibilities);
        }
        if(this->board[i][j].getPossibilities()[1] == possibilities[z]){
            possibilities.erase(possibilities.begin() + z);
            this->board[k][j].setPossiblities(possibilities);
        }
        if(this->board[i][j].getPossibilities()[2] == possibilities[z]){
            possibilities.erase(possibilities.begin() + z);
            this->board[k][j].setPossiblities(possibilities);
        }
    }
}

void Board::removeSquareNakedTriple(std::vector<int> &possibilities, int i, int j, int k, int p)
{
    for(uint8_t z = 0; z < possibilities.size(); ++z){
        if(this->board[i][j].getPossibilities()[0] == possibilities[z]){
            possibilities.erase(possibilities.begin() + z);
            this->board[k][p].setPossiblities(possibilities);
        }
        if(this->board[i][j].getPossibilities()[1] == possibilities[z]){
            possibilities.erase(possibilities.begin() + z);
            this->board[k][p].setPossiblities(possibilities);
        }
        if(this->board[i][j].getPossibilities()[2] == possibilities[z]){
            possibilities.erase(possibilities.begin() + z);
            this->board[k][p].setPossiblities(possibilities);
        }
    }
}

void Board::rowNakedTriple(int i, int j)
{
    for(int u = 0; u < 9; ++u){
        if(u != j){
            for(int v = 0; v < 9; ++v){

                // check if there are only 3 possibilities in 3 differents cells
                if( ( this->board[i][j].getPossibilities() == this->board[i][u].getPossibilities() )
                        && (this->board[i][j].getPossibilities() == this->board[i][v].getPossibilities() )
                        && (this->board[i][j].getPossibilities().size() == 3) ){
                    if(j != u && u != v && v != j){
                        for(int k = 0; k < 9; ++k){
                            if( k != u && k!= j && k != v){

                                std::vector<int> tmp = this->board[i][k].getPossibilities();
                                //std::cout << "ROW NAKED ! i :" << i << ", k : " << k << " ,TMP : " << tmp << std::endl;
                                removeRowNakedTriple(tmp, i, j, k);
                            }

                        }
                    }
                }
                //check if 2 cells have 3 same possibilities and the third have 2 of them
                if(this->board[i][j].getPossibilities().size() == 3
                        && (this->board[i][u].getPossibilities().size() == 3 || this->board[i][v].getPossibilities().size() == 3) ){
                    if( j != u && u != v && v != j){
                        std::vector<int> firstCell = this->board[i][j].getPossibilities();
                        std::vector<int> secondCell = this->board[i][u].getPossibilities();
                        std::vector<int> thirdCell = this->board[i][v].getPossibilities();
                        bool equal = false;
                        if(firstCell == secondCell){
                            if(thirdCell.size() == 2){
                                for(uint8_t z = 0; z < thirdCell.size(); ++z){
                                    for(uint8_t x = 0; x < firstCell.size(); ++x){
                                        if(firstCell[x] == thirdCell[z])
                                            equal = true;
                                        else
                                            equal = false;
                                    }
                                }
                                if(equal){
                                    for(int k = 0; k < 9; ++k){
                                        if(k != j && k != u && k !=j && k != v){
                                            std::vector<int> tmp = this->board[i][k].getPossibilities();
                                            for(uint8_t n = 0; n < firstCell.size(); ++n){
                                                for(uint8_t m = 0; m < tmp.size(); ++m){
                                                    if( firstCell[n] == tmp[m]){
                                                        tmp.erase(tmp.begin() + m);
                                                        //std::cout << "CHECK REMOVE ROW : "<< tmp << "\n \n " << tmp[m] << std::endl;
                                                        this->board[i][k].setPossiblities(tmp);
                                            //std::cout << "FIRST CELL : " << firstCell << " , i : " << i << ", j : " << j << ", u : "<< u << ", v :"<< v << ", k : " << k << std::endl;
                                                    }
                                                }

                                            }
                                            //removeRowNakedTriple(firstCell, i, j, k);
                                        }else if( (k >= j && k <= j +3)
                                                  && (k >= u && k <= u + 3)
                                                  && (k >= v && k <= v + 3)){

                                        }
                                    }
                                }
                            }
                        }else if(firstCell == thirdCell){
                            if(secondCell.size() == 2){
                                for(uint8_t z = 0; z < secondCell.size(); ++z){
                                    for(uint8_t x = 0; x < firstCell.size(); ++x){
                                        if(firstCell[x] == secondCell[z])
                                            equal = true;
                                        else
                                            equal = false;
                                    }
                                }
                                if(equal){
                                    for(int k = 0; k < 9; ++k){
                                        if(k != j && k != u && k !=j && k != v){
                                            std::vector<int> tmp = this->board[i][k].getPossibilities();
                                            for(uint8_t n = 0; n < firstCell.size(); ++n){
                                                for(uint8_t m = 0; m < tmp.size(); ++m){
                                                    if( firstCell[n] == tmp[m]){
                                                        tmp.erase(tmp.begin() + m);
                                                        //std::cout << "CHECK REMOVE ROW : "<< tmp << "\n \n " << tmp[m] << std::endl;
                                                        this->board[i][k].setPossiblities(tmp);
                                            //std::cout << "FIRST CELL : " << firstCell << " , i : " << i << ", j : " << j << ", u : "<< u << ", v :"<< v << ", k : " << k << std::endl;
                                                    }
                                                }

                                            }
                                            //removeRowNakedTriple(firstCell, i, j, k);
                                        }else if( (k >= j && k <= j +3)
                                                  && (k >= u && k <= u + 3)
                                                  && (k >= v && k <= v + 3)){

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Board::columnNakedTriple(int i, int j)
{
    for(int u = 0; u < 9; ++u){
        if(u != i){
            for(int v = 0; v < 9; ++v){
                if( (this->board[i][j].getPossibilities() == this->board[u][j].getPossibilities() )
                        && ( this->board[i][j].getPossibilities() == this->board[v][j].getPossibilities() )
                        && this->board[i][j].getPossibilities().size() == 3){
                    if(i != u && u != v && v != i){
                        for(int k = 0; k < 9; ++k){
                            if(k != u && k != i && k != v){
                                std::cout << "COLUMN NAKED ! k :" << k << ", j : " << j << std::endl;
                                std::vector<int> tmp = this->board[k][j].getPossibilities();
                                removeColumnNakedTriple(tmp, i, j, k);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Board::squareNakedTriple(int i, int j)
{
    int l, y;
    squareIndex(i, j, &l, &y);

    for(int k = l; k < (l + 3); ++k){
        for(int p = y; p < (y + 3); ++p){
            if(i != k && p != j){
                for(int m = l; m < (l + 3); ++m){
                    for(int n = p; n < (p + 3); ++n){
                        if(m != k && p != n){
                            if( ( this->board[i][j].getPossibilities() == this->board[k][p].getPossibilities() )
                                    && ( this->board[i][j].getPossibilities() == this->board[m][n].getPossibilities() )
                                    && this->board[i][j].getPossibilities().size() == 3){
                                for(int v = l; v < (l + 3); ++v){
                                    for(int w = y; w < (y + 3); ++w){
                                        if( ( ( (v != i && w != j) && (v == i && w != j) && (v != i && w == j) )
                                             && ( (v != k && w != p) && (v == k && w != p) && (v != k && w == p) ) )
                                             && ( (m != i && n != j) && (m == i && n != j) && (m != i && n == j) )
                                                 && ( (m != k && n != p) || (m == k && n != p) || (m != k && n == p)  )  ){
                                            std::vector<int> tmp = this->board[v][w].getPossibilities();
                                            std::cout << "SQUARE" << this->board[v][w].getPossibilities() << ", i : " << i << ", j : " << j << ", v : " << v << ", w : " << w << ", m : " << m << ", n : " << n << std::endl;
                                            removeSquareNakedTriple(tmp, i, j, v, w);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Board::nakedTriple()
{
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(this->board[i][j].getNumber() == 0){
                rowNakedTriple(i, j);
                columnNakedTriple(i, j);
                squareNakedTriple(i, j);
            }
        }
    }
}

bool Board::pointingPair()
{
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(this->board[i][j].getNumber() == 0){
                int value = 0;
                if(rowPointingPair(i, j, &value) || columnPointingPair(i, j, &value)) return true;

            }
        }
    }
    return false;
}

bool Board::rowPointingPair(int i, int j, int *value)
{
    std::vector<int> allPossibilities;
    int l, c;
    squareIndex(i, j, &l, &c);
    for(int v = l; v < (l + 3); ++v){
        for(int w = c; w < (c + 3); ++w){
            for(uint8_t z = 0; z < this->board[v][w].getPossibilities().size(); ++z){
                allPossibilities.push_back(this->board[v][w].getPossibilities()[z]);
            }
        }
    }
    int occurence = 0;
    std::map<int, int> res;
    std::map<int, int>::iterator it;
    for (uint8_t x = 0; x < this->board[i][j].getPossibilities().size(); ++x) {
        for (uint8_t z = 0; z < allPossibilities.size(); ++z) {
            if (allPossibilities[z] == this->board[i][j].getPossibilities()[x]) {
                ++occurence;
            }
        }
        res.insert(std::pair<int, int>(occurence, this->board[i][j].getPossibilities()[x]));
        occurence = 0;
    }
    //if there is only one occurence of one number in cellPossibilities
    //return the number in value and true
    it = res.find(2);
    if (it != res.end()) {
        *value = it->second;
    }

    bool remove = false;
    for(int x = c; x < (c + 2); ++x){
        if(x != j){
            for(uint8_t z = 0; z < this->board[i][j].getPossibilities().size(); ++z){
                for(uint8_t v = 0; v < this->board[i][x].getPossibilities().size(); ++v){
                    if( (this->board[i][j].getPossibilities()[z] == *value && this->board[i][x].getPossibilities()[v] == *value )
                            || (this->board[i][j].getPossibilities()[z] == *value && this->board[i][x].getPossibilities()[v] == *value && this->board[i][x + 1].getPossibilities()[v] == *value ) ){

                        for(int w = 0; w < 9; ++w){
                            if( (w != j) && (w != x) && ( w != (x + 1) ) ){
                               //std::cout<< "TEST : " <<this->board[i][w].getPossibilities() << ", i : " << i << ", w : " << w << ", it : " << it->second <<std::endl;
                               std::vector<int> tmp = this->board[i][w].getPossibilities();
                               for(uint8_t y = 0; y < tmp.size(); ++y){
                                   if(tmp[y] == *value){
                                       std::cout<< "REMOVE ROW: " << *value << "i : " << i << ", w : " << w << std::endl;
                                       tmp.erase(tmp.begin() + y);
                                       this->board[i][w].setPossiblities(tmp);
                                       remove = true;
                                   }
                               }

                            }else
                                remove = false;
                            if(!remove){
                                for(int m = l; m < (l + 3); ++m){
                                    for(int n = c; n < (c + 3); ++n){
                                        if( ( (m != i && n != j) || ( m == i && n != j) || (m != i && n == j)  || ( m != w && n != j) || ( m == w && n != j) || (m != w && n ==j) )){

                                               if(inclusive())
                                                  return true;
                                               else if(hiddenSingle())
                                                  return true;


                                            std::vector<int> tmp = this->board[m][n].getPossibilities();

                                            for(uint8_t z = 0; z < tmp.size(); ++z){
                                                if(tmp[z] == *value){
                                                    std::cout << "m : " << m << ", n : " << n << ", value : " << *value << ", tmp : " << tmp << std::endl;
                                                    tmp.erase(tmp.begin() + z);
                                                    this->board[m][n].setPossiblities(tmp);
                                                    //std::cout << "VALUE " << *value << ";  m : "<< m << ", n : " << n << std::endl;
                                                }
                                            }
                                        }
                                    }
                                }
                            }


                        }


                        return true;

                    }
                }
            }
        }
    }
    return false;
}

bool Board::columnPointingPair(int i, int j, int *value)
{
    std::vector<int> allPossibilities;
    int l, c;
    squareIndex(i, j, &l, &c);
    for(int v = l; v < (l + 3); ++v){
        for(int w = c; w < (c + 3); ++w){
            for(uint8_t z = 0; z < this->board[v][w].getPossibilities().size(); ++z){
                allPossibilities.push_back(this->board[v][w].getPossibilities()[z]);
            }
        }
    }
    int occurence = 0;
    std::map<int, int> res;
    std::map<int, int>::iterator it;
    for (uint8_t x = 0; x < this->board[i][j].getPossibilities().size(); ++x) {
        for (uint8_t z = 0; z < allPossibilities.size(); ++z) {
            if (allPossibilities[z] == this->board[i][j].getPossibilities()[x]) {
                ++occurence;
            }
        }
        res.insert(std::pair<int, int>(occurence, this->board[i][j].getPossibilities()[x]));
        occurence = 0;
    }
    //if there is only one occurence of one number in cellPossibilities
    //return the number in value and true
    it = res.find(2);
    if (it != res.end()) {
        *value = it->second;
    }
    bool remove = false;
    for(int x = l; x < (l + 2); ++x){
        if(x != i){
            for(uint8_t z = 0; z < this->board[i][j].getPossibilities().size(); ++z){
                for(uint8_t v = 0; v < this->board[x][j].getPossibilities().size(); ++v){
                   // std::cout << "VALUE : " << *value << std::endl;

                    if( ( this->board[i][j].getPossibilities()[z] == *value && this->board[x][j].getPossibilities()[v] == *value)
                            || (this->board[i][j].getPossibilities()[z] == *value && this->board[x][j].getPossibilities()[v] == *value && this->board[x + 1][j].getPossibilities()[v] == *value ) ){

                        for(int w = 0; w < 9; ++w){
                            if( (w != i) && (w != x) && ( w != (x + 1) ) ){
                                //std::cout << "VALUE 2 : " << *value << ", possi : " << this->board[w][j].getPossibilities() <<std::endl;
                               //std::cout<< "TEST : " <<this->board[i][w].getPossibilities() << ", i : " << i << ", w : " << w << ", it : " << it->second <<std::endl;
                               std::vector<int> tmp = this->board[w][j].getPossibilities();
                              // std::cout << "w : " << w << ", j : " << j << std::endl;
                               for(uint8_t y = 0; y < tmp.size(); ++y){
                                   //std::cout << "POINTING COLUMN ! w : "<< w << ", j : " << j << ", value : " << *value << ", tmp : " << tmp << std::endl;
                                   if(tmp[y] == *value){
                                       std::cout<< "REMOVE COLUMN: " << *value << "w : " << w << ", j : " << j << std::endl;

                                       tmp.erase(tmp.begin() + y);
                                       this->board[w][j].setPossiblities(tmp);
                                       remove = true;
                                   }
                               }

                            }else
                                remove = false;
                            if(!remove){
                                for(int m = l; m < (l + 3); ++m){
                                    for(int n = c; n < (c + 3); ++n){
                                        if( ( (m != i && n != j) || ( m == i && n != j) /*|| (m != i && n == j) */ || ( m != w && n != j) || ( m == w && n != j) /*|| (m != x && n ==j) */)){

                                               if(inclusive())
                                                  return true;
                                               else if(hiddenSingle())
                                                  return true;


                                            std::vector<int> tmp = this->board[m][n].getPossibilities();

                                            for(uint8_t z = 0; z < tmp.size(); ++z){
                                                if(tmp[z] == *value){
                                                    std::cout << "COLUMN ! m : " << m << ", n : " << n << ", value : " << *value << ", tmp : " << tmp << std::endl;
                                                    tmp.erase(tmp.begin() + z);
                                                    this->board[m][n].setPossiblities(tmp);
                                                    //std::cout << "VALUE " << *value << ";  m : "<< m << ", n : " << n << std::endl;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        return true;

                    }
                }
            }
        }
    }
   return false;
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
