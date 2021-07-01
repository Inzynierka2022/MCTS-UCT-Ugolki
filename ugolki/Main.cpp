#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Simulation.h"
#include <windows.h>
#include "MCTS.h"
#include "PlayerMove.h"

int main()
{
	std::cout << "Controls: \n Left click - select and move \n Right click - next round\n"<<std::endl;
		
/*
*****INITS*****
*/
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(800, 800), "program", sf::Style::Titlebar);
	Board board;
	int turnNumber = 0;
	int winner = 0;

	sf::Mouse mouse;
	int selected_tile[2] = { -1,-1 };

	MCTS mcts(board.getTiles(), 2);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}

		PlayerMove::run(event, window, board, selected_tile, 1);

		window.clear();
		board.draw(window);
		window.display();

		std::cout << "Ruch: " << turnNumber << "\n";
		std::cout << "\Szukanie najlepszego ruchu\n";

		mcts.reset_tree(board.getTiles());//generate new tree search

		std::pair<int, int> move = mcts.run(window, turnNumber);//bot move

		board.moveAI(move.first, move.second);

		winner = board.checkIfGameEnded(turnNumber);
		if (winner != 0)
		{
			std::cout << winner << " wygral\n";
			printf("Turn:%d\n", turnNumber);
			board.reset();
			turnNumber = 0;
			winner = 0;
		}
		turnNumber++;
	}
	return 0;
}

