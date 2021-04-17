#pragma once
#include <vector>
#include "Move.h"
#include "Board.h"
class Simulation
{
private:
	std::vector<int> tiles;
	std::vector<int> initState;
public:
	Simulation(std::vector<int>);
	int getTile(int);
	void move(int, int);
	//znajdowanie ruchów dla jednego pionka
	std::vector<Move> findPossibleMoves(int);
	std::vector<Move> findSimpleMoves(int);
	bool canJump(int, int);
	std::vector<Move> findMultipleJumps(Move);

	//znajdowanie wszystkich ruchów
	std::vector<Move> findAllMoves(int);

	void makeRandomMove(int);
	bool verifyMoveForPlayer1(Move);
	bool verifyMoveForPlayer2(Move);

	int checkIfGameEnded();
	std::vector<int> getTiles();
	void reset();
};

