#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
class PlayerMove
{
public:
	static void run(sf::Event& event, sf::RenderWindow& window, Board& board, int* selected_tile, int player);
	static int select_tile(sf::Vector2i v);
};

