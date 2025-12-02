#include <cstdint>

namespace sgl
{      
    enum BlockType : uint8_t {
        AIR = 0,
        GRASS = 1
    };

    uint8_t getBlockColor(uint8_t blockType, uint8_t faceIndex)
    {
        switch (blockType)
        {
        case BlockType::GRASS:
            if (faceIndex == 5)
                return 5;
            return 1;
        default:
            return blockType;
        }
    }
}