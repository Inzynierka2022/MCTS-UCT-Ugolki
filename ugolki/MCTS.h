#pragma once
#include <vector>
#include "TreeNode.h"
#include "Simulation.h"
#include "Board.h"
class MCTS
{
private:
	std::vector<int> board;
	std::vector<int> initialBoard;
	std::shared_ptr<TreeNode> root;
	int player;
	int nextPlayer;
	int turn;
	Simulation sim;
public:
	MCTS(std::vector<int> board, int player);
	void appendAllChildren(std::shared_ptr<TreeNode> move);
	std::shared_ptr<TreeNode> chooseMoveToSimulate();
	void makeAllMovesFromBranch(std::shared_ptr<TreeNode> move);
	std::pair<int, int> run(sf::RenderWindow& window);
	bool simulate();
};

