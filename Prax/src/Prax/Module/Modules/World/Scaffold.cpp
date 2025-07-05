//
// Created by vastrakai on 1/5/2024.
//

#include "Scaffold.h"

#include <imgui_internal.h>
#include <Prax/Hook/Hooks/PacketSender.h>
#include <Prax/SDK/Game/Network/Other/MobEquipmentPacket.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/Module/Modules/Combat/Aura.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/SDK/Game/Network/Other/UpdateBlockPacket.h>
#include <Prax/Hook/Hooks/RakPeerHooks.h>
#include <Prax/Module/Modules/Visual/HUD.h>
#include <Prax/UI/Notifications.h>


float Scaffold::BlockAmountAnimPerc = 0.f;
float Scaffold::currentExtend = 0.f;
float Scaffold::startY = 0.f;
Vector3 Scaffold::LastBlock = {0, 0, 0};
int Scaffold::LastFace = 0;
bool Scaffold::ShouldRotate = false;
int Scaffold::LastSlot = -1;
bool Scaffold::HasTowered = false;
bool Scaffold::oldSpace = false;
bool Scaffold::IsDiaging = false;
uint64_t Scaffold::LastSwitchTime = 0; // Get MS with Misc::GetCurrentMs()
std::map<Vector3, uint64_t> Scaffold::recentlyUpdatedBlocks = {};
std::map<Vector3, uint64_t> Scaffold::ghostedBlocks = {};
std::map<Vector3, uint64_t> Scaffold::recentlyPlacedBlocks = {};
bool Scaffold::HasTouchedGround = false;

#define NOW Misc::GetCurrentMs()



Vector3 Scaffold::getRotationBasedPos(float extend, float yPos, float yawOffset = 0.f)
{
    auto player = Minecraft::ClientInstance->ThePlayer(); // no need to check for null
    Vector3 *playerPos = player->getPos();
    BlockPos blockSel = {INT_MAX, INT_MAX, INT_MAX};


    Vector2 playerRots = Vector2(player->GetActorRotationComponent()->pitch,
                                 player->GetActorRotationComponent()->yaw + Misc::GetRotationKeyOffset());

    playerRots.y += yawOffset;

    if (PlacementMode == Flareon) {
        playerRots.y = Math::SnapYaw(playerRots.y);
    }

    float correctedYaw = (playerRots.y + 90) * ((float) IM_PI / 180);
    float inFrontX = cos(correctedYaw) * extend;
    float inFrontZ = sin(correctedYaw) * extend;
    float placeX = playerPos->x + inFrontX;
    float placeY = yPos;
    float placeZ = playerPos->z + inFrontZ;

    blockSel.x = floor(placeX);
    blockSel.y = floor(placeY);
    blockSel.z = floor(placeZ);

    return blockSel;
};

int lastSpoofedSlot = -1;

void Scaffold::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    // go thru the recently updated blocks and remove the ones that are too old and or not air
    for (auto it = recentlyUpdatedBlocks.begin(); it != recentlyUpdatedBlocks.end();)
    {
        // Remove the block if its not found in the recently placed blocks
        if (it->second < NOW - 1000 || !Misc::IsAirBlock(it->first))
        {
            it = recentlyUpdatedBlocks.erase(it);
        }
        else if (recentlyPlacedBlocks.contains(it->first))
        {
            it = recentlyUpdatedBlocks.erase(it);
        }
        else
        {
            it++;
        }
    }

    // go thru and remove the recently placed blocks that are too old
    for (auto it = recentlyPlacedBlocks.begin(); it != recentlyPlacedBlocks.end();)
    {
        if (it->second < Misc::GetCurrentMs() - 1000)
        {
            it = recentlyPlacedBlocks.erase(it);
        }
        else
        {
            it++;
        }
    }



    ghostedBlocks = recentlyUpdatedBlocks; // copy the recently updated blocks to the ghosted blocks (so we dont end up having any stupid blocks here for a tick)

    static uint64_t lastGhostWarn = 0;
    if (ghostedBlocks.size() > 2 && NOW - lastGhostWarn > 1000) {
        lastGhostWarn = NOW;
        Notifications::Notify("Scaffold", "Ghost blocks detected!", NotificationType::Warning, 0.75);
        // Play a low pitch note to incicate that there are ghosted blocks
        Minecraft::ClientInstance->playUI("note.pling", 0.8f, 0.5f);
    }

    bool fakeSpoof = SelectionMode.SelectedIndex == Serverhold;

    static bool lastEnabledState = false;

    if (lastEnabledState != Enabled && fakeSpoof && !Enabled) {
        Logger::Write("Scaffold", "Disabling serverhold", Logger::LogType::Debug);
        // If we are disabling and we are using serverhold then we need to spoof the slot
        auto player = Minecraft::ClientInstance->ThePlayer();

        if (player->supplies->selectedHotbarSlot != lastSpoofedSlot && lastSpoofedSlot != -1) {
            Logger::Write("Scaffold", "Spoofing slot back to " + std::to_string(player->supplies->selectedHotbarSlot), Logger::LogType::Debug);
            Misc::SpoofSwitch(player->supplies->selectedHotbarSlot);
        }


        lastEnabledState = Enabled;
        return;
    }
    if (!Enabled) return;

    if (player->isOnGround()) HasTouchedGround = true;

