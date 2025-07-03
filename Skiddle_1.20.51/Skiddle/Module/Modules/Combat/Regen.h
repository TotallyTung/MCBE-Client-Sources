#pragma once
#include <cstring>

class Regen : public Module
{
public:
    Vector3<int> blockPos;
    unsigned char side;
    bool destoryed = false;
    bool rdestory = false;
    float delay0 = 0;
    float delay = 40;

    Regen(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Regen", "Combat", "Test", keybind, enabled)
    {
        registerFloatSetting("Delay", "Broken delay", &delay, 0, 300);
    }

    void onEvent(UpdateEvent* event) {
        auto player = Game::GetLocalPlayer();
        if (!player) return;
        auto gamemode = player->getGameMode();
        if (!gamemode) return;

        auto pos = Game::GetLocalPlayer()->GetPosition();
        auto blockname = Game::GetInstance()->getBlockSource()->getBlock(blockPos)->GetTileName()->getText();
        if (std::strstr(blockname, "tile.lit_redstone_ore") != nullptr || std::strstr(blockname, "tile.redstone_ore") != nullptr) {
            for (int x = -5; x <= 5; x++) {
                for (int z = -5; z <= 5; z++) {
                    for (int y = -5; y <= 5; y++) {
                        blockPos = (Vector3<int>(x, y, z));
                    }
                }
            }
        }

        if (delay0 > delay)
        {
            rdestory = true;
            delay0 = 0;
        }
        else {
            rdestory = false;
            delay0++;
        }

        if (blockname != nullptr) {
            if (std::strstr(blockname, "tile.lit_redstone_ore") != nullptr || std::strstr(blockname, "tile.redstone_ore") != nullptr) {
                destoryed = false;

                if (rdestory) {
                    gamemode->stopDestroyBlock(blockPos);
                    gamemode->destroyBlock(blockPos, side, true);
                    destoryed = true;
                }
            }
        }
    }
    
    void onDisabled() {
        destoryed = false;
        rdestory = false;
        delay0 = 0;
    }

    void onEvent(PacketSendingEvent* event) override
    {
        auto player = Game::GetLocalPlayer();
        if (player == nullptr) return;

        auto packet = event->Packet;

        if (!Game::GetLocalPlayer() || !Game::GetInstance()->getPacketSender() && !destoryed)
        {
            return;
        }

        if (event->Packet->instanceOf<PlayerAuthInputPacket>())
        {
            PlayerAuthInputPacket* pkt = reinterpret_cast<PlayerAuthInputPacket*>(event->Packet);
            Vector2<float> angle = Game::GetLocalPlayer()->GetPosition().CalcAngle(Vector3<float>(blockPos.x, blockPos.y, blockPos.z));
            pkt->yawUnused = angle.y;
            pkt->yaw = angle.y;
            pkt->pitch = angle.x;
        }

        if (event->Packet->instanceOf<MovePlayerPacket>())
        {
            MovePlayerPacket* pkt = reinterpret_cast<MovePlayerPacket*>(event->Packet);
            Vector2<float> angle = Game::GetLocalPlayer()->GetPosition().CalcAngle(Vector3<float>(blockPos.x, blockPos.y, blockPos.z));
            pkt->headYaw = angle.y;
            pkt->yaw = angle.y;
            pkt->pitch = angle.x;
        }
    }

    // Rendering
	void onEvent(ImGUIRenderEvent* event) override
	{
		
	}

};