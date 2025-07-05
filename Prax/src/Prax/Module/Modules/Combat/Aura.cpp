#pragma once

#include "Aura.h"

#include "Antibot.h"
#include <imgui_internal.h>
#include <Prax/Hook/Hooks/RakPeerHooks.h>
#include <Prax/Utils/Math.h>
#include "../../../UI/ClickGui.h"
#include <Prax/Utils/Render/D2D.h>
#include <Prax/SDK/Game/Network/Network.h>
#include <Prax/Module/Modules/Player/Friends.h>
#include <Prax/Module/Modules/Visual/HUD.h>
#include <Prax/SDK/Game/Render/LevelRenderer.h>
#include <Prax/Module/Modules/World/Scaffold.h>
#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include <Prax/SDK/Game/Network/Other/MobEquipmentPacket.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/SDK/Game/Network/Other/RemoveEntityPacket.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/SDK/Game/Network/Other/CompletedUsingItemPacket.h>
#include <Prax/SDK/Game/Entity/Components/ActorOwnerComponent.h>
#include <Prax/SDK/Game/Entity/Components/ActorTypeComponent.h>
#include <Prax/Module/Modules/Misc/Surround.h>
#include <Prax/Module/Modules/Player/AntiRegen.h>
#include <Prax/Module/Modules/Visual/AntiInvis.h>

uint64_t Aura::LastBlockPlace = 0;

int Aura::GetArmorProtection(Actor* actor)
{
    if (!actor->isPlayer()) return false;
    ItemStack* helmetItem = actor->getArmor(ArmorSlot::Helmet);
    ItemStack* chestplateItem = actor->getArmor(ArmorSlot::Chestplate);
    ItemStack* leggingsItem = actor->getArmor(ArmorSlot::Leggings);
    ItemStack* bootsItem = actor->getArmor(ArmorSlot::Boots);

    int protectionFound = 0;

    if (helmetItem->item)
    {
        Item* item = *helmetItem->item;
        protectionFound += helmetItem->getEnchantValue(Enchant::PROTECTION) + helmetItem->getEnchantValue(Enchant::FIRE_PROTECTION);
    }

    if (chestplateItem->item)
    {
        Item* item = *chestplateItem->item;
        protectionFound += chestplateItem->getEnchantValue(Enchant::PROTECTION) + chestplateItem->getEnchantValue(Enchant::FIRE_PROTECTION);
    }

    if (leggingsItem->item)
    {
        Item* item = *leggingsItem->item;
        protectionFound += leggingsItem->getEnchantValue(Enchant::PROTECTION) + leggingsItem->getEnchantValue(Enchant::FIRE_PROTECTION);
    }

    if (bootsItem->item)
    {
        Item* item = *bootsItem->item;
        protectionFound += bootsItem->getEnchantValue(Enchant::PROTECTION) + bootsItem->getEnchantValue(Enchant::FIRE_PROTECTION);
    }

    return protectionFound;
}

bool Aura::IsFullDiamond(Actor* actor, bool noEnchants) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;

    if (!actor->isPlayer()) return false;

    ItemStack* helmetItem = actor->getArmor(ArmorSlot::Helmet);
    ItemStack* chestplateItem = actor->getArmor(ArmorSlot::Chestplate);
    ItemStack* leggingsItem = actor->getArmor(ArmorSlot::Leggings);
    ItemStack* bootsItem = actor->getArmor(ArmorSlot::Boots);

    if (!helmetItem->item || !chestplateItem->item || !leggingsItem->item || !bootsItem->item) return false;

    Item* helmet = *helmetItem->item;
    Item* chestplate = *chestplateItem->item;
    Item* leggings = *leggingsItem->item;
    Item* boots = *bootsItem->item;

    if (helmet->id != 310) return false;
    if (chestplate->id != 311) return false;
    if (leggings->id != 312) return false;
    if (boots->id != 313) return false;

    if (noEnchants && GetArmorProtection(actor) != 0) return false;

    return true;
}

