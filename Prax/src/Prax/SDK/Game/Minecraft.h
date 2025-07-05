#pragma once

#include <Prax/SDK/Game/Entity/Actor.h>
#include <Prax/SDK/Game/Render/GuiData.h>
#include <Prax/SDK/Game/Render/MinecraftUIRenderContext.h>
#include <Prax/SDK/Game/Internal/Options.h>
#include <Prax/SDK/Game/Internal/Timer.h>

#include "src/Prax/SDK/Game/World/Block.h"
#include "src/Prax/Utils/System/Internet.h"
#include "src/Prax/Utils/System/Memory.h"
#include "src/Prax/SDK/Game/Network/Network.h"
#include "src/Prax/SDK/Game/Misc/Structs.h"
#include "src/Prax/SDK/Game/Render/bgfx.h"

class UIProfanityContext
{
public:
    BUILD_ACCESS(this, bool, enabled, 0x18);
};

class SceneStack;

class GameSession
{
public:
    BUILD_ACCESS(this, void*, netEventCallback, 0x48);
};

class MinecraftC
{
public:
    BUILD_ACCESS(this, class GameSession*, gameSession, 0xC0);

};

class ClientInstance
{
public:
    BUILD_ACCESS(this, uintptr_t**, vTable, 0x0);
    BUILD_ACCESS(this, class ClientInstanceState*, clientInstanceState, 0xA0);
    BUILD_ACCESS(this, class MinecraftGame*, mcGame, 0xC8);
    BUILD_ACCESS(this, MinecraftC*, minecraft, 0xD0);
    BUILD_ACCESS(this, class LevelRenderer*, levelRenderer, 0xE0);
    BUILD_ACCESS(this, class LoopbackPacketSender*, packetSender, 0xF0);
    BUILD_ACCESS(this, class ClientInputHandler*, inputHandler, 0x110);
    BUILD_ACCESS(this, class KeyboardManager*, keyboardManager, 0x118);
    BUILD_ACCESS(this, class UIProfanityContext*, uiProfanityContext, 0x4D8);
    BUILD_ACCESS(this, class ActorRenderDispatcher*, actorRenderDispatcher, 0x530);
    BUILD_ACCESS(this, class GuiData*, guiData, 0x558);

    static bgfx_context* GetBGFX();

    void* GetNetEventCallback();
    void LogAllIndexes();
    glmatrixf* GetViewMatrix();
    Vector2 GetFOV();
    Actor* ThePlayer();
    BlockSource* GetBlockSource();
    Options* GetOptions();
    GameTimings GetTiming();
    float GetDeltaTime();
    float GetDeltaTime2();
    float GetTimer();
    float GetTimerMultiplier();
    void SetTimer(float timer);
    void SetTimerMultiplier(float timerMultiplier);
    bool isInHudScreen();
    bool isInDisconnectScreen();
    SceneStack* GetSceneStack();
    std::string GetScreenName();
    void setInGameInputEnabled(bool enabled);
    bool GetMouseGrabbed();
    bool IsInGame();
    void DisableInput(bool disable);
    void GrabMouse();
    void ReleaseMouse();
    void playUI(std::string str, float a1, float a2);
    void playUiTest(int index);
    UIProfanityContext* GetProfanityContext();
};

namespace Minecraft
{
    inline ClientInstance* ClientInstance = nullptr;

};
