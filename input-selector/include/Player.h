#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "Gamepad.h"
#include <list>

class Player : public sf::Drawable
{
    public:
        Player(const Gamepad& pad, int id, const sf::Color& color);
        virtual ~Player();
        float size = 150;
        void handleInput(const std::list<Player*>& players);

        static int counter;

        void setPosition(float x, float y, const std::list<Player*>& players);
    protected:

    private:

        int id;
        sf::Color color;
        float speed;
        void updatePosition();
        sf::CircleShape shape;
        sf::Sprite* p_sprite;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void collision(const std::list<Player*>& players);
        float radius;
};

#endif // PLAYER_H
