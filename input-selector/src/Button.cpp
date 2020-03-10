#include "Button.h"

Button::Button(sf::Color color, sf::Rect<float> rect)
{
    this->color = color;
}

Button::~Button()
{
    //dtor
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape, states);
}
