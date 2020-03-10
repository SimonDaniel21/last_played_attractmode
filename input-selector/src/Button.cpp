#include "Button.h"
#include "Player.h"
class Player;

Button::Button(sf::Color color, sf::Rect<float> rect)
{
    this->color = color;
    shape = sf::RectangleShape(sf::Vector2f(rect.width, rect.height));
    shape.setPosition(rect.left, rect.top);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(4);
    owner = nullptr;
}

Button::~Button()
{
    //dtor
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape, states);
}

void Button::select(Player& player)
{
    this->owner = &player;
    shape.setFillColor(owner->color);
}

void Button::unSelect()
{
    this->owner = nullptr;
    shape.setFillColor(sf::Color::Transparent);
}
