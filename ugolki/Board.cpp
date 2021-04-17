#include "Board.h"
#include <stdlib.h>
#include <time.h> 

Board::Board()
{
	textures.loadFromFile("tiles_pawns.png");
	tile.setTexture(textures);
	tile.setOrigin(40, 40);
	paw.setTexture(textures);
	paw.setOrigin(40, 40);
	selected_tile = -1;
}

void Board::reset()
{
	player1_pawns.resize(0);
	player2_pawns.resize(0);
	tiles.resize(0);
	for (int i = 0; i < this->height * this->width; i++)
	{
		if (i < 28 && i % 8 < 4)
		{
			tiles.push_back(1);
			player1_pawns.push_back(i);
		}
		else if (i > 35 && i % 8 > 3)
		{
			tiles.push_back(2);
			player2_pawns.push_back(i);
		}
		else
		{
			tiles.push_back(0);
		}
	}
}

void Board::draw(sf::RenderWindow& w)
{
	for (int i = 0; i < this->height * this->width; i++)
	{
		tile.setPosition(sf::Vector2f(BOARD_START_X + (i % 8 * 80), BOARD_START_Y + (i / 8 * 80)));

		if (selected_tile == i)
		{
			tile.setTextureRect(sf::IntRect(4 * 80, 0, 80, 80));
		}
		else
		{
			tile.setTextureRect(sf::IntRect(((i + (i / 8)) % 2) * 80, 0, 80, 80));
		}
		w.draw(tile);
		if (tiles[i] != 0)
		{
			paw.setTextureRect(sf::IntRect((1 + tiles[i]) * 80, 0, 80, 80));
			paw.setPosition(sf::Vector2f(BOARD_START_X + (i % 8 * 80), BOARD_START_Y + (i / 8 * 80)));
			w.draw(paw);
		}

	}
}

int Board::getTile(int x)
{
	if (x < 0 || x > 63) return -1;
	return tiles[x];
}

void Board::selectTile(int x)
{
	selected_tile = x;
}

bool Board::checkNextHoop(int x, int y) //x pionek do przeskoczenia, y miejsce za pionkiem
{
	if (x < 0 || x>63) return false;
	if (y < 0 || y>63) return false;
	if (getTile(x) != 0 && getTile(y) == 0)
	{
		if (((x / 8) == (y / 8)) || ((x % 8) == (y % 8)))return true;
		else return false;
	}
	else return false;
}

bool Board::move(int x, int y)
{
	bool can_move = false;
	int distance = x - y;
	if (distance == 0)
	{
		can_move = false;
	}
	else if (x / 8 != y / 8 && x % 8 != y % 8)
	{
		can_move = false;
	}
	else if ((abs(distance) == 1 || abs(distance) == 8) && !move_multiple) //bez przeskakiwania
	{
		can_move = true;
	}
	else //sprawdz czy przeskakuje
	{

		switch (distance)
		{
		case -2:
			if (this->getTile(x + 1) != 0 && last_visited_tile != y)
			{
				can_move = true;
			}
			break;
		case 2:
			if (this->getTile(x - 1) != 0 && last_visited_tile != y)
			{
				can_move = true;
			}
			break;
		case -16:
			if (this->getTile(x + 8) != 0 && last_visited_tile != y)
			{
				can_move = true;
			}
			break;
		case 16:
			if (this->getTile(x - 8) != 0 && last_visited_tile != y)
			{
				can_move = true;
			}
			break;
		default:
			break;
		}
		if (can_move)
		{
			move_multiple = false;
			int temp[8] = { 1,2,-1,-2,8,16,-8,-16 };
			for (int i = 1; i < 8; i += 2)
			{
				if (last_visited_tile == y + temp[i - 1]) continue;
				move_multiple = (move_multiple || checkNextHoop(y + temp[i - 1], y + temp[i]));
			}
		}
	}

	for (int i = 0; i < 16; i++)
	{
		if (player1_pawns[i] == x)
		{
			player1_pawns[i] = y;
			break;
		}
		if (player2_pawns[i] == x)
		{
			player1_pawns[i] = y;
			break;
		}
	}

	if (can_move && y != last_visited_tile)
	{
		tiles[y] = tiles[x];
		tiles[x] = 0;
		if (move_multiple)
		{
			last_visited_tile = x;
			selected_tile = y;
		}
		else
		{
			last_visited_tile = -1;
			selected_tile = -1;
		}
	}
	else if (move_multiple) //nieudany ruch
	{
		can_move = false;
	}
	else // nieudany pojedynczy ruch bez przeskokow
	{
		can_move = false;
		selected_tile = -1;
	}
	return can_move;
}

bool Board::canMoveMultiple()
{
	return move_multiple;
}

void Board::endTurn()
{
	move_multiple = false;
	selected_tile = -1;
	last_visited_tile = -1;
}

#pragma region finding_all_moves
std::vector<Move> Board::findPossibleMoves(int pawn)
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

std::vector<Move> Board::findSimpleMoves(int x)
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

bool Board::canJump(int x, int y)
{
	//::cout << "canJump(" << x << ", " << y << ")";
	int distance = y - x;
	//std::cout << (getTile(y) == 0 && getTile(x + (distance / 2)) != 0) << std::endl;
	return (getTile(y) == 0 && getTile(x + (distance / 2)) != 0);
}

std::vector<Move> Board::findMultipleJumps(Move m)
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
std::vector<Move> Board::findAllMoves(int pawn_type)
{
	std::vector<Move> moves, temp_moves;
	for (int i = 0; i < this->width * this->height; i++)
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
#pragma endregion

void Board::makeRandomMove(int player)
{
	srand(time(NULL));
	std::vector<Move> moves = findAllMoves(player);
	int random_move;
	Move chosen;
	bool make_move = false;

	//ta pętla to syf
	//jak robi losowe ruchy to gra się nie kończy
	//jak robi dobre ruchy to pionki utykają na swoim polu startowym

	//pomysł 1:
	//robić ruchy do przodu (nadal wyszukując ruchy do tyłu)
	//zezwalać na ruchy do tyłu w obrębie pola przeciwnika)

	//pomysł 4:
	//nowa funkcja wyszukująca tylko ruchy do przodu
	//jak się skończą to:
	//pionki w polu przeciwnika cofają w X% przypadków
	//pionki poza polem przeciwnika idą tylko do przodu

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

	for (int i = 0; i < chosen.steps.size() - 1; i++)
	{

		move(chosen.steps[i], chosen.steps[i + 1]);
	}
	endTurn();
}

bool Board::verifyMoveForPlayer1(Move m)
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

bool Board::verifyMoveForPlayer2(Move m)
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

int Board::checkIfGameEnded()
{
	bool player1_win = true, player2_win = true;
	for (int i = 0; i < this->height * this->width; i++)
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

