#include "PlayerList2.h"
#include "../../DrawUtils.h"
#include "../../../Utils/ColorUtil.h"
#include "../../../Utils/ImGuiUtils.h"
PlayerListx::PlayerListx() : IModule(0x0, Category::VISUAL, "draws a list of nearby players next to the array list") {
    registerBoolSetting("Show Armour", &this->armour, this->armour);

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

    playerListx.push_back(currentEntity);
    std::sort(playerListx.begin(), playerListx.end(), CompareTargetEnArray());
}

void PlayerListx::onEnable() {
    playerListx.clear();
}

void PlayerListx::onPostRender(MinecraftUIRenderContext* renderCtx) {

    static auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
    auto player = Game.getLocalPlayer();
    if (player == nullptr) return;
    Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
 
    if ((Game.getLocalPlayer() != nullptr) && Game.canUseMoveKeys()) {
        int runs2 = 0;
        playerListx.clear();
        Game.forEachEntity(findPlayerscc);


        if (!playerListx.empty()) {
            for (auto& ye : playerListx) {
                Vec3* currPos = ye->getPos();
                char eeeee = *ye->getNameTag()->getText();
				std::string why = ye->getNameTag()->getText() + std::string(" | ") + std::to_string((int)floorf(currPos->x)) + ", " + std::to_string((int)floorf(currPos->y - 1)) + ", " + std::to_string((int)floorf(currPos->z)) + std::string("");

                why = Utils::sanitize(why); why = why.substr(0, why.find('\n'));
                std::string* isThisSoBroken = &why;

                Vec2 testdx = Vec2(windowSize.x - 262.5f, 10.f + (10.f * runs2)); // Adjusted y position
				DrawUtils::drawText(Vec2(windowSize.x - 260.f, 8.5f + (10.f * runs2)), isThisSoBroken, MC_Color(110, 0, 255, 0), 1.f, 1.f, true);
                if (ye->damageTime > 1) {
                    DrawUtils::drawRectangle(Vec4(testdx.x - 7.5, testdx.y, testdx.x, testdx.y + 7.5), MC_Color(255, 0, 0), 1.f, 1.f);

                }
                else {
                    DrawUtils::drawRectangle(Vec4(testdx.x - 7.5, testdx.y, testdx.x, testdx.y + 7.5), MC_Color(0, 255, 0), 1.f, 1.f);

                }

                if (armour) {
                    static float constexpr scale2 = 0.6f;
                    static float constexpr opacity2 = 0.25f;
                    static float constexpr spacing2 = scale2 + 10.f;
                    float x2 = windowSize.x - 310.f;
                    float y2 = 9.f;
                    y2 = 10.f + (10.f * runs2); // Adjusted y position

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