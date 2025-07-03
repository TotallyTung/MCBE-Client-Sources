
#include "DamageText.h"
#include <chrono>
#include "../../../../../Client.h"

DamageText::DamageText() : Module("DamageText", "Display message when ur hitting someone", Category::COMBAT) {}

static bool envaledcharr(char c) {
    return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
}

std::string sanitizexB(const std::string& text) {
    std::string out;
    bool wasValid = true;
    for (char c : text) {
        bool isValid = !envaledcharr(c);
        if (wasValid) {
            if (!isValid) {
                wasValid = false;
            }
            else {
                out += c;
            }
        }
        else {
            wasValid = isValid;
        }
    }
    return out;
}

void DamageText::onNormalTick(LocalPlayer* localPlayer) {
    if (localPlayer == nullptr) return;
    Level* level = localPlayer->level;
    if (level == nullptr) return;

    DamageList.clear();

    for (auto& ent : level->getRuntimeActorList()) {
        if (TargetUtil::isTargetValid(ent)) {
            DamageList.push_back(ent);
        }
    }

    for (Actor* ent : DamageList) {
        int damage = ent->getHurtTime();
        if (damage > 8) {
            static int counter = 0; 
            counter++;
            std::string playerName = *ent->getNameTag();
            playerName = sanitizexB(playerName);
            Client::DisplayClientMessage("%sEnemy damaged:%s %s %s ", MCTF::RED, MCTF::WHITE, playerName.c_str(), std::to_string(counter));
        }
    }
}