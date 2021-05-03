#pragma once
#include <vector>
#include "Move.h"
#include "Board.h"
class Simulation
{
private:
	std::vector<int> tiles;
	std::vector<int> initState;
	int player1_pawns_in_own_base;
	int player2_pawns_in_own_base;
	int player1_pawns_in_opponents_base;
	int player2_pawns_in_opponents_base;
	int pawns_in_base1;
	int pawns_in_base2;
	std::pair<int, int> lastMovePlayer1;
	std::pair<int, int> lastMovePlayer2;
public:
	Simulation();
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
	bool isForwardP1(int, int);
	bool isForwardP2(int, int);
	bool isInOpponentsBaseP1(int);
	bool isInOpponentsBaseP2(int);

	int checkIfGameEnded();
	void updatePawns(std::pair<int,int>, int player);
	std::vector<int> getTiles();
	void reset();
};

