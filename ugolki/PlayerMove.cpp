#include "PlayerMove.h"

void PlayerMove::run(sf::Event& event,sf::RenderWindow& window, Board& board, int* selected_tile, int player)
{
    bool mouse_pressed = false;
    bool keyboard_pressed = false;
    sf::Mouse mouse;
    int turn = 0;
    bool moved = false;
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
                board.selectTile(-1, 2);

                if (selected_tile[0] >= 0)
                {
                    selected_tile[1] = select_tile(mouse.getPosition(window));

                    if (selected_tile[0] == selected_tile[1])
                    {
                        selected_tile[0] = -1;
                        selected_tile[1] = -1;
                        board.selectTile(-1,1);
                    }

                    if (selected_tile[1] >= 0)
                    {
                        if (board.getTile(selected_tile[1]) != 0)
                            selected_tile[1] = -1;
                        else
                        {
                            if (board.movePlayer(selected_tile[0], selected_tile[1]))
                            {
                                moved = true;
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
                    if (player == board.getTile(select_tile(mouse.getPosition(window))))
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
            if (!mouse_pressed && moved)
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
}

int PlayerMove::select_tile(sf::Vector2i v)
{
    if (v.x >= 80 && v.x <= 720 && v.y >= 80 && v.y <= 720)
    {
        return ((v.x / 80) + ((v.y / 80) - 1) * 8) - 1;
    }
    return -1;
}
