#include "Simulation.h"
Simulation::Simulation(std::vector<int> _tiles)
{
	tiles = _tiles;
	initState = _tiles;
}

int Simulation::getTile(int x)
{
	if (x < 0 || x > 63) return -1;
	return tiles[x];
}

void Simulation::move(int x, int y)
{
	int temp = tiles[x];
	tiles[x] = 0;
	tiles[y] = temp;
}

std::vector<Move> Simulation::findPossibleMoves(int pawn)
{
	//std::cout << "findPossibleMoves" << std::endl;
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
	srand(time(NULL));
	std::vector<Move> moves = findAllMoves(player);
	int random_move;
	Move chosen;
	bool make_move = false;

	//ta pętla to syf
	//jak robi losowe ruchy to gra się nie kończy
	//jak robi dobre ruchy to pionki utykają na swoim polu startowym

	//pomysł 1 (AKTUALNY):
	//robić ruchy do przodu (nadal wyszukując ruchy do tyłu)
	//zezwalać na ruchy do tyłu w obrębie pola przeciwnika)

	//pomysł 4:
	//nowa funkcja wyszukująca tylko ruchy do przodu
	//jak się skończą to:
	//pionki w polu przeciwnika cofają w X% przypadków
	//pionki poza polem przeciwnika idą tylko do przodu

	//Można śledzić ilość pionków w polu przeciwnika i jeśli jest ich już 13-14, skupiać się na ruchach pozostałymi.
	//Dodatkowo jeśli pionek przeciwnika utknie w swoim polu można zacząć robić losowe ruchy dopóki z niego nie wyjdzie

	//POMYSŁ 5(wydaje się szybszy):
	//zamiast szukać wszystkich możliwych ruchów
	//wylosować jeden pionek i wykonać losowy ruch
	//jeśli nie ma możliwego ruchu losujemy kolejny pionek
	//powinno być szybsze niż wyszukiwanie wszystkich ruchów za każdym razem
	//PROBLEM: może nie być dobrego ruchu dla wylosowanego pionka - sprawdzanie tego może być mało wydajne

	//wtedy wyszukiwanie wszystkich ruchów było by wykorzystywane do budowania drzewa
	//symulacja wykorzystywałaby tylko jeden pionek na raz



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
}

bool Simulation::verifyMoveForPlayer1(Move m)
{
	int start_i, start_j, end_i, end_j;
	start_i = m.steps[0] / 8;
	start_j = m.steps[0] % 8;
	end_i = m.steps.back() / 8;
	end_j = m.steps.back() % 8;
	bool result = false;

	//do przodu
	if ((end_j - start_j) + (end_i - start_i) > 0)
	{
		result = true;
	}
	else
	{
		//jeśli cofa pozostając w polu przeciwnika
		if (end_i > 3 && end_j % 8 > 3) result = true;
	}

	return result;
}

bool Simulation::verifyMoveForPlayer2(Move m)
{
	int start_i, start_j, end_i, end_j;
	start_i = m.steps[0] / 8;
	start_j = m.steps[0] % 8;
	end_i = m.steps.back() / 8;
	end_j = m.steps.back() % 8;
	bool result = false;
	//do przodu
	if ((end_j - start_j) + (end_i - start_i) < 0)
	{
		result = true;
	}
	else
	{
		//jeśli cofa pozostając w polu przeciwnika
		if (end_i < 4 && end_j % 8 < 4) result = true;
	}
	return result;
}

//Przydatne warunki:
	//Player 1:
		//ruch do przodu
		//if ((end_j - start_j) + (end_i - start_i) > 0)

		//pionek jest już w polu przeciwnika
		//if (start_i > 3 && start_j % 8 > 3)

		//pionek nie jest jeszcze w polu przeciwnika
		//if (start_i <= 3 || start_j % 8 <= 3)

		//pionek kończy ruch w polu przeciwnika
		//if (end_i > 3 && end_j % 8 > 3)

	//Player 2:
		//ruch do przodu
		//if ((end_j - start_j) + (end_i - start_i) < 0)

		//pionek jest już w polu przeciwnika
		//if (start_i < 4 && start_j % 8 < 4)

		//pionek nie jest jeszcze w polu przeciwnika
		//if (start_i >= 4 || start_j % 8 >= 4)

		//pionek kończy ruch w polu przeciwnika
		//if (end_i < 4 && end_j % 8 < 4)


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
}
