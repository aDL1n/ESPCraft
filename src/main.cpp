#include <TFT_eSPI.h>
#include <sgl.h>

using namespace sgl;

// TODO
// дописать vec2 и ivec2
// сделать вывод и рендер в разных ядрах есп (также как вариант 1 ядро графика, другое физика и логика)

// вот тут еще что то можно посмотреть https://github.com/EmberGL-org/EmberGL/

TFT_eSPI tft;
Camera camera(320, 240, 120);
Renderer *renderer;

Hud *hud;

// Vec3 av = Vec3();

std::vector<sgl::Chunk *> chunks;


void setup()
{
    tft.init();
    tft.setRotation(1);

    camera.position = sgl::Vec3(120, 230, -200);
    camera.rotation.x = 40.0f * DEG_TO_RAD;
    camera.update();

    renderer = new Renderer(&tft, &camera);
    renderer->init(false);

    hud = new Hud(renderer->getSprite());

    for (int16_t x = 0; x < 2; x++)
    {   
        Chunk *chunk = new sgl::Chunk(IVec3((x * 2) - 1, -1, 0));
        for (uint8_t x = 0; x < 16; x++)
            for (uint8_t z = 0; z < 16; z++)
                for (uint8_t y = 0; y < 16; y++)
                    chunk->setBlock(IVec3(x, y, z), 1);
        
        chunk->rebuildMesh();
        chunk->compress(); 

        chunks.push_back(chunk);
    }
}

uint8_t xi = 0;
uint8_t zi = 0;
uint8_t yi = 15;

bool place = false;

void loop()
{   
    renderer->drawChunk(chunks);
    hud->render();

    renderer->draw();
    renderer->clear();
    
    chunks[0]->setBlock(IVec3(xi, yi, zi), place);
    chunks[1]->setBlock(IVec3(xi, yi, zi), place);
    chunks[0]->rebuildMesh();
    chunks[1]->rebuildMesh();

    xi++;

    if (xi >= 16)
    {
        xi = 0;
        zi++;
    } 

    if (zi >= 16)
    {
        zi = 0;
        yi--;
    }

    if (yi <= 0)
    {
        yi = 15;
        place = !place;
    }
}
