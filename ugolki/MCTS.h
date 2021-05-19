#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include "TreeNode.h"
#include "Simulation.h"
#include "Board.h"

constexpr auto __THREAD_NUMBER = 4;


class MCTS
{
private:
	std::vector<int> board;
	std::shared_ptr<TreeNode> root;
	int player;
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
	void simulate(const Simulation &,int turnNumber);
	void reset_tree(std::vector<int> board);
};

