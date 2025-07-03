//
// Created by vastrakai on 7/3/2024.
//

#include "BlockLegacy.hpp"

#include <SDK/OffsetProvider.hpp>
#include <SDK/SigManager.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>

#include "BlockSource.hpp"

uint16_t BlockLegacy::getBlockId()
{
    return hat::member_at<uint16_t>(this, OffsetProvider::BlockLegacy_mBlockId);
}

bool BlockLegacy::mayPlaceOn(glm::ivec3 pos)
{
    BlockSource* blockSource = ClientInstance::get()->getBlockSource();
    static auto vIndex = OffsetProvider::BlockLegacy_mayPlaceOn;
    return MemUtils::callVirtualFunc<bool, void*, const glm::ivec3&>(vIndex, this, blockSource, pos);
}

bool BlockLegacy::isAir()
{
    return getBlockId() == 0;
}

template <typename T>
class optional_ref
{
private:
    T* mPtr = nullptr;
};

AABB BlockLegacy::getCollisionShape(Block* block, BlockSource* source, BlockPos pos)
{
    //  virtual class AABB getCollisionShape(class Block const&, class IConstBlockSource const&, class BlockPos const&, class optional_ref<class GetCollisionShapeInterface const>) const;
    static int index = OffsetProvider::BlockLegacy_getCollisionShape;
    return MemUtils::callVirtualFunc<AABB, Block*, BlockSource*, BlockPos, optional_ref<void>>(index, this, block, source, pos, optional_ref<void>());
}
