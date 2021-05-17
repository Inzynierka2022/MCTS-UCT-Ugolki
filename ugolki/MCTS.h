#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include "TreeNode.h"
#include "Simulation.h"
#include "Board.h"

constexpr auto __THREAD_NUMBER = 50;


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

	std::mutex simulations_mutex;
	std::atomic_int successed_simulations;
public:
	MCTS(std::vector<int> board, int player);
	void appendAllChildren(std::shared_ptr<TreeNode> move);
	std::shared_ptr<TreeNode> chooseMoveToSimulate();
	void makeAllMovesFromBranch(std::shared_ptr<TreeNode> move);
	std::pair<int, int> run(sf::RenderWindow& window, int turn);
	bool simulate(const Simulation &,int turnNumber);
	void reset_tree(std::vector<int> board, int player);
};

