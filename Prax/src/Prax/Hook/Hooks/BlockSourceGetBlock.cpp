//
// Created by vastrakai on 1/26/2024.
//

#include "BlockSourceGetBlock.h"

#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Visual/OreESP.h>
#include <Prax/SDK/Game/Misc/Structs.h>

/*
// Class: BlockSource
virtual Block* getBlock(int, int, int); // 0 (0x0)
virtual Block* getBlock(BlockPos const &); // 1 (0x8)
virtual Block* getBlock(BlockPos const &, unsigned int); // 2 (0x10)
    */

// Hook methods
class Block; // Forward declaration
std::map<int, std::unique_ptr<Detour>> getBlockDetours;

Block* getBlockHook0(BlockSource* _this, int x, int y, int z) {
    auto oFunc = getBlockDetours.at(0)->GetFastcall<Block*, BlockSource*, int, int, int>();

    const auto block = oFunc(_this, x, y, z);
    auto blockPos = Vector3(x, y, z);
    static auto oreESP = ModuleManager::GetModule<OreESP>();

    if (oreESP->Enabled && block) {
        oreESP->OnGetBlock(blockPos, block);
    }

    return block;
}

Block* getBlockHook1(BlockSource* _this, const BlockPos& blockPos) {
    auto oFunc = getBlockDetours.at(1)->GetFastcall<Block*, BlockSource*, BlockPos>();


    static auto oreESP = ModuleManager::GetModule<OreESP>();
    auto blockPosVec = Vector3(blockPos.x, blockPos.y, blockPos.z);
    const auto block = oFunc(_this, blockPos);

    if (oreESP->Enabled && block) {
        oreESP->OnGetBlock(blockPosVec, block);
    }

    return block;
}

Block* getBlockHook2(BlockSource* _this, const BlockPos& blockPos, unsigned int flag) {
    auto oFunc = getBlockDetours.at(2)->GetFastcall<Block*, BlockSource*, BlockPos, unsigned int>();

    static auto oreESP = ModuleManager::GetModule<OreESP>();
    Vector3 blockPosVec = Vector3(blockPos.x, blockPos.y, blockPos.z);
    const auto block = oFunc(_this, blockPos, flag);

    if (oreESP->Enabled && block) {
        oreESP->OnGetBlock(blockPosVec, block);
    }

    return block;
}




void BlockSourceGetBlock::Init()
{
    auto vtable = Minecraft::ClientInstance->GetBlockSource()->vTable();
    getBlockDetours[0] = std::make_unique<Detour>("BlockSource::getBlockA", vtable[0], &getBlockHook0);
    getBlockDetours[1] = std::make_unique<Detour>("BlockSource::getBlockB", vtable[1], &getBlockHook1);
    getBlockDetours[2] = std::make_unique<Detour>("BlockSource::getBlockC", vtable[2], &getBlockHook2);

    getBlockDetours[0]->Enable();
    getBlockDetours[1]->Enable();
    getBlockDetours[2]->Enable();

    this->Detours.emplace_back(getBlockDetours.at(0).get());
    this->Detours.emplace_back(getBlockDetours.at(1).get());
    this->Detours.emplace_back(getBlockDetours.at(2).get());
}

void BlockSourceGetBlock::UnHook()
{

}