#ifdef __DEBUG__
    if (Test.Enabled && TestDesync.Enabled && !HasTouchedGround)
    {
        Logger::Write("Scaffold", "Waiting for player to touch ground", Logger::LogType::Debug);
        return;
    }
#endif

    lastEnabledState = Enabled;

    static int tick = 0;

    tick++;

    if (tick > TickGap.Value) {
        tick = 0;
    } else if (!HasTowered) {
        return;
    }

    // call Tick() function x amount of times (indicated by PlacesPerTick)
    bool placedBlock = false;
    float places = PlacesPerTick.Value;

    Vector3 pos = getRotationBasedPos(0.f, startY);
    Vector3 extendedPos = getRotationBasedPos(3.f, startY);

    IsDiaging = pos.x != extendedPos.x && pos.z != extendedPos.z && Misc::IsUsingMovementKeys();

    if (UseDiagSettings.Enabled)
    {
#ifndef __DEBUG__
        if (IsDiaging) places = DiagPlacesPerTick.Value;
#else
        if (IsDiaging && !Test.Enabled) places = DiagPlacesPerTick.Value;
        else if (Test.Enabled && IsDiaging && !player->isOnGround()) places = PlacesPerTick.Value;
#endif
    }

    if (InstantClutch.Enabled) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (player->GetFallDistance() >= 3.5f) {
            places = ClutchSpeed.Value;
            startY = player->getPos()->y - 3.f;
        }
    }

    if (places >= 1.f)
    {
        for (int i = 0; i < places; i++) {
            placedBlock = Tick();
            if (!placedBlock) break;
        }
    } else
    {
        uint64_t msPerPlace = (uintptr_t) (50 * (1.f / places));
        // if we are towering set the msPerPlace to 0
        if (HasTowered) msPerPlace = 0;
        if (NOW - LastSwitchTime > msPerPlace) {
            placedBlock = Tick();
        }
    }

    if (!placedBlock) {
        if (SwitchBack.Enabled && LastSlot != -1) {
            if (NOW - LastSwitchTime > SwitchBackDelay.Value * 50) {
                if (fakeSpoof) Misc::SpoofSwitch(LastSlot);
                player->supplies->selectedHotbarSlot = LastSlot;
                LastSlot = -1;
            }
        }
    }

    if(fakeSpoof && LastSlot != -1 && player->supplies->selectedHotbarSlot != LastSlot){
        player->supplies->selectedHotbarSlot = LastSlot;
    }


}


