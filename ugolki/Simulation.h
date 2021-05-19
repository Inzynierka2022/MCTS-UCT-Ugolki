#pragma once
#include <vector>
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
	std::vector<std::pair<int, int>> findPossibleMoves(int);
	std::vector<std::pair<int, int>> findSimpleMoves(int);
	bool canJump(int, int);
	bool wasAlreadyVisited(int tile, std::vector<int>&);
	std::vector<std::pair<int, int>> findMultipleJumps(int x, int y, std::vector<int> visitedTiles);

	//znajdowanie wszystkich ruchów
	std::vector<std::pair<int, int>> findAllMoves(int);

	void makeRandomMove(int, int);
	bool verifyMoveForPlayer1(std::pair<int, int>, int);
	bool verifyMoveForPlayer2(std::pair<int, int>, int);
	bool isForwardP1(int, int);
	bool isForwardP2(int, int);
	bool isInBase2(int);
	bool isInBase1(int);
	bool isInBase2Border(int x);
	bool isInBase1Border(int x);

	int checkIfGameEnded(int turn);
	void updatePawns(std::pair<int, int>, int player);
	std::vector<int> getTiles();
	void setBoard(std::vector<int> newBoard);
	void reset();
};

