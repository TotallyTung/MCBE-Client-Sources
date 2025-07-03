//
// Created by vastrakai on 10/2/2024.
//

#include "WorldLib.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>

void WorldLib::initialize(lua_State* L)
{
    getGlobalNamespace(L)
        .beginClass<BlockSource>("BlockSource")
        .addFunction("getBlockVec", [](BlockSource* blockSource, glm::vec3 pos) {
            return blockSource->getBlock(pos.x, pos.y, pos.z);
        })
        .addFunction("getBlock", [](BlockSource* blockSource, int x, int y, int z) {
            return blockSource->getBlock(x, y, z);
        })
        .addFunction("getBuildHeight", &BlockSource::getBuildHeight)
        .addFunction("getBuildDepth", &BlockSource::getBuildDepth)
        .addFunction("checkRayTrace", [](BlockSource* blockSource, glm::vec3 start, glm::vec3 end) {
            return blockSource->checkRayTrace(start, end, ClientInstance::get()->getLocalPlayer());
        })
        .endClass()
        .beginClass<Block>("Block")
        .addFunction("toLegacy", &Block::toLegacy)
        .endClass()
        .beginClass<BlockLegacy>("BlockLegacy")
        .addFunction("getBlockId", &BlockLegacy::getBlockId)
        .addFunction("mayPlaceOn", &BlockLegacy::mayPlaceOn)
        .addFunction("isAir", &BlockLegacy::isAir)
        .addFunction("isSolid", [](BlockLegacy* blockLegacy) {
            return blockLegacy->mMaterial->mIsBlockingMotion;
        })
        .addFunction("getTileName", [](BlockLegacy* blockLegacy) {
            return blockLegacy->mTileName;
        })
        .addFunction("getName", [](BlockLegacy* blockLegacy) {
            return blockLegacy->mName;
        })
        .addFunction("getMaterial", [](BlockLegacy* blockLegacy) {
            return blockLegacy->mMaterial;
        })
        .endClass()
        .beginClass<Material>("Material")
        .addFunction("isTopSolid", &Material::isTopSolid)
        .addFunction("getType", &Material::getmType)
        .addFunction("isFlammable", &Material::getmIsFlammable)
        .addFunction("isNeverBuildable", &Material::getmIsNeverBuildable)
        .addFunction("isLiquid", &Material::getmIsLiquid)
        .addFunction("isBlockingMotion", &Material::getmIsBlockingMotion)
        .addFunction("isSuperHot", &Material::getmIsSuperHot)
        .endClass()
        .beginClass<BlockUtils>("BlockUtils")
        .addStaticFunction("getBlockPlaceFace", &BlockUtils::getBlockPlaceFace)
        .addStaticFunction("getExposedFace", &BlockUtils::getExposedFace)
        .addStaticFunction("isValidPlacePos", &BlockUtils::isValidPlacePos)
        .addStaticFunction("destroyBlock", &BlockUtils::destroyBlock)
        .addStaticFunction("placeBlock`", &BlockUtils::placeBlock)
        .endClass()


    ;
}
