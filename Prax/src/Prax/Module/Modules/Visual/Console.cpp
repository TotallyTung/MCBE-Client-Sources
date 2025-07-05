#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "Console.h"

#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/D2D.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/SDK/Game/Network/Network.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include <cmath>

bool _lastCanUseKeys = false;

static int _scrollOffset = 0;

void ConsoleMod::OnEnable() {
    _lastCanUseKeys = Minecraft::ClientInstance->mcGame->canUseKeys();
    Minecraft::ClientInstance->ReleaseMouse();
    _scrollOffset = 0;
}

void ConsoleMod::OnTick() {
    if (Minecraft::ClientInstance->GetScreenName() != "hud_screen")
        Minecraft::ClientInstance->DisableInput(true);
    else Minecraft::ClientInstance->DisableInput(false);

    if (KeyManager::Keys[VK_OEM_PLUS]) {
        _scrollOffset++;
    } else if (KeyManager::Keys[VK_OEM_MINUS] && _scrollOffset > 0) {
        _scrollOffset--;
    }

    if (KeyManager::Keys[VK_BACK]) {
        _scrollOffset = 0;
    }

}

void ConsoleMod::OnRender() {
    static float anim = 0;

    const auto delta = ImGui::GetIO().DeltaTime;

    if (this->Enabled) {
        anim = Math::Lerp(anim, 1, delta * 10 * (anim > 0.75 ? 2 * anim : (3 * (1 - anim))));
    } else {
        anim = Math::Lerp(anim, 0, delta * 10 * (anim > 0.75 ? 2 * anim : (3 * (1 - anim))));;
    }

    if (anim == 0) return;

    const auto size = ImGui::GetIO().DisplaySize;

    static float _width = 0, _height = 0;
    static float _x = 0, _y = 0;

    if (_width == 0) {
        _width = fmax(size.x / 2, 1450);
        _height = fmin(size.y - 100, 900);
        _x = size.x / 2 - _width / 2;
        _y = fmin(fmax(size.y / 2 - _height / 2, 38), 100);
    }

    static float _minWidth = 0;

    if (_minWidth == 0) {
        _minWidth = _width - 46;
    }

    const auto bgColor = ImColor(0, 0, 0, static_cast<int>(150 * anim));

    const float width = _minWidth + 46 * anim, height = _height, x = _x + (46 * (1 - anim)) / 2, y = _y + 62 * (1 - anim);

    Render::PushDrawList(ImGui::GetBackgroundDrawList());

    Render::DrawRect(ImVec2(x, y), ImVec2(x + width, y + height), bgColor);

    const auto accentColor = ImColor(255, 255, 255, static_cast<int>(255 * anim));
    
    Render::DrawGlowLine(ImVec2(x, y), ImVec2(x + width, y), accentColor, 1.5);
    Render::DrawGlowLine(ImVec2(x, y + height), ImVec2(x + width, y + height), accentColor, 1.5);
    Render::DrawGlowLine(ImVec2(x, y), ImVec2(x, y + height), accentColor, 1.5);
    Render::DrawGlowLine(ImVec2(x + width, y), ImVec2(x + width, y + height), accentColor, 1.5);

    Render::PushDrawList(ImGui::GetForegroundDrawList());

    Render::FillAreaWithBlur(3 * anim - 0.2, ImVec4(x, y, x + width, y + height));

    Render::DrawGlowLine(ImVec2(x, y), ImVec2(x + width, y), accentColor, 1.5);
    Render::DrawGlowLine(ImVec2(x, y + height), ImVec2(x + width, y + height), accentColor, 1.5);
    Render::DrawGlowLine(ImVec2(x, y), ImVec2(x, y + height), accentColor, 1.5);
    Render::DrawGlowLine(ImVec2(x + width, y), ImVec2(x + width, y + height), accentColor, 1.5);

    const auto font = Font::Fonts["sarabun_light"];

    ImGui::PushFont(font);

    const auto maxLines = static_cast<int>(height / 25);

    auto logs = Logger::GetLogs();

    const auto fgList = ImGui::GetForegroundDrawList();
    const auto bgList = ImGui::GetBackgroundDrawList();

    auto drawLine = [&](const char* line, const ImVec2& pos, const ImColor& color, float multi) {
        multi += 0.2;
        if (multi > 1) multi = 1;
        bgList->AddText(font, ceil((17 + 6 * multi) * 10) / 10.f, ImVec2(pos.x, pos.y + 1), color, line);
        fgList->AddText(font, ceil((17 + 6 * multi) * 10) / 10.f, ImVec2(pos.x, pos.y + 1), color, line);
    };

    const auto debugColor = ImColor(200, 200, 200, static_cast<int>(255 * anim));
    const auto infoColor = ImColor(162, 191, 254, static_cast<int>(255 * anim));
    const auto warnColor = ImColor(255, 105, 97, static_cast<int>(255 * anim));
    const auto errorColor = ImColor(194, 59, 34, static_cast<int>(255 * anim));

    int i = 0;

    for (auto& [message, level] : logs
        | std::views::reverse
        | std::views::filter([](const std::pair<std::string, int>& elem) -> bool { return elem.second != Logger::Debug || KeyManager::Keys[VK_LCONTROL] || KeyManager::Keys[VK_CONTROL] || KeyManager::Keys[VK_RCONTROL]; })
        | std::views::take(maxLines + _scrollOffset)
        | std::views::drop(_scrollOffset)) {
        drawLine(message.c_str(), ImVec2(x + 4, y + height - (i + 1.f) * 25), level == Logger::Debug ? debugColor : level == Logger::Info ? infoColor : level == Logger::Warning ? warnColor : errorColor, anim);
        i++;
    }

    ImGui::PopFont();

    Render::PopDrawList();
}

void ConsoleMod::OnDisable() {
    if (_lastCanUseKeys) Minecraft::ClientInstance->GrabMouse();
    Minecraft::ClientInstance->DisableInput(false);
}