bool Scaffold::Tick()
{
    bool fakeSpoof = SelectionMode.SelectedIndex == Serverhold;
    bool fullSwitch = SwitchPriority.SelectedIndex == FullSwitch || SwitchPriority.SelectedIndex == Serverhold;

    bool hasPlacedBlock = false;
    auto player = Minecraft::ClientInstance->ThePlayer(); // no need to check for null
    player->GetMoveInputComponent()->isSneakDown = false;

    // Check if the player is holding a placeable block

    auto blockSource = Minecraft::ClientInstance->GetBlockSource();
    auto currentY = player->getPos()->y - 2.62f;
    if (!LockY.Enabled) startY = player->getPos()->y - 2.62f;
    if (player->getPos()->y - 2.62f < startY) startY = player->getPos()->y - 2.62f;
    float yaw = player->GetActorRotationComponent()->yaw + Misc::GetRotationKeyOffset() + 90.f;
    HasTowered = false;
    Vector3 velocity = player->GetStateVectorComponent()->velocity;

    float maxExtend = ExtendSetting.Value;
    float extendIncrease = ExtendSpeed.Value / 10.f;

    // If they have no blocks do nothing
    if (player->supplies->GetAllPlaceableBlocks(HotbarOnly.Enabled) == 0) {
        // If HasTowered is true then set reset velocity and set timer to 20
        if (HasTowered) {
            Minecraft::ClientInstance->SetTimer(20.f);
            player->GetStateVectorComponent()->velocity.y = -5.f;
            HasTowered = false;
        }

        return false;
    }

    static uint64_t lastClip = 0;

    bool clipNow = false;

    static float easedVel = TowerSpeed.Value / 10.f;

    if (!Misc::IsUsingMovementKeys() && velocity.x < 0.1f && velocity.z < 0.1f)
    {
        switch ((enum TowerMode)TowerMode.SelectedIndex)
        {
        case TowerMode::Vanilla:
            break;
        case TowerMode::Timer: {
                bool space_ = KeyManager::Keys[VK_SPACE];
                if (space_ && player->GetFallDistance() <= 0.25f) {
                    HasTowered = true;
                    Minecraft::ClientInstance->SetTimer(TowerSpeed.Value * 10.f);
                } else {
                    Minecraft::ClientInstance->SetTimer(20.f);
                }

                startY = player->getPos()->y - 2.62f;
                maxExtend = 0.f;

                oldSpace = space_;
                break;
        }

        case TowerMode::Velocity: {
                float speed = TowerSpeed.Value / 10.f;
                bool space = KeyManager::Keys[VK_SPACE];
                if (space) {
                    Minecraft::ClientInstance->SetTimerMultiplier(1);
                    HasTowered = true;
                    player->GetStateVectorComponent()->velocity.y = speed;
                } else if (oldSpace) {
                    player->GetStateVectorComponent()->velocity.y = -5.f;
                }

                oldSpace = space;

                startY = player->getPos()->y - 2.62f;
                maxExtend = 0.f;

                oldSpace = space;
                break;
            }

        case TowerMode::Smooth: {
            // Smooth tower.
            // Smoothly goes up and then applies down motion when a block is placed
            // This is to prevent the player from taking fall damage
            float speed = TowerSpeed.Value / 10.f;

            bool space = KeyManager::Keys[VK_SPACE];
            if (space) {
                Minecraft::ClientInstance->SetTimerMultiplier(1);
                player->GetMoveInputComponent()->isJumping = false;
                HasTowered = true;
                // ease down to 0. if the value is 0 then reset it to the speed
                if (easedVel == 0.f) easedVel = speed;
                easedVel *= TowerSmoothing.Value;
                player->GetStateVectorComponent()->velocity.y = easedVel;
            } else if (oldSpace) {
                player->GetStateVectorComponent()->velocity.y = -5.f;
                easedVel = 0.f;
            }

            oldSpace = space;

            startY = player->getPos()->y - 2.60f;
            maxExtend = 0.f;

            oldSpace = space;


        }
        case TowerMode::ClipVel:
            {
                uint64_t msPerClip = TowerSpeed.Value * 10;
                float speed = TowerSpeed.Value / 10.f;
                bool space = KeyManager::Keys[VK_SPACE];
                if (space) {
                    maxExtend = 0.f;
                    if (NOW - lastClip > msPerClip)
                    {
                        startY = player->getPos()->y - 2.62f;
                        HasTowered = true;
                        clipNow = true;
                    }

                    player->GetStateVectorComponent()->velocity.y = speed;
                } else if (oldSpace) {
                    player->GetStateVectorComponent()->velocity.y = -5.f;
                    Minecraft::ClientInstance->SetTimer(20.f);
                }

                maxExtend = 0.f;
                oldSpace = space;
                break;
            }
        case TowerMode::Clip:
            {
                uint64_t msPerClip = TowerSpeed.Value * 10;
                bool space = KeyManager::Keys[VK_SPACE];
                if (space) {
                    maxExtend = 0.f;
                    if (NOW - lastClip > msPerClip)
                    {
                        startY = player->getPos()->y - 2.62f;
                        HasTowered = true;
                        clipNow = true;
                    }
                }
                
                maxExtend = 0.f;
                oldSpace = space;
                break;
            }

        }
    }

    //if(HasTowered) {
    //    // Set x and z velocity to 0
    //    player->GetStateVectorComponent()->velocity.x = 0.f;
    //    player->GetStateVectorComponent()->velocity.z = 0.f;
    //}

    if (HasTowered) {
        IsDiaging = false;
    }

    if (!HasTowered && oldSpace && Misc::IsUsingMovementKeys())
    {
        player->GetStateVectorComponent()->velocity.y = -5.f;
        Minecraft::ClientInstance->SetTimer(20.f);
        oldSpace = false;
    }

    if (IsDiaging && UseDiagSettings.Enabled) {
        yaw += DiagYawOffset.Value;
        maxExtend = DiagExtend.Value;
    }

    if (PlacementMode == Flareon)
    {
        yaw = Math::SnapYaw(yaw);
    }

    Vector3 blockPos = GetBlockPlacePos(yaw, 0.f);

    if (!Misc::IsAirBlock(blockPos))
    {
        for (float i = 0.f; i < maxExtend; i += extendIncrease)
        {
            blockPos = GetBlockPlacePos(yaw, i);
            if (Misc::IsAirBlock(blockPos))
            {
                currentExtend = i;
                break;
            }
        }
    }

    if (!IsDiaging && Andromeda.Enabled && Misc::IsUsingMovementKeys())
    {
        // If the current block is not air, then offset ypos by 2 and check if that block is air
        if (!Misc::IsAirBlock(blockPos))
        {
            blockPos = getRotationBasedPos(0.f, currentY + 3.f);
            if (Misc::IsAirBlock(blockPos) && Misc::IsValidPlacePos(blockPos))
            {
                currentExtend = 0.f;
                startY = currentY + 3.f;
            }
        }

        if (!Misc::IsAirBlock(blockPos))
        {
            // do extend
            for (float i = 0.f; i < maxExtend; i += extendIncrease)
            {
                blockPos = GetBlockPlacePos(yaw, i, currentY + 3.f);
                if (Misc::IsAirBlock(blockPos))
                {
                    currentExtend = i;
                    break;
                }
            }
        }
    }

    // check if the position is valid
    if (!Misc::IsValidPlacePos(blockPos)) {
        return false;
    }

    BlockPos finalBlockPos = blockPos;
    int side = Misc::GetBlockPlaceFace(blockPos);

    // Select slot if applicable
    auto slot = Misc::GetPlaceableItemOnBlock(finalBlockPos, HotbarOnly.Enabled, SwitchPriority.SelectedIndex == 1);

    if (fullSwitch) {
        if (slot == -1) return false;
        if (LastSlot == -1) LastSlot = player->supplies->selectedHotbarSlot;

        if (fakeSpoof && LastSlot != slot) {
            Misc::SpoofSwitch(slot);
        }
        player->supplies->selectedHotbarSlot = slot;
        LastSwitchTime = NOW;
    }

    LastSwitchTime = NOW;

    // Set place info
    LastBlock = finalBlockPos;
    LastFace = side;
    ShouldRotate = true;

    static uint64_t lastDownTime = 0;

    if (NOW - lastDownTime > DownDelay.Value && KeyManager::Keys[VK_SHIFT]) {
        lastDownTime = NOW;

        // Return if the face is 0 (bottom) and we are higher than the start Y then return
        startY = currentY - 0.5f;

        if (side == 0 && currentY - 0.4f > startY) {
            return false;
        }
    }

    bool space = KeyManager::Keys[VK_SPACE];

    if (space && oldSpace && clipNow && (TowerMode == Clip || TowerMode == ClipVel))
    {
        player->setPosition(*player->getPos() + Vector3(0.f, 1.f, 0.f));
        HasTowered = true;
        startY = currentY;
        startY = floor(startY);
        lastClip = NOW;
    }

    if (side == 0 && currentY - 0.4f > startY && KeyManager::Keys[VK_SHIFT]) {
        return false;
    }

    Misc::PlaceBlock(finalBlockPos, side, Swing.Enabled, false, true, player->supplies->selectedHotbarSlot, 0);
    if (AuthMode == ServerSide)
    {
        Minecraft::ClientInstance->GetBlockSource()->clearBlock(finalBlockPos);
    }

    recentlyPlacedBlocks[finalBlockPos] = NOW;

    if (Underplace.Enabled && IsDiaging) {
        BlockPos underplace = getRotationBasedPos(0.f, currentY - 1.f);

        if (Misc::IsAirBlock(underplace)) {
            Misc::PlaceBlock(underplace, 0, Swing.Enabled, true, true, player->supplies->selectedHotbarSlot, 0);
            if (AuthMode == ServerSide)
            {
                Minecraft::ClientInstance->GetBlockSource()->clearBlock(underplace);
            }
            recentlyPlacedBlocks[underplace] = NOW;
        }
    }

    if (HasTowered && TowerMode.SelectedIndex == Smooth) {
        player->GetStateVectorComponent()->velocity.y = -10.f;
    }

    easedVel = TowerSpeed.Value / 10.f;

    return true;

}


