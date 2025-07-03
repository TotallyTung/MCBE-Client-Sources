#include "PlayerList.h"
#include "../../../../../../Utils/TimerUtil.h"
#include "../../../ModuleManager.h"

PlayerList::PlayerList() : Module("PlayerList", "Displays a stylish list of players.", Category::PLAYER) {
    registerSetting(new SliderSetting<float>("Size", "NULL", &size, 1.0f, 0.5f, 1.5f));
    registerSetting(new SliderSetting<float>("Opacity", "NULL", &backgroundOpacity, 0.85f, 0.0f, 1.0f));
    registerSetting(new SliderSetting<float>("Rounding", "NULL", &rounding, 12.0f, 0.0f, 30.0f));
    registerSetting(new SliderSetting<float>("Text Size", "NULL", &textSize, 0.85f, 0.5f, 1.2f));
    registerSetting(new SliderSetting<float>("Spacing", "NULL", &spacing, 4.0f, 2.0f, 8.0f));
    registerSetting(new BoolSetting("Glow", "Enable glow effect", &glow, false));
}

PlayerList::~PlayerList() {
}

std::vector<std::string> PlayerList::getCurrentPlayerList() {
    std::vector<std::string> playerList;
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    Level* level = localPlayer->level;
    if (level) {
        const auto& playerMap = level->getPlayerMap();
        for (const auto& pair : playerMap) {
            playerList.push_back(pair.second.mName);
        }
    }
    return playerList;
}

void PlayerList::onD2DRender() {
    if (!Game.isKeyDown(VK_TAB)) return;

    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (localPlayer == nullptr) return;

    std::vector<std::string> playerList = getCurrentPlayerList();
    size_t totalPlayers = playerList.size();

    Vec2<float> windowSize = Game.getClientInstance()->guiData->windowSizeReal;
    float columnWidth = 160.0f * size;
    float columnSpacing = 25.0f * size;
    float textPadding = spacing * size;
    int maxPlayersPerColumn = 20;
    float actualTextSize = textSize * size;

    UIColor accentUIColor(
        static_cast<uint8_t>(accentColor[0] * 255),
        static_cast<uint8_t>(accentColor[1] * 255),
        static_cast<uint8_t>(accentColor[2] * 255),
        255
    );

    UIColor backgroundColor(15, 15, 20, static_cast<uint8_t>(255 * backgroundOpacity));
    float backgroundPadding = 15.0f * size;
    float roundingValue = rounding * size;

    int numColumns = (totalPlayers > 40) ? 4 : (totalPlayers > 20) ? 2 : 1;
    int playersPerColumn = (totalPlayers + numColumns - 1) / numColumns;
    playersPerColumn = std::min(playersPerColumn, maxPlayersPerColumn);

    float titleHeight = D2D::getTextHeight("PlayerList", 1.1f) + 8.0f;
    float columnHeight = playersPerColumn * (D2D::getTextHeight("", actualTextSize) + textPadding);
    float totalHeight = columnHeight + titleHeight + 25.0f;
    float maxHeight = windowSize.y - 120.0f;
    totalHeight = std::min(totalHeight, maxHeight);

    float totalWidth = (numColumns * columnWidth) + ((numColumns - 1) * columnSpacing);
    float xOffset = (windowSize.x - totalWidth) / 2.0f;
    float yOffset = 60.0f;

    // Background rendering
    Vec2<float> bgMin(xOffset - backgroundPadding, yOffset - backgroundPadding);
    Vec2<float> bgMax(xOffset + totalWidth + backgroundPadding, yOffset + totalHeight + backgroundPadding);

    if (glow) {
        D2D::fillGlowRectangle(Vec4<float>(bgMin.x, bgMin.y, bgMax.x, bgMax.y), backgroundColor, 0.5f);
    }
    else {
        D2D::fillRoundingRectangle(Vec4<float>(bgMin.x, bgMin.y, bgMax.x, bgMax.y), backgroundColor, roundingValue);
    }

    // Draw border
    UIColor borderColor(
        static_cast<uint8_t>(accentColor[0] * 255),
        static_cast<uint8_t>(accentColor[1] * 255),
        static_cast<uint8_t>(accentColor[2] * 255),
        static_cast<uint8_t>(100 * backgroundOpacity)
    );
    D2D::drawRectangle(Vec4<float>(bgMin.x, bgMin.y, bgMax.x, bgMax.y), borderColor, 1.0f);

    float titleX = xOffset + (totalWidth - D2D::getTextWidth("PlayerList", 1.1f)) / 2.0f;
    D2D::drawText(Vec2<float>(titleX, yOffset), "PlayerList", accentUIColor, 1.1f);

    // Player count
    std::string playerCountText = "Players Online: " + std::to_string(totalPlayers);
    float countX = xOffset + (totalWidth - D2D::getTextWidth(playerCountText, 0.9f)) / 2.0f;
    D2D::drawText(Vec2<float>(countX, yOffset + titleHeight - 3.0f), playerCountText, UIColor(200, 200, 200, 255), 0.9f);

    // Player list
    size_t playerIndex = 0;
    for (int column = 0; column < numColumns; ++column) {
        float columnOffsetX = xOffset + column * (columnWidth + columnSpacing);
        float columnYOffset = yOffset + titleHeight + 20.0f;

        for (int row = 0; row < playersPerColumn; ++row) {
            if (playerIndex >= totalPlayers) break;

            std::string playerName = playerList[playerIndex];
            D2D::drawText(Vec2<float>(columnOffsetX, columnYOffset), playerName, UIColor(220, 220, 220, 255), actualTextSize);

            columnYOffset += D2D::getTextHeight("", actualTextSize) + textPadding;
            ++playerIndex;
        }
    }
}
