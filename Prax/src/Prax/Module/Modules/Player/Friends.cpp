//
// Created by vastrakai on 10/3/2023.
//

#include <Prax/Utils/String.h>
#include "Friends.h"

bool Friends::FriendsEnabled = false;

bool Friends::IsFriend(Actor *actor) {
    if (!FriendsEnabled) return false;
    auto player = Minecraft::ClientInstance->ThePlayer();
    std::string playerName = actor->_getNameTag();
    playerName = String::RemoveColorCodes(playerName);
    playerName = String::SanitizeName(playerName);

    for (const auto& friendName : Prax::Preferences->Module.Friends) {
        if (String::ContainsIgnoreCase(playerName, friendName)) {
            return true;
        }
    }

    return false;
}

bool Friends::IsFriend(std::string playerName) {
    if (!FriendsEnabled) return false;
    playerName = String::RemoveColorCodes(playerName);
    playerName = String::SanitizeName(playerName);

    for (const auto& friendName : Prax::Preferences->Module.Friends) {
        if (String::ContainsIgnoreCase(playerName, friendName)) {
            return true;
        }
    }

    return false;
}

void Friends::OnTick() {
    FriendsEnabled = Enabled;
}

void Friends::OnEnable() {
    FriendsEnabled = true;
}

void Friends::OnDisable() {
    FriendsEnabled = false;
}