void Scaffold::OnEnable()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    startY = player->getPos()->y - 2.62f;

    if (player->isOnGround()) HasTouchedGround = true;

}

static std::vector<std::shared_ptr<PlayerAuthInputPacket>> qpackets = {};

void Scaffold::OnDisable()
{
    HasTouchedGround = false;
    auto player = Minecraft::ClientInstance->ThePlayer();

#ifdef __DEBUG__
    if (Test.Enabled && TestDesync.Enabled && !qpackets.empty())
    {
        for (auto p : qpackets)
        {
            // log the ticksAlive
            bool t = false;
            PacketSender::HandleAuthInputTick(p.get(), &t);
            Minecraft::ClientInstance->packetSender->sendToServer(p.get());
        }
        qpackets.clear();
    }
#endif

    currentExtend = 0.f;
    startY = 0.f;
    LastBlock = {0, 0, 0};
    ShouldRotate = false;
    if (LastSlot != -1) {
        if (player) player->supplies->selectedHotbarSlot = LastSlot;
        LastSlot = -1;
    }

    if (HasTowered) {
        Minecraft::ClientInstance->SetTimer(20.f);
        Minecraft::ClientInstance->SetTimerMultiplier(1);
    }



    if (player && oldSpace && !Misc::IsUsingMovementKeys())
    {
        player->GetStateVectorComponent()->velocity.y = -5.f;
        oldSpace = false;
    }

    // clear all maps
    recentlyUpdatedBlocks.clear();
    ghostedBlocks.clear();
    recentlyPlacedBlocks.clear();

}

