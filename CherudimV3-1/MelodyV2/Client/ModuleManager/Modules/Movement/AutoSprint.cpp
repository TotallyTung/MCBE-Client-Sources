#include "AutoSprint.h"

#include <Windows.h>
#include "../../../Client.h"
AutoSprint::AutoSprint() : Module("AutoSprint", "Sprint for you.", Category::MOVEMENT) {
    addEnumSetting("Mode", "Change sprint modes.", { "Auto" }, &Mode);
}

void AutoSprint::onNormalTick(Actor* actor) {
    static PacketMine* NotificationsMod = (PacketMine*)client->moduleMgr->getModule("PacketMine");
    if (mc.getLocalPlayer() == nullptr) return;

    bool isMoving = mc.isKeyDown('W') || mc.isKeyDown('A') || mc.isKeyDown('S') || mc.isKeyDown('D');
    if (isMoving) {
        mc.getLocalPlayer()->setSprinting(true);
    }
    else {
        mc.getLocalPlayer()->setSprinting(false);
    }
}