bool Aura::IsPlayerProtected(Actor* actor)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;

    if (!actor->isPlayer()) return false;

    return GetArmorProtection(actor) != 0;
}

int Aura::GetSword(Actor* target)
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (FistFriends.Enabled && Friends::IsFriend(target))
    {
        // Look for a TROPICAL_FISH (id 267) in the inventory
        int index = 0;
        for (auto itemStack : player->supplies->GetInventoryItems())
        {
            if (!itemStack->item) {
                index++;
                continue;
            }

            if ((*itemStack->item)->id == 267) return index;

            index++;
        }

        return 40;

    }


    // Get the best sword
    int bestSlot = player->supplies->GetBestSword(HotbarOnly.Enabled);
    if (bestSlot == -1) return -1;

    if (!AutoFireSword.Enabled) return bestSlot;
    int goldSword = player->supplies->GetGoldSword(HotbarOnly.Enabled);
    if (goldSword == -1) return bestSlot;

    bool isFullDia = IsFullDiamond(target, true);

    if (isFullDia)
    {
        return goldSword;
    }

    if (target->isOnFire())
    {
        return bestSlot;
    } else if (ShouldUseFireSword(target))
    {
        return goldSword;
    }
}

bool Aura::ShouldUseFireSword(Actor* target)
{
    if (IsPlayerProtected(target)) return false;
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;

    if (Misc::GetCurrentMs() - LastBlockPlace < 400 && FireSwordSpoof.Enabled)
    {
        return false;
    }

    if (!AutoFireSword.Enabled) return false;

    int bestSlot = player->supplies->GetBestSword(HotbarOnly.Enabled);
    if (bestSlot == -1)
    {
        return false;
    }

    int goldSword = player->supplies->GetGoldSword(HotbarOnly.Enabled);
    if (goldSword == -1)
    {
        return false;
    }

    bool isFullDia = IsFullDiamond(target, true);

    if (isFullDia)
    {
        return true;
    }

    if (target->isOnFire())
    {
        return false;
    } else
    {
        return true;
    }
}

bool rotate = false;
Vector2 rots = Vector2(0.001f, 0.001f);
__int64 nextHitTime = 0;
Actor* Aura::LastTargetedActor = nullptr;
Actor* Aura::LastAttackedActor = nullptr;

void Aura::OnEnable() {
    // Clear all stuff
    LastTargetedActor = nullptr;
    nextHitTime = Misc::GetCurrentMs();
}

// We need a way of sorting actors by the last attack time
std::unordered_map<Actor*, __int64> LastAttackTimes;

// Comparator function to sort actors by lastAttackTime
bool CompareByLastAttackTime(Actor* a, Actor* b) {
    return LastAttackTimes[a] < LastAttackTimes[b];
}

bool Aura::HasTarget = false;
int Aura::OldSlot = -1;
int Aura::SpoofedSlot = -1;
uint64_t Aura::LastSwitchTime = 0;
AABB Aura::LastTargetedAABB = AABB(Vector3(0, 0, 0), Vector3(0, 0, 0));
bool rotateFlick = false;

void Aura::OnTick(){
    auto player = Minecraft::ClientInstance->ThePlayer();

    bool result = Tick();
    if (result) rotateFlick = true;

    /*if (AutoSelect.Enabled && !result && SwitchBack.Enabled && !SpoofSlot.Enabled) {
        uint64_t now = Misc::GetCurrentMs();
        uint64_t diff = now - LastSwitchTime;
        if (diff > (Aura::SwitchBackDelay.Value * 20)) {
            if (!result && OldSlot != -1) {
                Minecraft::ClientInstance->ThePlayer()->SelectSlot(OldSlot);
                OldSlot = -1;
            }
        }
    } else if (AutoSelect.Enabled && !result && SwitchBack.Enabled && SpoofSlot.Enabled) {
        uint64_t now = Misc::GetCurrentMs();
        uint64_t diff = now - LastSwitchTime;
        if (diff > (Aura::SwitchBackDelay.Value * 20)) {
            if (!result && OldSlot != -1) {
                Misc::SpoofSwitch(OldSlot);
                OldSlot = -1;
            }
        }
    }*/


}

