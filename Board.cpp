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

Cells Board::getCells(int i, int j)
{
    return this->board[i][j];
}

void Board::setCells(int i, int j, int number)
{
    return this->board[i][j].setNumber(number);
}

bool Board::isValid()
{
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
    if(isValid())
        return true;
    else
        return false;
}

void Board::step()
{
    //show possibilities
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
    //insert one number into cell and break the loop
    while(!insert){

        nakedPair();
        nakedTriple();

        //hiddenPair();
        if(pointingPair() )
            break;

        if(inclusive() || hiddenSingle())
           break;

        if(lockedTriple())
            break;

        if(isSolved())
            insert = true;
        else
            break;
    }

     std::cout << '\n';
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
                //if there are only one possibility, set the possibility in the cell
                if (rowSingle(i, j, &value) || columnSingle(i, j, &value) || squareSingle(i, j, &value)) {
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

//TODO
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
//TODO
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

/* TODO
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

bool Board::squareSingle(int i, int j, int * value)
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
    //remove one number into a list of possiblities
    for (uint8_t z = 0; z < possibilities.size(); ++z) {
        if (possibilities[z] == *value) {
            possibilities.erase(possibilities.begin() + z);
            this->board[i][j].setPossiblities(possibilities);
        }
    }
}

void Board::removeRowPairPossibilities(std::vector<int>& possibilities, int i, int j, int x)
{
    //remove two number into a list of possiblities for a row
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
    //remove two number into a list of possiblities for a column
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
    //remove two number into a list of possiblities for a square
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
    //open file and create a board with the file containts
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
    //set the list of possibilities for each cells are equals to 0
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
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
    //check if two cells have the two same possibilities in two different cells
    //if true, remove this two possibilities in this row
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
    //check if two cells have the two same possibilities in two different cells
    //if true, remove this two possibilities in this column
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
    //check if two cells have the two same possibilities in two different cells
    //if true, remove this two possibilities in this square
    for(int k = l; k < (l + 3); ++k){
        for(int p = y; p < (y + 3); ++p){
            //check a cell different to i, j
            if( (i != k && p != j) || (i == k && p != j) || (i != k && p == j)){
                if(this->board[i][j].getPossibilities() == this->board[k][p].getPossibilities() && this->board[i][j].getPossibilities().size() == 2){
                    //std::cout << "SQUARE NAKED PAIR ! i : "<< i << ", j :" << j << "k : "<< k << ", p :" << p << std::endl;

                    for(int v = l; v < (l+3); ++v){
                        for(int w = y; w < (y+3); ++w){
                            //check a cell different to i, j and l, p
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
    //chekc in board if there are naked pair in row, column or square
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
    //remove three numbers into a list of possiblities for a row
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
    //remove three numbers into a list of possiblities for a column
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
    //remove three numbers into a list of possiblities for a square
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
                                                            this->board[i][k].setPossiblities(tmp);
                                                        }
                                                    }

                                                }
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
                                                            this->board[i][k].setPossiblities(tmp);
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
                                std::vector<int> tmp = this->board[k][j].getPossibilities();
                                removeColumnNakedTriple(tmp, i, j, k);
                            }
                        }
                    }

                    //check if 2 cells have 3 same possibilities and the third have 2 of them
                    if(this->board[i][j].getPossibilities().size() == 3
                            && (this->board[u][j].getPossibilities().size() == 3 || this->board[v][j].getPossibilities().size() == 3) ){
                        if( i != u && u != v && v != i){
                            std::vector<int> firstCell = this->board[i][j].getPossibilities();
                            std::vector<int> secondCell = this->board[u][i].getPossibilities();
                            std::vector<int> thirdCell = this->board[v][i].getPossibilities();
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
                                            if(k != i && k != u && k != v){
                                                std::vector<int> tmp = this->board[k][j].getPossibilities();
                                                for(uint8_t n = 0; n < firstCell.size(); ++n){
                                                    for(uint8_t m = 0; m < tmp.size(); ++m){
                                                        if( firstCell[n] == tmp[m]){
                                                            tmp.erase(tmp.begin() + m);
                                                            this->board[k][j].setPossiblities(tmp);
                                                        }
                                                    }
                                                }
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
                                            if(k != i && k != u && k != v){
                                                std::vector<int> tmp = this->board[k][i].getPossibilities();
                                                for(uint8_t n = 0; n < firstCell.size(); ++n){
                                                    for(uint8_t m = 0; m < tmp.size(); ++m){
                                                        if( firstCell[n] == tmp[m]){
                                                            tmp.erase(tmp.begin() + m);
                                                            this->board[k][i].setPossiblities(tmp);
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
            //check a cell different to i, j
            if( (i != k && p != j) || (i == k && j != p) || (i != k && j == p) ){
                for(int m = l; m < (l + 3); ++m){
                    for(int n = p; n < (p + 3); ++n){
                        //check a cell different to i, j and m, n
                        if( ( (m != k && p != n) || (m == k && n != p) || (m != k && n == p) )
                                && ( (i != m && n != j) || (i == m && j != n) || (i != m && j == n)) ){
                            if( ( this->board[i][j].getPossibilities() == this->board[k][p].getPossibilities() )
                                    && ( this->board[i][j].getPossibilities() == this->board[m][n].getPossibilities() )
                                    && this->board[i][j].getPossibilities().size() == 3){
                                for(int v = l; v < (l + 3); ++v){
                                    for(int w = y; w < (y + 3); ++w){
                                        //check a cell different to i, j and m, n and v, w
                                        if( ( ( (v != i && w != j) || (v == i && w != j) || (v != i && w == j) )
                                             && ( (v != k && w != p) || (v == k && w != p) || (v != k && w == p) ) )
                                             && ( (m != i && n != j) || (m == i && n != j) || (m != i && n == j) )
                                                 && ( (m != k && n != p) || (m == k && n != p) || (m != k && n == p)  )
                                                && ( (v != m && w != n) || (v == m && w != n) || (v != m && w == n))){
                                            std::vector<int> tmp = this->board[v][w].getPossibilities();
                                            //std::cout << "SQUARE" << this->board[v][w].getPossibilities() << ", i : " << i << ", j : " << j << ", v : " << v << ", w : " << w << ", m : " << m << ", n : " << n << std::endl;
                                            removeSquareNakedTriple(tmp, i, j, v, w);
                                        }
                                    }
                                }

                                if( (this->board[i][j].getPossibilities().size() == 3 )
                                        && ( (this->board[k][p].getPossibilities().size() == 3 && this->board[m][n].getPossibilities().size() == 2)
                                             || (this->board[k][p].getPossibilities().size() == 2 && this->board[m][n].getPossibilities().size() == 3) )){
                                    if( ( i == k && j != p && j != n)
                                            || ( i!= k && j == p  )){

                                        std::vector<int> firstCell = this->board[i][j].getPossibilities();
                                        std::vector<int> secondCell = this->board[k][p].getPossibilities();
                                        std::vector<int> thirdCell = this->board[m][n].getPossibilities();
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
                                            }
                                            if(equal){
                                                for(int v = l; v < (l + 3); ++v ){
                                                    for(int w = y; w < (y + 3); ++w){
                                                        //check a cell different to i, j and m, n and v, w
                                                        if( ( ( (v != i && w != j) || (v == i && w != j) || (v != i && w == j) )
                                                              && ( (v != k && w != p) || (v == k && w != p) || (v != k && w == p) ) )
                                                              && ( (m != i && n != j) || (m == i && n != j) || (m != i && n == j) )
                                                                  && ( (m != k && n != p) || (m == k && n != p) || (m != k && n == p)  )
                                                                 && ( (v != m && w != n) || (v == m && w != n) || (v != m && w == n))){
                                                            std::vector<int> tmp = this->board[v][w].getPossibilities();
                                                            for(uint8_t a = 0; a < firstCell.size(); ++a){
                                                                for(uint8_t b = 0; b < tmp.size(); ++b){
                                                                    if(firstCell[a] == tmp[b]){
                                                                        tmp.erase(tmp.begin() + b);
                                                                        this->board[v][w].setPossiblities(tmp);
                                                                    }
                                                                }
                                                            }
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
                                            }
                                            if(equal){
                                                for(int v = l; v < (l + 3); ++v ){
                                                    for(int w = y; w < (y + 3); ++w){
                                                        //check a cell different to i, j and m, n and v, w
                                                        if( ( ( (v != i && w != j) || (v == i && w != j) || (v != i && w == j) )
                                                              && ( (v != k && w != p) || (v == k && w != p) || (v != k && w == p) ) )
                                                              && ( (m != i && n != j) || (m == i && n != j) || (m != i && n == j) )
                                                                  && ( (m != k && n != p) || (m == k && n != p) || (m != k && n == p)  )
                                                                 && ( (v != m && w != n) || (v == m && w != n) || (v != m && w == n))){
                                                            std::vector<int> tmp = this->board[v][w].getPossibilities();
                                                            for(uint8_t a = 0; a < firstCell.size(); ++a){
                                                                for(uint8_t b = 0; b < tmp.size(); ++b){
                                                                    if(firstCell[a] == tmp[b]){
                                                                        tmp.erase(tmp.begin() + b);
                                                                        this->board[v][w].setPossiblities(tmp);

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

                        }
                    }
                }
            }
        }
    }
}

void Board::nakedTriple()
{
    //check if in three different cells, there are the same possiblities number
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

bool Board::lockedTriple()
{
    //check if one cell has exactly 3 possibilities and two other cells has 2 possibilities
    //and if one number of the cell with 3 possibilites is present in the other cells
    //then we can remove this number in the row, column or square
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(this->board[i][j].getNumber() == 0){
                if(rowLockedTriple(i, j) || columnLockedTriple(i, j) || squareLockedTriple(i, j))
                    return true;
            }
        }
    }
    return false;
}

bool Board::rowLockedTriple(int i, int j)
{
    bool locked = false;
    int value;
    for(int k = 0; k < 9; ++k){
        if(k != j){
            for(int u = 0; u < 9; ++u){
                if(k != j && k != u && u != j ){
                    //check if one number is present in 3 cells and 3 cells have 3 or less possibilities
                    if(this->board[i][j].getPossibilities().size() == 3
                            && this->board[i][k].getPossibilities().size() == 2
                            && this->board[i][u].getPossibilities().size() == 2){
                        std::vector<int> firstCell = this->board[i][j].getPossibilities();
                        std::vector<int> secondCell = this->board[i][k].getPossibilities();
                        std::vector<int> thirdCell = this->board[i][u].getPossibilities();

                        for(uint8_t z = 0; z < firstCell.size(); ++z){
                            for(uint8_t v = 0; v < secondCell.size(); ++v){
                                for(uint8_t x = 0; x < thirdCell.size(); ++x){
                                    if(firstCell[z] == secondCell[v] && firstCell[z] == thirdCell[x]){
                                        locked = true;
                                        value = firstCell[z];
                                    }else
                                        locked = false;
                                }
                            }
                        }
                        if(locked){
                            for(int p = 0; p < 9; ++p){
                                if(p != j && p != u && p != k ){
                                    std::vector<int> tmp = this->board[i][p].getPossibilities();
                                    for(uint8_t x = 0; x < firstCell.size(); ++x){
                                        for(uint8_t z = 0; z < tmp.size(); ++z){
                                            if(tmp[z] == firstCell[x]){
                                                tmp.erase(tmp.begin() + z);
                                                this->board[i][p].setPossiblities(tmp);
                                                //std::cout << "i :" << i << ", j : "<< j << ", u :" << u << ", k :" << k << ", p :" << p << " ROW ! Value : " << value << std::endl;
                                            }
                                        }
                                    }
                                    if(inclusive()) return true;
                                    else if(hiddenSingle()) return true;
                                }
                            }
                        }

                    }
                }
            }
        }
    }
    return false;
}

bool Board::columnLockedTriple(int i, int j)
{
    bool locked = false;
    int value;
    for(int k = 0; k < 9; ++k){
        if(k != i){
            for(int u = 0; u < 9; ++u){
                if(k != i && k != u && u != i ){
                    //check if one number is present in 3 cells and 3 cells have 3 or less possibilities
                    if(this->board[i][j].getPossibilities().size() == 3
                            && this->board[k][j].getPossibilities().size() == 2
                            && this->board[u][j].getPossibilities().size() == 2){
                        std::vector<int> firstCell = this->board[i][j].getPossibilities();
                        std::vector<int> secondCell = this->board[k][j].getPossibilities();
                        std::vector<int> thirdCell = this->board[u][j].getPossibilities();

                        for(uint8_t z = 0; z < firstCell.size(); ++z){
                            for(uint8_t v = 0; v < secondCell.size(); ++v){
                                for(uint8_t x = 0; x < thirdCell.size(); ++x){
                                    if(firstCell[z] == secondCell[v] && firstCell[z] == thirdCell[x]){
                                        locked = true;
                                        value = firstCell[z];
                                    }else
                                        locked = false;
                                }
                            }
                        }
                        if(locked){
                            for(int p = 0; p < 9; ++p){
                                if(p != i && p != u && p != k ){
                                    std::vector<int> tmp = this->board[p][j].getPossibilities();
                                    for(uint8_t x = 0; x < firstCell.size(); ++x){
                                        for(uint8_t z = 0; z < tmp.size(); ++z){
                                            if(tmp[z] == firstCell[x]){
                                                tmp.erase(tmp.begin() + z);
                                                this->board[p][j].setPossiblities(tmp);
                                                //std::cout << "i :" << i << ", j : "<< j<< ", p : " << p << "COLUMN ! Value : " << value << std::endl;
                                            }
                                        }
                                    }
                                    if(inclusive()) return true;
                                    else if(hiddenSingle()) return true;
                                }
                            }
                        }

                    }
                }
            }
        }
    }
    return false;
}

bool Board::squareLockedTriple(int i, int j)
{
    int l, c;
    squareIndex(i, j, &l, &c);
    bool locked = false;
    int value;

    for(int k = l; k < (l + 3); ++k){
        for(int p = c; p < (c + 3); ++p){
            //check a cell different to i, j
            if( (i != k && p != j) || (i == k && j != p) || (i != k && j == p) ){
                for(int m = l; m < (l + 3); ++m){
                    for(int n = p; n < (p + 3); ++n){
                        //check a cell different to i, j and k, p
                        if( ( (m != k && p != n) || (m == k && n != p) || (m != k && n == p) )
                                && ( (i != m && n != j) || (i == m && j != n) || (i != m && j == n)) ){
                            //check if one number is present in 3 cells and 3 cells have 3 or less possibilities
                            if(this->board[i][j].getPossibilities().size() == 3
                                    && this->board[k][p].getPossibilities().size() == 2
                                    && this->board[m][n].getPossibilities().size() == 2){
                                for(int v = l; v < (l + 3); ++v){
                                    for(int w = c; w < (c + 3); ++w){
                                        //check a cell different to i, j and k, p and v, w
                                        if( ( ( (v != i && w != j) || (v == i && w != j) || (v != i && w == j) )
                                             && ( (v != k && w != p) || (v == k && w != p) || (v != k && w == p) ) )
                                             && ( (m != i && n != j) || (m == i && n != j) || (m != i && n == j) )
                                                 && ( (m != k && n != p) || (m == k && n != p) || (m != k && n == p)  )
                                                && ( (v != m && w != n) || (v == m && w != n) || (v != m && w == n))){
                                            std::vector<int> firstCell = this->board[i][j].getPossibilities();
                                            std::vector<int> secondCell = this->board[k][p].getPossibilities();
                                            std::vector<int> thirdCell = this->board[m][n].getPossibilities();

                                            for(uint8_t z = 0; z < firstCell.size(); ++z){
                                                for(uint8_t t = 0; t < secondCell.size(); ++t){
                                                    for(uint8_t x = 0; x < thirdCell.size(); ++x){
                                                        if(firstCell[z] == secondCell[t] && firstCell[z] == thirdCell[x]){
                                                            locked = true;
                                                            value = firstCell[z];
                                                        }else
                                                            locked = false;
                                                    }
                                                }
                                            }
                                            if(locked){
                                                for(int a = l; a < (l + 3); ++a){
                                                    for(int b = c; b < (c + 3); ++b){
                                                        if( ( ( (a != i && b != j) || (a == i && b != j) || (a != i && b == j) )
                                                             && ( (a != k && b != p) || (a == k && b != p) || (a != k && b == p) ) )
                                                             && ( (m != i && n != j) || (m == i && n != j) || (m != i && n == j) )
                                                                 && ( (m != k && n != p) || (m == k && n != p) || (m != k && n == p)  )
                                                                && ( (a != m && b != n) || (a == m && b != n) || (a != m && b == n))){

                                                            std::vector<int> tmp = this->board[a][b].getPossibilities();
                                                            for(uint8_t e = 0; e < firstCell.size(); ++e){
                                                                for(uint8_t d = 0; d < tmp.size(); ++d){
                                                                    if(tmp[d] == firstCell[e]){
                                                                        tmp.erase(tmp.begin() + d);
                                                                        this->board[a][b].setPossiblities(tmp);
                                                                    }
                                                                }
                                                            }
                                                            if(inclusive()) return true;
                                                            else if(hiddenSingle()) return true;
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
            }
        }
    }
    return false;
}

bool Board::pointingPair()
{
    //check if a single possibilities number is present just two or three cells next to each other( in same row or same column)
    //and not present in another cells in the same square
    //then remove this number
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(this->board[i][j].getNumber() == 0){
                int value = 0;
                if(rowPointingPair(i, j, &value) || columnPointingPair(i, j, &value))
                    return true;
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
                               std::vector<int> tmp = this->board[i][w].getPossibilities();
                               for(uint8_t y = 0; y < tmp.size(); ++y){
                                   if(tmp[y] == *value){
                                       //std::cout<< "REMOVE ROW: " << *value << "i : " << i << ", w : " << w << std::endl;
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
                                        if( ( (m != i && n != j) || ( m == i && n != j) || (m != i && n == j)  || ( m != i && n != w) || ( m == i && n != w) || (m != i && n == w) )){

                                            if(inclusive())
                                                return true;
                                            else if(hiddenSingle())
                                                return true;
                                            std::vector<int> tmp = this->board[m][n].getPossibilities();

                                            for(uint8_t z = 0; z < tmp.size(); ++z){
                                                if(tmp[z] == *value){
                                                    //std::cout << "ROW ici?! i : "<< i << ", j : " << j << ", m : " << m << ", n : " << n << ", value : " << *value << ", tmp : " << tmp << std::endl;
                                                    tmp.erase(tmp.begin() + z);
                                                    this->board[m][n].setPossiblities(tmp);
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
                    if( ( this->board[i][j].getPossibilities()[z] == *value && this->board[x][j].getPossibilities()[v] == *value)
                            || (this->board[i][j].getPossibilities()[z] == *value && this->board[x][j].getPossibilities()[v] == *value && this->board[x + 1][j].getPossibilities()[v] == *value ) ){

                        for(int w = 0; w < 9; ++w){
                            if( (w != i) && (w != x) && ( w != (x + 1) ) ){
                               std::vector<int> tmp = this->board[w][j].getPossibilities();
                               for(uint8_t y = 0; y < tmp.size(); ++y){
                                   if(tmp[y] == *value){
                                       //std::cout<< "REMOVE COLUMN: " << *value << "w : " << w << ", j : " << j << std::endl;

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
                                                    //std::cout << "COLUMN ! m : " << m << ", n : " << n << ", value : " << *value << ", tmp : " << tmp << std::endl;
                                                    tmp.erase(tmp.begin() + z);
                                                    this->board[m][n].setPossiblities(tmp);
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
