#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "Gamepad.h"
#include <list>
#include "Button.h"

class Player : public sf::Drawable
{
    public:
        Player(const Gamepad& pad, int id, const sf::Color& color);
        virtual ~Player();
        float size = 175;
        void handleInput(const std::list<Player*>& players, Button* buttons, int blength);

        static int counter;

        void setPosition(float x, float y, const std::list<Player*>& players);

         sf::Color color;
    protected:

    private:

        int id;

        float speed;
        void updatePosition();
        sf::CircleShape shape;
        sf::Sprite* p_sprite;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void collision(const std::list<Player*>& players);
        float radius;
        Button* selected;
};

#endif // PLAYER_H
