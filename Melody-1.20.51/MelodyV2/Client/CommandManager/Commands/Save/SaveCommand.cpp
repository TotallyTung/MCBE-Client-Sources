// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../Command.h"
#include "SaveCommand.h"
#include "../../CommandManager.h"
#include "../../../Client.h"

Save::Save() : Command("config", "configs settings", { "c", "conf", "confi" }, "<u got some stupititys>") {}

void Save::execute(const std::vector<std::string>& args) {
    static Notifications* NotificationsMod = (Notifications*)client->moduleMgr->getModule("Notifications");
    configMgr = new ConfigManager();
    if (args.size() < 2) {
        char errorMsg[] = "[%sMelody%s] %sInvalid command! Usage: .(c,conf,confi) <saves/s> | <load/l> <confignames> | <deletes/d> <confignames> | <create/c> <confignames>  | <trash>";
        mc.DisplayClientMessage(errorMsg, DARK_PURPLE, WHITE, RED);
        return;
    }

    std::string subCommand = args[1];
    switch (subCommand[0]) {
    case 's':
        if (subCommand == "saves") {
            configMgr->saveConfig();
            mc.DisplayClientMessage("[%sMelody%s] %s Config saved successfully!", DARK_PURPLE, WHITE, GREEN);
            NotificationsMod->addNotifBox("successfully save config", 5.0f);
        }
        else {
            mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .(c,conf,confi) saves", DARK_PURPLE, WHITE, RED);
        }
        break;
    case 'l':
        if (subCommand == "load") {
            if (args.size() < 3) {
                mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .(c,conf,confi) load <confignames>", DARK_PURPLE, WHITE, RED);
                return;
            }
            if (std::filesystem::exists(Utils::getClientPath() + "Configs\\" + args[2] + ".json")) {
                configMgr->loadConfig(args[2]);
                char output[100];
                sprintf(output, "[%sMelody%s] %s Load config successfully!", DARK_PURPLE, WHITE, GREEN, args[2].c_str());
                NotificationsMod->addNotifBox(output, 5.0f);
                mc.DisplayClientMessage(output);
            }
            else {
                mc.DisplayClientMessage("[%sMelody%s] %s Config file does not exist!", DARK_PURPLE, WHITE, RED);
            }
        }
        else {
            mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .(c,conf,confi) load", DARK_PURPLE, WHITE, RED);
        }
        break;
    case 'c':
        if (subCommand == "create") {
            if (args.size() < 3) {
                mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .(c,conf,confi) create <confignames>", DARK_PURPLE, WHITE, RED);
                return;
            }
            configMgr->createConfig(args[2]);
            char output[100];
            sprintf(output, "[%sMelody%s] %s successfully Created %s", DARK_PURPLE, WHITE, GREEN, args[2].c_str());
            NotificationsMod->addNotifBox(output, 5.0f);
            mc.DisplayClientMessage(output);
        }
        else if (subCommand == "conf" || subCommand == "confi") {
            // Handle command aliases
            mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .(c,conf,confi) create", DARK_PURPLE, WHITE, RED);
        }
        else {
            mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .(c,conf,confi) create", DARK_PURPLE, WHITE, RED);
        }
        break;
    case 'd':
        if (subCommand == "deletes") {
            if (args.size() < 3) {
                mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .(c,conf,confi) deletes <confignames>", DARK_PURPLE, WHITE, RED);
                return;
            }
            if (std::filesystem::exists(Utils::getClientPath() + "Configs\\" + args[2] + ".json")) {
                configMgr->deleteConfig(args[2]);
                char output[100];
                sprintf(output, "[%sMelody%s] %s successfully deleted %s", DARK_PURPLE, WHITE, GREEN, args[2].c_str());
                NotificationsMod->addNotifBox(output, 5.0f);
                mc.DisplayClientMessage(output);
            }
            else {
                mc.DisplayClientMessage("[%sMelody%s] %s Config file does not exist!", DARK_PURPLE, WHITE, RED);
            }
        }
        else {
            mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .(c,conf,confi) deletes", DARK_PURPLE, WHITE, RED);
        }
        break;
    case 't':
        if (subCommand == "trash") {
            configMgr->dumpDummyData();
        }
        break;
    default:
        mc.DisplayClientMessage("[%sMelody%s] %sInvalid command!", DARK_PURPLE, WHITE, RED);
        break;
    }

}
//configMgr->dumpDummyData();

