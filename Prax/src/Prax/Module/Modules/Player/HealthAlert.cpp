//
// Created by vastrakai on 1/19/2024.
//

#include "HealthAlert.h"

#include <Prax/UI/Notifications.h>

void HealthAlert::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    static bool alerted = false;
    // If health is above the minimum, reset the alert
    float health = player->getHealth();
    if (health > MinHealth.Value) {
        alerted = false;
        return;
    }

    // If the alert has already been sent, don't send it again
    if (alerted) return;

    // Send the alert
    alerted = true;
    std::string message = "Your health is low!";
    player->addChatMessage("&c" + message, 1);

    Notifications::Notify(Prax::ClientName, message, NotificationType::Warning, 5.f);
    // sound, volume, pitch
    Minecraft::ClientInstance->playUI("note.iron_xylophone", 1.f, 1.f);
    Minecraft::ClientInstance->playUI("note.iron_xylophone", 1.f, 1.f);

}
