#ifndef GAMEPAD_H
#define GAMEPAD_H
#include <string>
#include <SFML/Graphics.hpp>

class Gamepad
{
    public:
        Gamepad(const std::string& name, const std::string& imagePath);
        virtual ~Gamepad();

       // static Gamepad XBOX_ONE_S_WIRELESS("Xbox Wireless Controller", "assets/icon_xbox_one_s.png");

       // inline static Gamepad pad("", "");
        std::string identifier_name;
        sf::Texture tex;

    protected:

    private:

};


#endif // GAMEPAD_H
