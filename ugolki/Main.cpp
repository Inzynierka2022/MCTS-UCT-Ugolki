#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Simulation.h"
#include <windows.h>
#include "MCTS.h"




int select_tile(sf::Vector2i v)
{
    if (v.x >= 80 && v.x <= 720 && v.y >= 80 && v.y <= 720)
    {
        return ((v.x/80) + ((v.y/80)-1)*8)-1;
    }
    return -1;
}

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(800, 800), "program",sf::Style::Titlebar);
    Board board;
    board.reset();

    sf::Mouse mouse;
    int selected_tile[2] = { -1,-1 };
    std::vector<int>move_queue;
    bool mouse_pressed = false;
    bool keyboard_pressed = false;

    int player = 1;
    /*int player1_wins = 0, player2_wins = 0;
    int moves = 0;
    int games = 0;
    int avgMoves = 0;*/
    MCTS mcts(board.getTiles(), 1);

    while (window.isOpen())
    {
        sf::Event event;
        

        while (true)
        {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                if (!keyboard_pressed)
                {
                    keyboard_pressed = true;
                    board.reset();
                    selected_tile[0] = -1;
                    selected_tile[1] = -1;
                }
            }
            else
            {
                keyboard_pressed = false;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (!mouse_pressed)
                {
                    mouse_pressed = true;
                    if (selected_tile[0] >= 0)
                    {
                        selected_tile[1] = select_tile(mouse.getPosition(window));
                        if (selected_tile[1] >= 0)
                        {
                            if (board.getTile(selected_tile[1]) != 0)
                                selected_tile[1] = -1;
                            else
                            {
                                if (board.move(selected_tile[0], selected_tile[1]))
                                {
                                    if (board.canMoveMultiple())
                                    {
                                        selected_tile[0] = selected_tile[1];
                                        //board.selectTile(selected_tile[0]);
                                        selected_tile[1] = -1;
                                    }
                                    else
                                    {
                                        selected_tile[0] = -1;
                                        //board.selectTile(selected_tile[0]);
                                        selected_tile[1] = -1;
                                    }
                                }
                                else if (board.canMoveMultiple())
                                {
                                    selected_tile[1] = -1;
                                }
                                else
                                {
                                    selected_tile[0] = -1;
                                    //board.selectTile(selected_tile[0]);
                                    selected_tile[1] = -1;
                                }
                            }
                        }
                    }
                    else
                    {
                        selected_tile[0] = select_tile(mouse.getPosition(window));
                        if (selected_tile[0] >= 0)
                        {
                            if (board.getTile(selected_tile[0]) == 0)
                                selected_tile[0] = -1;
                            else board.selectTile(selected_tile[0]);
                        }
                    }

                }
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                if (!mouse_pressed)
                {
                    mouse_pressed = true;
                    board.endTurn();
                    selected_tile[0] = -1;
                    selected_tile[1] = -1;
                    break;
                }
            }
            else
            {
                mouse_pressed = false;
            }
            window.clear();
            board.draw(window);
            window.display();
        }

        window.clear();
        board.draw(window);
        window.display();

        //moves++;


        std::cout << "\nZnajdowanie najlepszego ruchu\n";
        mcts = MCTS(board.getTiles(), 2);
        std::pair<int,int> move = mcts.run(window);
        board.move(move.first, move.second);

        if (board.checkIfGameEnded() != 0)
        {
            std::cout << board.checkIfGameEnded() << " wygral\n";
            board.reset();
            /*if (player == 1) player1_wins++;
            else player2_wins++;
            avgMoves = (games * avgMoves + moves)/(games+1);
            games++;
            std::cout << "Player1: " << player1_wins<<"\n";
            std::cout << "Player2: " << player2_wins<<"\n";
            std::cout << "Moves: " << moves << "\n";
            std::cout << "Avg moves: " << avgMoves << "\n";
            moves = 0;*/
        }
        if (player == 1)player = 2;
        else player = 1;
    }
    return 0;
}