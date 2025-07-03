#include "WallClip.h"

WallClip::WallClip() : IModule(0x0, Category::PLAYER, "Clips u to the nearest wall")
{
    registerEnumSetting("Mode", &mode, 0, "How the client should clip");
    mode.addEntry("Normal", 0);
    mode.addEntry("Silent", 1);
    registerFloatSetting("Range", &range, range, 0.f, 1.0f, 0.1f, "The range to detect if theres a clippable block");
    registerFloatSetting("Offset", &offset, offset, 0.f, 1.0f, 0.1f, "The offset of the clip");
}

WallClip::~WallClip()
{
}
const char *WallClip::getRawModuleName()
{
    return ("WallClip");
}

void WallClip::onTick(C_GameMode *gm)
{
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr)
        return;

    vec3_t *pos = g_Data.getLocalPlayer()->getPos();
    for (int x = (int)pos->x - range; x < pos->x + range; x++)
    {
        for (int z = (int)pos->z - range; z < pos->z + range; z++)
        {
            for (int y = (int)pos->y - range; y < pos->y + range; y++)
            {
                vec3_t air = vec3_t(x, y + 1, z);
                vec3_t air2 = vec3_t(x, y + 2, z);
                vec3_t blockPos = vec3_t(x, y, z);

                vec3_t blockPos2 = vec3_t(x + 1, y, z);
                vec3_t blockPos3 = vec3_t(x, y, z + 1);

                vec3_t blockPos5 = vec3_t(x - 1, y, z);
                vec3_t blockPos6 = vec3_t(x, y, z - 1);

                C_Block *airpos = g_Data.getLocalPlayer()->region->getBlock(air);
                C_Block *airpos2 = g_Data.getLocalPlayer()->region->getBlock(air2);

                C_Block *block = g_Data.getLocalPlayer()->region->getBlock(blockPos);
                C_Block *block2 = g_Data.getLocalPlayer()->region->getBlock(blockPos2);
                C_Block *block3 = g_Data.getLocalPlayer()->region->getBlock(blockPos3);

                C_Block *block5 = g_Data.getLocalPlayer()->region->getBlock(blockPos5);
                C_Block *block6 = g_Data.getLocalPlayer()->region->getBlock(blockPos6);

                int aircheck = airpos->toLegacy()->blockId;
                int aircheck2 = airpos2->toLegacy()->blockId;
                int blockId = block->toLegacy()->blockId;
                int blockId2 = block2->toLegacy()->blockId;
                int blockId3 = block3->toLegacy()->blockId;
                int blockId5 = block5->toLegacy()->blockId;
                int blockId6 = block6->toLegacy()->blockId;

                C_MovePlayerPacket teleportPacket;
                C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
                g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
                C_LocalPlayer *player = g_Data.getLocalPlayer();
                vec3_t *pos = player->getPos();
                auto posXZ = (blockId2 == 49 || blockId2 == 7) && (blockId3 == 49 || blockId3 == 7) && (blockId == 0) && (aircheck == 0) && !((blockId2 == 7 && blockId3 == 7 && blockId == 0) && (aircheck == 0));
                auto negXZ = (blockId5 == 49 || blockId5 == 7) && (blockId6 == 49 || blockId6 == 7) && (blockId == 0) && (aircheck == 0) && !((blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0));

                if (mode.getSelectedValue() == 0)
                {
                    // Positive XZ
                    if (posXZ)
                        player->setPos(vec3_t(pos->x + offset, pos->y, pos->z + offset));

                    // Negative XZ
                    if (negXZ)
                        player->setPos(vec3_t(pos->x - offset, pos->y, pos->z - offset));
                }
            }
        }
    }
}
void WallClip::onSendPacket(C_Packet *packet)
{
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr)
        return;

    vec3_t *pos = g_Data.getLocalPlayer()->getPos();
    for (int x = (int)pos->x - range; x < pos->x + range; x++)
    {
        for (int z = (int)pos->z - range; z < pos->z + range; z++)
        {
            for (int y = (int)pos->y - range; y < pos->y + range; y++)
            {
                vec3_t air = vec3_t(x, y + 1, z);
                vec3_t air2 = vec3_t(x, y + 2, z);
                vec3_t blockPos = vec3_t(x, y, z);

                vec3_t blockPos2 = vec3_t(x + 1, y, z);
                vec3_t blockPos3 = vec3_t(x, y, z + 1);

                vec3_t blockPos5 = vec3_t(x - 1, y, z);
                vec3_t blockPos6 = vec3_t(x, y, z - 1);

                C_Block *airpos = g_Data.getLocalPlayer()->region->getBlock(air);
                C_Block *airpos2 = g_Data.getLocalPlayer()->region->getBlock(air2);

                C_Block *block = g_Data.getLocalPlayer()->region->getBlock(blockPos);
                C_Block *block2 = g_Data.getLocalPlayer()->region->getBlock(blockPos2);
                C_Block *block3 = g_Data.getLocalPlayer()->region->getBlock(blockPos3);

                C_Block *block5 = g_Data.getLocalPlayer()->region->getBlock(blockPos5);
                C_Block *block6 = g_Data.getLocalPlayer()->region->getBlock(blockPos6);

                int aircheck = airpos->toLegacy()->blockId;
                int aircheck2 = airpos2->toLegacy()->blockId;
                int blockId = block->toLegacy()->blockId;
                int blockId2 = block2->toLegacy()->blockId;
                int blockId3 = block3->toLegacy()->blockId;
                int blockId5 = block5->toLegacy()->blockId;
                int blockId6 = block6->toLegacy()->blockId;

                C_MovePlayerPacket teleportPacket;
                C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
                g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
                C_LocalPlayer *player = g_Data.getLocalPlayer();
                vec3_t *pos = player->getPos();

                auto posXZ = (blockId2 == 49 || blockId2 == 7) && (blockId3 == 49 || blockId3 == 7) && (blockId == 0) && (aircheck == 0) && !((blockId2 == 7 && blockId3 == 7 && blockId == 0) && (aircheck == 0));
                auto negXZ = (blockId5 == 49 || blockId5 == 7) && (blockId6 == 49 || blockId6 == 7) && (blockId == 0) && (aircheck == 0) && !((blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0));

                if (mode.getSelectedValue() == 1)
                {
                    // Positive XZ
                    if (posXZ && packet->isInstanceOf<PlayerAuthInputPacket>())
                    {
                        PlayerAuthInputPacket *InputPacket = reinterpret_cast<PlayerAuthInputPacket *>(packet);
                        InputPacket->pos.x = static_cast<float>(pos->x + offset);
                        InputPacket->pos.z = static_cast<float>(pos->z + offset);
                    }
                    if (posXZ && packet->isInstanceOf<C_MovePlayerPacket>())
                    {
                        C_MovePlayerPacket *movePacket = reinterpret_cast<C_MovePlayerPacket *>(packet);
                        movePacket->Position.x = static_cast<float>(pos->x + offset);
                        movePacket->Position.z = static_cast<float>(pos->z + offset);
                    }

                    // Negative XZ
                    if (negXZ && packet->isInstanceOf<PlayerAuthInputPacket>())
                    {
                        PlayerAuthInputPacket *InputPacket = reinterpret_cast<PlayerAuthInputPacket *>(packet);
                        InputPacket->pos.x = static_cast<float>(pos->x - offset);
                        InputPacket->pos.z = static_cast<float>(pos->z - offset);
                    }

                    if (negXZ && packet->isInstanceOf<C_MovePlayerPacket>())
                    {
                        C_MovePlayerPacket *movePacket = reinterpret_cast<C_MovePlayerPacket *>(packet);
                        movePacket->Position.x = static_cast<float>(pos->x - offset);
                        movePacket->Position.z = static_cast<float>(pos->z - offset);
                    }
                }
            }
        }
    }
}