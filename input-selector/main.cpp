#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Gamepad.h"
#include "Player.h"
#include "Button.h"
#include <list>

int width = 1680, height = 1050;

void updateControllers(std::list<Player*>& players, Gamepad* gamepads, int length);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1680, 1050), "SFML works!");
    window.setFramerateLimit(60);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    sf::Texture reloadTex;
    reloadTex.loadFromFile("assets/reload.png");
    sf::Sprite reloadSprite(reloadTex);
    reloadSprite.setOrigin(reloadTex.getSize().x/2.0f, reloadTex.getSize().y / 2.0f);
    reloadSprite.setPosition(width /2, height / 4);
    reloadSprite.setScale(100.0f/reloadTex.getSize().x, 100.0f/reloadTex.getSize().y);



    //sf::Sprite test(t_XBOX_WIRELESS);
    //test.setPosition(100.0f, 100.0f);
    //test.setScale(0.5f, 0.5f);
    Gamepad gamepads[] ={
        Gamepad("generic game pad", "assets/icon_generic.png"),
        Gamepad("DragonRise Inc.   Generic   USB  Joystick  ", "assets/icon_arcade.png"),
        Gamepad("Sony PLAYSTATION(R)3 Controller", "assets/icon_ps3.png"),
        Gamepad("Sony PLAYSTATION(R)4 Controller", "assets/icon_ps4.png"),
        Gamepad("wii remote", "assets/icon_wii_remote.png"),
        Gamepad("Xbox Wireless Controller", "assets/icon_xbox_one_s.png")
        };
    int length = 6;

    std::list<Player*> players;
    int buttonsLength = 4;
    float bw = width / 4.0f;
    float bh = height/ 2.0f;
    Button buttons[] =
    {
        Button(sf::Color::Red, sf::Rect<float>(0*bw, bh, bw, bh)),
        Button(sf::Color::Blue, sf::Rect<float>(1*bw, bh, bw, bh)),
        Button(sf::Color::Green, sf::Rect<float>(2*bw, bh, bw, bh)),
        Button(sf::Color::Yellow, sf::Rect<float>(3*bw, bh,bw, bh))
    };
   // Player p(gamepads[1],2, sf::Color::Blue);
   // players.push_back(p);
    updateControllers(players , gamepads, length);

    sf::Color background(80,80,80);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        }

         for(auto it = players.begin(); it != players.end(); it++)
        {
            (*it)->handleInput(players, buttons, buttonsLength);
        }

        window.clear(background);
        for(int i = 0; i < buttonsLength; i++)
        {
            window.draw(buttons[i]);
        }

        window.draw(reloadSprite);

        for(auto it = players.begin(); it != players.end(); it++)
        {
            window.draw(**it);
        }

        window.display();
    }

    for(auto it = players.begin(); it != players.end(); it++)
    {
                delete *it;
    }
    return 0;
}

sf::Color colors[] = {sf::Color(51, 102, 255),
    sf::Color(204, 0, 153),
    sf::Color(255, 102, 0),
    sf::Color(51, 204, 51),
    sf::Color(51, 153, 102),
    sf::Color(128, 0, 0)};
int colorsLength = 6;

void updateControllers(std::list<Player*>& players, Gamepad* gamepads, int length)
{

    for(auto it = players.begin(); it != players.end(); it++)
    {
                delete *it;
    }
    players.clear();

    for(int i = 0; i < colorsLength; i++)
    {
        if(!sf::Joystick::isConnected(i))
        {
            break;
        }
        std::string id =  sf::Joystick::getIdentification(i).name;
        std::cout << "connecting: " << id << std::endl;
        Player* p_player;
        bool found = false;
        for(int j = 0; j < length; j++)
        {
            if(gamepads[j].identifier_name.compare(sf::Joystick::getIdentification(i).name) == 0)
            {
                found = true;
                p_player = new Player(gamepads[j], i,  colors[i]);
                break;
            }
        }
        if(!found)
            p_player = new Player(gamepads[0], i,  colors[i]);

         players.push_back(p_player);
    }

    float yStart = height /2.0f;
    float xStep = width / (players.size()+1);
    float xStart = 0;
    for(auto it = players.begin(); it != players.end(); it++)
    {
           xStart += xStep;
           (*it)->setPosition(xStart, yStart, players);
    }
}
