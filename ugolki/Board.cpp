#include "Board.h"
#include <stdlib.h>
#include <time.h> 
#include "Simulation.h"

Board::Board()
{
	textures.loadFromFile("tiles_pawns.png");
	tile.setTexture(textures);
	tile.setOrigin(40, 40);
	paw.setTexture(textures);
	paw.setOrigin(40, 40);
	selected_tile = -1;
	selected_tile2 = -1;
	this->reset();
}

void Board::reset()
{
	tiles.resize(0);
	for (int i = 0; i < this->height * this->width; i++)
	{
		if (i < 28 && i % 8 < 4)
		{
			tiles.push_back(1);
		}
		else if (i > 35 && i % 8 > 3)
		{
			tiles.push_back(2);
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

		if (selected_tile == i || selected_tile2 == i)
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

void Board::drawSimulation(sf::RenderWindow& w, std::vector<int> _tiles)
{
	for (int i = 0; i < 64; i++)
	{
		tile.setPosition(sf::Vector2f(BOARD_START_X + (i % 8 * 80), BOARD_START_Y + (i / 8 * 80)));

		tile.setTextureRect(sf::IntRect(((i + (i / 8)) % 2) * 80, 0, 80, 80));
		w.draw(tile);
		if (_tiles[i] != 0)
		{
			paw.setTextureRect(sf::IntRect((1 + _tiles[i]) * 80, 0, 80, 80));
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

void Board::selectTile(int x, int type)
{
	if (type == 1)
		selected_tile = x;
	else
		selected_tile2 = x;
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

bool Board::movePlayer(int x, int y)
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

void Board::moveAI(int x, int y)
{
	selected_tile = y;
	selected_tile2 = x;
	tiles[y] = tiles[x];
	tiles[x] = 0;
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

std::vector<int> Board::getTiles()
{
	return tiles;
}

int Board::checkIfGameEnded(int turnNumber)
{
	Simulation sim(tiles);
	return sim.checkIfGameEnded(turnNumber);
}
