#pragma once

class Dimension {
public:
    CLASS_FIELD(class Weather*, Weather, 0x1B0); // 1.21.50
    CLASS_FIELD(class BlockSource*, BlockSource, 0xD0); // 1.21.50
    CLASS_FIELD(class ChunkSource*, ChunkSource, 0x1A0); // 1.21.50
};

// Dimension::getWeather 0x1B0 
// Dimension::getBlockSourceFromMainChunkSource 0xD0
// Dimension::getChunkSource 0x1A0