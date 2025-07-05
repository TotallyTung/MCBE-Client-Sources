//
// Created by Flash on 10/15/2023.
//


#pragma once


#include "src/Prax/Utils/System/Memory.h"

class GuiMessage {
public:
    int what;
    int type;
    std::string message;
    std::string ttsMessage;
    std::string username;
    std::string fullMessage;
    std::string xuid;
    bool forceVisible;
    bool unknownBool;
private:
    char pad2[0x2];
public:
    float duration;
    bool hasBeenSeen;
private:
    char pad3[0x7];
};

static_assert(sizeof(GuiMessage) == 0xB8, "GuiMessage size is invalid");

class GuiData
{
public:
    BUILD_ACCESS(this, Vector2, resolution, 0x30);
    BUILD_ACCESS(this, Vector2, resolutionRounded, 0x38);
    BUILD_ACCESS(this, Vector2, resolutionScaled, 0x40);

    BUILD_ACCESS(this, float, guiScale, 0x4C);
    BUILD_ACCESS(this, float, scalingMultiplier, 0x50);
    BUILD_ACCESS(this, std::vector<GuiMessage>, guiMessages, 0x140);

    virtual void Constructor();
    virtual void onConfigChanged(uintptr_t Config);
private:
    virtual void Function4();
    virtual void Function5();
public:
    virtual void onLowMemory(void);
    virtual void onAppPaused(void);
    virtual void onAppUnpaused(void);
    virtual void onAppPreSuspended(void);
    virtual void onAppSuspended(void);
    virtual void onAppResumed(void);
    virtual void onAppFocusLost(void);
    virtual void onAppFocusGained(void);
    virtual void onAppTerminated(void);
    virtual void onOperationModeChanged(uintptr_t OperationMode);
    virtual void onPerformanceModeChanged(bool);
    virtual void onPushNotificationReceived(uintptr_t PushNotificationMessage);
    virtual void onResizeBegin(void);
    virtual void onResizeEnd(void);
    virtual void onDeviceLost(void);
};
