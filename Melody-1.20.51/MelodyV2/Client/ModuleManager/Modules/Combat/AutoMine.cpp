// Created by Tony on 2024-10-10 10:01:54

#include "AutoMine.h"
bool MoOb = false;
AutoMine::AutoMine() : Module("AutoMine", "NULL", Category::PLAYER) {
    addSlider<float>("Range", "NULL", ValueType::FLOAT_T, &range, 3.f, 10.f);
    addBoolCheck("Countinue", "Still Mines that blocks", &countinue);
    addSlider<float>("Countinue Speed", "NULL", ValueType::FLOAT_T, &Cspeed, 0.01f, 1.f);
    addBoolCheck("Silent Switch", "Auto switch to the best pickaxe in hotbar silent", &silenSwitch);
    addBoolCheck("Silent Back", "Switch back to old hotbar after silent switch", &silentBack);
    addEnumSetting("Break Mode", "NULL", { "Auto", "Keybind" }, &breakMode);
    addKeybindSetting("Break keybind", "NULL", &breakKeybind);
    addBoolCheck("Mob", "for testing", &MoOb);
}

Vec2<float> AutoMine::GetRotations(Vec3<float> playerEyePos, Vec3<float> targetPos) {
    Vec3<float> delta = targetPos.sub(playerEyePos);
    float yaw = atan2(delta.z, delta.x) * 180.0f / M_PI;
    float pitch = atan2(delta.y, sqrt(delta.x * delta.x + delta.z * delta.z)) * 180.0f / M_PI;
    return { -pitch, yaw - 90 };
}

std::pair<int, float> AutoMine::getBestPickaxeSlot(Block* block) {
    GameMode* gm = mc.getGameMode();
    PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
    Inventory* inv = plrInv->inventory;

    int oldSlot = plrInv->selectedSlot;
    int bestSlot = plrInv->selectedSlot;
    float destroyRate = gm->getDestroyRate(block);

    for (int i = 0; i < 9; i++) {
        plrInv->selectedSlot = i;
        float value = gm->getDestroyRate(block);
        if (value > destroyRate) {
            destroyRate = value;
            bestSlot = i;
        }
    }

    plrInv->selectedSlot = oldSlot;
    if (block->blockLegacy->blockId != 0) {
        lastDestroyRate = destroyRate;
    }
    else {
        if (countinue) {
            destroyRate = lastDestroyRate;
        }
    }
    return std::make_pair(bestSlot, destroyRate);
}

