//
// Created by Flash on 2/4/2024.
//

#include "AutoWeb.h"

#include <Prax/Module/Modules/Combat/Antibot.h>

Actor* AutoWeb::GetClosestActor() const
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto actors = Antibot::GetActors();

    if (!player || actors.empty()) {
        return nullptr;
    }

    // get the closest actor
    Actor* closestActor = nullptr;
    float closestDistance = -1.f;
    for (auto actor : actors) {
        if (actor == player) continue;

        float distance = actor->getPos()->Distance(*player->getPos());
        if (closestDistance == -1.f || distance < closestDistance) {
            closestDistance = distance;
            closestActor = actor;
        }
    }



    if (!closestActor) {
        return nullptr;
    }

    // Distance check
    if (closestDistance > Distance.Value) {
        return nullptr;
    }

    return closestActor;

}

void AutoWeb::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (!player || !player->isAlive()) {
        return;
    }

    // Check for webs in inventory
    int webSlot = Misc::GetItemSlot("web", HotbarOnly.Enabled);
    if (webSlot == -1) return;

    // Get the closest actor
    Actor* closestActor = GetClosestActor();
    if (!closestActor) return;

    // Select the slot
    player->SelectSlot(webSlot);

    // Get their position
    Vector3 pos = *closestActor->getPos();

    { // Prediction
        Vector3 posPrev = *closestActor->getPosPrev();

        // Predict their next position
        Vector3 prediction = pos - posPrev;
        prediction.x *= Prediction.Value;
        prediction.y *= Prediction.Value;
        prediction.z *= Prediction.Value;

        // Add the prediction to their current position
        //player->addChatMessage("&bPredicted delta: &aX: &6" + std::to_string(prediction.x) + " &aY: &6" + std::to_string(prediction.y) + " &aZ: &6" + std::to_string(prediction.z), true);
        pos.x += prediction.x;
        pos.y += prediction.y;
        pos.z += prediction.z;
    }

    // Get the position to place the web
    BlockPos placePos = Misc::GetClosestPlacePos(BlockPos(pos), Distance.Value, false, pos.y);

    //player->addChatMessage("&bAttempting to place at pos &aX: &6" + std::to_string(placePos.x) + " &aY: &6" + std::to_string(placePos.y) + " &aZ: &6" + std::to_string(placePos.z), true);

    // Return if this block isnt air
    if (!Misc::IsAirBlock(placePos)) return;

    // Place the web
    Misc::PlaceBlock(placePos, 0, true, true, true);
}