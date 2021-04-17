#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Simulation.h"
#include <iostream>
#include <windows.h>

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
    sf::RenderWindow window(sf::VideoMode(800, 800), "program",sf::Style::Titlebar);

    Board board;
    board.reset();
    sf::Mouse mouse;
    int selected_tile[2] = { -1,-1 };

    std::vector<int>move_queue;
    bool mouse_pressed = false;
    bool keyboard_pressed = false;
    int player = 1;
    int player1_wins = 0, player2_wins = 0;

    Simulation sim(board.getTiles());

    while (window.isOpen())
    {
        sf::Event event;
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
            }
        }
        else
        {
            mouse_pressed = false;
        } 
        
        //wykonywanie losowych ruchów
        sim.makeRandomMove(player);

        window.clear();
        //board.draw(window);
        board.drawSimulation(window, sim.getTiles());
        window.display();

        if (sim.checkIfGameEnded() != 0)
        {
            std::cout << sim.checkIfGameEnded() << " wygral\n";
            if (player == 1) player1_wins++;
            else player2_wins++;
            std::cout << "Player1: " << player1_wins<<"\n";
            std::cout << "Player2: " << player2_wins<<"\n";
            sim.reset();
        }
        if (player == 1)player = 2;
        else player = 1;
    }
    return 0;
}