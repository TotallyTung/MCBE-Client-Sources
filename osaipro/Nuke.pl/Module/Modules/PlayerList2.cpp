#include "PlayerList2.h"
#include "../../DrawUtils.h"
#include "../../../Utils/ColorUtil.h"
#include "../../../Utils/ImGuiUtils.h"
PlayerListx::PlayerListx() : IModule(0x0, Category::VISUAL, "draws a list of nearby players next to the array list") {
    registerBoolSetting("Show Armour", &this->armour, this->armour);
    registerBoolSetting("Show Pos", &this->pos, this->pos);
    registerBoolSetting("Show Distance", &this->dist, this->dist);

    registerFloatSetting("pos-x", &this->posX, this->posX, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.x);
    registerFloatSetting("pos-y", &this->posY, this->posY, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.y);

    registerIntSetting("R-Friends", &this->rf, this->rf, 0, 255);
    registerIntSetting("G-Friends", &this->gf, this->gf, 0, 255);
    registerIntSetting("B-Friends", &this->bf, this->bf, 0, 255);

    registerIntSetting("R-Enemy", &this->re, this->re, 0, 255);
    registerIntSetting("G-Enemy", &this->ge, this->ge, 0, 255);
    registerIntSetting("B-Enemy", &this->be, this->be, 0, 255);

    registerIntSetting("R-Pos/Dist", &this->rpd, this->rpd, 0, 255);
    registerIntSetting("G-Pos/Dist", &this->gpd, this->gpd, 0, 255);
    registerIntSetting("B-Pos/Dist", &this->bpd, this->bpd, 0, 255);
}

PlayerListx::~PlayerListx() {
}

const char* PlayerListx::getModuleName() {
    return ("PlayerList");
}

struct CompareTargetEnArray {
    bool operator()(Entity* lhs, Entity* rhs) {
        LocalPlayer* localPlayer = Game.getLocalPlayer();
        return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
    }
};


static std::vector<Entity*> playerListx;

void findPlayerscc(Entity* currentEntity, bool isRegularEntity) {
    if (currentEntity == nullptr) return;
    if (!Game.getLocalPlayer()->canAttack(currentEntity, false)) return;
    if (!Game.getLocalPlayer()->isAlive()) return;
    if (!currentEntity->isAlive()) return;
    if (currentEntity->getEntityTypeId() != 319) return;
    if (currentEntity->getNameTag()->getTextLength() <= 1) return;
    if (currentEntity->isLocalPlayer()) return;
    std::string invalids[6] = {
    "-", ".", "/",
    "+", ",", "#"
    };
    for (auto invalid : invalids) {
        size_t invalidChar = std::string(currentEntity->getNameTag()->getText()).find(invalid);
        if (invalidChar != std::string::npos) return;
    }
    playerListx.push_back(currentEntity);
    std::sort(playerListx.begin(), playerListx.end(), CompareTargetEnArray());
}

void PlayerListx::onEnable() {
    playerListx.clear();
}

void PlayerListx::onPostRender(MinecraftUIRenderContext* renderCtx) {
    playerListx.clear();
    static auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
    auto player = Game.getLocalPlayer();
    if (player == nullptr) return;
    Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;

    if ((Game.getLocalPlayer() != nullptr) && Game.canUseMoveKeys()) {
        int runs2 = 0;
        Game.forEachEntity(findPlayerscc);
        //playerListx = player->getlevel()->getMiscEntityList();

        if (!playerListx.empty()) {
            for (auto& ye : playerListx) {
                Vec3* currPos = ye->getPos();
                char eeeee = *ye->getNameTag()->getText();
                std::string why = ye->getNameTag()->getText();
                std::string posi = std::to_string((int)floorf(currPos->x)) + ", " + std::to_string((int)floorf(currPos->y - 1)) + ", " + std::to_string((int)floorf(currPos->z));
                char dista[50];
                sprintf_s(dista, 50, "%.1fm", currPos->floor().dist(Game.getLocalPlayer()->eyePos0));
                why = Utils::sanitize(why); why = why.substr(0, why.find('\n'));
                std::string* isThisSoBroken = &why;

                Vec2 testdx = Vec2(windowSize.x - 262.5f, 10.f + (10.f * runs2)); // Adjusted y position
                if (!FriendsManager::findFriend(ye->getNameTag()->getText())) {
                    DrawUtils::drawText(Vec2(this->posX, this->posY + (10.f * runs2)), isThisSoBroken, MC_Color(re, ge, be, 0), 1.f, 1.f, true);
                }
                if (FriendsManager::findFriend(ye->getNameTag()->getText())) {
                    DrawUtils::drawText(Vec2(this->posX, this->posY + (10.f * runs2)), isThisSoBroken, MC_Color(rf, gf, bf, 0), 1.f, 1.f, true);
                }
                if (pos && !dist) {
                    DrawUtils::drawText(Vec2(this->posX + DrawUtils::getTextWidth(isThisSoBroken, 1.f), this->posY + (10.f * runs2)), &std::string(" [" + posi + "] "), MC_Color(rpd, gpd, bpd, 0), 1.f, 1.f, true);
                }
                if (!pos && dist) {
                    DrawUtils::drawText(Vec2(this->posX + DrawUtils::getTextWidth(isThisSoBroken, 1.f), this->posY + (10.f * runs2)), &std::string(" [" + std::string(dista) + "] "), MC_Color(rpd, gpd, bpd, 0), 1.f, 1.f, true);
                }
                if (pos && dist) {
                    DrawUtils::drawText(Vec2(this->posX + DrawUtils::getTextWidth(isThisSoBroken, 1.f), this->posY + (10.f * runs2)), &std::string(" {" + posi + "} [" + std::string(dista) + "] "), MC_Color(rpd, gpd, bpd, 0), 1.f, 1.f, true);
                }

                if (armour) {
                    static float constexpr scale2 = 0.6f;
                    static float constexpr opacity2 = 0.25f;
                    static float constexpr spacing2 = scale2 + 10.f;
                    float x2 = this->posX - 40;
                    float y2 = this->posY + (10.f * runs2);

                    for (int i2 = 0; i2 < 4; i2++) {
                        ItemStack* stack2 = ye->getArmor(i2);
                        if (stack2->item != nullptr) {
                            DrawUtils::drawItem(stack2, Vec2(x2, y2), opacity2, scale2, false);
                            x2 += scale * spacing2;
                        }
                    }
                }
                runs2++;
            }
        }
    }
}
void PlayerListx::onDisable() {
    playerListx.clear();
}

void PlayerListx::onJoiningServer() {
    playerListx.clear();
}