bool Aura::ThrowProjectiles(){
    if (!ThrowProjectilesSetting.Enabled) return false;

    static uint64_t lastThrow = 0;

    // Only allow 4 throws per second (250ms)
    if (Misc::GetCurrentMs() - lastThrow < 1000 / ThrowRate.Value ) return false;

    auto player = Minecraft::ClientInstance->ThePlayer();

    // Look for a projectile in the inventory
    int index = 0;
    for (auto itemStack : player->supplies->GetInventoryItems()){
        if (!itemStack->item) {
            index++;
            continue;
        }

        auto item = *itemStack->item;

        // Check if the item is a snowball
        std::string name = item->GetName();

        if (name.contains("snowball")) {
            // Found a snowball, throw it

            OldSlot = player->supplies->selectedHotbarSlot;
            player->SelectSlot(index);

            int oldSlotLmao = player->supplies->selectedHotbarSlot;
            player->SelectSlot(index);
            player->gamemode->baseUseItem(itemStack);
            player->SelectSlot(oldSlotLmao);
            lastThrow = Misc::GetCurrentMs();
            return true;
        }

        index++;
    }

    return false;
}

void swing()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    int oldSwingProgress = player->swingProgress;
    player->swing();
    player->swingProgress = 0;
    player->isSwinging = false;
    player->swing();
    player->swingProgress = oldSwingProgress;

}



