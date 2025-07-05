#pragma once

#include <Prax/Utils/Render/D2D.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/String.h>
#include <Prax/SDK/Game/Network/Network.h>
#include <Prax/SDK/Game/Network/Other/TextPacket.h>

#include <random>
#include <Prax/UI/Notifications.h>
#include <Prax/SDK/Game/Network/Other/ActorEventPacket.h>
#include <Prax/SDK/Game/Network/Other/RemoveEntityPacket.h>
#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include "Killsults.h"

#include <Prax/Module/Modules/Visual/Statistics.h>
#include <Prax/SDK/Game/Network/Other/SetTitlePacket.h>

#include "src/Prax/UI/ClickGui.h"

void Killsults::OnInitialize() {
    if (FileSystem::CreateFileF(FileSystem::AssetDirectory + "\\killsults.txt")) {
        std::ofstream file(FileSystem::AssetDirectory + "\\killsults.txt");
        file << "// Custom kill messages go in this file" << std::endl;
        file << "// You can use !target! to insert the name of the player you killed" << std::endl;
        file << "// Keep messages to one line only" << std::endl;
        file << "!target! got praxxed." << std::endl;
        // close the opened file.
        file.flush();
        file.close();
        Logger::Write("Killsults", "Created killsults.txt", Logger::LogType::Debug);
    }
}


std::vector<std::string> Killsults::GetCurrentKillsultsList(){
    std::vector<std::string> killsults = std::vector<std::string>();
    if (Mode.SelectedIndex == (int)KillsultsMode::Custom){
        std::string path = FileSystem::AssetDirectory + "\\killsults.txt";
        std::ifstream file(path);
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '/' || line[0] == '\n') continue;
            killsults.push_back(line);
        }
        file.close();
    } else if (Mode.SelectedIndex == (int)KillsultsMode::Prax) killsults = praxKillsults;
    else if (Mode.SelectedIndex == (int)KillsultsMode::Sigma) killsults = sigmaKillsults;
    else if (Mode.SelectedIndex == (int)KillsultsMode::HvH) killsults = hvhKillsults;
    else if (Mode.SelectedIndex == (int)KillsultsMode::Toxic) killsults = toxicKillsults;
    else if (Mode.SelectedIndex == (int)KillsultsMode::UwUSpeak) killsults = uwuSpeakKillsults;
    else if (Mode.SelectedIndex == (int)KillsultsMode::Zephyr) killsults = zephyrKillsults;
    else if (Mode.SelectedIndex == (int)KillsultsMode::Catgirl) killsults = catgirlKillsults;

    return killsults;
}

std::string Killsults::GetRandomKillsult(std::string const& target)
{
    // return a blank string if the target is empty
    if (target.empty()) {
        return "";
    }

    if (KillSounds.Enabled) {
        if (KillSound.SelectedIndex == (int)KillSounds::PopDing)
        {
            Minecraft::ClientInstance->playUI("random.orb", SoundVolume.Value, 1.0f);
            Minecraft::ClientInstance->playUI("firework.blast", SoundVolume.Value, 1.0f);
        } else
        {
            Misc::PlaySoundFromUrl("https://github.com/Prax-Client/Releases/raw/main/killsound.wav", SoundVolume.Value, false);
        }
    }
    Statistics::Kills++;

    if (ShowNotification.Enabled) {
        Notifications::Notify(Prax::ClientName, "You killed " + target + "!", 5.0f);
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (player) {
            // args: msg, autoReplaceColorcodes, fadeintime, fadeouttime, duration
            if (ShowActionbarMessage.Enabled)
                player->displayActionBarMessage("&aYou killed &6" + target + "&a!", true, 1, 1, 5);
        }
    }
    if (Mode.SelectedIndex == (int)KillsultsMode::None) return "";
    std::vector<std::string> killsults = GetCurrentKillsultsList();
    // use random library to get a random index
    std::string killsult = killsults[Misc::RandomNum<int>(0, killsults.size() - 1)];

    // Check if the string contains !target!
    if(killsult.find("!target!") != std::string::npos){
        // Replace !target! with the target's name
        killsult.replace(killsult.find("!target!"), 8, "@" + target);
    }


    return killsult;
}

void Killsults::sendMessage(std::string str){
    if (str.empty()) {
        return;
    }

    auto player = Minecraft::ClientInstance->ThePlayer();
    std::shared_ptr<TextPacket> textPacket = TextPacket::Create(Minecraft::ClientInstance->ThePlayer(), std::move(str));
    Minecraft::ClientInstance->getpacketSender()->sendToServer(textPacket.get());

}

// string vector of the last 5 chat messages
std::vector<std::string> Killsults::LastChatMessages = {};
std::map<uint64_t, int64_t> Killsults::LastAttackedActors = {};
std::map<int64_t, std::string> Killsults::LastAttackedActorNames = {};


void Killsults::OnTick() {
    if (Method.SelectedIndex == 0) return;
    // Limit the size of the last chat messages vector to 5
    if (LastChatMessages.size() > 5) LastChatMessages.erase(LastChatMessages.begin());

    uint64_t now = Misc::GetCurrentMs();

    // Limit the last attacked actors map to attacks within the last 5 seconds based on the first value which is ms
    for (auto it = LastAttackedActors.begin(); it != LastAttackedActors.end();) {
        if (now - it->first > 5000) {
            it = LastAttackedActors.erase(it);
        } else {
            ++it;
        }
    }

    // store LastAttackedActorNames
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    for (auto it = LastAttackedActors.begin(); it != LastAttackedActors.end(); ++it) {
        Actor* actor = Misc::GetActorFromRuntimeID(it->second);
        if (!actor) continue;
        std::string name = Misc::GetNameFromActor(actor);
        if (name.empty()) {
            continue;
        }
        LastAttackedActorNames[it->second] = name;
    }

    // Remove actors in the LastAttackedActorNames map that return nullptr when their first values are used in the GetActorFromRuntimeID function
    for (auto it = LastAttackedActorNames.begin(); it != LastAttackedActorNames.end();) {
        Actor* actor = Misc::GetActorFromRuntimeID(it->first);
        if (!actor) {
            it = LastAttackedActorNames.erase(it);
        } else {
            ++it;
        }
    }

}

