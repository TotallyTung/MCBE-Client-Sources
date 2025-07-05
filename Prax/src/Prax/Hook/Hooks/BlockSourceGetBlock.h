//
// Created by vastrakai on 1/26/2024.
//


#pragma once
#include <Prax/Hook/Hook.h>

class BlockSourceGetBlock : public Hook {
public:
    BlockSourceGetBlock() : Hook("BlockSource::getBlock") {
        InitOnConstruct = false;
    }

    std::unique_ptr<Detour> BlockSourceGetBlockDetour;

    void Init() override;
    void UnHook() override;

};