void Scaffold::OnRender() {
    if (!Minecraft::ClientInstance->ThePlayer()) return;

    for (auto it = ghostedBlocks.begin(); it != ghostedBlocks.end();)
    {
        Vector3 pos = it->first;
        uint64_t time = it->second;

        ImColor color = ImColor(1.f, 1.f, 0.f, 1.f);

        // fade the alpha to 0 from 0.4 based on time
        color.Value.w = 0.4f - (float)(Misc::GetCurrentMs() - time) / 1000.f;

        Render::DrawBoxAroundBlock(pos, color, color);

        it++;
    }

    if (BlockHudStyle.SelectedIndex == Original) RenderOgBlockAmountDisplay();
    else if (BlockHudStyle.SelectedIndex == New) RenderNewBlockAmountDisplay();
}

void Scaffold::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::UpdateBlock) {
        auto updateBlockPacket = std::reinterpret_pointer_cast<UpdateBlockPacket>(packet);
        auto blockPos = updateBlockPacket->Pos;

        // Check if this block is air and continue if it is
        if (Misc::IsAirBlock(blockPos)) return;

        // Check if this block was recently placed at and return if not
        if (!recentlyPlacedBlocks.contains(blockPos)) return;

        // Add da block to the recently updated blocks
        recentlyUpdatedBlocks[blockPos] = NOW;

    }
}