bool Aura::Tick() {
    static std::vector<std::shared_ptr<InventoryTransactionPacket>> QueuedTransactions;

    bool result = false;
    auto player = Minecraft::ClientInstance->ThePlayer();

    std::vector<Actor*> actors = Antibot::GetActors();
    std::vector<Actor*> allActors = player->level->getRuntimeActorList();

    for (std::shared_ptr<InventoryTransactionPacket>& transac : QueuedTransactions) {
        // validate the Actor id and make sure it exists
        auto* balls = reinterpret_cast<ItemUseOnActorInventoryTransaction*>(transac->mTransaction.get());
        bool found = false;
        Actor* daActor = nullptr;
        for (auto actor : allActors) {
            if (actor->GetRuntimeID() == balls->actorId) {
                found = true;
                daActor = actor;
                break;
            }
        }

        if (!found) continue;

        int prevSlot = player->supplies->selectedHotbarSlot;
        bool spoofBack = false;
        if (ShouldUseFireSword(daActor) && FireSwordSpoof.Enabled) {
            Misc::SpoofSwitch(GetSword(daActor));
            spoofBack = true;
        }

        Minecraft::ClientInstance->packetSender->sendToServer(transac.get());


        bool shouldUseFireSword = ShouldUseFireSword(daActor);
        if (spoofBack) {
            Misc::SpoofSwitch(prevSlot);
        }
    }

    QueuedTransactions.clear();


    bool useHurtTime = HurtTimeCompensationSetting.Enabled;
    float hurtTimeAdjustment = HurtTimeAdjustment.Value;
    float apsSetting = ApsSetting.Value;

    if (RandomizeAPS.Enabled) {
        apsSetting = RandomizeAPSMin.Value + (rand() % (int)(ApsSetting.Value - RandomizeAPSMin.Value));
    }


    // Sort the list by distance
    std::sort(actors.begin(), actors.end(), [&](Actor* a, Actor* b) {
        return a->getPos()->Distance(*player->getPos()) < b->getPos()->Distance(*player->getPos());
    });

    // Sort the actors by lastAttackTime
    if (AuraModeSetting.SelectedIndex == 2) // Switch
        std::ranges::sort(actors, CompareByLastAttackTime);

    bool foundAttackable = false;
    for (auto actor : actors) {
        if (actor == player) continue;
        if (Friends::IsFriend(actor) && !FistFriends.Enabled) continue;

        if (GetDistanceFromClosestAABBPoint(actor, player->getPos()) > TargetRange.Value) continue;
        if (!actor->isAlive()) continue;

        if (Hive.Enabled) {
            if (auto a = CheckForObstructingActor(actor, player->getPos())) {
                foundAttackable = true;

                RotateToEntity(actor);

                // If they are outside of the hit range, break if not multi else continue
                if (GetDistanceFromClosestAABBPoint(actor, player->getPos()) > HitRange.Value) {
                    if (AuraModeSetting.SelectedIndex != 1) break;
                    else continue;
                }

                ThrowProjectiles();

                LastAttackedActor = actor;

                if (Interact.Enabled) player->gamemode->interact(a, *actor->getPos());
                swing();

                if (AttackModeSetting == (int)AttackMode::Synched)
                {
                    //player->addChatMessage("&6[Aura]&f Attacking" + a->entityIdString, true);
                    auto transac = player->MakeAttackTransaction(a);
                    QueuedTransactions.push_back(transac);
                } else
                {
                    //player->addChatMessage("&6[Aura]&f Attacking" + a->entityIdString, true);
                    player->gamemode->attack(a);
                }
                if (AuraModeSetting.SelectedIndex != 1) break; // Break if the mode is not multi
                else continue;
            }
        }


        foundAttackable = true;

        RotateToEntity(actor);

        LastTargetedActor = actor;

        if (GetDistanceFromClosestAABBPoint(actor, player->getPos()) > HitRange.Value) {
            if (AuraModeSetting.SelectedIndex != 1) break;
            else continue;
        }

        __int64 currentTime = Misc::GetCurrentMs();
        auto aps = (__int64)(1000 / apsSetting);

        if (currentTime > nextHitTime || abs(currentTime - nextHitTime) < 15) {
            nextHitTime = currentTime + aps;
        }
        else break;

        ThrowProjectiles();


        if (useHurtTime && actor->isPlayer() && actor->hurtTime && *actor->hurtTime - (int)hurtTimeAdjustment > 0)
        {
            swing();
            continue;
        }

        int slot = GetSword(actor);
        if (slot == -1) slot = player->supplies->selectedHotbarSlot;
        bool shouldUseFireSword = ShouldUseFireSword(actor);

        bool SpoofSlot = this->SpoofSlot.Enabled;
        bool AutoSelect = this->AutoSelect.Enabled;


        static Scaffold* scaffoldMod = ModuleManager::GetModule<Scaffold>();
        static Surround* surroundMod = ModuleManager::GetModule<Surround>();

#ifndef __RELEASEFREE__
        if (AntiRegen::LastPlaceMs + 200 > Misc::GetCurrentMs()) { // If we placed a block in the last 50ms, don't attack
            SpoofSlot = true;
        }
#endif
        if (scaffoldMod && scaffoldMod->Enabled && SpoofWhenScaffolding.Enabled || surroundMod && surroundMod->Enabled && SpoofWhenScaffolding.Enabled) {
            SpoofSlot = true;
        }

        if (AutoSelect && !SpoofSlot) {
            if (slot == -1) slot = player->supplies->selectedHotbarSlot;
            if (OldSlot == -1) OldSlot = player->supplies->selectedHotbarSlot;

            player->supplies->selectedHotbarSlot = slot;
            LastSwitchTime = Misc::GetCurrentMs();
        }

        result = true;

        int oldSlotLol = player->supplies->selectedHotbarSlot;

        if(SpoofSlot) player->SelectSlot(slot);
        if (Interact.Enabled) player->gamemode->interact(actor, *actor->getPos());
        swing();
        if (AttackModeSetting == (int)AttackMode::Synched)
        {
            auto transac = player->MakeAttackTransaction(actor);
            QueuedTransactions.push_back(transac);
        } else
        {
            //player->addChatMessage("&6[Aura]&f Attacking" + actor->entityIdString, true);
            int prevSlot = player->supplies->selectedHotbarSlot;
            if (shouldUseFireSword && FireSwordSpoof.Enabled) {
                Misc::SpoofSwitch(slot);
            }
            player->gamemode->attack(actor);
            if (shouldUseFireSword && FireSwordSpoof.Enabled) {
                Misc::SpoofSwitch(prevSlot);
            }
        }
        LastAttackedActor = actor;
        if(SpoofSlot) player->SelectSlot(oldSlotLol);

        // Update the lastAttackTime for the actor
        if (AuraModeSetting.SelectedIndex == 2) // Switch
            LastAttackTimes[actor] = currentTime;

        if (AuraModeSetting.SelectedIndex != 1) break; // Break if the mode is not multi
    }

    if (!foundAttackable && rotate) {
        LastTargetedActor = nullptr;
        rotate = false;
        // Clear the lastAttackTimes
        if (AuraModeSetting.SelectedIndex == 2) LastAttackTimes.clear();
    }

    HasTarget = foundAttackable;

    return result;
}

