//
// Created by vastrakai on 4/14/2024.

//

#include <imgui_internal.h>
#include <Prax/Module/Modules/Combat/Aura.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/SDK/Game/Network/Other/CompletedUsingItemPacket.h>
#include <Prax/Module/Modules/Combat/Antibot.h>
#include <Prax/SDK/Game/Network/Other/LevelEventPacket.h>
#include "Anticheat.h"

#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>
#include <Prax/SDK/Game/Entity/Components/ActorOwnerComponent.h>
#include <Prax/SDK/Game/Entity/Components/ActorTypeComponent.h>

bool Anticheat::IsExposed(Vector3 pos)
{
    std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(pos);

    bool isExposed = false;

    bool redstoneFound = false;

    for (auto& block : surroundingBlocks) {
        // Return true if the block is air
        if (block.second.block->blockLegacy->id == 0) return true;
        // Return true if the block is a chest
        if (block.second.block->blockLegacy->id == 54) return true;
        if (!block.second.block->blockLegacy->isSolid) return true;
        if (block.second.block->blockLegacy->id == 73 || block.second.block->blockLegacy->id == 74) redstoneFound = true;
    }

    // if the block at the pos is redstone here then return false
    auto block = Minecraft::ClientInstance->GetBlockSource()->getBlock(BlockPos(pos));

    if (block->blockLegacy->id == 73 || block->blockLegacy->id == 74) return false;
    if (redstoneFound) return false;

    return true;
}

void Anticheat::OnTick() {
    if (StrafeCheck.Enabled) {
        // Strafe check
        auto player = Minecraft::ClientInstance->ThePlayer();

        Vector3 velocity = player->GetStateVectorComponent()->velocity;
        float speed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

        float velAngle = atan2(velocity.z, velocity.x) * (180 / IM_PI) - 90.f;
        static float lastVelAngle = 0.f;

        // Calculate the difference between the current velocity angle and the last velocity angle
        // and include compensation for ranges that cross -180/180
        float diff = velAngle - lastVelAngle;
        if (diff > 180) diff -= 360;
        if (diff < -180) diff += 360;



        if (abs(diff) > 16 && speed > 0.2f && !player->isCollidingHorizontal() && !player->isOnGround()) {
            player->addChatMessage("&6[Anticheat]&f &7You failed &cStrafe &7| &fdiff:" + std::to_string(diff) + " &7| &fspeed:" + std::to_string(speed), true);
        }

        lastVelAngle = velAngle;
    }
/*
    if (CombatCheck.Enabled) {
        // Combat check
        auto player = Minecraft::ClientInstance->ThePlayer();

        static Aura* aura = (Aura*)ModuleManager::GetModuleByName("Aura");

        Actor* auraTarget = Aura::HasTarget ? Aura::LastAttackedActor : nullptr;

        if (auraTarget != nullptr) {
            auto actors = player->level->getRuntimeActorList();
            // Look for a pig entity near the player
            Actor* pig = nullptr;
            for (auto const& actor : actors) {
                if (actor->entityIdString == "minecraft:pig") {
                    float distance = actor->GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos());

                    if (distance < aura->HitRange.Value) {
                        pig = actor;
                    }
                }
            }

            // Fail if the player isnt attacking the pig bot
            if (pig != nullptr && pig != auraTarget) {
                player->addChatMessage("&6[Anticheat]&f &7You &cfailed Combat &7| &fnot hitting the pig!", true);
            }
        }

    }*/

}

bool IsExposedCheck(Vector3 pos)
{
    std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(pos);

    bool isExposed = false;

    for (auto& block : surroundingBlocks) {
        // Return true if the block is air
        if (block.second.block->blockLegacy->id == 0) return true;
        // Return true if the block is a chest
        if (block.second.block->blockLegacy->id == 54) return true;
    }

    return false;
}

struct DaFlag {
    uint64_t lastFlag;
    int timesFlagged;
};

