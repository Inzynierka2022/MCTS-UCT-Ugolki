#include "Simulation.h"
Simulation::Simulation(std::vector<int> tiles)
{
	this->tiles = tiles;
	this->initState = tiles;
	this->player1_pawns_in_own_base = 0;
	this->player2_pawns_in_own_base = 0;
	this->player1_pawns_in_opponents_base = 0;
	this->player2_pawns_in_opponents_base = 0;
	for (int i = 0; i < 64; i++)
	{
		if (tiles[i] == 1)
		{
			player1_pawns_in_own_base += 1 * isInBase1(i);
			player1_pawns_in_opponents_base += 1 * isInBase2(i);
		}
		else if (tiles[i] == 2)
		{
			player2_pawns_in_own_base += 1 * isInBase2(i);
			player2_pawns_in_opponents_base += 1 * isInBase1(i);
		}
	}
	pawns_in_base1 = player1_pawns_in_own_base + player2_pawns_in_opponents_base;
	pawns_in_base2 = player2_pawns_in_own_base + player1_pawns_in_opponents_base;
}

int Simulation::getTile(int x)
{
	if (x < 0 || x > 63) return -1;
	return tiles[x];
}

void Simulation::move(int x, int y)
{
	//śledzenie ilości pionków na polach końcowych	
	updatePawns(std::pair<int, int>(x, y), getTile(x));
	int temp = tiles[x];
	tiles[x] = 0;
	tiles[y] = temp;
}

std::vector<std::pair<int, int>> Simulation::findPossibleMoves(int pawn)
{
	std::vector<std::pair<int, int>> moves, temp_moves;
	temp_moves = findSimpleMoves(pawn);

	if (temp_moves.size() > 0)
		moves = temp_moves;

	int temp[] = { -2, 2, -16, 16 };
	for (int i = 0; i < 4; i++)
	{
		if (pawn / 8 != (pawn + temp[i]) / 8 && pawn % 8 != (pawn + temp[i]) % 8) continue;
		if (canJump(pawn, pawn + temp[i]))
		{
			moves.push_back(std::pair<int, int>(pawn, pawn + temp[i]));
			temp_moves = findMultipleJumps(pawn, pawn + temp[i], std::vector<int>({ pawn,pawn + temp[i] }));
			if (temp_moves.size() > 0)
				moves.insert(moves.end(), temp_moves.begin(), temp_moves.end());
		}
	}

	/*for (int i = 0; i < moves.size(); i++)
	{
		for (int j = 0; j < moves[i].steps.size(); j++)
		{
			std::cout << moves[i].steps[j] << ", ";
		}
		std::cout << "\n";
	}*/
	//std::cout << "Possible moves: " << moves.size() << "\n";
	return moves;
}

Simulation::Simulation()
{
}

std::vector<std::pair<int, int>> Simulation::findSimpleMoves(int x)
{
	//std::cout << "findSimpleMoves" << std::endl;
	std::vector<std::pair<int, int>> moves;
	int temp[] = { -1,1,-8,8 };
	for (int i = 0; i < 4; i++)
	{
		if (x / 8 != (x + temp[i]) / 8 && x % 8 != (x + temp[i]) % 8) continue;
		if (getTile(x + temp[i]) == 0) moves.push_back(std::pair<int, int>(x, x + temp[i]));

	}
	return moves;
}

bool Simulation::canJump(int x, int y)
{
	//::cout << "canJump(" << x << ", " << y << ")";
	int distance = y - x;
	//std::cout << (getTile(y) == 0 && getTile(x + (distance / 2)) != 0) << std::endl;
	return (getTile(y) == 0 && getTile(x + (distance / 2)) != 0);
}

bool Simulation::wasAlreadyVisited(int tile, std::vector<int>& steps)
{
	return std::find(steps.begin(), steps.end(), tile) != steps.end();
}

std::vector<std::pair<int, int>> Simulation::findMultipleJumps(int x, int y, std::vector<int> visitedTiles)
{
	//std::cout << "findMultipleJumps" << std::endl;
	std::vector<std::pair<int, int>> moves, temp_moves;

	int temp[] = { -2, 2, -16, 16 };
	for (int i = 0; i < 4; i++)
	{
		if (y / 8 != (y + temp[i]) / 8 && y % 8 != (y + temp[i]) % 8) continue;
		if (canJump(y, y + temp[i]) && !wasAlreadyVisited(y + temp[i], visitedTiles))
		{
			visitedTiles.push_back(y + temp[i]);
			moves.push_back(std::pair<int, int>(x, y + temp[i]));
			temp_moves = findMultipleJumps(x, y + temp[i], visitedTiles);
			moves.insert(moves.end(), temp_moves.begin(), temp_moves.end());
			visitedTiles.pop_back();
		}
	}
	//std::cout << moves.size() << std::endl;
	return moves;
}


