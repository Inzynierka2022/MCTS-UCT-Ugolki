#include "MCTS.h"
#include <chrono>
#include <SFML/System/Sleep.hpp>
#include <iostream>
#include <windows.h>

MCTS::MCTS(std::vector<int> board, int player)
{
	this->board = board;
	this->initialBoard = board;
	this->player = player;
	this->nextPlayer = player;
	this->root = std::make_shared<TreeNode>();
	this->sim = Simulation(board);
}

//inicjuje wszystkie węzły dzieci dla danego ruchu
void MCTS::appendAllChildren(std::shared_ptr<TreeNode> move)
{
	std::vector<Move> moves = sim.findAllMoves(nextPlayer);
	std::shared_ptr<TreeNode> temp;
	for (int i = 0; i < moves.size(); i++)
	{
		temp = std::make_shared<TreeNode>(TreeNode(std::pair<int, int>(moves[i].steps[0], moves[i].steps.back()), move));
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
	nextPlayer = player;
	//wybieranie ruchów w drzewie dopóki nie dojdziemy do takiego, dla którego nie było żadnej symulacji
	do
	{
		if (nextPlayer == 1) nextPlayer = 2;
		else nextPlayer = 1;
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
std::pair<int, int> MCTS::run(sf::RenderWindow& window)
{
	appendAllChildren(root);
	Board board;
	auto startTime = std::chrono::high_resolution_clock::now();
	int maxdepth = 0;
	while (std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::high_resolution_clock::now() - startTime)).count() < 5000)
	//while(true)
	{
		auto move = chooseMoveToSimulate();		

		turn = player;
		makeAllMovesFromBranch(move);
		appendAllChildren(move);
		move->update(simulate());
		sim.reset();
		if (move->depth > maxdepth) maxdepth = move->depth;
	}

	//wyświetlanie informacji i wybór ruchu z największą ilością symulacji
	root->showInfo();
	auto moves = root->GetChildren();
	int mostSimulations = 0;
	int index;
	for (int i = 0; i < moves.size();i++) {
		moves[i]->showInfo();
		if (mostSimulations < moves[i]->getSimulations())
		{
			index = i;
			mostSimulations = moves[i]->getSimulations();
		}
	}
	printf("Max depth: %d",maxdepth);
	return moves[index]->move;
}

//zwraca true jeśli wygrana
bool MCTS::simulate()	
{
	while (!sim.checkIfGameEnded())
	{
		sim.makeRandomMove(turn);
		if (turn == 1) turn = 2;
		else turn = 1;
	}
	return sim.checkIfGameEnded() == player;
}

//wykonanie wybranego ruchu i wszystkich poprzedzających
void MCTS::makeAllMovesFromBranch(std::shared_ptr<TreeNode> move)
{
	if (move->getParent() != root) makeAllMovesFromBranch(move->getParent());
	sim.move(move->move.first, move->move.second);
	if (turn == 1) turn = 2;
	else turn = 1;
}
