#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ClickGui.h"

#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/D2D.h>
#include <Prax/SDK/Game/Network/Network.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include <Prax/UI/ClickGui.h>
#include <Prax/UI/GUIManager.h>
#include <Prax/UI/NewClickGui.h>

bool lastCanUseKeys = false;

void ClickGuiMod::OnEnable() {
    lastCanUseKeys = Minecraft::ClientInstance->mcGame->canUseKeys();
    GUIManager::Visible = true;
    Minecraft::ClientInstance->ReleaseMouse();
}

void ClickGuiMod::OnTick() {
    GUIManager::Visible = true;
    if (Minecraft::ClientInstance->GetScreenName() != "hud_screen")
        Minecraft::ClientInstance->DisableInput(true);
    else Minecraft::ClientInstance->DisableInput(false);
}

void ClickGuiMod::OnRender() {
    GUIManager::Render(this);
}

void ClickGuiMod::OnDisable() {
    if (lastCanUseKeys) Minecraft::ClientInstance->GrabMouse();
    Minecraft::ClientInstance->DisableInput(false);
    GUIManager::Visible = false;
}


