//
// Created by vastrakai on 9/15/2023.
//
#include "AutoQueue.h"

#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include <Prax/SDK/Game/Network/Other/CommandRequestPacket.h>
#include <Prax/SDK/Game/Network/Other/CommandOutputPacket.h>
#include <Prax/Utils/String.h>
#include <Prax/UI/Notifications.h>


std::string AutoQueue::LastGame = "sky";
std::map<uint64_t, std::string> QueuedCommands;

void SendCommandRequest(std::string command){
    QueuedCommands[Misc::GetCurrentMs()] = command;
}

bool AutoQueue::queueForGame = false;
uint64_t AutoQueue::lastQueueTime = 0;

void AutoQueue::QueueForGame() {
    SendCommandRequest("/q " + LastGame);
}

int tickslol = 0;
std::string lastNametagWithTeam = "";
uint64_t lastCommandExecuted = 0;

void AutoQueue::OnTick(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (String::ContainsIgnoreCase(player->_getNameTag(), "§")) {
        lastNametagWithTeam = player->_getNameTag();
    }

    if(!queueForGame) {
        tickslol++;
        if (tickslol > 700) {
            tickslol = 0;
            //SendCommandRequest("/connection");
        }
    }
    else if(Misc::GetCurrentMs() - lastQueueTime > QueueDelay.Value){
        queueForGame = false;
        QueueForGame();
        if(ShowInChat.Enabled) player->addChatMessage("§aQueued for game §b" + LastGame + "§a!", 0);
    }


    for (auto const& [_, val] : QueuedCommands)
    {
        if (Misc::GetCurrentMs() - lastCommandExecuted < 1000) continue;

        std::shared_ptr<CommandRequestPacket> commandPacket = MinecraftPackets::CreatePacket<CommandRequestPacket>();
        commandPacket->Command = val;
        Minecraft::ClientInstance->packetSender->sendToServer(commandPacket.get());
        Logger::Write("Queue", "Ran command " + val, Logger::LogType::Info);

        lastCommandExecuted = Misc::GetCurrentMs();

        // erase the command from the map
        QueuedCommands.erase(_);
        break;
    }
}

int64_t lastCommandOutput = 0;

void AutoQueue::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    auto p = packet.get();

    if (p->getId() == (int) PacketID::Text) {
        auto player = Minecraft::ClientInstance->ThePlayer();

        TextPacket *textPacket = reinterpret_cast<TextPacket *>(p);
        std::string text = textPacket->message.getText();

        if (String::ContainsIgnoreCase(text, "§c§l» §r§c§lGame OVER!") && QueueOnGameEnd.Enabled) {
            queueForGame = true;
            lastQueueTime = Misc::GetCurrentMs();
            Notifications::Notify("AutoQueue", "Queuing for " + LastGame + "!" , NotificationType::Info, 1.0f + (QueueDelay.Value / 1000.f));
        }

        // &9&l» &r&9&9Blue Team &7has been &cELIMINATED&7!
        // get the first two characters of the nametag
        if (!player) return;
        std::string playerName = player->_getNameTag();
        if (playerName.find("§r") != std::string::npos) playerName.erase(playerName.find("§r"), 2);
        if (playerName.find("§l") != std::string::npos) playerName.erase(playerName.find("§l"), 2);

        std::string playerTeam = "§" + playerName.substr(playerName.find("§") + 2, 1);

        if (String::ContainsIgnoreCase(text, "§7has been §cELIMINATED§7!") && String::StartsWith(text, playerTeam + "§l»") && QueueOnDeath.Enabled) {
            queueForGame = true;
            lastQueueTime = Misc::GetCurrentMs();
            Notifications::Notify("AutoQueue", "Queuing for " + LastGame + "!" , 1.0f + (QueueDelay.Value / 1000.f));
        }
        else if (String::ContainsIgnoreCase(text, "You are connected to public IP ")) {
            *cancel = true;
            return;
        }
        else if (String::ContainsIgnoreCase(text, "You are connected to internal IP ")) {
            *cancel = true;
            return;
        }
        else if (String::ContainsIgnoreCase(text, "You are connected to proxy ")) {
            *cancel = true;
            return;
        }
        else if (String::ContainsIgnoreCase(text, "You are connected to server name ")){
            std::string game = text.substr(text.find("server name ") + 12, text.length() - 1);
            // remove any numbers from the game name
            game.erase(std::remove_if(game.begin(), game.end(), ::isdigit), game.end());

            if(String::ToLower(LastGame) != String::ToLower(game) && ShowInChat.Enabled) {
                player->addChatMessage("§r§aConnected to game §b" + String::ToLower(game) + "§a!", 0);
            }
            LastGame = String::ToLower(game);
            *cancel = true;
        }
        else if (String::ContainsIgnoreCase(text, "You are connected to server ")){
            std::string game = text.substr(text.find("server ") + 7, text.length() - 1);
            // make sure the game name is lowercase contains less than 3 numbers
            if (std::count_if(game.begin(), game.end(), ::isdigit) > 3)
            {
                *cancel = true;
                return;
            }
            // remove any numbers from the game name
            game.erase(std::remove_if(game.begin(), game.end(), ::isdigit), game.end());

            if(String::ToLower(LastGame) != String::ToLower(game) && ShowInChat.Enabled) {
                player->addChatMessage("§r§aConnected to game §b" + String::ToLower(game) + "§a!", 0);
            }
            LastGame = String::ToLower(game);
            *cancel = true;
        }


    } else if (p->getId() == (int)PacketID::ChangeDimension) {
        if (!queueForGame && abs(Misc::GetCurrentMs() - lastCommandOutput) > 2000)
        {
            SendCommandRequest("/connection");
            lastCommandOutput = Misc::GetCurrentMs();
        }
    }
}

void AutoQueue::OnEnable() {
    SendCommandRequest("/connection");
}