void Aura::RotateToEntity(Actor* actor){
    if (RotateMode.SelectedIndex == 0) return;
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    /*Vector3 pos = Misc::GetClosestAabbPos(actor, player->getPos());
    if (pos.Distance(*player->getPos()) < 2.2f) pos = Misc::GetLerpedActorPos(actor) + Vector3(0.f, 1.62f, 0.f);
    Vector2 rotations = Misc::GetRotations(player->getPos(), &pos);
    rots = rotations;
    rotate = true;*/

    Vector3 pos = *actor->getPos();
    Vector3 posOld = *actor->getPosPrev();
    Vector3 predPos = pos;
    predPos.y -= 1.62f;
    AABB aabb = actor->GetAABBForPos(pos);
    LastTargetedAABB = aabb;
    rotate = true;
}

bool isValidEntity(Actor* actor)
{
    // search for the actor in Antibot::GetActors()
    auto actors = Antibot::GetActors();
    for (auto a : actors)
    {
        if (a == actor) return true;
    }
    return false;
}

float Aura::GetDistanceFromClosestAABBPoint(Actor* actor, Vector3* playerPos){
    Vector3 closestPos = Misc::GetClosestAabbPos(actor, playerPos);

    return closestPos.Distance(*playerPos);
}


void Aura::OnPacket(class LoopbackPacketSender* lbps, Packet* packet, bool* canceled) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (packet->getId() == (int)PacketID::InventoryTransaction && FireSwordSpoof.Enabled)
    {
        auto it = (InventoryTransactionPacket*)packet;
        if (it->mTransaction->type == ComplexInventoryTransaction::Type::ItemUseTransaction)
        {
            ItemUseInventoryTransaction* transac = reinterpret_cast<ItemUseInventoryTransaction*>(it->mTransaction.get());
            if (transac->actionType == ItemUseInventoryTransaction::ActionType::Place)
            {
                Aura::LastBlockPlace = Misc::GetCurrentMs();
            }
        }
    }

    if (RotateMode.SelectedIndex == 2 && !rotateFlick) return;

    AABB aabb;

    if (!LastTargetedActor && !isValidEntity(LastTargetedActor)) return;

    if (LastTargetedActor) {
        Vector3 pos = *LastTargetedActor->getPos();
        Vector3 posOld = *LastTargetedActor->getPosPrev();
        Vector3 posDelta = pos - posOld;

        Vector3 predPos = pos;
        float lastPing = RakPeerHooks::LastPing;

        if (lastPing > 0) {
            predPos += posDelta * (lastPing / 50.f);
        }

        predPos.y -= 1.62f;

        aabb = LastTargetedActor->GetAABBForPos(predPos);
    } else {
        aabb = LastTargetedAABB;
    }



    Vector2 rots = Misc::GetRotations(*player->getPos(), aabb);

    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto* paip = (PlayerAuthInputPacket*)packet;

        if (RotateMode.SelectedIndex != 0 && rotate || RotateMode.SelectedIndex == 2 && rotateFlick) {
            paip->rotation = rots;
            paip->headYaw = paip->rotation.y;

            if (RotateMode.SelectedIndex == 2)
            {
                rotate = false;
                rotateFlick = false;
            }
        }

        paip->headYaw = paip->rotation.y;
    }
    if (packet->getId() == (int)PacketID::MovePlayer) {
        auto* mpp = (MovePlayerPacket*) packet;

        if (RotateMode.SelectedIndex != 0 && rotate || RotateMode.SelectedIndex == 2 && rotateFlick) {
            mpp->rotation = rots;
            mpp->headYaw = mpp->rotation.y;
        }

        mpp->headYaw = mpp->rotation.y;
    }

    if (packet->getId() == (int)PacketID::MobEquipment) {
        auto mep = (MobEquipmentPacket*)packet;
        SpoofedSlot = mep->HotbarSlot;
        auto player = Minecraft::ClientInstance->ThePlayer();
    }
}

