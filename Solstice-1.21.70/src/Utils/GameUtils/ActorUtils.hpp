#pragma once
//
// Created by vastrakai on 6/28/2024.
//


#include <complex.h>
#include <vector>
#include <SDK/Minecraft/Actor/ActorType.hpp>
#include <SDK/Minecraft/Network/Packets/InventoryTransactionPacket.hpp>

class ActorUtils {
public:
    static std::vector<class Actor*> getActorList(bool playerOnly = true, bool excludeBots = true);
    static std::vector<Actor*> getActorsOfType(ActorType type);
    template <typename T = Actor>
    static std::vector<T*> getActorsTyped(ActorType type) {
        const std::vector<Actor*> actors = getActorsOfType(type);
        std::vector<T*> result;
        for (auto actor : actors) {
            result.push_back(static_cast<T*>(actor));
        }
        return result;
    }
    static bool isBot(Actor* actor);
    static std::shared_ptr<InventoryTransactionPacket> createAttackTransaction(Actor* actor, int slot = -1);
    static Actor* getActorFromUniqueId(int64_t uniqueId);
    static Actor* getActorFromRuntimeID(int64_t runtimeId);
};
