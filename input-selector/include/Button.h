#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class Button : public sf::Drawable
{
    public:
        Button(sf::Color color, sf::Rect<float> rect);
        virtual ~Button();

    protected:

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        sf::Color color;
        sf::RectangleShape shape;
};

#endif // BUTTON_H
