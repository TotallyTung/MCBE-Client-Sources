//
// Created by vastrakai on 1/5/2024.
//


#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include "src/Prax/Module/Module.h"

class Scaffold : public ModuleBase<Scaffold> {
public:
    enum class RotationMode {
        None,
        Normal,
        Down,
        Moonwalk,
        Pitch
    };

    enum TowerMode {
        Vanilla,
        Timer,
        Velocity,
        Smooth,
        Clip,
        ClipVel
    };

    enum class SwitchPriority {
        First,
        MostBlocks
    };

    enum BlockHudStyles {
        Original,
        New
    };

    enum SelectionMode {
        FullSwitch,
        Serverhold
    };

    enum PlaceMode {
        Normal,
        Flareon
    };

    enum AuthMode {
        ClientSide,
        ServerSide
    };

    ModeSetting RotateMode = ModeSetting("Rotate Mode", "The mode for rotation", {"None", "Normal", "Down", "Moonwalk", "Pitch"}, 1);
    ModeSetting PlacementMode = ModeSetting("Placement Mode", "The mode for block placement", {"Normal", "Flareon"}, 0);
    BoolSetting FlareonV2Placement = BoolSetting("Flareon V2 Placement", "Whether or not to use Flareon V2 placement", false);
    ModeSetting AuthMode = ModeSetting("Auth Mode", "The auth mode for placement", {"Client-Side", "Server-Side"}, 0);
    NumberSetting PlacesPerTick = NumberSetting("Places Per Tick", "The amount of blocks to place per tick", 0.01f, 30, 1, 0.01f);
    BoolSetting Queue = BoolSetting("Queue", "Queues blocks to place until Places Per Tick is met, then the blocks are placed all at once", true);
    NumberSetting DistanceSetting = NumberSetting("Distance", "The maximum distance for the block placing", 0, 18, 8, 0.01);
    NumberSetting ExtendSetting = NumberSetting("Extend", "The amount of blocks to extend forward", 0, 12, 3, 0.01);
    NumberSetting ExtendSpeed = NumberSetting("Extend Speed", "The speed at which the block extends", 1, 10, 10, 0.01);

    BoolSetting InstantClutch = BoolSetting("Instant Clutch", "Instantly clutches the block to the edge", false);
    NumberSetting ClutchSpeed = NumberSetting("Clutch Speed", "The speed at which the block clutches", 0, 10, 5, 0.01, &InstantClutch);

    ModeSetting TowerMode = ModeSetting("Tower Mode", "The mode for tower", {"Vanilla", "Timer", "Velocity", "Smooth", "Clip", "ClipVel"}, 2);
    NumberSetting TowerSpeed = NumberSetting("Tower Speed", "The speed at which the tower goes up", 0, 20, 8.5, 0.01);
    NumberSetting TowerSmoothing = NumberSetting("Tower Smoothing", "The smoothing for the tower. Only works with Smooth tower mode", 0, 1, 0.8f, 0.01);
    BoolSetting LockY = BoolSetting("Lock Y", "Locks the Y position to the block you are standing on", true);
    ModeSetting SelectionMode = ModeSetting("Selection Mode", "The mode for selecting blocks", {"Full Switch", "Serverhold"}, 0);
    ModeSetting SwitchPriority = ModeSetting("Switch Priority", "The priority for switching blocks", {"First", "Most Blocks"}, 0);
    BoolSetting HotbarOnly = BoolSetting("Hotbar Only", "Only selects blocks from the hotbar", true);
    BoolSetting SwitchBack = BoolSetting("Switch Back", "Switches back to the old slot after placing", true);
    NumberSetting SwitchBackDelay = NumberSetting("Switch Back Delay", "The delay before switching back (in ticks)", 0, 20, 5, 0.01, &SwitchBack);
    BoolSetting ShowGUI = BoolSetting("Block Amount HUD", "Whether or not to show a gui for the amount of blocks left", true);
    ModeSetting BlockHudStyle = ModeSetting("Block HUD Style", "The style for the block hud", {"Original", "New"}, 1, &ShowGUI);
    NumberSetting TickGap = NumberSetting("Tick Gap", "The amount of ticks to wait between placing blocks", 0, 20, 0, 1);
    BoolSetting Swing = BoolSetting("Swing", "Swings your hand while placing blocks", true);
    BoolSetting Andromeda = BoolSetting("Andromeda", "Makes the scaffold place above and below you", false);
    NumberSetting DownDelay = NumberSetting("Down Delay", "The delay before placing the block below you for down scaffold (in milliseconds)", 0, 1000, 500, 1);
    BoolSetting UseDiagSettings = BoolSetting("Use Diag Settings", "Whether or not to use diagonal settings", false);
    NumberSetting DiagYawOffset = NumberSetting("Diag Yaw Offset", "The yaw offset for diagonal", -180, 180, -45, 0.01, &UseDiagSettings);
    BoolSetting Underplace = BoolSetting("Underplace", "Underplace diag bypass", false);
    NumberSetting DiagPlacesPerTick = NumberSetting("Diag Places Per Tick", "The amount of blocks to place per tick while placing diagonally", 0.01f, 30, 1, 0.01f, &UseDiagSettings);
    NumberSetting DiagExtend = NumberSetting("Diag Extend", "The amount of blocks to extend forward while placing diagonally", 0, 12, 3, 0.01, &UseDiagSettings);
#ifdef __DEBUG__
    BoolSetting Test = BoolSetting("Test", "Test", false);
    BoolSetting TestDesync = BoolSetting("Test Desync", "Test Desync", false, &Test);
#endif