void Aura::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::MobEquipment) {
        auto mep = std::reinterpret_pointer_cast<MobEquipmentPacket>(packet);
        Aura::SpoofedSlot = mep->HotbarSlot;
        auto player = Minecraft::ClientInstance->ThePlayer();
    }

    if (packet->getId() == (int)PacketID::RemoveEntity) {
        // If the targeted actor was removed, clear the LastTargetedActor
        auto rem = std::reinterpret_pointer_cast<RemoveActorPacket>(packet);

        uint64_t runtimeId = rem->ActorRuntimeID;
        if (LastTargetedActor && LastTargetedActor->GetRuntimeID() == runtimeId) {
            LastTargetedActor = nullptr;
            HasTarget = false;
        }

    }
}



void Aura::OnRender(){
    static auto scaffoldModule = ModuleManager::GetModule<Scaffold>();

    if (rotate && !ServerSide.Enabled && !scaffoldModule->Enabled) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        auto rotComponent = player->GetActorRotationComponent();
        if (!rotComponent) return;
        auto headRotComponent = player->GetActorHeadRotationComponent();
        if (!headRotComponent) return;

        Vector2 rots = Misc::GetRotations(*player->getPos(), LastTargetedAABB);

        rotComponent->pitch = rots.x;
        rotComponent->oldPitch = rots.x;
        rotComponent->yaw = rots.y;
        rotComponent->oldYaw = rots.y;
        headRotComponent->headRot = rots.x;
        headRotComponent->oldHeadRot = rots.x;
    }

    if (!Minecraft::ClientInstance->levelRenderer->levelRendererPlayer)
    {
        LastTargetedActor = nullptr;
        return;
    }

    if (Visualize.Enabled)
    {
        // render a circle around the target
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        auto actor = LastTargetedActor;
        if (!actor || !actor->isPlayer())
        {
            return;
        }
        // Make the pos move up and down (min = realPos - height, max = realPos)
        float height = actor->GetAABBShapeComponent()->hitboxHeight;
        Vector3 realPos = *actor->GetRenderPositionComponent();
        realPos.y = realPos.y - 1.62f;
        realPos.y += height;
        static Vector3 oldPos = realPos;
        Vector3 pos = realPos;
        pos = Math::Lerp(oldPos, pos, ImGui::GetIO().DeltaTime * 40.f);




        AABB aabb;
        {
            Vector3 pos = *actor->GetRenderPositionComponent() - Vector3(0.f, 1.62f, 0.f);
            if (!actor->isPlayer())
            {
                pos = *actor->GetRenderPositionComponent();
            }

            if (actor == player){
                Vector3 pos2 = actor->GetStateVectorComponent()->pos - Vector3(0.f, 1.62f, 0.f);
                Vector3 posOld = actor->GetStateVectorComponent()->posOld - Vector3(0.f, 1.62f, 0.f);
                if (!actor->isPlayer())
                {
                    pos2 = actor->GetStateVectorComponent()->pos;
                    posOld = actor->GetStateVectorComponent()->posOld;
                }

                pos = posOld + (pos2 - posOld) * Minecraft::ClientInstance->GetDeltaTime();
            }

            float hitboxWidth = actor->GetAABBShapeComponent()->hitboxWidth;
            float hitboxHeight = actor->GetAABBShapeComponent()->hitboxHeight;
            // since the pos is in the center of the hitbox
            Vector3 aabbMin = Vector3(pos.x - hitboxWidth / 2, pos.y, pos.z - hitboxWidth / 2);
            Vector3 aabbMax = Vector3(pos.x + hitboxWidth / 2, pos.y + hitboxHeight, pos.z + hitboxWidth / 2);

            // Slightly increase the size of the AABB so it matches the actual hitbox
            aabbMin = aabbMin - Vector3(0.1f, 0.1f, 0.1f);
            aabbMax = aabbMax + Vector3(0.1f, 0.1f, 0.1f);


            aabb = AABB(aabbMin, aabbMax, true);
        }

        Vector3 bottomOfHitbox = aabb.lower;
        Vector3 topOfHitbox = aabb.upper;
        // make the x and z the same as pos for both of these
        bottomOfHitbox.x = pos.x;
        bottomOfHitbox.z = pos.z;
        topOfHitbox.x = pos.x;
        topOfHitbox.z = pos.z;
        topOfHitbox.y += 0.1f;

        static float pct = 0.f;
        static bool reversed = false;
        static uint64_t lastTime = Misc::GetCurrentMs();

        float speed = VisualizeSpeed.Value;
        uint64_t visualTime = 800 / (speed - 0.2);

        if (Misc::GetCurrentMs() - lastTime > visualTime) {
            reversed = !reversed;
            lastTime = Misc::GetCurrentMs();
            // depending on the reversed bool, set the pct to 0 or 1
            pct = reversed ? 1.f : 0.f;
        }

        pct += !reversed ? (speed * ImGui::GetIO().DeltaTime) : -(speed * ImGui::GetIO().DeltaTime);
        pct = Math::Lerp(0.f, 1.f, pct);
        oldPos = pos;
        pos = bottomOfHitbox.Lerp(topOfHitbox, Math::EaseInOutExpo(pct));

        auto corrected = Render::Transform.modelView;
        // W2S the renderPos

        float pointCount = VisualizePoints.Value;
        Vector2 screenPos1 = {0, 0};
        Vector3 hatTip = {pos.x, pos.y, pos.z};


        if (!corrected.OWorldToScreen(Render::Transform.origin,
                                       hatTip, screenPos1, Math::Fov,
                                       Minecraft::ClientInstance->guiData->resolution))
            return;

        float radius = 0.9f;


        Vector2 screenPos2 = {0, 0};
        Vector2 screenPos3 = {0, 0};
        static float angleOffset = 0.f;
        angleOffset += (VisualizeSpin.Value * 30.f) * ImGui::GetIO().DeltaTime;
        auto rotComponent = actor->GetActorRotationComponent();
        Vector2 rotations = {rotComponent->pitch, rotComponent->yaw};
        Vector2 rotations2 = {rotComponent->oldPitch, rotComponent->oldYaw};
        rotations = Math::Lerp(rotations2, rotations, Minecraft::ClientInstance->GetDeltaTime());
        rotations.y = Math::Wrap(rotations.y, -180, 180);
        if (VisualizeSpin.Value == 0.f) angleOffset = rotations.y;


        ImColor rainbowColor = ColorUtil::GetDefaultRainbow(0, ColorUtil::GlobalRGBInfo.Time, ColorUtil::GlobalRGBInfo.Saturation, ColorUtil::GlobalRGBInfo.Value, 10);
        if (VisualizeRainbow.Enabled) {
            rainbowColor = HUD::GetColor(0);
        } else {
            rainbowColor = ImColor(1.f, 1.f, 1.f, 0.5f);
        }
        ImColor color = ImColor(rainbowColor.Value.x, rainbowColor.Value.y, rainbowColor.Value.z, 0.5f);

        for (int i = 0; i <= pointCount; i++) {
            float angle = (i / (float) pointCount) * 360.f;
            // Make it spin
            angle += angleOffset;
            angle = Math::Wrap(angle, -180.f, 180.f);

            float rad = Math::ToRadians(angle);

            float x = pos.x + radius * cosf(rad);
            float y = pos.y;
            float z = pos.z + radius * sinf(rad);

            Vector3 thisPos = {x, y, z};

            if (!corrected.OWorldToScreen(
                    Render::Transform.origin,
                    thisPos, screenPos3, Math::Fov,
                    Minecraft::ClientInstance->guiData->resolution))
                continue;

            if (i > 0 && (screenPos2.x != 0 && screenPos2.y != 0)) {

                ImGui::GetBackgroundDrawList()->AddTriangleFilled(
                        ImVec2(screenPos1.x, screenPos1.y),
                        ImVec2(screenPos2.x, screenPos2.y),
                        ImVec2(screenPos3.x, screenPos3.y),
                        color);

                ImVec2 points[3] = {screenPos1,
                                    screenPos2,
                                    screenPos3};



                ImGui::GetBackgroundDrawList()->AddShadowConvexPoly(points, 3, color, 80.0f,
                                                                    ImVec2(0.0f, 0.0f),
                                                                    // Make sure only the outline of the shadow is visible
                                                                    ImDrawFlags_RoundCornersAll);

                // Draw another one underneath so the lines are not visible from the bottom
                ImGui::GetBackgroundDrawList()->AddTriangleFilled(
                        ImVec2(screenPos1.x, screenPos1.y),
                        ImVec2(screenPos2.x, screenPos2.y),
                        ImVec2(screenPos3.x, screenPos3.y),
                        ImColor(1.f, 1.f, 1.f, 0.5f));


            }


            corrected.OWorldToScreen(Render::Transform.origin,
                                      thisPos, screenPos2, Math::Fov,
                                      Minecraft::ClientInstance->guiData->resolution);
        }



    }
}















