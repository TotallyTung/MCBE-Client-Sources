//
// Created by vastrakai on 12/4/2023.
//

#include "MidclickPearl.h"

#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/UI/Notifications.h>

static bool midClick = false;
static bool oldMidClick = false;

void MidclickPearl::OnEnable()
{
}

void MidclickPearl::OnDisable()
{
    midClick = false;
    oldMidClick = false;
}

bool thrownow = false;
bool rotatenow = false;


void MidclickPearl::OnTick()
{
    if (!Minecraft::ClientInstance->mcGame->canUseKeys()) return;
    oldMidClick = midClick;
    midClick = ImGui::IsMouseDown(2);

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto supplies = player->supplies;
    if (!supplies) return;

    ItemStack* pearlItem = nullptr;
    int slot = -1;

    // Look for ender_pearl
    for (int i = 0; i < 36; i++)
    {
        auto itemStack = supplies->inventory->getItem(i);
        if (!itemStack) continue;
        if (!itemStack->item) continue;

        auto item = *itemStack->item;


        if (item->GetName() == "ender_pearl")
        {
            pearlItem = itemStack;
            slot = i;
            break;
        }
    }

    if (thrownow)
    {
        thrownow = false;
        int lastSlot = supplies->selectedHotbarSlot;
        supplies->selectedHotbarSlot = slot;
        if (Swing.Enabled) player->swing();
        player->gamemode->baseUseItem(pearlItem);
        supplies->selectedHotbarSlot = lastSlot;
    }

    if (!oldMidClick && midClick)
    {
        if (!pearlItem)
        {
            //player->addChatMessage("§cNopearls found!§r");
            Notifications::Notify(Prax::ClientName, "No pearls found!", NotificationType::Error, 3.f);

            // play warning sound with note.pling (https://minecraft.gamepedia.com/Sounds.json)
            Minecraft::ClientInstance->playUI("note.bass", 0.75f, 0.5);
            return;
        }

        thrownow = true;
        rotatenow = true;
    }

}

void MidclickPearl::OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled)
{
    if (packet->getId() == (int)PacketID::PlayerAuthInput && thrownow)
    {
        auto paip = (PlayerAuthInputPacket*)packet;
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        Vector2 realRots = { player->GetActorRotationComponent()->pitch, player->GetActorRotationComponent()->yaw };

        paip->rotation = realRots;
        paip->headYaw = realRots.y;
        rotatenow = false;
    }
}
