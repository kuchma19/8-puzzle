#include "solver.h"

#include <iostream>

int main()
{
    Board board;
    Solver solver(board);
    std::cout << solver.moves() << std::endl;

    for (const auto move : solver) {
        std::cout << move << std::endl;
    }

}
