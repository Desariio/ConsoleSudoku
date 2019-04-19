#include <iostream>
#include "Board.h"

int main(int ac, char **argv){

    if(ac < 2){
        std::cerr << "Usage: " << argv[0] << "<path/to/board>" << std::endl;
        return 0;
    }

    Board b(argv[1]);
    if(!b.isValid()){
        std::cerr << "This sudoku board is not valid!" << std::endl;
        return 0;
    }

    int steps = 0;
    Board tmp;
    do
    {
        std::cout << b << '\n' << std::endl;
        tmp = b;
        b.step();
        std::cout << "TMP \n" << tmp << '\n' << std::endl;
        ++steps;
    }while(b != tmp);
    std::cout << "steps " << steps << " steps!" << std::endl;
   /* if(b.isSolved())
        std::cout << "Board solved in " << steps << " steps!" << std::endl;
    else
        std::cout << "This board cannot be solved...";*/
    return 0;
}
