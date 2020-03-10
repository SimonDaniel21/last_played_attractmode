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
    speed = 5.0f/100.0f;

    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(color);
    shape.setOutlineThickness(5.0f);
    this->radius = rad + 8.0f;
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
    if(x + radius > 1280)
       x = 1280-radius;
    if(y - radius < 0)
        y = radius;
    if(y + radius > 720)
        y = 720-radius;
    p_sprite->setPosition(x, y);

    //player collision
     for(auto it = players.begin(); it != players.end(); it++)
    {
        if(*it != this)
        {
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

void Player::handleInput(const std::list<Player*>& players)
{
    if(sf::Joystick::isButtonPressed(id, 0)){
        shape.setOutlineColor(sf::Color::White);
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
