#include "buttons.h"

namespace scl
{
    template <uint8_t N>
    Buttons<N>::Buttons(const uint8_t (&pins)[N])
    {   
        size = N;
        for (uint8_t i = 0; i < size; i++)
        {   
            pinMode(pins[i], INPUT_PULLUP);
            button_pins[i] = pins[i];
            button_values[i] = 0;
        }
    }

    template <uint8_t N>
    void Buttons<N>::read()
    {
        for (uint8_t i = 0; i < size; i++)
            button_values[i] = digitalRead(button_pins[i]) == LOW;
    }

    template <uint8_t N>
    bool Buttons<N>::getValue(uint8_t index)
    {   
        if (index >= size) return false;
        return button_values[index];
    }
}