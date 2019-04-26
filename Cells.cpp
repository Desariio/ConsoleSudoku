#include "Cells.h"


Cells::Cells()
{
}

Cells::Cells(int number) :number(number)
{
}

Cells::~Cells()
{
}

int Cells::getNumber() const
{
    return this->number;
}
std::vector<int> Cells::getPossibilities()
{
    return this->possibilities;
}

void Cells::setPossiblities(std::vector<int> possi)
{
    this->possibilities = possi;
}


std::ostream & operator<<(std::ostream & flux, std::vector<int> const &possi) {
    for (uint8_t i = 0; i < possi.size(); ++i) {
        flux << possi[i] << '\n';
    }
    return flux;
}
