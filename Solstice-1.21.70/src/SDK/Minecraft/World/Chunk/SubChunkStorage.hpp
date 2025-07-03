#pragma once

// Not fully reversed

template<typename T>
class SubChunkStorage
{
    uint16_t mElements[1368]; // 0x0008
    T* mTypes; // 0x0AB8
};