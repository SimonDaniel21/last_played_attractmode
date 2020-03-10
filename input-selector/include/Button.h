#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>

class Player;

class Button : public sf::Drawable
{
    public:
        Button(sf::Color color, sf::Rect<float> rect);
        virtual ~Button();
        void select(Player& player);
        void unSelect();
        sf::RectangleShape shape;
        Player* owner;

    protected:

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        sf::Color color;

};

#endif // BUTTON_H
