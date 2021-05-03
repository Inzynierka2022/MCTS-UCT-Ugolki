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
			player1_pawns_in_own_base += 1 * isInOpponentsBaseP2(i);
			player1_pawns_in_opponents_base += 1 * isInOpponentsBaseP1(i);
		}
		else if (tiles[i] == 2)
		{
			player2_pawns_in_own_base += 1 * isInOpponentsBaseP1(i);
			player2_pawns_in_opponents_base += 1 * isInOpponentsBaseP2(i);
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

std::vector<Move> Simulation::findPossibleMoves(int pawn)
{
	std::vector<Move> moves, temp_moves;
	temp_moves = findSimpleMoves(pawn);

	if (temp_moves.size() > 0)
		moves = temp_moves;

	int temp[] = { -2, 2, -16, 16 };
	for (int i = 0; i < 4; i++)
	{
		if (pawn / 8 != (pawn + temp[i]) / 8 && pawn % 8 != (pawn + temp[i]) % 8) continue;
		if (canJump(pawn, pawn + temp[i]))
		{
			moves.push_back(Move(pawn, pawn + temp[i]));
			temp_moves = findMultipleJumps(moves[moves.size() - 1]);
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

std::vector<Move> Simulation::findSimpleMoves(int x)
{
	//std::cout << "findSimpleMoves" << std::endl;
	std::vector<Move> moves;
	int temp[] = { -1,1,-8,8 };
	for (int i = 0; i < 4; i++)
	{
		if (x / 8 != (x + temp[i]) / 8 && x % 8 != (x + temp[i]) % 8) continue;
		if (getTile(x + temp[i]) == 0) moves.push_back(Move(x, x + temp[i]));

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

std::vector<Move> Simulation::findMultipleJumps(Move m)
{
	//std::cout << "findMultipleJumps" << std::endl;
	std::vector<Move> moves, temp_moves;
	int x = m.steps.back();

	int temp[] = { -2, 2, -16, 16 };
	for (int i = 0; i < 4; i++)
	{
		if (x / 8 != (x + temp[i]) / 8 && x % 8 != (x + temp[i]) % 8) continue;
		Move newMove = m;
		if (canJump(x, x + temp[i]) && !m.wasAlreadyVisited(x + temp[i]))
		{
			newMove.steps.push_back(x + temp[i]);
			moves.push_back(newMove);
			temp_moves = findMultipleJumps(newMove);
			moves.insert(moves.end(), temp_moves.begin(), temp_moves.end());
		}
	}
	//std::cout << moves.size() << std::endl;
	return moves;
}

//trzeba jakoś uprościć wyszukiwanie ruchów
std::vector<Move> Simulation::findAllMoves(int pawn_type)
{
	std::vector<Move> moves, temp_moves;
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

void Simulation::makeRandomMove(int player)
{
	std::vector<Move> moves = findAllMoves(player);
	int random_move;
	Move chosen;
	bool make_move = false;
	do
	{
		//losowanie ruchu
		random_move = rand() % moves.size();
		chosen = moves.at((size_t)random_move);

		//decyzja czy wykonać wylosowany ruch
		if (player == 1)
		{
			make_move = verifyMoveForPlayer1(chosen);
		}
		else
		{
			make_move = verifyMoveForPlayer2(chosen);
		}
	} while (make_move == false);

	move(chosen.steps[0], chosen.steps.back());


	if (player == 1)
		lastMovePlayer1 = std::pair<int, int>(chosen.steps[0], chosen.steps.back());
	if (player == 2)
		lastMovePlayer2 = std::pair<int, int>(chosen.steps[0], chosen.steps.back());
}

void Simulation::updatePawns(std::pair<int, int> m, int player)
{
	bool x_isInOpponentsBase, y_isInOpponentsBase, x_isInOwnBase, y_isInOwnBase;
	if (player == 1)
	{
		x_isInOpponentsBase = isInOpponentsBaseP1(m.first);
		y_isInOpponentsBase = isInOpponentsBaseP1(m.second);
		x_isInOwnBase = isInOpponentsBaseP2(m.first);
		y_isInOwnBase = isInOpponentsBaseP2(m.second);
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
		x_isInOpponentsBase = isInOpponentsBaseP2(m.first);
		y_isInOpponentsBase = isInOpponentsBaseP2(m.second);
		x_isInOwnBase = isInOpponentsBaseP1(m.first);
		y_isInOwnBase = isInOpponentsBaseP1(m.second);
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


bool Simulation::verifyMoveForPlayer1(Move m)
{
	//jeśli cofa poprzedni ruch
	if (lastMovePlayer1.first == m.steps.back() && lastMovePlayer1.second == m.steps[0])
		return false;
	//do przodu
	if (isForwardP1(m.steps[0], m.steps.back()))
	{
		return true;
	}
	else
	{
		//jeśli cofa pozostając w polu przeciwnika
		if (isInOpponentsBaseP1(m.steps.back())) return true;
		//jeśli ruch wychodzący z bazy przeciwnika, a przeciwnik ma pionki w bazie (zapobieganie blokowaniu)
		else if (isInOpponentsBaseP1(m.steps[0]) && (player2_pawns_in_own_base != 0 && player2_pawns_in_own_base <= 4 && pawns_in_base2 > 14 || pawns_in_base2 == 16)) return true;
	}
	//można dodać jakies warunki jak tylko np. 2 pionki zostały do końca
	//if (player1_pawns_in_opponents_base > 13 && !isInOpponentsBaseP1(m.steps[0])) return true;
	return false;
}

bool Simulation::verifyMoveForPlayer2(Move m)
{
	//jeśli cofa poprzedni ruch
	if (lastMovePlayer2.first == m.steps.back() && lastMovePlayer2.second == m.steps[0])
		return false;
	//do przodu
	if (isForwardP2(m.steps[0], m.steps.back()))
	{
		return true;
	}
	else
	{
		//jeśli cofa pozostając w polu przeciwnika
		if (isInOpponentsBaseP2(m.steps.back())) return true;
		//jeśli ruch wychodzący z bazy przeciwnika, a przeciwnik ma pionki w bazie (zapobieganie blokowaniu)
		else if (isInOpponentsBaseP2(m.steps[0]) && (player1_pawns_in_own_base != 0 && player1_pawns_in_own_base <= 4 && pawns_in_base1 > 14 || pawns_in_base1 == 16)) return true;
	}
	//można dodać jakies warunki jak tylko np. 2 pionki zostały do końca
	//if (player2_pawns_in_opponents_base > 13 && !isInOpponentsBaseP2(m.steps[0])) return true;
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

bool Simulation::isInOpponentsBaseP1(int x)
{
	return x > 35 && x % 8 > 3;
}

bool Simulation::isInOpponentsBaseP2(int x)
{
	return x < 28 && x % 8 < 4;
}

int Simulation::checkIfGameEnded()
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
	return (2 * player2_win) + (1 * player1_win);
}

std::vector<int> Simulation::getTiles()
{
	return tiles;
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
			player1_pawns_in_own_base += 1 * isInOpponentsBaseP2(i);
			player1_pawns_in_opponents_base += 1 * isInOpponentsBaseP1(i);
		}
		else if (tiles[i] == 2)
		{
			player2_pawns_in_own_base += 1 * isInOpponentsBaseP1(i);
			player2_pawns_in_opponents_base += 1 * isInOpponentsBaseP2(i);
		}
	}
	this->pawns_in_base1 = player1_pawns_in_own_base + player2_pawns_in_opponents_base;
	this->pawns_in_base2 = player2_pawns_in_own_base + player1_pawns_in_opponents_base;
}
