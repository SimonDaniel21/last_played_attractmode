#include "Player.h"
#include <math.h>
#include <iostream>

Player::Player(const Gamepad& pad,int id, const sf::Color& color) : id(id)
{
    p_sprite = new sf::Sprite(pad.tex);
    float xScale = size / p_sprite->getTexture()->getSize().x ;
    float yScale = size /  p_sprite->getTexture()->getSize().y ;

    float w = p_sprite->getTexture()->getSize().x;
    float h = p_sprite->getTexture()->getSize().y;
    float scale = w < h ? size /  h : size / w;
    p_sprite->setScale(scale, scale);
    p_sprite->setOrigin(w/2.0f, h/2.0f);

    float rad = std::sqrt(std::pow(size/2.0f, 2)*2);
    shape = sf::CircleShape(rad);
    shape.setPosition(0.0f, 0.0f);
    shape.setOrigin(rad, rad);
    speed = 9.0f/60.0f;

    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(color);
    this->color = color;
    shape.setOutlineThickness(5.0f);
    this->radius = rad + 5.0f;
    selected = nullptr;
}

Player::~Player()
{
    delete p_sprite;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
     target.draw(shape, states);
    target.draw(*p_sprite, states);
}

 void Player::updatePosition()
 {
    shape.setPosition(p_sprite->getPosition());
 }

 float distance(float x1, float y1, float x2, float y2)
 {
    return std::sqrt(std::pow(std::abs(x1-x2), 2) + std::pow(std::abs(y1-y2), 2));
 }

 float distance(const sf::Vector2f& v1, const sf::Vector2f& v2)
 {
    return distance(v1.x, v1.y, v2.x, v2.y);
 }

 void Player::collision(const std::list<Player*>& players)
 {
    float x = p_sprite->getPosition().x;
    float y = p_sprite->getPosition().y;
    if(x - radius < 0)
        x = radius;
    if(x + radius > 1680)
       x = 1680-radius;
    if(y - radius < 0)
        y = radius;
    if(y + radius > 1050)
        y = 1050-radius;
    p_sprite->setPosition(x, y);

    //player collision
     for(auto it = players.begin(); it != players.end(); it++)
    {
        if(*it != this)
        {
            if((*it)->selected != nullptr)
                continue;
            Player* p = *it;

            if(distance(p->p_sprite->getPosition(), p_sprite->getPosition()) <= radius*2)
            {
                float x1 = p->p_sprite->getPosition().x,
                    y1 = p->p_sprite->getPosition().y,
                    x2 = p_sprite->getPosition().x,
                    y2 = p_sprite->getPosition().y;

                sf::Vector2f v(x2-x1, y2-y1);
                float l = std::sqrt(v.x * v.x + v.y * v.y);
                v.x = v.x / l;
                v.y = v.y / l;
                v.x *= (radius*2);
                v.y *= (radius*2);
                p_sprite->setPosition(x1 + v.x, y1 + v.y);

            }
            else
            {
                //shape.setFillColor(sf::Color::White);
            }
        }
    }
 }

 bool inBounds(sf::Vector2f pos, Button b)
 {
    return b.shape.getGlobalBounds().contains(pos);
 }

void Player::handleInput(const std::list<Player*>& players, Button* buttons, int blength)
{
    if(selected != nullptr)
    {
        if(sf::Joystick::isButtonPressed(id, 1))
        {
            selected->unSelect();
            selected = nullptr;
        }
        else
        {
            return;
        }
    }

    if(sf::Joystick::isButtonPressed(id, 0)){

    std::cout << "select: " << p_sprite->getPosition().x  << " - " <<p_sprite->getPosition().y<< std::endl;
        for(int i = 0; i < blength; i++)
        {

         std::cout << "testing: " <<i  << buttons[i].owner << std::endl;
            if(buttons[i].owner == nullptr && inBounds(p_sprite->getPosition(), buttons[i]))
            {
                 std::cout << "success: " <<i  << std::endl;
                buttons[i].select(*this);
                selected = &buttons[i];
                setPosition(selected->shape.getPosition().x + selected->shape.getSize().x/2,
                    selected->shape.getPosition().y + selected->shape.getSize().y/2,
                    std::list<Player*>());


                return;
            }
        }
    }


    float x = sf::Joystick::getAxisPosition(id, sf::Joystick::X);
    float y = sf::Joystick::getAxisPosition(id, sf::Joystick::Y);
    //std::cout << x << std::endl;
    p_sprite->move(x*speed, y*speed);
    collision(players);
    updatePosition();

}

void Player::setPosition(float x, float y, const std::list<Player*>& players)
{
    p_sprite->setPosition(x, y);
    collision(players);
    updatePosition();
}
