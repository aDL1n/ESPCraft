#include <TFT_eSPI.h>

#include "overlay/hudOverlay.h"

namespace renderer
{
    class HudRenderer
    {
    public: 
        HudRenderer(TFT_eSprite& sgl_sprite);
        void render();

    private:
        TFT_eSprite& sgl_sprite;    

        void renderCrosshair();
    };
}