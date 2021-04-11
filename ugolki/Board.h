#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

constexpr auto BOARD_START_X = 120;
constexpr auto BOARD_START_Y = 120;

class Board
{
private:
	const int height = 8;
	const int width = 8;
	std::vector<std::vector<int>> tiles;
	sf::Texture textures;
	sf::Sprite tile; //0 black tile, 1 white tile
	sf::Sprite paw; //2 paw, 3 paw
	int last_move = -1;
public:
	Board();
	void reset();
	void draw(sf::RenderWindow& w);
	int getTile(int);
	bool checkNextHoop(int, int);
	bool move(int, int);
};

