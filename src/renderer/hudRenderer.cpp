#include "hudRenderer.h"

namespace renderer
{
    HudRenderer::HudRenderer(TFT_eSprite &sgl_sprite) : sgl_sprite(sgl_sprite)
    {   
    }

    void HudRenderer::render()
    {
        this->renderCrosshair();
    }

    void HudRenderer::renderCrosshair()
    {   
        sgl_sprite.pushImage((sgl_sprite.width() - 8) / 2, (sgl_sprite.height() - 8) / 2, 16, 16, (uint16_t*)overlay::HudOverlay::crosshair);
    }
}