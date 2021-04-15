#include "Move.h"
#include <iostream>

Move::Move(int x, int y)
{
    steps.push_back(x); 
    steps.push_back(y);
}

bool Move::wasAlreadyVisited(int tile)
{
    //std::cout << "alreadyVisited" <<std::endl;
    return count(steps.begin(), steps.end(), tile);
}
