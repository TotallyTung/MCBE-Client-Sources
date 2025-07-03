#pragma once

struct ChunkPos
{
    int x;
    int z;
    ChunkPos(int x = 0, int z = 0)
    {
        this->x = x >> 4;
        this->z = z >> 4;
    }
    ChunkPos(Vector3<int>& vec3)
    {
        this->x = vec3.x >> 4;
        this->z = vec3.z >> 4;
    }
};
struct ChunkBlockPos {
    uint8_t x;
    uint8_t z;
    short y;

    ChunkBlockPos(Vector3<int> pos) {
        this->x = pos.x & 0xF,
            this->y = pos.y,
            this->z = pos.z & 0xF;
    };

    ChunkBlockPos() {
        this->x = 0,
            this->y = 0,
            this->z = 0;
    };

    uint16_t index() const {
        return (y & 0xF) + 16 * (z + 16 * x);
    };
};