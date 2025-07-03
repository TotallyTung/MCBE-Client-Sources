#pragma once

#include "../../Module.h"

class TestModule : public Module {
public:
    TestModule(Category* c) : Module(c, "TestModule", "Example module") {}

public:
    TestModule(); // Default constructor

    void onInitialize();
    void RenderEnemyPlayer(Actor* enemyPlayer);
    void RenderEnemyPlayer(Actor* enemyPlayer, Actor* localPlayer);
    void onImGuiRender() override;
    void onEnable(void) override;
    void drawFPSCounter();
    static void InitializeIcons();


private:
    double lastFrameTime = 0.0;
    double lastCounterUpdateTime = 0.0;
    int displayedFPS = 0;
    bool isDragging = false;
};