#pragma once

using namespace std;
class Scaffold : public Module
{
public:
    float distance = 5;

    float targetY = 0.f;
    bool hasTowered = false;
    Vector3<float> targetBlock = {};

    int towerMode = 0;
    int rotationMode = 0;
    float towertimer = 30;
    bool yLock = false;
    bool blockleft = false;

    float extend = 1;
    Vector3<float> calcPos = Vector3 <float>{ 0, 0, 0 };

    Scaffold(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Scaffold", "Player", "Basic version of scaffold", keybind, enabled)
    {
        ingameOnly = true;

        registerBoolSetting("Keep Y", "Kepps your y level when placing blocks", &yLock);
        registerBoolSetting("Block Left", "Show left block", &blockleft);
        registerEnumSetting("Rotations", "The tower mode", { "Normal", "Down" }, &rotationMode);
        registerEnumSetting("Tower", "The tower mode", { "None", "Motion", "Timer" }, &towerMode);
        registerFloatSetting("Extend", "Distance your position to target block", &extend, 0, 10);
        registerFloatSetting("Tower Timer", "The timer tower speed", &towertimer, 20, 60);
    }

    bool canPlace(Vector3<float> pos) {
        return Game::GetInstance()->getBlockSource()->getBlock(pos.floor().ToInt())->GetMaterialType() == MaterialType::Air;
    }

    bool findBlock() {
        PlayerInventory* supplies = Game::GetLocalPlayer()->getSupplies();
        Inventory* inv = supplies->inventory;
        auto prevSlot = supplies->hotbarSlot;

        for (int i = 0; i < 9; i++) {
            ItemStack* stack = inv->getItem(i);
            if (stack->item != nullptr) {
                if (stack->isBlockType()) {
                    if (prevSlot != i)
                        supplies->hotbarSlot = i;
                    return true;
                }
            }
        }

        return false;
    }

    bool buildBlock(Vector3<float> pos, GameMode* gm) {
        static std::vector<Vector3<float>> neighbours = {
            Vector3<float>(0, -1, 0), Vector3<float>(0, 1, 0),
            Vector3<float>(0, 0, -1), Vector3<float>(0, 0, 1),
            Vector3<float>(-1, 0, 0), Vector3<float>(1, 0, 0),
        };

        for (int face = 0; face < neighbours.size(); face++) {
            Vector3<float> offset = Vector3<float>(pos.floor()).submissive(neighbours[face]);
            if (!canPlace(offset.ToFloat()) && findBlock()) {
                gm->buildBlock(offset.ToInt(), face, false);
                calcPos = offset.add(Vector3<float>(0.78f, 0.78f, 0.78f));

                return true;
            }
        }
        return false;
    }

    bool predictBlock(Vector3<float> pos, GameMode* gm) {
        static std::vector<Vector3<float>> blocks;
        if (blocks.empty()) {
            for (int y = -distance; y <= 0; y++) {
                for (int x = -distance; x <= distance; x++) {
                    for (int z = -distance; z <= distance; z++) {
                        blocks.push_back(Vector3<float>(x, y, z));
                    }
                }
            }
            std::sort(blocks.begin(), blocks.end(), [](Vector3<float> start, Vector3<float> end) {
                return sqrtf((start.x * start.x) + (start.y * start.y) + (start.z * start.z)) < sqrtf((end.x * end.x) + (end.y * end.y) + (end.z * end.z));
                });
        }

        for (const Vector3<float>& offset : blocks) {
            Vector3<float> currentBlock = Vector3<float>(pos.floor()).add(offset);
            if (buildBlock(currentBlock, gm)) {
                return true;
            }
        }
        return false;
    }

    bool spoofSlot() {
        auto supplies = Game::GetLocalPlayer()->getSupplies();
        auto inv = supplies->inventory;
        auto prevSlot = supplies->hotbarSlot;

        for (int n = 0; n < 9; n++) {
            ItemStack* stack = inv->getItem(n);
            Item* it = stack->item;
            if (stack->item) {
                if (prevSlot != n) {
                    supplies->hotbarSlot = n;
                };
                return true;
            };
        };

        return false;
    }



    void onEvent(UpdateTickEvent* event) override
    {
        auto player = Game::GetLocalPlayer();
        auto gm = player->getGameMode();
        auto stateVec = player->try_get<StateVectorComponent>();

        if (!player || !gm || !stateVec) {
            return;
        }

        Vector3<float> velocity = stateVec->r_Velocity.Normalize();

        auto supplies = Game::GetLocalPlayer()->getSupplies();

        // Set our yLevel
        if (player->getMovementProxy()->isOnGround() || player->GetPosition().y < targetY + 0.5f || !MoveUtil::isMoving() && GetAsyncKeyState(VK_SPACE) || (!yLock && player->GetPosition().y > targetY))
            targetY = player->GetPosition().floor().y - 0.5f;

        // Handle tower modes
        switch (towerMode) {
        case 0:
            break;
        case 1:
            if (!MoveUtil::isMoving() && GetAsyncKeyState(VK_SPACE) && velocity.y > -0.2) {
                // stateVec->r_Velocity.y = .85f;
                stateVec->r_Velocity.y = .25f;
                hasTowered = true;
            }
            else {
                if (!player->getMovementProxy()->isOnGround() && hasTowered) {
                    stateVec->r_Velocity.y = -1.5f;
                    hasTowered = false;
                }
            }
            break;
        case 2:
            if (!MoveUtil::isMoving() && GetAsyncKeyState(VK_SPACE)) {
                Game::GetInstance()->getMinecraft()->setTimerSpeed(towertimer);
                hasTowered = true;
            }
            else {
                if (!player->getMovementProxy()->isOnGround() && hasTowered) {
                    Game::GetInstance()->getMinecraft()->setTimerSpeed(20.f);
                    hasTowered = false;
                }
            }
            break;
        }

        targetBlock = player->GetPosition();
        targetBlock.x += velocity.x * extend;
        targetBlock.y = targetY;
        targetBlock.z += velocity.z * extend;

        if (canPlace(targetBlock)) {
            predictBlock(targetBlock, gm);
        }
    }

    void onEvent(PacketSendingEvent* event) override
    {
        auto player = Game::GetLocalPlayer();
        if (player == nullptr) return;

        auto packet = event->Packet;

        if (!Game::GetLocalPlayer() || !Game::GetInstance()->getPacketSender())
        {
            return;
        }


        switch (rotationMode) {
        case 0: // Normal
            if (event->Packet->instanceOf<PlayerAuthInputPacket>())
            {
                PlayerAuthInputPacket* pkt = reinterpret_cast<PlayerAuthInputPacket*>(event->Packet);
                pkt->pitch = 65.f;
            }

            if (event->Packet->instanceOf<MovePlayerPacket>())
            {
                MovePlayerPacket* pkt = reinterpret_cast<MovePlayerPacket*>(event->Packet);
                pkt->pitch = 65.f;
            }
            break;
        case 1: // Down
            if (event->Packet->instanceOf<PlayerAuthInputPacket>())
            {
                PlayerAuthInputPacket* pkt = reinterpret_cast<PlayerAuthInputPacket*>(event->Packet);
                pkt->pitch = 90.f;
            }

            if (event->Packet->instanceOf<MovePlayerPacket>())
            {
                MovePlayerPacket* pkt = reinterpret_cast<MovePlayerPacket*>(event->Packet);
                pkt->pitch = 90.f;
            }
            break;
        }
    }

    void onEnabled() override {
        Player* player = Game::GetLocalPlayer();
        if (!player) {
            return;
        }
        hasTowered = false;
    }

    void onDisabled() override {
        auto player = Game::GetLocalPlayer();
        auto stateVec = player->try_get<StateVectorComponent>();
        if (!player || !stateVec) {
            return;
        }

        if (hasTowered) {
            stateVec->r_Velocity.y = -1.5f;
        }
    }

    void onEvent(ImGUIRenderEvent* event)
    {
        if (getModuleByName("clickgui")->isEnabled()) return;
        if (!Game::Core::showMenu) return;

        if (blockleft) {
            float renderx = Game::GetInstance()->getGuiData()->WindowResolution.x / 2;
            float rendery = roundf(Game::GetInstance()->getGuiData()->WindowResolution.y * 0.75f) + 0.5f;
            Vector4<float> plusRect = Vector4<float>(renderx, rendery + 6, 55 + renderx, rendery + 16);
            Vector2<float> textPos = Vector2<float>(renderx - 7, rendery - 25.f);

            auto supplies = Game::GetLocalPlayer()->getSupplies();
            auto inv = supplies->inventory;

            int totalCount = 0;

            for (int i = 0; i < 8; i++) {
                ItemStack* stack = inv->getItem(i);
                if (stack->item != nullptr) {
                    if (stack->isBlockType()) {
                        totalCount += (int)(*(uint8_t*)(((uintptr_t)stack) + 0x22));
                    }
                }
            }

            string text = "Blocks: " + to_string(totalCount);
            float textLen = ImRenderUtil::getTextWidth(&text, 1) / 2;
            textPos.x -= textLen;
            plusRect.x -= textLen;
            Vector4<float> rectpos = Vector4<float>(plusRect.x - 25, plusRect.y - 40, plusRect.z + 7, plusRect.w - 20);

            ImRenderUtil::fillRectangle(rectpos, UIColor(0, 0, 0), 0.6f, 5.f);
            ImRenderUtil::drawText(textPos, &text, UIColor(255, 255, 255), 1.2f, 1.f, true);
        }
    }
};