void Anticheat::OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled) {

    if (packet->getId() == (int)PacketID::InventoryTransaction && CombatCheck.Enabled) {
        auto itp = reinterpret_cast<InventoryTransactionPacket *>(packet);
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        if (itp->mTransaction->GetTransacType() == TransacType::ItemUseOnEntityTransaction) {
            auto actorTransac = reinterpret_cast<ItemUseOnActorInventoryTransaction*>(itp->mTransaction.get());

            Actor* actor = nullptr;
            Actor* pigActor = nullptr;

            // Find the actor with this runtimeId
            for (auto&& [_, moduleOwner, type, ridc] : player->entityCtx.registry->view<ActorOwnerComponent, ActorTypeComponent, RuntimeIDComponent>().each())
            {
                if (type.type == ActorType::Pig) {
                    pigActor = moduleOwner.actor;
                    if (pigActor == nullptr) return;
                }

                if (ridc.runtimeID == actorTransac->actorId && type.type == ActorType::Player) {
                    actor = moduleOwner.actor;
                    if (actor == nullptr) return;
                }

                if (moduleOwner.actor->entityCtx.getComponent<ActorUniqueIDComponent>()->uniqueID == actorTransac->actorId) {
                    actor = moduleOwner.actor;
                    if (actor == nullptr) return;
                }
            }

            if (actor == nullptr || pigActor == nullptr) return;



            if (actor != pigActor) {
                player->addChatMessage("&6[Anticheat]&f &7You failed &cCombat &7| &fnot hitting the pig!", true);
                Logger::Write("Anticheat", "actor: " + std::to_string(actor->GetRuntimeID()) + " pig: " + std::to_string(pigActor->GetRuntimeID()) + " [attack invalid]", Logger::LogType::Debug);
            } else {
                Logger::Write("Anticheat", "actor: " + std::to_string(actor->GetRuntimeID()) + " pig: " + std::to_string(pigActor->GetRuntimeID()) + " [attack valid]", Logger::LogType::Debug);
            }
        }
    }

    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto paip = reinterpret_cast<PlayerAuthInputPacket *>(packet);
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        static std::map<Actor*, DaFlag> lastFlags;

        // if inputflags has AuthInputAction::PERFORM_BLOCK_ACTIONS
        if ((paip->inputFlags & AuthInputAction::PERFORM_BLOCK_ACTIONS) == AuthInputAction::PERFORM_BLOCK_ACTIONS) {
            for (auto const &block: paip->playerBlockActions.mActions) {
                if (block.action == PlayerActionType::START_DESTROY_BLOCK || block.action == PlayerActionType::PREDICT_DESTROY_BLOCK) {
                    if (ExposedCheck.Enabled && !IsExposed(block.position)) {
                        // if the last flag is less than 50ms ago, return
                        if (lastFlags.contains(player) && Misc::GetCurrentMs() - lastFlags[player].lastFlag < 50) return;

                        lastFlags[player] = {(uint64_t)Misc::GetCurrentMs(), lastFlags[player].timesFlagged + 1};

                        player->addChatMessage("&6[Anticheat]&f &7You failed &cUnderground Mining &l&7[&cFLAREON&7]&r &6x" + std::to_string(lastFlags[player].timesFlagged) + "&f | &7" + std::to_string((int)block.position.x) + " " + std::to_string((int)block.position.y) + " " + std::to_string((int)block.position.z), true);
                    }
                }
            }
        }
    }
}

struct InvSwap
{
    std::map<uint64_t, int> swapCount = {};

    void incSwapCount()
    {
        // get da last swap time
        uint64_t lastSwapCount = swapCount.empty() ? 0 : swapCount.rbegin()->second;
        swapCount[Misc::GetCurrentMs()] = lastSwapCount + 1;
    }

    void clearOldSwaps(uint64_t timeframe)
    {
        for (auto it = swapCount.begin(); it != swapCount.end(); ) {
            if (Misc::GetCurrentMs() - it->first > timeframe) {
                it = swapCount.erase(it);
            } else {
                ++it;
            }
        }
    }

    int getRecentSwapCount(uint64_t timeframe)
    {
        clearOldSwaps(timeframe);

        int count = 0;
        // sort the swaps by time
        std::map<uint64_t, int> sortedSwaps(swapCount.begin(), swapCount.end());

        // get the oldest swap count
        int oldestSwapCount = sortedSwaps.begin()->second;

        // get the newest swap count
        int newestSwapCount = sortedSwaps.rbegin()->second;

        // get the difference between the newest and oldest swap count
        count = newestSwapCount - oldestSwapCount;

        Logger::Write("Anticheat", "count: " + std::to_string(count), Logger::LogType::Debug);

        return count;
    }
};

