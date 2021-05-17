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
	std::vector<int> tiles;
	sf::Texture textures;
	sf::Sprite tile; //0 black tile, 1 white tile
	sf::Sprite paw; //2 paw1, 3 paw2
	int selected_tile;	//wybrane pole/pionek lub ostatnio ruszony pionek przez AI
	int selected_tile2;	//pole z którego AI ruszyło pionek
	int last_visited_tile = -1; //poprzednia pozycja
	bool move_multiple = false; //czy mo¿e wykonaæ kolejny rych w tej samej turze
public:
	Board();
	void reset();
	void draw(sf::RenderWindow& w);
	//potrzebne do testowania symulacji
	void drawSimulation(sf::RenderWindow& w, std::vector<int>);
	int getTile(int);
	void selectTile(int x, int type=1);
	bool checkNextHoop(int, int);
	bool movePlayer(int, int);
	void moveAI(int, int);
	bool canMoveMultiple();
	void endTurn();
	std::vector<int> getTiles();
	int checkIfGameEnded(int);

	
};