bool Killsults::CanSendKillsult(int64_t target) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;

    std::string targetName = LastAttackedActorNames[target];

    // Check if the last 5 chat messages contain the target's name and if none of them contain it then return false
    {
        bool found = false;
        for (auto message: LastChatMessages) {
            if (message.find(targetName) != std::string::npos) {
                found = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }
    // Check if the last attacked actors map contains the target and if it doesn't then return false
    {
        bool found = false;
        for (auto it = LastAttackedActors.begin(); it != LastAttackedActors.end(); ++it) {
            if (it->second == target) {
                found = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }
    return true;
}


void Killsults::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    static auto statistics = ModuleManager::GetModule<Statistics>();

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    Packet* p = packet.get();

    if (p->getId() == (int)PacketID::SetTitle && ShowActionbarMessage.Enabled) {
        auto* setTitlePacket = reinterpret_cast<SetTitlePacket*>(p);

        if (setTitlePacket->mTitleText.contains("ELIMINATED")) *cancel = true;
        setTitlePacket->mXuid = player->getXuid();
    }

    if (p->getId() == (int)PacketID::EntityEvent) {
        if (Method.SelectedIndex != 1) return; // Only return if the method isn't set to "OnActorRemove"
        auto *entityEventPacket = reinterpret_cast<ActorEventPacket *>(p);

        if (entityEventPacket->Event == ActorEvent::HURT) {
            auto entityList = player->level->getRuntimeActorList();
            Actor *target = nullptr;

            target = Misc::GetActorFromRuntimeID(entityEventPacket->ActorRuntimeID);

            if (target) {
                if (player->getPos()->Distance(*target->getPos()) > 10) {
                    return;
                }
//                player->addChatMessage("added target to list: " + Misc::GetNameFromActor(target), 0);
                LastAttackedActors[Misc::GetCurrentMs()] = target->GetRuntimeID();
            } else {
//                player->addChatMessage("target not found", 0);
            }
        }
    } else if (p->getId() == (int)PacketID::RemoveEntity){
        auto* removeEntityPacket = reinterpret_cast<RemoveActorPacket*>(p);
        if (Method.SelectedIndex != 1) return; // Only return if the method isn't set to "OnActorRemove"



        std::string targetName = LastAttackedActorNames[removeEntityPacket->ActorRuntimeID];
        if (CanSendKillsult(removeEntityPacket->ActorRuntimeID)) {

            if (String::ContainsIgnoreCase(targetName, " ")) targetName = "\"" + targetName + "\"";
            sendMessage(GetRandomKillsult(targetName));
        } else {
        }

    } else if (p->getId() == (int)PacketID::Text) {
        TextPacket* textPacket = reinterpret_cast<TextPacket*>(p);
        std::string text = textPacket->message.getText();

        LastChatMessages.push_back(text);
        if (LastChatMessages.size() > 5) LastChatMessages.erase(LastChatMessages.begin());

        if (String::ContainsIgnoreCase(text, "§c§l» §r§c§lGame OVER!")) {
            if (AutoGG.Enabled) sendMessage("GG");
        }

        if (Method.SelectedIndex != 0) return; // Only return if the method isn't set to "Chat"


        std::string playerNameTag = player->_getNameTag();
        // If the player has a newline in their name, remove it and everything after it
        if (playerNameTag.find("\n") != std::string::npos) playerNameTag = playerNameTag.substr(0, playerNameTag.find("\n"));
        playerNameTag = String::RemoveColorCodes(playerNameTag);

        if (String::ContainsIgnoreCase(text, playerNameTag + " §ckilled") || String::ContainsIgnoreCase(text, playerNameTag + " §7killed")) {
            std::string target = text.substr(text.find("killed") + 7, text.find(playerNameTag) - text.find("killed") - 8);
            target = String::SanitizeName(target);
            if (String::ContainsIgnoreCase(target, " ")) target = "\"" + target + "\"";
            sendMessage(GetRandomKillsult(target));

            if (KillSounds.Enabled) {
                Minecraft::ClientInstance->playUI("random.orb", 0.75f, 1.0f);
                Minecraft::ClientInstance->playUI("firework.blast", 0.75f, 1.0f);
            }

            Statistics::Kills++;
        }
    } else if (packet->getId() == (int)PacketID::MovePlayer) {
        auto movePlayerPacket = reinterpret_cast<MovePlayerPacket*>(p);
        // Check if the runtimeId is the same as the player's runtimeId and clear the maps if the Position is more than 15 blocks away from the player's position
        if (movePlayerPacket->actorRuntimeID == player->GetRuntimeID()) {
            if (player->getPos()->Distance(movePlayerPacket->position) > 15) {
                LastAttackedActors.clear();
                LastAttackedActorNames.clear();
                LastChatMessages.clear();
            }
        }
    } else if (packet->getId() == (int)PacketID::ChangeDimension) {
        LastAttackedActors.clear();
        LastAttackedActorNames.clear();
        LastChatMessages.clear();
    }
}