void Anticheat::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::LevelEvent) {
        auto lep = std::reinterpret_pointer_cast<LevelEventPacket>(packet);

        if (lep->eventId == 3600 && ExposedCheck.Enabled && !IsExposedCheck(lep->pos)) {
            static std::map<Actor*, DaFlag> lastFlags;
            if (auto player = Minecraft::ClientInstance->ThePlayer()) {
                // Get the closest actor to this pos
                Actor *closestActor = player;

                auto actors = Antibot::GetActors(true);
                float closestDistance = -1.f;

                for (auto const &actor: actors) {
                    float distance = actor->GetAABB().GetClosestPoint(lep->pos).Distance(lep->pos);
                    if (closestDistance == -1.f || distance < closestDistance) {
                        closestDistance = distance;
                        closestActor = actor;
                    }
                }

                // if the last flag is less than 50ms ago, return
                if (lastFlags.contains(closestActor) && Misc::GetCurrentMs() - lastFlags[closestActor].lastFlag < 50) return;

                std::string username = closestActor->_getNameTag();
                if (username.empty()) username = "Unknown";
                // If the username contains a newline, remove the newline and everything after it
                if (username.find("\n") != std::string::npos) {
                    username = username.substr(0, username.find("\n"));
                }

                if (closestActor == player) {
                    if (IsExposed(lep->pos)) {
                        lastFlags[closestActor] = {(uint64_t) Misc::GetCurrentMs(),
                                                   lastFlags[closestActor].timesFlagged + 1};
                        player->addChatMessage("&6[Anticheat]&f &7You failed &cUnderground Mining &6x" +
                                               std::to_string(lastFlags[closestActor].timesFlagged) + "&f | &7" +
                                               std::to_string((int) lep->pos.x) + " " +
                                               std::to_string((int) lep->pos.y) + " " +
                                               std::to_string((int) lep->pos.z), true);
                    } else {
                        lastFlags[closestActor] = {(uint64_t) Misc::GetCurrentMs(),
                                                   lastFlags[closestActor].timesFlagged + 1};
                        player->addChatMessage("&6[Anticheat]&f &7You failed &cUnderground Mining &l&7[&cFLAREON&7]&r &6x" +
                                               std::to_string(lastFlags[closestActor].timesFlagged) + "&f | &7" +
                                               std::to_string((int) lep->pos.x) + " " +
                                               std::to_string((int) lep->pos.y) + " " +
                                               std::to_string((int) lep->pos.z), true);
                    }
                    return;
                }

                if (IsExposed(lep->pos)) {
                    lastFlags[closestActor] = {(uint64_t) Misc::GetCurrentMs(),
                                               lastFlags[closestActor].timesFlagged + 1};
                    player->addChatMessage("&6[Anticheat]&f &7" + username + "&r&f failed &cUnderground Mining &6x" +
                                           std::to_string(lastFlags[closestActor].timesFlagged) + "&f | &7" +
                                           std::to_string((int) lep->pos.x) + " " + std::to_string((int) lep->pos.y) +
                                           " " + std::to_string((int) lep->pos.z), true);
                } else {
                    lastFlags[closestActor] = {(uint64_t) Misc::GetCurrentMs(),
                                               lastFlags[closestActor].timesFlagged + 1};
                    player->addChatMessage("&6[Anticheat]&f &7" + username +
                                           "&r&f failed &cUnderground Mining &l&7[&cFLAREON&7]&r &6x" +
                                           std::to_string(lastFlags[closestActor].timesFlagged) + "&f | &7" +
                                           std::to_string((int) lep->pos.x) + " " + std::to_string((int) lep->pos.y) +
                                           " " + std::to_string((int) lep->pos.z), true);
                }
            }
        }
    }
    if (packet->getId() == (int)PacketID::MoveEntityAbsolute && HeadYaw.Enabled) {
        auto maap = std::reinterpret_pointer_cast<MoveActorAbsolutePacket>(packet);
        auto player = Minecraft::ClientInstance->ThePlayer();

        Actor* daActor = nullptr;

        for (auto const& actor : Antibot::GetActors()) {
            if (actor->GetRuntimeID() == maap->mRuntimeId) {
                daActor = actor;
                break;
            }

        }

        if (daActor == nullptr) return;

        static std::map<Actor*, MoveActorAbsolutePacket> lastPkts;
        static std::map<Actor*, DaFlag> lastFlags;

        MoveActorAbsolutePacket lastPacket = lastPkts.find(daActor) != lastPkts.end() ? lastPkts[daActor] : MoveActorAbsolutePacket();

        if (lastPacket.mRuntimeId == maap->mRuntimeId) { // A way of validating if we have a prev packet to compare to

            std::string username = daActor->_getNameTag();
            if (username.empty()) username = "Unknown";
            if (username.find('\n') != std::string::npos) {
                username = username.substr(0, username.find('\n'));
            }

            if (maap->GetHeadYaw() == 0.f) return;

            // Just log the diff between the headYaw and yaw
            float headYawDiff = abs(maap->GetHeadYaw() - maap->GetYaw());
            float lastYawDiff = abs(lastPacket.GetYaw() - maap->GetYaw());
            // Discard if diff is less than 140 and more than 200
            if (!(headYawDiff < 30 || lastYawDiff > 20) && headYawDiff < 200) {
                lastFlags[daActor] = {(uint64_t) Misc::GetCurrentMs(),
                                      lastFlags[daActor].timesFlagged + 1};

                int timesFlagged = lastFlags[daActor].timesFlagged;
                player->addChatMessage("&6[Anticheat]&f &7" + username + "&r&f failed &cHeadYaw &r&6x" + std::to_string(timesFlagged) + "&7 | &fdiff: " + std::to_string(headYawDiff), true);
                Logger::Write("Anticheat", username + ": headYaw: " + std::to_string(maap->GetHeadYaw()) + " yaw: " + std::to_string(maap->GetYaw()) + " diff: " + std::to_string(abs(maap->GetHeadYaw() - maap->GetYaw())), Logger::LogType::Debug);
            }
        }



        lastPkts[daActor] = *maap;
    }


    if (packet->getId() == (int)PacketID::MobArmorEquipment && InvManagerCheck.Enabled)
    {
        auto maep = std::reinterpret_pointer_cast<MobArmorEquipmentPacket>(packet);

        static std::map<uint64_t, InvSwap> invManagerThings;

        uint64_t targetRuntimeId = maep->mRuntimeId;
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;
        if (player->GetRuntimeID() == targetRuntimeId) return;

        auto actors = Antibot::GetActors();
        Actor* target = nullptr;
        for (auto const& actor : actors) {
            if (actor->GetRuntimeID() == targetRuntimeId) {
                target = actor;
                break;
            }
        }
        if (target == nullptr) return;

        ItemStack* currentHelmetStack = target->getArmor(ArmorSlot::Helmet);
        ItemStack* currentChestplateStack = target->getArmor(ArmorSlot::Chestplate);
        ItemStack* currentLeggingsStack = target->getArmor(ArmorSlot::Leggings);
        ItemStack* currentBootsStack = target->getArmor(ArmorSlot::Boots);

        Item* currentHelmet = currentHelmetStack->item ? *currentHelmetStack->item : nullptr;
        Item* currentChestplate = currentChestplateStack->item ? *currentChestplateStack->item : nullptr;
        Item* currentLeggings = currentLeggingsStack->item ? *currentLeggingsStack->item : nullptr;
        Item* currentBoots = currentBootsStack->item ? *currentBootsStack->item : nullptr;

        Item* newHelmet = maep->mHead.baseItem->getItem();
        Item* newChestplate = maep->mTorso.baseItem->getItem();
        Item* newLeggings = maep->mLegs.baseItem->getItem();
        Item* newBoots = maep->mFeet.baseItem->getItem();

        bool changedHelmet = currentHelmet != newHelmet;
        bool changedChestplate = currentChestplate != newChestplate;
        bool changedLeggings = currentLeggings != newLeggings;
        bool changedBoots = currentBoots != newBoots;

        // Continue if no armor pieces were changed
        if (!changedHelmet && !changedChestplate && !changedLeggings && !changedBoots) return;



        if (!invManagerThings.contains(targetRuntimeId)) {
            invManagerThings[targetRuntimeId] = {};
        }

        invManagerThings[targetRuntimeId].incSwapCount();

        uint64_t timeframe = InvTimeframe.Value;
        uint64_t maxSwaps = MaxSwaps.Value;
        int swap = invManagerThings[targetRuntimeId].getRecentSwapCount(timeframe);


        if (swap >= maxSwaps) {
            std::string username = target->_getNameTag();
            if (username.empty()) username = "Unknown";
            if (username.find('\n') != std::string::npos) {
                username = username.substr(0, username.find('\n'));
            }
            player->addChatMessage("&6[Anticheat]&f &7" + username + "&r&f failed &cInvManager &7| &fswaps: " + std::to_string(swap), true);
        }

        // clear swaps older than timeframe
        invManagerThings[targetRuntimeId].clearOldSwaps(timeframe);
    }
}

void Anticheat::OnEnable() {

}
