// Created by Tony on 2024-10-10 10:01:54

#include "PlayerListMenu.h"
#include "../../../Client.h"
#include "../../../../SDK/GameData.h"
#include "../../../../Utils/TargetUtils.h"
#include <cctype>  // for isprint and toupper
#include <algorithm>
#include <unordered_set>

PlayerListMenu::PlayerListMenu() : Module("PlayerListMenu", "Show player list", Category::CLIENT, VK_TAB) {
    windowPos = Vec2<float>(100.f, 100.f);
    searchQuery = "";
}

PlayerListMenu::~PlayerListMenu() {}

void PlayerListMenu::onEnable() {}

void PlayerListMenu::onDisable() {}

bool PlayerListMenu::isVisible() {
    return false;
}

void PlayerListMenu::Render(ImDrawList* drawlist) {
    if (!client->isInitialized()) return;

    static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");
    ClientInstance* ci = mc.getClientInstance();
    ImGuiIO& io = ImGui::GetIO();
    Vec2<float> mousePos = ci->getMousePos();

    static const float textSize = 1.f;
    const float textHeight = ImGuiUtils::getTextHeight(textSize);
    static const float textPadding = 2.f * textSize;

    float windowWidth = 300.f;
    float windowHeight = 400.f;
    float titleBarHeight = 30.f;

    Vec4<float> windowRect = Vec4<float>(windowPos.x, windowPos.y, windowPos.x + windowWidth, windowPos.y + windowHeight);
    Vec4<float> titleBarRect = Vec4<float>(windowPos.x, windowPos.y, windowPos.x + windowWidth, windowPos.y + titleBarHeight);

    // Handle dragging (only from title bar)
    static bool isDragging = false;
    static Vec2<float> dragOffset;

    if (titleBarRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
        isDragging = true;
        dragOffset = mousePos - windowPos;
    }

    if (isDragging) {
        if (ImGui::IsMouseDown(0)) {
            windowPos = mousePos - dragOffset;
        }
        else {
            isDragging = false;
        }
    }

    // Background
    ImGuiUtils::fillRectangle(windowRect, UIColor(29, 29, 29, 255), 10.f);
    ImGuiUtils::drawRectangle(windowRect, UIColor(60, 60, 60, 255), 2.f, 10.f);

    // Title bar
    ImGuiUtils::fillRectangle(titleBarRect, UIColor(45, 45, 45, 255), 10.f, ImDrawFlags_RoundCornersTop);
    std::string title = "Player List";
    float titleWidth = ImGuiUtils::getTextWidth(title, textSize * 1.2f);
    ImGuiUtils::drawText(Vec2<float>(windowPos.x + (windowWidth - titleWidth) / 2, windowPos.y + (titleBarHeight - textHeight) / 2),
        title, UIColor(255, 255, 255, 255), textSize * 1.2f);

    // Search bar
    static const float searchWidth = windowWidth - 20.f;
    static const float searchHeight = 30.f;

    Vec4<float> searchRectPos = Vec4<float>(windowPos.x + 10,
        windowPos.y + titleBarHeight + 10,
        windowPos.x + 10 + searchWidth,
        windowPos.y + titleBarHeight + 10 + searchHeight);

    Vec4<float> typeRectPos = Vec4<float>(searchRectPos.x + 5.f,
        searchRectPos.y + 5.f,
        searchRectPos.z - ImGuiUtils::getTextWidth("Search", textSize) - 10.f,
        searchRectPos.w - 5.f);

    if (typeRectPos.contains(mousePos) && ImGui::IsMouseClicked(0)) {
        isSearchFocused = true;
    }
    else if (ImGui::IsMouseClicked(0)) {
        isSearchFocused = false;
    }

    ImGuiUtils::fillRectangle(searchRectPos, UIColor(45, 45, 45, 255), 5.f);
    ImGuiUtils::fillRectangle(typeRectPos, UIColor(35, 35, 35, 255), 5.f);
    ImGuiUtils::drawText(Vec2<float>(typeRectPos.z + 5.f, typeRectPos.y + textPadding), "Search", UIColor(255, 255, 255, 255), textSize);

    Vec2<float> typeTextPos = Vec2<float>(typeRectPos.x + 5.f, typeRectPos.y + textPadding);

    // Render search query text
    float textWidth = ImGuiUtils::getTextWidth(searchQuery, textSize);
    if (!searchQuery.empty()) {
        drawlist->PushClipRect(ImVec2(typeRectPos.x + 5.f, typeRectPos.y), ImVec2(typeRectPos.z - 5.f, typeRectPos.w), true);
        if (typeRectPos.x + textWidth > typeRectPos.z - 15.f) {
            typeTextPos.x = typeRectPos.z - 15.f - textWidth;
        }
        ImGuiUtils::drawText(typeTextPos, searchQuery, UIColor(255, 255, 255, 255), textSize);
        drawlist->PopClipRect();
    }

    // Blinking cursor
    static float cursorBlinkTime = 0.0f;
    cursorBlinkTime += io.DeltaTime;
    if (isSearchFocused && fmodf(cursorBlinkTime, 1.0f) < 0.5f) {
        float cursorX = typeTextPos.x + textWidth;
        if (cursorX < typeRectPos.z - 5.f) {
            ImGuiUtils::fillRectangle(Vec4<float>(cursorX, typeTextPos.y, cursorX + 1, typeTextPos.y + textHeight), UIColor(255, 255, 255, 255));
        }
    }

    if (!isSearchFocused && searchQuery.empty()) {
        ImGuiUtils::drawText(typeTextPos, "Search for players...", UIColor(125, 125, 125, 255), textSize);
    }

    if (isSearchFocused) {
        static bool keyStates[256] = { false };
        static float keyPressTime[256] = { 0.0f };
        static const float KEY_REPEAT_DELAY = 0.5f;
        static const float KEY_REPEAT_RATE = 0.03f;

        for (int i = 0; i < 256; i++) {
            bool isKeyDown = mc.isKeyDown(i);

            if (isKeyDown && !keyStates[i]) {
                // Key just pressed
                keyStates[i] = true;
                keyPressTime[i] = 0.0f;

                if (i == VK_BACK) {
                    if (!searchQuery.empty()) searchQuery.pop_back();
                }
                else if (i == VK_RETURN) {
                    isSearchFocused = false;
                }
                else if (isprint(i)) {
                    char c = static_cast<char>(i);
                    bool isShiftDown = mc.isKeyDown(VK_SHIFT);
                    bool isCapsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

                    if (isShiftDown ^ isCapsLockOn) {
                        c = toupper(c);
                    }
                    else {
                        c = tolower(c);
                    }
                    searchQuery += c;
                }
            }
            else if (isKeyDown && keyStates[i]) {
                // Key held down
                keyPressTime[i] += io.DeltaTime;
                if (keyPressTime[i] > KEY_REPEAT_DELAY) {
                    float repeatDuration = keyPressTime[i] - KEY_REPEAT_DELAY;
                    if (fmodf(repeatDuration, KEY_REPEAT_RATE) < io.DeltaTime) {
                        if (i == VK_BACK) {
                            if (!searchQuery.empty()) searchQuery.pop_back();
                        }
                        else if (isprint(i)) {
                            char c = static_cast<char>(i);
                            bool isShiftDown = mc.isKeyDown(VK_SHIFT);
                            bool isCapsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

                            if (isShiftDown ^ isCapsLockOn) {
                                c = toupper(c);
                            }
                            else {
                                c = tolower(c);
                            }
                            searchQuery += c;
                        }
                    }
                }
            }
            else if (!isKeyDown && keyStates[i]) {
                // Key released
                keyStates[i] = false;
            }
        }
    }

    // Content area
    Vec4<float> contentRect = Vec4<float>(windowPos.x + 10, windowPos.y + titleBarHeight + searchHeight + 20,
        windowPos.x + windowWidth - 10, windowPos.y + windowHeight - 10);
    ImGuiUtils::fillRectangle(contentRect, UIColor(35, 35, 35, 255), 5.f);

    // Player list
    std::vector<std::string> playerList = getCurrentPlayerList();
    float yOffset = contentRect.y + textPadding;
    float contentHeight = (textHeight + textPadding * 3) * playerList.size();
    float visibleHeight = contentRect.w - contentRect.y;
    float maxScroll = std::max(0.f, contentHeight - visibleHeight);

    // Scrollbar
    float scrollbarWidth = 10.f;
    float scrollbarHeight = (visibleHeight / contentHeight) * visibleHeight;
    float scrollbarY = contentRect.y + (scrollOffset / maxScroll) * (visibleHeight - scrollbarHeight);
    Vec4<float> scrollbarRect = Vec4<float>(contentRect.z - scrollbarWidth, scrollbarY, contentRect.z, scrollbarY + scrollbarHeight);

    // Handle scrolling
    static bool isScrolling = false;
    static float scrollStartY = 0.f;
    static float scrollStartOffset = 0.f;

    if (contentRect.contains(mousePos)) {
        scrollOffset -= io.MouseWheel * 20;
    }

    if (scrollbarRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
        isScrolling = true;
        scrollStartY = mousePos.y;
        scrollStartOffset = scrollOffset;
    }

    if (isScrolling) {
        if (ImGui::IsMouseDown(0)) {
            float scrollDelta = (mousePos.y - scrollStartY) / (visibleHeight - scrollbarHeight) * maxScroll;
            scrollOffset = std::clamp(scrollStartOffset + scrollDelta, 0.f, maxScroll);
        }
        else {
            isScrolling = false;
        }
    }

    // Clicking on the scrollbar track
    if (Vec4<float>(scrollbarRect.x, contentRect.y, scrollbarRect.z, contentRect.w).contains(mousePos) && ImGui::IsMouseClicked(0)) {
        float clickPos = (mousePos.y - contentRect.y) / visibleHeight;
        scrollOffset = clickPos * maxScroll;
    }

    scrollOffset = std::clamp(scrollOffset, 0.f, maxScroll);

    drawlist->PushClipRect(ImVec2(contentRect.x, contentRect.y), ImVec2(contentRect.z, contentRect.w), true);

    for (const auto& playerName : playerList) {
        if (!searchQuery.empty() && playerName.find(searchQuery) == std::string::npos) {
            continue;
        }

        Vec4<float> playerRect = Vec4<float>(
            contentRect.x + textPadding,
            yOffset - scrollOffset,
            contentRect.z - textPadding - scrollbarWidth,
            yOffset + textHeight + textPadding * 2 - scrollOffset
        );

        if (playerRect.y + playerRect.w > contentRect.y && playerRect.y < contentRect.w) {
            ImGuiUtils::fillRectangle(playerRect, UIColor(45, 45, 45, 255), 5.f);

            // Player name
            ImGuiUtils::drawText(Vec2<float>(playerRect.x + textPadding, playerRect.y + textPadding),
                playerName, UIColor(255, 255, 255, 255), textSize);

            // Add/Remove friend button
            std::string buttonText = std::find(TargetUtils::Friend.begin(), TargetUtils::Friend.end(), playerName) != TargetUtils::Friend.end() ? "Remove" : "Add";
            float buttonWidth = ImGuiUtils::getTextWidth(buttonText, textSize) + textPadding * 2;
            Vec4<float> buttonRect = Vec4<float>(
                playerRect.z - buttonWidth - textPadding,
                playerRect.y + textPadding,
                playerRect.z - textPadding,
                playerRect.w - textPadding
            );

            ImGuiUtils::fillRectangle(buttonRect, colorsMod->getColor(), 5.f);
            ImGuiUtils::drawText(Vec2<float>(buttonRect.x + textPadding, buttonRect.y + textPadding),
                buttonText, UIColor(255, 255, 255, 255), textSize);

            if (buttonRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
                toggleFriend(playerName);
            }
        }

        yOffset += textHeight + textPadding * 3;
    }

    drawlist->PopClipRect();

    // Draw scrollbar
    ImGuiUtils::fillRectangle(Vec4<float>(scrollbarRect.x, contentRect.y, scrollbarRect.z, contentRect.w), UIColor(60, 60, 60, 255));
    ImGuiUtils::fillRectangle(scrollbarRect, UIColor(100, 100, 100, 255), 5.f);

    if (scrollbarHeight < contentRect.w - contentRect.y) {
        ImGuiUtils::fillRectangle(Vec4<float>(contentRect.z - 5, scrollbarY, contentRect.z, scrollbarY + scrollbarHeight), UIColor(100, 100, 100, 255), 2.5f);
    }
}

std::vector<std::string> PlayerListMenu::getCurrentPlayerList() {
    std::vector<std::string> playerList;
    Level* level = mc.getLocalPlayer()->getLevel();
    if (level) {
        const auto& playerMap = level->getPlayerMap();
        for (const auto& pair : playerMap) {
            playerList.push_back(pair.second.name);
        }
    }
    return playerList;
}

void PlayerListMenu::toggleFriend(const std::string& playerName) {
    auto it = std::find(TargetUtils::Friend.begin(), TargetUtils::Friend.end(), playerName);
    if (it != TargetUtils::Friend.end()) {
        TargetUtils::Friend.erase(it);
        mc.DisplayClientMessage("[%sMelody%s] %s%s removed from friends list!", DARK_PURPLE, WHITE, GREEN, playerName.c_str());
    }
    else {
        TargetUtils::Friend.push_back(playerName);
        mc.DisplayClientMessage("[%sMelody%s] %s%s added to friends list!", DARK_PURPLE, WHITE, GREEN, playerName.c_str());
    }
}