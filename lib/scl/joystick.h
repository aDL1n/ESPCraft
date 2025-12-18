#include <cstdint>
#include <Arduino.h>

namespace scl {
    enum State : uint8_t
    {
        UP = 1,
        NONE = 0,
        DOWN = 2
    };

    class Joystick
    {
    public:
        Joystick(uint8_t xPin, uint8_t yPin, uint16_t buttonPin, uint16_t deadzone);

        int16_t getRawX();
        int16_t getRawY();

        void read();

        State getX();
        State getY();
        bool getButton();
    private:
        uint8_t xPin, yPin, buttonPin;
        int16_t x, y;
        uint16_t deadzone;
    };
}