void Scaffold::OnPacket(LoopbackPacketSender* sender, Packet* packet, bool* cancel)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

#ifdef __DEBUG__
    if (Test.Enabled && TestDesync.Enabled)
    {
        if (packet->getId() == (int)PacketID::NetworkStackLatency)
        {
            *cancel = true;
        }
    }
#endif

    if (packet->getId() == (int)PacketID::MobEquipment) {
        auto me = (MobEquipmentPacket*)packet;
        if (me->RuntimeId == player->GetRuntimeID()) {
            lastSpoofedSlot = me->HotbarSlot;
        }
    }

    if (packet->getId() == (int)PacketID::InventoryTransaction && FlareonV2Placement.Enabled)
    {
        auto it = (InventoryTransactionPacket*)packet;
        if (it->mTransaction->type == ComplexInventoryTransaction::Type::ItemUseTransaction)
        {
            ItemUseInventoryTransaction* transac = reinterpret_cast<ItemUseInventoryTransaction*>(it->mTransaction.get());
            if (transac->actionType == ItemUseInventoryTransaction::ActionType::Place)
                transac->clickPos = Vector3(0, 0, 0);
        }
    }

    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket*)packet;

        RotationMode mode = (RotationMode)RotateMode.SelectedIndex;

        if (ShouldRotate && mode != RotationMode::None)
        {
            //AABB aabb = AABB(LastBlock + Misc::SideMap[LastFace], Vector3(1.f, 1.f, 1.f));
            Vector3 side = Misc::SideMap[LastFace];
            Vector3 aabb = LastBlock + side;

            Vector2 rotations = Misc::GetRotations(player->getPos(), &aabb);

            if (mode == RotationMode::Down) rotations.x = 89.f;
            if (mode == RotationMode::Moonwalk)
            {
                // Reverse the yaw
                rotations.y += 180.f;
                if (rotations.y > 180.f) rotations.y -= 360.f;
                if (rotations.y < -180.f) rotations.y += 360.f;
            }

            if (mode == RotationMode::Pitch) {
                rotations.y = player->GetActorRotationComponent()->yaw +
                              Misc::GetRotationKeyOffset(true);
                rotations.y += DiagYawOffset.Value;
                if (rotations.y > 180.f) rotations.y -= 360.f;
                if (rotations.y < -180.f) rotations.y += 360.f;
                rotations.x = fmax(82, rotations.x);
            }

            bool flickRotate = false;

            // If aura is enabled and has a target, then flick rotate
            static Aura* aura = ModuleManager::GetModule<Aura>();

            if (aura && aura->Enabled && aura->HasTarget) flickRotate = true;

            if (flickRotate) ShouldRotate = false;
            else
            {
                // If the last block placed was more than 500ms ago, then ShouldRotate = false
                if (NOW - LastSwitchTime > 500) ShouldRotate = false;
            }

            paip->rotation = rotations;
            paip->headYaw = rotations.y;
        }


#ifdef __DEBUG__
        if (Test.Enabled && !HasTowered && TestDesync.Enabled || !TestDesync.Enabled && !KeyManager::Keys[VK_SPACE] && Test.Enabled)
        {
            paip->position.y = paip->position.y - 0.01f;

            if (TestDesync.Enabled)
            {
                if (!player->isOnGround())
                {
                    auto cloned = MinecraftPackets::CreatePacket<PlayerAuthInputPacket>();
                    cloned->CopyFrom(paip);
                    qpackets.push_back(cloned);
                    *cancel = true;
                } else
                {
                    // send all other queued packets
                    int index = 0;
                    for (auto p : qpackets)
                    {
                        // log the ticksAlive
                        bool t = false;
                        PacketSender::HandleAuthInputTick(p.get(), &t);
                        Minecraft::ClientInstance->packetSender->sendToServer(p.get());
                        index++;
                    }
                    qpackets.clear();
                }
            }
        }
#endif
    }
};


