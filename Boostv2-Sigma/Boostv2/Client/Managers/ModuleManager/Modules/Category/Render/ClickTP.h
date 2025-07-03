#pragma once
#include "../../ModuleBase/Module.h"
#include <vector>


class ClickTP : public Module {
private:
    std::vector<Actor*> targetListTP;
    bool works = false;
    bool isTeleporting = false;
    bool ad = false;          // Ancient Debris
    bool spawner = false;     // Spawner
    bool dOre = false;        // Diamond Ore
    bool eOre = false;        // Emerald Ore
    bool rOre = false;        // Redstone Ore
    bool gOre = false;        // Gold Ore
    bool iOre = false;        // Iron Ore
    bool cOre = false;        // Coal Ore
    bool cuOre = false;       // Custom Ore
    bool entity = false;      // Entity targeting
    int range = 50;           // Mining range
    int range2 = 4;           // Range for anti-lava checks
    bool border = true;       // Enable border checking
    int breakKeybind = 0x0;   // Keybind para teleport
    int why = 2;              // Y offset para teleport
    int timerV = 480;         // Timer value para teleport
    float closestDistance = 0;
    int speed = 1;            // Speed adjustment
    int cuid = 1;             // Custom block ID
    int i;                    // Iterator ou index
    bool al = true;           // Anti-lava
    Vec3<float> goal;         // Posição de destino para o teleport
    bool TP = false;          // Teleport flag
    bool tpdone = false;      // Teleport completed flag
    bool ab = true;           // Anti-bedrock
    bool as = true;           // Additional flag
    UIColor color = UIColor(255, 0, 0);
    int alpha = 0;
    int lineAlpha = 255;

    bool slide = false;
    float slideSpeed = 5.f;
    AABB oldRenderAABB;

    // Variável para armazenar a posição do bloco destacado (highlight)
    BlockPos highlightBlockPos;

public:
    float reachBlockk = 300;
    ClickTP();

    bool sortByDist(Actor* a1, Actor* a2);
    virtual void onEnable() override;
    virtual void onNormalTick(LocalPlayer* actor) override;
    virtual void onDisable() override;
    virtual void onLevelRender() override;
};
