//
// Created by vastrakai on 1/10/2024.
//

#include "Minecraft.h"

#include "Internal/MinecraftGame.h"

// Define indexes
#define i_getLocalPlayer 29
#define i_getBlockSource 28
#define i_getOptions 189
#define i_getScreenName 261
#define i_getMouseGrabbed 159
#define i_disableInput 329
#define i_grabMouse 330
#define i_releaseMouse 331
#define i_playUI 338

// Define the direct access offsets
#define OFFSET_VIEWMATRIX 0x330
#define OFFSET_FOV_X 0x6F0
#define OFFSET_FOV_Y 0x704
#define OFFSET_TIMINGS 0xD0

glmatrixf* ClientInstance::GetViewMatrix()
{
    return &direct_access<glmatrixf>(this, OFFSET_VIEWMATRIX);
}

Vector2 ClientInstance::GetFOV()
{
    float x = direct_access<float>(this, OFFSET_FOV_X);
    float y = direct_access<float>(this, OFFSET_FOV_Y);

    return { x, y };
}

Actor* ClientInstance::ThePlayer()
{
    if (this == nullptr) return nullptr;
    return Memory::CallVFunc<i_getLocalPlayer, Actor*>(this);
}

BlockSource* ClientInstance::GetBlockSource()
{
    if (this == nullptr) return nullptr;
    return Memory::CallVFunc<i_getBlockSource, BlockSource*>(this);
}

Options* ClientInstance::GetOptions()
{
    if (this == nullptr) return nullptr;
    return Memory::CallVFunc<i_getOptions, Options*>(this);
}

GameTimings ClientInstance::GetTiming()
{
    //return *(*reinterpret_cast<class GameTimings**>(reinterpret_cast<__int64>(this) + 0xD0));
    // use direct_access
    return *direct_access<GameTimings*>(this, OFFSET_TIMINGS);
}

float ClientInstance::GetDeltaTime()
{
    return GetTiming().timer->deltaTime;
}

float ClientInstance::GetDeltaTime2()
{
    return GetTiming().renderTimer->deltaTime;
}

float ClientInstance::GetTimer()
{
    return GetTiming().timer->timer;
}

float ClientInstance::GetTimerMultiplier()
{
    return GetTiming().timer->timerMultiplier;
}

void ClientInstance::SetTimer(float timer)
{
    GetTiming().timer->timer = timer;
    GetTiming().renderTimer->timer = timer;
}

void ClientInstance::SetTimerMultiplier(float timerMultiplier)
{
    GetTiming().timer->timerMultiplier = timerMultiplier;
    GetTiming().renderTimer->timerMultiplier = timerMultiplier;
}

bool ClientInstance::isInHudScreen()
{
    return GetScreenName() == "hud_screen";
}

bool ClientInstance::isInDisconnectScreen()
{
    return GetScreenName().find("disconnect_screen") != std::string::npos;
}

/*SceneStack* ClientInstance::GetSceneStack()
{
    return Memory::CallVFunc<i_getSceneStack, SceneStack*>(this);
}*/

std::string ClientInstance::GetScreenName()
{
    if (this == nullptr)
        return "no_screen";
    std::string silly = "no_screen";
    silly = Memory::CallVFunc<i_getScreenName, std::string&, std::string&>(this, silly);
    return silly;
}

/*bool ClientInstance::isInGameInputEnabled()
{
    if (this == nullptr)
        return false;
    return Memory::CallVFunc<i_isInGameInputEnabled, bool>(this);
}

void ClientInstance::setInGameInputEnabled(bool enabled)
{
    if (this == nullptr)
        return;
    return Memory::CallVFunc<i_setInGameInputEnabled, void>(this, enabled);
}*/

/*bool ClientInstance::GetMouseGrabbed()
{
    return Memory::CallVFunc<i_getMouseGrabbed, bool>(this);
}*/

/*
bool ClientInstance::IsInGame()
{
    return Memory::CallVFunc<i_isInGame, bool>(this);
}*/

void ClientInstance::DisableInput(bool disable)
{
    return Memory::CallVFunc<i_disableInput, void>(this, disable);
}

void ClientInstance::GrabMouse()
{
    return Memory::CallVFunc<i_grabMouse, void>(this);
}

void ClientInstance::ReleaseMouse()
{
    return Memory::CallVFunc<i_releaseMouse, void>(this);
}

void ClientInstance::playUI(std::string str, float a1, float a2)
{
    if (this == nullptr)
        return;

    Memory::CallVFunc<i_playUI, void>(this, str, a1, a2);
}

void ClientInstance::playUiTest(int index) {
    void* func = vTable[index];
    // Call with args this, "beacon.activate", 1, 1.0f);
    Memory::CallFastcall<void, void*>(func, this);
}

UIProfanityContext* ClientInstance::GetProfanityContext()
{
    return mcGame->GetProfanityContext();
}

bgfx_context* ClientInstance::GetBGFX()
{
    static auto pCtx = Memory::ScanSig("48 8B 0D ? ? ? ? 66 44 89 74 24 ? E8 ? ? ? ? 0F B7 08 66 89 0F E9 ? ? ? ? 8B 53 3C 49 2B C8", 3);
    return *reinterpret_cast<bgfx_context**>(pCtx);
}

void* ClientInstance::GetNetEventCallback()
{
    return minecraft->gameSession->netEventCallback;
}