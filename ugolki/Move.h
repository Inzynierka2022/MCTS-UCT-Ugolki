#pragma once
#include <vector>
class Move
{
public:
	std::vector<int> steps;

	Move(int, int);
	bool wasAlreadyVisited(int);
};

