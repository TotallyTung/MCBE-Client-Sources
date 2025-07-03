//
// Created by vastrakai on 7/10/2024.
//

#include "Level.hpp"

#include <libhat.hpp>
#include <SDK/OffsetProvider.hpp>
#include <SDK/Minecraft/Actor/SyncedPlayerMovementSettings.hpp>

std::unordered_map<mce::UUID, PlayerListEntry>* Level::getPlayerList()
{
    static auto vIndex = OffsetProvider::Level_getPlayerList;
    return MemUtils::callVirtualFunc<std::unordered_map<mce::UUID, PlayerListEntry>*>(vIndex, this);
}

HitResult* Level::getHitResult()
{
    static auto vIndex = OffsetProvider::Level_getHitResult;
    return MemUtils::callVirtualFunc<HitResult*>(vIndex, this);
}

SyncedPlayerMovementSettings* Level::getPlayerMovementSettings()
{
    static auto vIndex = OffsetProvider::Level_getPlayerMovementSettings;
    return MemUtils::callVirtualFunc<SyncedPlayerMovementSettings*>(vIndex, this);
}

std::vector<Actor*> Level::getRuntimeActorList()
{
    static auto func = SigManager::Level_getRuntimeActorList;
    std::vector<Actor*> actors;
    MemUtils::callFastcall<void>(func, this, &actors);
    return actors;
}

LevelData* Level::getLevelData()
{
    static auto vIndex = OffsetProvider::Level_getLevelData;
    return MemUtils::callVirtualFunc<LevelData*>(vIndex, this);
}