std::vector<std::pair<int, int>> Simulation::findAllMoves(int pawn_type)
{
	std::vector<std::pair<int, int>> moves, temp_moves;
	for (int i = 0; i < 64; i++)
	{
		temp_moves.clear();
		if (getTile(i) == pawn_type) temp_moves = findPossibleMoves(i);
		if (temp_moves.size() > 0) moves.insert(moves.end(), temp_moves.begin(), temp_moves.end());
	}

	/*for (int i = 0; i < moves.size(); i++)
	{
		for (int j = 0; j < moves[i].steps.size(); j++)
		{
			std::cout << moves[i].steps[j] << ", ";
		}
		std::cout << "\n";
	}
	std::cout << "All possible moves: " << moves.size() << std::endl;*/
	return moves;
}


void Simulation::makeRandomMove(int player, int turnNumber)
{
	std::vector<std::pair<int, int>> moves = findAllMoves(player);
	int random_move;
	std::pair<int, int> chosen;
	bool make_move = false;
	int i = 0;
	do
	{
		//losowanie ruchu
		if (moves.size() == 0)
		{
			reset();
			break;
		}
		random_move = rand() % moves.size();
		chosen = moves.at((size_t)random_move);

		//decyzja czy wykonać wylosowany ruch
		if (player == 1)
		{
			make_move = verifyMoveForPlayer1(chosen, turnNumber);
		}
		else
		{
			make_move = verifyMoveForPlayer2(chosen, turnNumber);
		}
		i++;
		if (i > 10000)
		{
			reset();
			break;
		}
	} while (make_move == false);

	move(chosen.first, chosen.second);


	if (player == 1)
		lastMovePlayer1 = std::pair<int, int>(chosen.first, chosen.second);
	if (player == 2)
		lastMovePlayer2 = std::pair<int, int>(chosen.first, chosen.second);
}


bool Simulation::verifyMoveForPlayer1(std::pair<int, int> m, int turnNumber)
{
	if (player1_pawns_in_own_base != 0 && !isInBase1(m.first) && rand() % 100 > 5) return false;
	//do przodu
	if (isForwardP1(m.first, m.second))
	{
		//jeśli cofa poprzedni ruch
		if (lastMovePlayer1.first == m.second && lastMovePlayer1.second == m.first)
			return false;
		if (turnNumber <= 40 && !isInBase1(m.first) && rand() % 100 > 98) return false;
		if ((abs(m.first - m.second) == 1 || abs(m.first - m.second) == 8) && turnNumber <= 40 && rand() % 100 > 98) return false;
		return true;
	}
	else
	{
		if (turnNumber <= 40 && isInBase1Border(m.first) && isInBase1Border(m.second)) return true;
		if (turnNumber > 40 && isInBase2Border(m.first) && isInBase2Border(m.second)) return true;
	}
	return false;
}

bool Simulation::verifyMoveForPlayer2(std::pair<int, int> m, int turnNumber)
{
	if (player2_pawns_in_own_base != 0 && !isInBase2(m.first) && rand() % 100 > 5) return false;
	//jeśli cofa poprzedni ruch
	if (lastMovePlayer2.first == m.second && lastMovePlayer2.second == m.first)
		return false;
	//do przodu
	if (isForwardP2(m.first, m.second))
	{
		//jeśli cofa poprzedni ruch
		if (lastMovePlayer2.first == m.second && lastMovePlayer2.second == m.first)
			return false;
		if (turnNumber <= 40 && !isInBase2(m.first) && rand() % 100 > 98) return false;
		if ((abs(m.first - m.second) == 1 || abs(m.first - m.second) == 8) && turnNumber <= 40 && rand() % 100 > 98) return false;
		return true;
	}
	else
	{
		if (turnNumber <= 40 && isInBase2Border(m.first) && isInBase2Border(m.second)) return true;
		if (turnNumber > 40 && isInBase1Border(m.first) && isInBase1Border(m.second)) return true;
	}
	return false;
}

bool Simulation::isForwardP1(int x, int y)
{
	return (y % 8 - x % 8) + (y / 8 - x / 8) > 0;
}

bool Simulation::isForwardP2(int x, int y)
{
	return (y % 8 - x % 8) + (y / 8 - x / 8) < 0;
}

bool Simulation::isInBase2(int x)
{
	return x > 35 && x % 8 > 3;
}

bool Simulation::isInBase1(int x)
{
	return x < 28 && x % 8 < 4;
}

bool Simulation::isInBase2Border(int x)
{
	return (x > 35 && x<40) || (x >35 && x%8==4);
}

bool Simulation::isInBase1Border(int x)
{
	return (x > 23 && x < 28) || (x < 28 && x % 8 == 3);
}

