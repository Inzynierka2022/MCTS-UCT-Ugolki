#include "Board.h"

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
	tiles.resize(0);
	for (int i = 0; i < this->height; i++)
	{
		std::vector<int>temp;
		for (int j = 0; j < this->width; j++)
		{
			if (i / 4 == 0 && j / 4 == 0)
			{
				temp.push_back(1);
			}
			else if (i / 4 == 1 && j / 4 == 1)
			{
				temp.push_back(2);
			}
			else
			{
				temp.push_back(0);
			}
		}
		this->tiles.push_back(temp);
	}
	selected_tile = -1;
}

void Board::draw(sf::RenderWindow &w)
{
	for (int i = 0; i < this->height; i++)
	{
		for (int j = 0; j < this->width; j++)
		{
			tile.setPosition(sf::Vector2f(BOARD_START_X + (j * 80), BOARD_START_Y + (i * 80)));

			
			if (selected_tile == (i*8)+j)
			{
				tile.setTextureRect(sf::IntRect(4 * 80, 0, 80, 80));
			}
			else
			{
				tile.setTextureRect(sf::IntRect(((i + j) % 2) * 80, 0, 80, 80));
			}
			w.draw(tile);
			if (tiles[i][j] != 0)
			{
				paw.setTextureRect(sf::IntRect((1 + tiles[i][j]) * 80, 0, 80, 80));
				paw.setPosition(sf::Vector2f(BOARD_START_X + (j * 80), BOARD_START_Y + (i * 80)));
				w.draw(paw);
			}

		}
	}
}

int Board::getTile(int x)
{
	return tiles[x / 8][x % 8];
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
			for (int i = 1; i < 8; i+=2)
			{
				if (last_visited_tile == y + temp[i - 1]) continue;
				move_multiple = (move_multiple || checkNextHoop(y + temp[i - 1], y + temp[i]));
			}
		}
	}
	if (can_move && y != last_visited_tile)
	{
		tiles[y / 8][y % 8] = tiles[x / 8][x % 8];
		tiles[x / 8][x % 8] = 0;
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
		std::cout << "C" << '\n';
	}
	else // nieudany pojedynczy ruch bez przeskokow
	{
		can_move = false;
		selected_tile = -1;
		std::cout << "A" << '\n';
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
