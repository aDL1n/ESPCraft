#include "hud.h"

namespace sgl
{
    Hud::Hud(TFT_eSprite *sprite)
    {   
        this->sprite = sprite;
    }

    void Hud::render()
    {   
        sprite->pushImage((sprite->width() - 8) / 2, (sprite->height() - 8) / 2, 16, 16, (uint16_t*)crosshair);
    }
}