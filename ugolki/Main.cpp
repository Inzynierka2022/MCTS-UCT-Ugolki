#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <iostream>

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
    std::cout << "Hello World!\n";
    sf::RenderWindow window(sf::VideoMode(800, 800), "program");

    Board board;
    board.reset();
    sf::Mouse mouse;
    int selected_tile[2] = { -1,-1 };

    std::vector<int>move_queue;
    bool mouse_pressed = false;

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
            board.reset();
            selected_tile[0] = -1;
            selected_tile[1] = -1;
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
        else
        {
            mouse_pressed = false;
        }

        window.clear();
        board.draw(window);
        window.display();
    }
    return 0;
}