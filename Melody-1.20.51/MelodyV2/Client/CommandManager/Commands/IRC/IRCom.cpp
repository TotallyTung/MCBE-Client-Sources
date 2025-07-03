// Created by Tony on 2024-10-10 10:01:54

// IRCOMMAND.cpp
#include "IRCom.h"
#include "../../CommandManager.h"
#include "../../../Client.h"
IRCOMMAND::IRCOMMAND() : Command("IRC", "Send messages to IRC", { "irc" }, "<u got some stupititys>") {}

void IRCOMMAND::execute(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        mc.getLocalPlayer()->displayClientMessage("Usage: .irc <message>");
        return;
    }

    auto* ircMod = static_cast<IRC*>(client->moduleMgr->getModule("IRC"));
    if (!ircMod || !ircMod->isEnabled()) {
        mc.getLocalPlayer()->displayClientMessage("IRC module is not enabled!");
        return;
    }

    std::string message = args[1];
    for (size_t i = 2; i < args.size(); ++i) {
        message += " " + args[i];
    }

    //bircMod->sendMessage(" :" + message);
    mc.getLocalPlayer()->displayClientMessage("IRC message sent: " + message);
}