    Scaffold() : ModuleBase("Scaffold", "Places blocks under you", "World", 0, false) {
        RotateMode.Display = true;
        TickOnDisabled = true;
        PacketPriority = Priority::VeryLast;

        AddSetting(&RotateMode);
        AddSetting(&PlacementMode);
        AddSetting(&FlareonV2Placement);
        AddSetting(&AuthMode);
        AddSetting(&PlacesPerTick);
        AddSetting(&DistanceSetting);
        AddSetting(&ExtendSetting);
        AddSetting(&ExtendSpeed);
        AddSetting(&InstantClutch);
        AddSetting(&ClutchSpeed);
        AddSetting(&TowerMode);
        AddSetting(&TowerSpeed);
        AddSetting(&TowerSmoothing);
        AddSetting(&LockY);
        AddSetting(&SelectionMode);
        AddSetting(&SwitchPriority);
        AddSetting(&HotbarOnly);
        AddSetting(&SwitchBack);
        AddSetting(&SwitchBackDelay);
        AddSetting(&ShowGUI);
        AddSetting(&BlockHudStyle);
        AddSetting(&TickGap);
        AddSetting(&Swing);
        AddSetting(&Andromeda);
        AddSetting(&DownDelay);
        AddSetting(&UseDiagSettings);
        AddSetting(&DiagYawOffset);
        AddSetting(&Underplace);
        AddSetting(&DiagPlacesPerTick);
        AddSetting(&DiagExtend);
#ifdef __DEBUG__
        AddSetting(&Test);
        AddSetting(&TestDesync);
#endif

        DisableOnDimensionSwitch = true;
        RenderOnDisabled = true;
    }

    /* Static variables */
    static float BlockAmountAnimPerc;
    static float currentExtend;
    static float startY;
    static Vector3 LastBlock;
    static bool ShouldRotate;
    static int LastFace;
    static int LastSlot;
    static bool HasTowered;
    static uint64_t LastSwitchTime;
    static bool oldSpace;
    static bool IsDiaging;
    static float AnimationPerc;
    static bool HasTouchedGround;
    static std::map<Vector3, uint64_t> recentlyUpdatedBlocks;
    static std::map<Vector3, uint64_t> ghostedBlocks; // pretty much a copy of the above map
    static std::map<Vector3, uint64_t> recentlyPlacedBlocks;

    /* Module overrides */
    void OnTick() override;
    void OnEnable() override;
    void OnDisable() override;
    void OnRender() override;
    void OnPacket(LoopbackPacketSender* sender, Packet* packet, bool* cancel) override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;

    /* Module functions */
    Vector3 getRotationBasedPos(float extend, float yPos, float yawOffset);
    bool Tick();
    Vector3 GetBlockPlacePos(float yawDeg, float extend, float yPos = FLT_MAX, bool predict = true);
    void RenderOgBlockAmountDisplay();
    void RenderNewBlockAmountDisplay();



};