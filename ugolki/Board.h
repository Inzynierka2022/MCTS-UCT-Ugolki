#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Move.h"

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
	int selected_tile;	//wybrane pole/pionek
	int last_visited_tile = -1; //poprzednia pozycja
	bool move_multiple = false; //czy mo¿e wykonaæ kolejny rych w tej samej turze
public:
	Board();
	void reset();
	void draw(sf::RenderWindow& w);
	int getTile(int);
	void selectTile(int);
	bool checkNextHoop(int, int);
	bool move(int, int);
	bool canMoveMultiple();
	void endTurn();

	//znajdowanie ruchów dla jednego pionka
	std::vector<Move> findPossibleMoves(int);
	std::vector<Move> findSimpleMoves(int);
	bool canJump(int, int);
	std::vector<Move> findMultipleJumps(Move);

	//znajdowanie wszystkich ruchów
	std::vector<Move> findAllMoves(int);

	void makeRandomMove(int);
};

