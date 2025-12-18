#include "joystick.h"

namespace scl
{
    Joystick::Joystick(uint8_t xPin, uint8_t yPin, uint16_t buttonPin, uint16_t deadzone)
    : xPin(xPin), yPin(yPin), buttonPin(buttonPin), deadzone(deadzone)
    {
        
    }

    int16_t Joystick::getRawX()
    {
        return this->x;
    }

    int16_t Joystick::getRawY()
    {
        return this->y;
    }

    void Joystick::read()
    {
        this->x = analogRead(xPin);
        this->y = analogRead(yPin);
    }

    State Joystick::getX()
    {   
        if (x < deadzone || x < -deadzone) return State::NONE;
        return map(x, 0, 4095, -2048, 2048) > 0 ? State::UP : State::DOWN;
    }

    State Joystick::getY()
    {
        if (y < deadzone || y < -deadzone) return State::NONE;
        return map(y, 0, 4095, -2048, 2048) > 0 ? State::UP : State::DOWN;
    }

    bool Joystick::getButton()
    {
        return (buttonPin != -1) ? digitalRead(buttonPin) : false;
    }
}
    