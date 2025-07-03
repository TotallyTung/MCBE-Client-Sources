#include "BlockUtils.h"
#include "../Client/Client.h"
#include "../SDK/GameData.h"
#include "../SDK/Classes/BlockSource.h"
#include "../Client/ModuleManager/Modules/Module.h"
bool BlockUtils::isContainer(Vec3<int> pos) {
    BlockSource* src = mc.getLocalPlayer()->dimension->blockSource;
    Block* block = src->getBlock(pos);
    BlockLegacy* blockLegacy = block->blockLegacy;
	return blockLegacy->blockId == 130 || blockLegacy->blockId == 54 || blockLegacy->blockId == 146 || blockLegacy->blockId == 145;
}

int BlockUtils::getBlockId(Vec3<int> pos) {
    BlockSource* src = mc.getLocalPlayer()->dimension->blockSource;
    Block* block = src->getBlock(pos);
    BlockLegacy* blockLegacy = block->blockLegacy;
	return blockLegacy->blockId;
}

BLOCKTYPE BlockUtils::getBlockType(Vec3<int> pos) {
    BlockSource* src = mc.getLocalPlayer()->dimension->blockSource;
    Block* block = src->getBlock(pos);
    BlockLegacy* blockLegacy = block->blockLegacy;
    return static_cast<BLOCKTYPE>(blockLegacy->blockId);
}

bool BlockUtils::isReplaceable(Vec3<int> pos) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    BlockSource* src = localPlayer->dimension->blockSource;
    if (!src) return false;
    if (getBlockId(pos) == 0) return true;
    AABB blockAABB;
    blockAABB.lower = pos.toFloat();
    blockAABB.upper.x = pos.x + 1.f;
    blockAABB.upper.y = pos.y + 1.f;
    blockAABB.upper.z = pos.z + 1.f;
    if (getBlockName(pos) == "snow_layer") return true;
    if (src->containsAnyLiquid(blockAABB)) return true; // i know waterlogged blocks will fuck this up but WHATEVERs
    return false;
}

std::string BlockUtils::getBlockName(Vec3<int> pos) {
    BlockSource* src = mc.getLocalPlayer()->dimension->blockSource;
    Block* block = src->getBlock(pos);
    BlockLegacy* blockLegacy = block->blockLegacy;
    return blockLegacy->blockName;
}

std::vector<Vec3<int>> checklist = {
    {Vec3<int>(0, -1, 0)}, {Vec3<int>(0, 1, 0)},
    {Vec3<int>(0, 0, -1)}, {Vec3<int>(0, 0, 1)},
    {Vec3<int>(-1, 0, 0)}, {Vec3<int>(1, 0, 0)}
};

bool BlockUtils::buildBlock(Vec3<int> pos, bool airplace) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return false;
    BlockSource* blockSource = localPlayer->dimension->blockSource;
    if (!blockSource) return false;
    Block* block = blockSource->getBlock(pos);
    if (!block) return false;
    if (airplace) {
        if (isReplaceable(pos)) mc.getGameMode()->buildBlock(pos, 0, false);
        return true;
    }
    if (isReplaceable(pos)) {
        Vec3<int> placePos(pos);
        for (int face = 0; face < checklist.size(); ++face) {
            Vec3<int> calc = placePos.sub(checklist[face]);
            if (!isReplaceable(calc) && !isContainer(calc)) {
                mc.getGameMode()->buildBlock(calc, face, false);
                return true;
            }
        }
    }
    return false;
}

bool BlockUtils::canBuildBlock(Vec3<int> pos, bool airplace) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    BlockSource* blockSource = localPlayer->dimension->blockSource;
    Block* block = blockSource->getBlock(pos);
    Vec3<float> placePos(pos.x, pos.y, pos.z);
    if (airplace) {
        for (Actor* actor : localPlayer->getLevel()->getRuntimeActorList()) {
            if (!TargetUtils::isTargetValid(actor, true)) continue;
            if (actor->getAABB()->intersects(AABB(placePos, placePos.add(1.f, 1.f, 1.f)))) return false;
        }
        return true;
    }
    if (isReplaceable(pos)) {
        for (int face = 0; face < checklist.size(); ++face) {
            Vec3<int> calc = Vec3<int>(placePos.x, placePos.y, placePos.z).sub(checklist[face]);
            if (!isReplaceable(calc) && !isContainer(calc)) {
                for (Actor* actor : localPlayer->getLevel()->getRuntimeActorList()) {
                    if (!TargetUtils::isTargetValid(actor, true)) continue;
                    if (actor->getAABB()->intersects(AABB(placePos, placePos.add(1.f, 1.f, 1.f)))) return false;
                }
                return true;
            }
        }
    }
    else return true;
    return false;
}

bool BlockUtils::isReplaceable(Vec3<float> pos) {
    return isReplaceable(pos.toInt());
}

bool BlockUtils::isContainer(Vec3<float> pos) {
    return isContainer(pos.toInt());
}

int BlockUtils::getBlockId(Vec3<float> pos) {
    return getBlockId(pos.toInt());
}

bool BlockUtils::buildBlock(Vec3<float> pos, bool airplace) {
    return buildBlock(pos.toInt(), airplace);
}

bool BlockUtils::canBuildBlock(Vec3<float> pos, bool airplace) {
    return canBuildBlock(pos.toInt(), airplace);
}

std::string BlockUtils::getBlockName(Vec3<float> pos) {
    return getBlockName(pos.toInt());
}

BLOCKTYPE BlockUtils::getBlockType(Vec3<float> pos) {
    return getBlockType(pos.toInt());
}