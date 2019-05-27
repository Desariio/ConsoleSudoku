#ifndef CELLS_H
#define CELLS_H

#include <iostream>
#include <string>
#include <vector>

class Cells {

public:
    Cells();
    Cells(int num);
    ~Cells();

    int getNumber() const;
    void setNumber(int number);
    std::vector<int> getPossibilities();
    void setPossiblities(std::vector<int> possi);

private:
    int number;
    std::vector<int> possibilities;
};
std::ostream & operator<<(std::ostream & flux, std::vector<int> const &possi);

#endif // CELLS_H