void AutoMine::Reset() {
    static PacketMine* Packmod = (PacketMine*)client->moduleMgr->getModule("Packetmine");
    possrot = Vec3<float>(0, 0, 0);
    Packmod->setBreakPos(Vec3<int>(0, 0, 0), -1);
}
std::vector<Actor*> targetList;
void AutoMine::onNormalTick(Actor* actor) {
    targetList.clear();
    LocalPlayer* localPlayer = (LocalPlayer*)actor;
    GameMode* gm = localPlayer->getGameMode();
    auto* level = localPlayer->getLevel();

    // Handle silent switch back
    if (silenSwitch && shouldSwitchBack && lastSlot != -1) {
        MobEquipmentPacket pk(localPlayer->getRuntimeID(), localPlayer->getPlayerInventory()->inventory->getItemStack(lastSlot), lastSlot, lastSlot);
        mc.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
    }
    isbeingmine = false;
    shouldSwitchBack = false;

    // Get target position from AutoCrystal
    for (auto* target : level->getRuntimeActorList()) {
       
        if (TargetUtils::isTargetValid(target, MoOb) &&
            localPlayer->stateVectorComponent->pos.dist(target->stateVectorComponent->pos) < 5) {
            targetList.push_back(target);
            //formodnames.push_back(target);

        }
    }
    if (targetList.empty()) return;
    Vec3<float> targetPos = targetList[0]->stateVectorComponent->pos.floor();

    // Define block positions around the target
    std::vector<Vec3<float>> positions = {
        { targetPos.x + 1, targetPos.y, targetPos.z },
        { targetPos.x - 1, targetPos.y, targetPos.z },
        { targetPos.x, targetPos.y, targetPos.z + 1 },
        { targetPos.x, targetPos.y, targetPos.z - 1 }
    };

    for (auto& pos : positions) {
        if (breakPos.dist(pos) > range) {
            this->Reset();
            return;
        }

        Block* block = localPlayer->dimension->blockSource->getBlock(pos.toInt());
        if (!block || !block->blockLegacy) {
            this->Reset();
            return;
        }

        std::pair<int, float> bestSlot = getBestPickaxeSlot(block);
        if (gm->destroyProgress < 1.f) {
            isbeingmine = true;
            if (silenSwitch) gm->destroyProgress += bestSlot.second;
            else gm->destroyProgress += gm->getDestroyRate(block);
            if (gm->destroyProgress > 1.f) gm->destroyProgress = 1.f;
        }
        else {
            if (breakMode == 1) {
                if (breakKeybind == 0x0) return;
                if (!mc.isKeyDown(breakKeybind)) return;
            }
            int oldSlot = mc.getLocalPlayer()->getPlayerInventory()->selectedSlot;
            if (silenSwitch) {
                mc.getLocalPlayer()->getPlayerInventory()->selectedSlot = bestSlot.first;
                MobEquipmentPacket pk(localPlayer->getRuntimeID(), localPlayer->getPlayerInventory()->inventory->getItemStack(bestSlot.first), bestSlot.first, bestSlot.first);
                mc.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
            }

            bool oldIsDestroying = mc.getLocalPlayer()->isDestroying;
            mc.getLocalPlayer()->isDestroying = true;
            possrot = pos.toFloat();
            gm->destroyBlock(pos.toInt(), face);

            if (silenSwitch) {
                mc.getLocalPlayer()->getPlayerInventory()->selectedSlot = oldSlot;
                if (silentBack) {
                    shouldSwitchBack = true;
                    lastSlot = localPlayer->getPlayerInventory()->selectedSlot;
                }
            }
            mc.getLocalPlayer()->isDestroying = oldIsDestroying;

            if (countinue) {
                gm->destroyProgress = Cspeed;
            }
            else {
                this->Reset();
            }
        }
    }
    isbeingmine = false;
}

void AutoMine::onRender(MinecraftUIRenderContext* ctx) {
    GameMode* gm = mc.getGameMode();
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (!gm || !localPlayer || !mc.getClientInstance()->minecraftGame->canUseKeys) return;

    ImGuiIO& io = ImGui::GetIO();
    static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");

    float destroyProgress = gm->destroyProgress;
    if (destroyProgress > 1.f) destroyProgress = 1.f;

    UIColor fillColor(255 - int(destroyProgress * 255.f), int(destroyProgress * 255.f), 0, 40);
    UIColor lineColor(255 - int(destroyProgress * 255.f), int(destroyProgress * 255.f), 0, 225);
    if (destroyProgress > 0.f) {
        AABB aabb = localPlayer->makeAABB(&breakPos);
        RenderUtils::drawBox(aabb, fillColor, lineColor, 0.3f, true, false);
    }
}

void AutoMine::onSendPacket(Packet* packet, bool& shouldCancel) {
    if (packet->getId() == PacketID::PlayerAuthInput || packet->getId() == PacketID::MovePlayerPacket) {
        auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
        auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
        auto rots = GetRotations(mc.getLocalPlayer()->stateVectorComponent->pos, possrot);
        if (mc.getLocalPlayer()->isDestroying) {
            authPacket->ticksAlive = 20;
            movePacket->tick = 20;
            movePacket->actorRuntimeID = mc.getLocalPlayer()->getRuntimeID();
            authPacket->rotation = rots;
            movePacket->rotation = rots;
            authPacket->headYaw = rots.y;
            movePacket->headYaw = rots.y;
        }
    }
}

void AutoMine::onImGuiRender(ImDrawList* drawlist) {
    // Custom ImGui rendering logic if needed
}