void Simulation::updatePawns(std::pair<int, int> m, int player)
{
	bool x_isInOpponentsBase, y_isInOpponentsBase, x_isInOwnBase, y_isInOwnBase;
	if (player == 1)
	{
		x_isInOpponentsBase = isInBase2(m.first);
		y_isInOpponentsBase = isInBase2(m.second);
		x_isInOwnBase = isInBase1(m.first);
		y_isInOwnBase = isInBase1(m.second);
		//jeśli wychodzi z pola przeciwnika
		if (x_isInOpponentsBase && !y_isInOpponentsBase)
			player1_pawns_in_opponents_base--;
		//jeśli wchodzi do pola przeciwnika
		if (!x_isInOpponentsBase && y_isInOpponentsBase)
			player1_pawns_in_opponents_base++;
		//jeśli wychodzi ze swojego pola
		if (x_isInOwnBase && !y_isInOwnBase)
			player1_pawns_in_own_base--;
		//jeśli wchodzi do swojego pola
		if (!x_isInOwnBase && y_isInOwnBase)
			player1_pawns_in_own_base++;
	}
	else
	{
		x_isInOpponentsBase = isInBase1(m.first);
		y_isInOpponentsBase = isInBase1(m.second);
		x_isInOwnBase = isInBase2(m.first);
		y_isInOwnBase = isInBase2(m.second);
		//jeśli wychodzi z pola przeciwnika
		if (x_isInOpponentsBase && !y_isInOpponentsBase)
			player2_pawns_in_opponents_base--;
		//jeśli wchodzi do pola przeciwnika
		if (!x_isInOpponentsBase && y_isInOpponentsBase)
			player2_pawns_in_opponents_base++;
		//jeśli wychodzi ze swojego pola
		if (x_isInOwnBase && !y_isInOwnBase)
			player2_pawns_in_own_base--;
		//jeśli wchodzi do swojego pola
		if (!x_isInOwnBase && y_isInOwnBase)
			player2_pawns_in_own_base++;
	}
	pawns_in_base1 = player1_pawns_in_own_base + player2_pawns_in_opponents_base;
	pawns_in_base2 = player2_pawns_in_own_base + player1_pawns_in_opponents_base;
	/*std::cout << "Player1 pawns in own base: " << player1_pawns_in_own_base << "\n";
	std::cout << "Player2 pawns in opponents base: " << player2_pawns_in_opponents_base << "\n";
	std::cout << "Player2 pawns in own base: " << player2_pawns_in_own_base << "\n";
	std::cout << "Player1 pawns in opponents base: " << player1_pawns_in_opponents_base << "\n\n";*/
}


int Simulation::checkIfGameEnded(int turn)
{
	bool player1_win = true, player2_win = true;
	for (int i = 0; i < 64; i++)
	{
		if (i < 28 && i % 8 < 4)
		{
			if (tiles[i] != 2) player2_win = false;
		}
		else if (i > 35 && i % 8 > 3)
		{
			if (tiles[i] != 1) player1_win = false;
		}
	}
	if (turn >= 40)
	{
		if (player1_pawns_in_own_base > 0)	player2_win = true;
		if (player2_pawns_in_own_base > 0) player1_win = true;
	}
	/*if ((2 * player2_win) + (1 * player1_win) == 2)
	{
		for (int i = 0; i < 64; i++)
		{
			if (i % 8 == 0) std::cout << "\n";
			std::cout << tiles[i];
		}
		std::cout << std::endl;
		std::cout << player1_pawns_in_own_base << std::endl;
		std::cout << player2_pawns_in_own_base << std::endl;
	}*/
	return (2 * player2_win) + (1 * player1_win);
}

std::vector<int> Simulation::getTiles()
{
	return tiles;
}


void Simulation::setBoard(std::vector<int> newBoard)
{
	this->initState = newBoard;
	this->reset();
}

void Simulation::reset()
{
	tiles = initState;
	player1_pawns_in_own_base = 0;
	player2_pawns_in_own_base = 0;
	player1_pawns_in_opponents_base = 0;
	player2_pawns_in_opponents_base = 0;
	for (int i = 0; i < 64; i++)
	{
		if (tiles[i] == 1)
		{
			player1_pawns_in_own_base += 1 * isInBase1(i);
			player1_pawns_in_opponents_base += 1 * isInBase2(i);
		}
		else if (tiles[i] == 2)
		{
			player2_pawns_in_own_base += 1 * isInBase2(i);
			player2_pawns_in_opponents_base += 1 * isInBase1(i);
		}
	}
	this->pawns_in_base1 = player1_pawns_in_own_base + player2_pawns_in_opponents_base;
	this->pawns_in_base2 = player2_pawns_in_own_base + player1_pawns_in_opponents_base;
}
