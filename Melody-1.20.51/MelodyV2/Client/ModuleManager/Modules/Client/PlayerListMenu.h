// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"
class PlayerListMenu : public Module {
public:
    PlayerListMenu();
    ~PlayerListMenu();

    void Render(ImDrawList* drawlist);
    virtual bool isVisible() override;
    virtual void onEnable() override;
    virtual void onDisable() override;

private:
    Vec2<float> windowPos;
    float scrollOffset = 0.f;
    std::string searchQuery;
    bool isSearchFocused = false;

    std::vector<std::string> getCurrentPlayerList();
    void toggleFriend(const std::string& playerName);
};