void Aura::OnDisable() {
    Minecraft::ClientInstance->SetTimer(20.f);
    HasTarget = false;
}



























Actor* Aura::CheckForObstructingActor(Actor* ogTarget, Vector3 *pos) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return nullptr;

    std::vector<Actor*> actors = player->level->getRuntimeActorList();

    for (auto actor : actors) {
        if (actor == player) continue;
        if (actor == ogTarget) continue;
        float distanceToOgTarget = actor->getPos()->Distance(*ogTarget->getPos());
        if (actor->getPos()->Distance(*player->getPos()) > 10) continue;

        std::string id = actor->GetEntityId();
        if (id == "hivecommon:shadow" && distanceToOgTarget < 3)
        {
            return actor;
        }

        auto antiinvis = ModuleManager::GetModule<AntiInvis>();
        if (!actor->isInvisible() && !antiinvis->Enabled) continue;
        if (id != "minecraft:pig" && antiinvis->Enabled) continue;

        // Check if the actor is in between the player and the target by checking the players rotation
        Vector3 playerPos = *player->GetRenderPositionComponent();
        Vector3 actorPos = *actor->GetRenderPositionComponent();
        Vector3 ogTargetPos = *ogTarget->GetRenderPositionComponent();
        Vector2 rotations = Misc::GetRotations(&playerPos, &actorPos);
        Vector2 targetRotations = Misc::GetRotations(&playerPos, &ogTargetPos);
        float yawDelta = abs(rotations.y - targetRotations.y);
        // make sure this yawDelta accounts for the yaw range (-180, 180)
        if (yawDelta > 180) yawDelta = 360 - yawDelta;


        if (yawDelta < 20)
        {
            return actor;
        }
    }
    return nullptr;
}