Vector3 Scaffold::GetBlockPlacePos(float yawDeg, float extend, float yPos, bool predict)
{
    if (yPos == FLT_MAX) yPos = startY;
    auto player = Minecraft::ClientInstance->ThePlayer(); // no need to check for null
    Vector3 *playerPos = player->getPos();

    Vector3 defaultRet = {FLT_MAX, FLT_MAX, FLT_MAX};
    BlockPos blockSel = {INT_MAX, INT_MAX, INT_MAX};

    float yawoffset = 0.f;

#ifndef __RELEASEFREE__
    if (IsDiaging && UseDiagSettings.Enabled) {
        yawoffset = DiagYawOffset.Value;
    }
#endif



    blockSel = getRotationBasedPos(extend, yPos, yawoffset);

    int side = Misc::GetBlockPlaceFace(blockSel);

    if (side == -1 && predict)
    {
        // find the closest block to place on
        blockSel = Misc::GetClosestPlacePos(blockSel, DistanceSetting.Value, false, yPos);
        if (blockSel.x == INT_MAX) return defaultRet;
        side = Misc::GetBlockPlaceFace(blockSel);

        if (side == -1) return defaultRet;
    }

    if (blockSel.x == INT_MAX) return defaultRet;

    Vector3 result = blockSel;

    return result;
}

void Scaffold::RenderOgBlockAmountDisplay() {
    float delta = ImGui::GetIO().DeltaTime;

    if (BlockAmountAnimPerc <= 0.001f && (!ShowGUI.Enabled || !Enabled)) {
        return;
    }

    BlockAmountAnimPerc = Math::Animate(BlockAmountAnimPerc, ShowGUI.Enabled && Enabled ? 1.f : 0.f,
                                                    delta * 12.f);
    BlockAmountAnimPerc = std::clamp(BlockAmountAnimPerc, 0.f, 1.f);

    int opacity, startY = 0.f;


    // Render GUI to show the amount of blocks we have
    ImVec2 aboveHotBarPos = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y * 0.75f);

    opacity = Math::Lerp(0, 255, BlockAmountAnimPerc);
    startY = Math::Lerp(0, 40, BlockAmountAnimPerc);


    aboveHotBarPos.y -= startY;

    ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);

    // Get the full amount of block in the whole inventory
    int totalBlocks =
            Minecraft::ClientInstance->ThePlayer()->supplies->GetAllPlaceableBlocks(HotbarOnly.Enabled);

    // Create Text:
    std::string text = "Blocks: " + std::to_string(totalBlocks);

    // Get the size of the text
    ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(25, FLT_MAX, 0.0f, text.c_str());

    // Decide color based on the amount of blocks
    ImColor textColor = ImColor(249, 98, 94,opacity);

    static ImColor oldColor = textColor;


    if (totalBlocks < 32) {
        // Red
        textColor = ImColor(249, 98, 94, opacity);
    } else if (totalBlocks < 64) {
        // Yellow
        textColor = ImColor(255, 229, 61, opacity);
    } else {
        // Green
        textColor = ImColor(61, 245, 1, opacity);
    }

    // Lerp the color
    textColor = ImColor(Math::Lerp(oldColor.Value.x, textColor.Value.x, delta * 5.f),
                        Math::Lerp(oldColor.Value.y, textColor.Value.y, delta * 5.f),
                        Math::Lerp(oldColor.Value.z, textColor.Value.z, delta * 5.f),
                        textColor.Value.w); // The alpha is already lerped

    oldColor = textColor;


    float rectLength = textSize.x + 10.f;

    aboveHotBarPos.x -= rectLength / 2; // Center

    // Render the text
    Render::RenderTextWithBG(text, aboveHotBarPos, 25, textColor,
                             ImColor(0, 0, 0, (int)(100 * BlockAmountAnimPerc)), true, 5.f);

    ImGui::PopFont();
}

float Scaffold::AnimationPerc = 0.f;

