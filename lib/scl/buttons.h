#include <cstdint>
#include <Arduino.h>

namespace scl
{   
    template <uint8_t N>
    class Buttons
    {
    public:
        Buttons(const uint8_t (&pins)[N]);

        void read();
        bool getValue(uint8_t index);
    private:
        uint8_t *button_pins;
        bool *button_values;
        uint8_t size;
    };
}