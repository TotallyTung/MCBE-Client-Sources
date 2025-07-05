//
// Created by vastrakai on 9/4/2023.
//

#include <Prax/Module/Modules/Player/Teams.h>
#include <Prax/Utils/String.h>
#include "Antibot.h"

#include <Prax/Module/ModuleManager.h>

std::vector<Actor*> Antibot::Actors = {};
bool Antibot::AntibotEnabled = false;


void Antibot::OnEnable() {
    AntibotEnabled = true;
}

void Antibot::OnDisable() {
    AntibotEnabled = false;
    Antibot::Actors.clear();
}

void Antibot::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::ChangeDimension) {
        Antibot::Actors.clear();
    }
}

void Antibot::OnTick() {
    AntibotEnabled = Enabled;
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto actors = player->level->getRuntimeActorList();
    std::string playerName = player->_getNameTag();

    actors.erase(std::ranges::remove_if(actors, [&](Actor *actor)
    {
        if (actor == nullptr) {
            return true; // Remove null actors
        }

        if (IsPlayerCheckSetting.Enabled && (!actor->isPlayer() || actor->GetEntityId() != "minecraft:player")) {
            return true; // Remove non-players or non-player entities
        }

        if (InvisibleCheckSetting.Enabled && actor->isInvisible()) {
            return true; // Remove invisible actors
        }

        if (HitboxCheckSetting.Enabled) {
            float hitboxWidth = actor->GetAABBShapeComponent()->hitboxWidth;
            float hitboxHeight = actor->GetAABBShapeComponent()->hitboxHeight;
            float normalPlayerHeightMax = 1.81f;
            float normalPlayerHeightMin = 1.35f;
            float normalPlayerWidthMin = 0.54f;
            float normalPlayerWidthMax = 0.66f;
            if (hitboxWidth < normalPlayerWidthMin || hitboxWidth > normalPlayerWidthMax ||
                hitboxHeight < normalPlayerHeightMin || hitboxHeight > normalPlayerHeightMax) {
                return true; // Remove actors with incorrect hitbox dimensions
            }
        }

        if (NameCheckSetting.Enabled) {
            std::string nameTagString = actor->_getNameTag();
            if (std::ranges::count(nameTagString, '\n') > 0) {
                return true; // Remove actors with multiple lines in nametag
            }
        }

        if (ExtraNameCheckSetting.Enabled) {
            std::string nameTagString = actor->_getNameTag();
            if (!String::ContainsIgnoreCase(nameTagString, "§")) {
                return true; // Remove actors without a color code in nametag
            }
        }

        if (PlayerListCheckSetting.Enabled) {
            std::vector<std::string> playerListNames;
            for (const auto& pair : Minecraft::ClientInstance->ThePlayer()->level->playerList) {
                playerListNames.push_back(pair.second.name);
            }
            std::string nameTagString = actor->_getNameTag();
            bool found = false;
            for (const std::string& name : playerListNames) {
                if (String::ContainsIgnoreCase(nameTagString, name)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return true; // Remove actors not found in the player list
            }
        }

        return false; // Keep the actor
    }).begin(), actors.end());

    Antibot::Actors = actors;
}

void Antibot::OnRender()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) Antibot::Actors.clear();
}

bool Antibot::IsBot(Actor *actor) {
    if (!AntibotEnabled)
    {
        return false;
    }

    if (actor == nullptr) {
        return true; // Remove null actors
    }

    if (IsPlayerCheckSetting.Enabled && (!actor->isPlayer() || actor->GetEntityId() != "minecraft:player")) {
        return true; // Remove non-players or non-player entities
    }

    if (InvisibleCheckSetting.Enabled && actor->isInvisible())
    {
        return true; // Remove invisible actors
    }

    if (HitboxCheckSetting.Enabled) {
        float hitboxWidth = actor->GetAABBShapeComponent()->hitboxWidth;
        float hitboxHeight = actor->GetAABBShapeComponent()->hitboxHeight;
        float normalPlayerHeightMax = 1.81f;
        float normalPlayerHeightMin = 1.35f;
        float normalPlayerWidthMin = 0.54f;
        float normalPlayerWidthMax = 0.66f;
        if (hitboxWidth < normalPlayerWidthMin || hitboxWidth > normalPlayerWidthMax ||
        hitboxHeight < normalPlayerHeightMin || hitboxHeight > normalPlayerHeightMax) {
                return true; // Remove actors with incorrect hitbox dimensions
        }
    }

    if (NameCheckSetting.Enabled) {
        std::string nameTagString = actor->_getNameTag();
        if (std::ranges::count(nameTagString, '\n') > 0) {
            return true; // Remove actors with multiple lines in nametag
        }
    }

    if (ExtraNameCheckSetting.Enabled)
    {
        std::string nameTagString = actor->_getNameTag();
        if (!String::ContainsIgnoreCase(nameTagString, "§")) {
            return true; // Remove actors without a color code in nametag
        }

        if (PlayerListCheckSetting.Enabled) {
            std::vector<std::string> playerListNames;
            for (const auto& pair : Minecraft::ClientInstance->ThePlayer()->level->playerList) {
                playerListNames.push_back(pair.second.name);
            }
            std::string nameTagString = actor->_getNameTag();
            bool found = false;
            for (const std::string& name : playerListNames) {
                if (String::ContainsIgnoreCase(nameTagString, name)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return true; // Remove actors not found in the player list
            }
        }
    }

    return false; // Keep the actor
}

[[nodiscard]] std::vector<Actor *> Antibot::GetActors(bool includeTeamMembers) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return {};

    static Antibot* antibot = ModuleManager::GetModule<Antibot>();
    if (!antibot) return {};
    if (!antibot->Enabled) return player->level->getRuntimeActorList();

    if (!includeTeamMembers && Teams::TeamsEnabled) {
        auto actors = Antibot::Actors;

        // Remove team members
        std::erase_if(actors, [](Actor *actor) {
            return Teams::IsOnTeam(actor);
        });

        return actors;
    }

    return Antibot::Actors;
}
