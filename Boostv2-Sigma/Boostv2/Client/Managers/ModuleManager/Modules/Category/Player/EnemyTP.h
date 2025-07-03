#pragma once
#include "../../ModuleBase/Module.h"

class EnemyTp : public Module {
private:
    std::vector<Actor*> targetListTP;
    bool works = false;
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
    int breakKeybind = 0x0; // keybind for teleport
    int why = 2;              // Y offset for teleport
    int timerV = 480;              // Y offset for teleport
    float closestDistance = 0;              // Y offset for teleport
    int speed = 1;            // Speed adjustment
    int cuid = 1;             // Custom block ID
    int i;                    // Iterator or index
    bool al = true;           // Anti-lava
    Vec3<float> goal;         // Goal position for teleport
    bool TP = false;          // Teleport flag
    bool tpdone = false;      // Teleport completed flag
    bool ab = true;           // Anti-bedrock
    bool as = true;           // Additional flag

public:
    EnemyTp();
    bool sortByDist(Actor* a1, Actor* a2);
    virtual void onEnable() override;
    virtual void onNormalTick(LocalPlayer* actor) override;
    void onSendPacket(Packet* packet) override;
    void onDisable();
};