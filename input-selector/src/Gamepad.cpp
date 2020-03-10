#include "Gamepad.h"



Gamepad::Gamepad(const std::string& name, const std::string& imagePath)
{
    this->tex.loadFromFile(imagePath);
    this->identifier_name = name;
}

Gamepad::~Gamepad()
{
    //dtor
}