void Scaffold::RenderNewBlockAmountDisplay()
{
    float delta = ImGui::GetIO().DeltaTime;
    float lastAnimationPerc = AnimationPerc;

    ImVec2 pos = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y * 0.75f);

    int totalBlocks = Minecraft::ClientInstance->ThePlayer()->supplies->GetAllPlaceableBlocks(HotbarOnly.Enabled);

    std::string displayText = "Blocks: ";
    std::string text = displayText + std::to_string(totalBlocks);
    std::string numberText = std::to_string(totalBlocks);

    AnimationPerc = Math::Animate(AnimationPerc, ShowGUI.Enabled && Enabled ? 1.f : 0.f,
                                                    delta * 12.f);
    AnimationPerc = std::clamp(AnimationPerc, 0.f, 1.f);


    if (AnimationPerc <= 0.01f || text.empty()) return;

    ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);

    // Render the text
    float fontSize = 25.f;
    fontSize *= AnimationPerc;

    ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, displayText.c_str());
    float numberWidth = 13.f;

    textSize.x += numberText.size() * numberWidth * AnimationPerc;

    pos.x -= textSize.x / 2;
    pos.y -= textSize.y / 2;

    float rounding = 5.f;
    float bgAlpha = 0.45f;

    auto drawList = ImGui::GetBackgroundDrawList();

    drawList->AddShadowRect(ImVec2(pos.x - 5, pos.y - 5), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 5), ImColor(0.f, 0.f, 0.f, bgAlpha * AnimationPerc), 500, ImVec2(0, 0));
    drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 5), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 5), ImColor(0.f, 0.f, 0.f, (bgAlpha * AnimationPerc)), rounding, 0);

    textSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, displayText.c_str());


    //Render::RenderText(displayText, pos, fontSize, ImColor(color.Value.x, color.Value.y, color.Value.z, color.Value.w * AnimationPerc), true);
    // Draw each char seperately and get the color with ImColor color = HUD::GetColor(charIndex)
    ImColor color = ImColor(255, 255, 255, 255);

    for (int i = 0; i < displayText.size(); i++) {
        color = HUD::GetColor(i);
        ImVec2 ptextSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, std::string(1, displayText[i]).c_str());

        Render::RenderText(std::string(1, displayText[i]), pos, fontSize, ImColor(color.Value.x, color.Value.y, color.Value.z, color.Value.w * AnimationPerc), true);
        pos.x += ptextSize.x;
    }

    // Smoothly animate the number
    static std::vector<std::string> numbers = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    static std::string joinedNumbers = String::Join(numbers, "\n");

    int num = totalBlocks;
    std::string numStr = std::to_string(num);

    // Add a cliprect to the drawlist
    ImVec2 numTextSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, numStr.c_str());
    // force the x size to be 15.f per number
    numTextSize.x = 14.f * numStr.size() * AnimationPerc;

    ImVec2 clipRectMin = ImVec2(pos.x, pos.y);
    ImVec2 clipRectMax = ImVec2(pos.x + numTextSize.x + 5, pos.y + numTextSize.y);

    drawList->PushClipRect(clipRectMin, clipRectMax, true);

    // Render the number
    for (int i = 0; i < numStr.size(); i++) {
        // Calc text size
        std::string num = std::string(1, numStr[i]);
        int realNum = std::stoi(num);
        ImVec2 ptextSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, num.c_str());

        ptextSize.x = numberWidth * AnimationPerc;

        float offset = ptextSize.y * (realNum);
        // (i, offset) is the position of the number
        static std::map<int, float> indexOffsetMap;

        if (indexOffsetMap.find(i) == indexOffsetMap.end()) {
            indexOffsetMap[i] = offset;
        }

        // Scroll the number smoothly to the offset
        ImVec2 daPos = ImVec2((float)i * (float)ptextSize.x + pos.x, (float)-indexOffsetMap[i] + pos.y);

        color = HUD::GetColor(i + displayText.size());

        // Draw the number
        Render::RenderText(joinedNumbers, daPos, fontSize, ImColor(color.Value.x, color.Value.y, color.Value.z, color.Value.w * AnimationPerc), true);
        // we will cliprect this later

        // lerp da offset
        indexOffsetMap[i] = Math::Animate(indexOffsetMap[i], offset, ImGui::GetIO().DeltaTime * 10.f);
    }

    drawList->PopClipRect();

    ImGui::PopFont();
}

