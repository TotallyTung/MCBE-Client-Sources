//
// Created by vastrakai on 10/28/2023.
//

#include <Prax/Module/ModuleManager.h>
#include "AntiVoid.h"


void AntiVoid::OnEnable() {

}
void AntiVoid::OnDisable() {

}

std::vector<Vector3> AntiVoid::LastOnGroundPositions = std::vector<Vector3>();

void AntiVoid::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();

    // Add the position to the list if we are on the ground and if the position isnt already in the list
    if (player->isOnGround() && player->wasOnGround()) {
        //Logger::Write("AntiVoid", "Adding position to list");
        LastOnGroundPositions.push_back(*player->getPos() - player->GetHeightOffset());
    }

    Vector3 closestOnGroundPos = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

    // use .Distance for this
    for (auto pos: LastOnGroundPositions) {
        if (closestOnGroundPos.Distance(*player->getPos()) > pos.Distance(*player->getPos())) {
            closestOnGroundPos = pos;
        }
    }

    // Also limit the size of the list to 100
    if (LastOnGroundPositions.size() > 100) {
//        Logger::Write("AntiVoid", "Removing position [too many positions]");
        LastOnGroundPositions.erase(LastOnGroundPositions.begin());
    }

    // Check if we are falling
    if (player->GetFallDistance() > Distance.Value) {
//        Logger::Write("AntiVoid", "Falling");

        if (closestOnGroundPos == Vector3(FLT_MAX, FLT_MAX, FLT_MAX)) {
//            Logger::Write("AntiVoid", "No closest position found, size: " + std::to_string(LastOnGroundPositions.size()));
            return;
        }

        if (VoidCheck.Enabled && !Misc::IsOverVoid(*player->getPos(), false )) return;
        Logger::Write("AntiVoid", "Teleporting to: " + std::to_string(closestOnGroundPos.x) + ", " + std::to_string(closestOnGroundPos.y) + ", " + std::to_string(closestOnGroundPos.z));
        if (Teleport.Enabled) player->teleportTo(*player->getPos(), false, 0, 0, false);
        if (EnableFreecam.Enabled) ModuleManager::EnableModule(xorstr_("Freecam"));
        if (EnableScaffold.Enabled) ModuleManager::EnableModule(xorstr_("Scaffold"));
        if (DisableRegen.Enabled) ModuleManager::DisableModule(xorstr_("Regen"));
        if (TpOnce.Enabled) {
            player->SetFallDistance(0);
            LastOnGroundPositions.clear();
        }


    }
}
