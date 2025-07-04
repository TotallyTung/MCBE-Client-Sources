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
        char errorMsg[] = "[%snhack%s] %sInvalid command! Usage: .(c,conf,confi) <saves/s> | <load/l> <confignames> | <deletes/d> <confignames> | <create/c> <confignames>  | <trash>";
        mc.DisplayClientMessage(errorMsg, BLUE, WHITE, RED);
        return;
    }

    std::string subCommand = args[1];
    switch (subCommand[0]) {
    case 's':
        if (subCommand == "saves") {
            configMgr->saveConfig();
            mc.DisplayClientMessage("[%snhack%s] %s Config saved successfully!", BLUE, WHITE, GREEN);
            NotificationsMod->addNotifBox("successfully save config", 5.0f);
        }
        else {
            mc.DisplayClientMessage("[%snhack%s] %sInvalid command! Usage: .(c,conf,confi) saves", BLUE, WHITE, RED);
        }
        break;
    case 'l':
        if (subCommand == "load") {
            if (args.size() < 3) {
                mc.DisplayClientMessage("[%snhack%s] %sInvalid command! Usage: .(c,conf,confi) load <confignames>", BLUE, WHITE, RED);
                return;
            }
            if (std::filesystem::exists(Utils::getClientPath() + "Configs\\" + args[2] + ".json")) {
                configMgr->loadConfig(args[2]);
                char output[100];
                sprintf(output, "[%snhack%s] %s Load config successfully!", BLUE, WHITE, GREEN, args[2].c_str());
                NotificationsMod->addNotifBox(output, 5.0f);
                mc.DisplayClientMessage(output);
            }
            else {
                mc.DisplayClientMessage("[%snhack%s] %s Config file does not exist!", BLUE, WHITE, RED);
            }
        }
        else {
            mc.DisplayClientMessage("[%snhack%s] %sInvalid command! Usage: .(c,conf,confi) load", BLUE, WHITE, RED);
        }
        break;
    case 'c':
        if (subCommand == "create") {
            if (args.size() < 3) {
                mc.DisplayClientMessage("[%snhack%s] %sInvalid command! Usage: .(c,conf,confi) create <confignames>", BLUE, WHITE, RED);
                return;
            }
            configMgr->createConfig(args[2]);
            char output[100];
            sprintf(output, "[%snhack%s] %s successfully Created %s", BLUE, WHITE, GREEN, args[2].c_str());
            NotificationsMod->addNotifBox(output, 5.0f);
            mc.DisplayClientMessage(output);
        }
        else if (subCommand == "conf" || subCommand == "confi") {
            // Handle command aliases
            mc.DisplayClientMessage("[%snhack%s] %sInvalid command! Usage: .(c,conf,confi) create", BLUE, WHITE, RED);
        }
        else {
            mc.DisplayClientMessage("[%snhack%s] %sInvalid command! Usage: .(c,conf,confi) create", BLUE, WHITE, RED);
        }
        break;
    case 'd':
        if (subCommand == "deletes") {
            if (args.size() < 3) {
                mc.DisplayClientMessage("[%snhack%s] %sInvalid command! Usage: .(c,conf,confi) deletes <confignames>", BLUE, WHITE, RED);
                return;
            }
            if (std::filesystem::exists(Utils::getClientPath() + "Configs\\" + args[2] + ".json")) {
                configMgr->deleteConfig(args[2]);
                char output[100];
                sprintf(output, "[%snhack%s] %s successfully deleted %s", BLUE, WHITE, GREEN, args[2].c_str());
                NotificationsMod->addNotifBox(output, 5.0f);
                mc.DisplayClientMessage(output);
            }
            else {
                mc.DisplayClientMessage("[%snhack%s] %s Config file does not exist!", BLUE, WHITE, RED);
            }
        }
        else {
            mc.DisplayClientMessage("[%snhack%s] %sInvalid command! Usage: .(c,conf,confi) deletes", BLUE, WHITE, RED);
        }
        break;
    case 't':
        if (subCommand == "trash") {
            configMgr->dumpDummyData();
        }
        break;
    default:
        mc.DisplayClientMessage("[%snhack%s] %sInvalid command!", BLUE, WHITE, RED);
        break;
    }

}
//configMgr->dumpDummyData();

