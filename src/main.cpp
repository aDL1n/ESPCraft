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

// Vec3 av = Vec3();

std::vector<sgl::Chunk *> chunks;


void setup()
{
    tft.init();
    tft.setRotation(1);

    camera.position = sgl::Vec3(120, 280, -100);
    camera.rotation.x = 40.0f * DEG_TO_RAD;
    camera.update();

    renderer = new Renderer(&tft, &camera);
    renderer->init();

    for (int16_t x = -2; x < 3; x++)
        for (int16_t z = -2; z < 3; z++)
        {   
            Chunk *chunk = new sgl::Chunk(IVec3(x, -1, z));
            for (uint8_t x = 0; x < 16; x++)
                for (uint8_t z = 0; z < 16; z++)
                    for (uint8_t y = 0; y < 16; y++)
                        if (x % 2 != 0)
                            chunk->setBlock(IVec3(x, y, z), 1);
                chunk->rebuildMesh();

            chunk->compress(); 

            chunks.push_back(chunk);
        }

}

void loop()
{
    renderer->drawChunk(chunks);
    // renderer->drawCube(av, IVec3(120, 70, 160));

    renderer->draw();
    renderer->clear();

    chunks[0]->setBlock(IVec3(0, 0, 0), 0);
    chunks[0]->rebuildMesh();

    // av.x += 0.03f;
    // av.y += 0.02f;
}
