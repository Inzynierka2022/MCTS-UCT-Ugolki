#include "MCTS.h"
#include <chrono>
#include <SFML/System/Sleep.hpp>
#include <iostream>
#include <windows.h>

MCTS::MCTS(std::vector<int> board, int player)
{
	this->board = board;
	this->player = player;
	this->root = std::make_shared<TreeNode>();
	this->sim = Simulation(this->board);
}

//inicjuje wszystkie węzły dzieci dla danego ruchu
void MCTS::appendAllChildren(std::shared_ptr<TreeNode> move)
{
	int p;
	if (move->depth % 2 == 0) p = player;
	else p = player == 1 ? 2 : 1;
	std::vector<std::pair<int, int>> moves = sim.findAllMoves(p);
	std::shared_ptr<TreeNode> temp;
	for (int i = 0; i < moves.size(); i++)
	{
		temp = std::make_shared<TreeNode>(TreeNode(std::pair<int, int>(moves[i].first, moves[i].second), move));
		move->appendChild(temp);
	}
}

//wybiera ruch na podstawie UCT
std::shared_ptr<TreeNode> MCTS::chooseMoveToSimulate()
{
	std::vector<std::shared_ptr<TreeNode>> moves;
	double highestUCT = 0;
	int index = 0;
	double uct;
	std::shared_ptr<TreeNode> temp = root;
	bool lookingForHighest = true;
	//wybieranie ruchów w drzewie dopóki nie dojdziemy do takiego, dla którego nie było żadnej symulacji
	do
	{
		moves = temp->GetChildren();
		for (int i = 0; i < moves.size(); i++)
		{
			if (lookingForHighest)
				uct = moves[i]->calculateUCT();
			else
				uct = moves[i]->calculateUCTOpponent();
			if (uct == std::numeric_limits<double>::max())
			{
				return moves[i];
			}
			if (uct > highestUCT)
			{
				highestUCT = uct;
				index = i;
			}
		}
		temp = moves[index];
		lookingForHighest = !lookingForHighest;
		highestUCT = 0;
	} while (temp->hasChildren());
	return temp;
}

//zwraca najlepszy ruch
std::pair<int, int> MCTS::run(sf::RenderWindow& window, int turnNumber)
{
	appendAllChildren(root);
	auto startTime = std::chrono::high_resolution_clock::now();
	int maxdepth = 0;

	auto t1 = std::chrono::high_resolution_clock::now();

	for (auto start = std::chrono::steady_clock::now(), now = start; now < start + std::chrono::seconds{ 5 }; now = std::chrono::steady_clock::now())
	{
		this->successed_simulations.store(0);
		std::shared_ptr<TreeNode> move = chooseMoveToSimulate();

		this->turn = this->player;
		makeAllMovesFromBranch(move);
		appendAllChildren(move);
		
		
		std::vector<std::thread> threads;
		for (unsigned int i=0;i< __THREAD_NUMBER; i++)
		{
			threads.push_back(std::thread(&MCTS::simulate,this, sim, turnNumber));
		}
		
		for (unsigned int i = 0; i < __THREAD_NUMBER; i++)
		{
			threads[i].join();
		}

		move->update(this->successed_simulations.load(), __THREAD_NUMBER);
		sim.setBoard(board);
		
		if (move->depth > maxdepth) maxdepth = move->depth;
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
	std::cout << "loop run lasted:  ---" << fp_ms.count() << std::endl;

	//wyświetlanie informacji i wybór ruchu z największą ilością symulacji
	root->showInfo();
	auto moves = root->GetChildren();
	int mostSimulations = 0;
	int index = 0;
	for (int i = 0; i < moves.size(); i++) {
		moves[i]->showInfo();
		if (mostSimulations < moves[i]->getSimulations())
		{
			index = i;
			mostSimulations = moves[i]->getSimulations();
		}
	}
	printf("Max depth: %d\n", maxdepth);
	return moves[index]->move;
}

void MCTS::simulate(const Simulation &sim1, int turnNumber)
{
	Simulation sim2 = sim1;

	while (!sim2.checkIfGameEnded(turnNumber))
	{
		sim2.makeRandomMove(turn, turnNumber);
		if (turn == 2) turnNumber++;
		if (turn == 1) turn = 2;
		else turn = 1;
	}

	if (sim2.checkIfGameEnded(turnNumber) == player)
	{
		std::lock_guard<std::mutex> lock(this->simulations_mutex);
		this->successed_simulations.store(this->successed_simulations.load()+1);
	}
}

void MCTS::reset_tree(std::vector<int> board)
{
	this->board = board;
	this->sim.setBoard(board);
	this->root = std::make_shared<TreeNode>();
}

//wykonanie wybranego ruchu i wszystkich poprzedzających
void MCTS::makeAllMovesFromBranch(std::shared_ptr<TreeNode> move)
{
	if (move->getParent() != root) makeAllMovesFromBranch(move->getParent());
	sim.move(move->move.first, move->move.second);
	if (turn == 1) turn = 2;
	else turn = 1;
}
