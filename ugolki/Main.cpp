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
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(800, 800), "program", sf::Style::Titlebar);
	Board board;
	board.reset();
	int turnNumber = 0;
	int winner = 0;

	sf::Mouse mouse;
	int selected_tile[2] = { -1,-1 };
	std::vector<int>move_queue;
	bool mouse_pressed = false;
	bool keyboard_pressed = false;

	MCTS mcts(board.getTiles(), 2);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}

		PlayerMove::run(event, window, board, selected_tile);
		window.clear();
		board.draw(window);
		window.display();
		std::cout << "Ruch: " << turnNumber << "\n";


		std::cout << "\Szukanie najlepszego ruchu\n";
		mcts = MCTS(board.getTiles(), 2);
		std::pair<int, int> move = mcts.run(window, turnNumber);
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

//int main()
//{
//	srand(time(NULL));
//	sf::RenderWindow window(sf::VideoMode(800, 800), "program", sf::Style::Titlebar);
//	Board board;
//	board.reset();
//	int turnNumber = 0;
//
//	int player = 1;
//	int player1_wins = 0, player2_wins = 0, draws = 0;
//	int moves = 0;
//	int games = 0;
//	int wins = 0;
//	int avgMoves = 0;
//	int winner = 0;
//	int winsByTurn40 = 0;
//	Simulation sim(board.getTiles());
//	while (window.isOpen())
//	{
//		sf::Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed)
//				window.close();
//
//		}
//
//
//		auto startTime = std::chrono::high_resolution_clock::now();
//		sim.makeRandomMove(player, turnNumber);
//		//std::cout << std::chrono::duration_cast<std::chrono::microseconds>((std::chrono::high_resolution_clock::now() - startTime)).count() << " microseconds\n";
//		window.clear();
//		board.drawSimulation(window, sim.getTiles());
//		window.display();
//		//Sleep(200);
//		winner = sim.checkIfGameEnded(turnNumber);
//		if (winner != 0)
//		{
//			std::cout << winner << " wygral\n";
//			printf("Turn:%d\n", turnNumber);
//			sim.reset();
//			if (winner != 3) wins++;
//			if (turnNumber == 40 && winner != 3) winsByTurn40++;
//			turnNumber = 0;
//			if (winner == 1) player1_wins++;
//			if (winner == 2) player2_wins++;
//			if (winner == 3) draws++;
//			avgMoves = (games * avgMoves + moves) / (games + 1);
//			games++;
//			std::cout << "Player1: " << player1_wins << "\n";
//			std::cout << "Player2: " << player2_wins << "\n";
//			std::cout << "Draws: " << draws << "\n";
//			std::cout << "Wins by turn 40: " << winsByTurn40 << "\n";
//			std::cout << "Wins: " << wins << "\n";
//			std::cout << "Moves: " << moves << "\n";
//			std::cout << "Avg moves: " << avgMoves << "\n";
//			moves = 0;
//			winner = 0;
//			//Sleep(10000);
//		}
//		if (player == 1)player = 2;
//		else player = 1;
//		if (player == 2)
//			turnNumber++;
//		moves++;
//	}
//	return 0;
//}