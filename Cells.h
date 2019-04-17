#ifndef CELLS_H
#define CELLS_H

#include <iostream>
#include <string>
#include <vector>

class Cells{

    public:
        Cells();
        Cells(int number);
        ~Cells();

        int getNumber();
    private:
        int number;
        std::vector<int> possibilities;
};
#endif // CELLS_H
