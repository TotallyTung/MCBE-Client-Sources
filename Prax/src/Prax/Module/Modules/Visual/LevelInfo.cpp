//
// Created by Flash on 1/11/2024.
//

#include "LevelInfo.h"

#include <imgui_internal.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/Render.h>

#include "Prax/Hook/Hooks/RakPeerHooks.h"

float LevelInfo::Bps = 0;
float LevelInfo::LastBps = 0;
float LevelInfo::AvgBps = 0;
std::map<uint64_t, float> LevelInfo::BpsHistory = std::map<uint64_t, float>();

#define NOW Misc::GetCurrentMs()

int pearlCount = 0;

void LevelInfo::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (ShowPearlCount.Enabled)
    {
        int pearls = 0;
        for (auto& stack : player->supplies->GetInventoryItems()) {
            if (!stack->item) continue;
            Item *item = *stack->item;
            if (item->GetName() == "ender_pearl") {
                pearls += stack->amount;
                break;
            }
        }

        pearlCount = pearls;
    }


    static Vector3 posPrev = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
    Vector3 pos = *player->getPos();
    if (posPrev.x == FLT_MAX) posPrev = pos;

    Vector2 posxz = Vector2(pos.x, pos.z);
    Vector2 posPrevxz = Vector2(posPrev.x, posPrev.z);
    LastBps = Bps;
    Bps = posxz.Distance(posPrevxz) * (Minecraft::ClientInstance->GetTimer() * Minecraft::ClientInstance->GetTimerMultiplier());

    // Add the new pair
    BpsHistory[NOW] = Bps + 0.0000001f;

    // Remove pairs older than 1 second
    for (auto it = BpsHistory.begin(); it != BpsHistory.end();)
    {
        if (NOW - it->first > 1000)
        {
            it = BpsHistory.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Average the pairs
    float total = 0;
    int count = 0;
    for (auto it = BpsHistory.begin(); it != BpsHistory.end(); ++it)
    {
        total += it->second;
        count++;
    }
    AvgBps = total / count;


    posPrev = pos;
}

void LevelInfo::OnRender() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    if (!Minecraft::ClientInstance->levelRenderer) return;
    if (Minecraft::ClientInstance->GetScreenName() != "hud_screen") return;

    ImGui::PushFont(Font::Fonts["greycliff_large"]);
    ImGui::GetCurrentContext()->FontSize = SizeSetting.Value;
    auto io = ImGui::GetIO();

    TextRenderInfo info;
    info.Color = ImColor((int)255, 255, 255);
    info.RenderShadow = true;
    info.Size = ImGui::GetFontSize();
    info.Pos = { 2, io.DisplaySize.y };

    if (ShowPosition.Enabled)
    {
        auto pos = Render::Transform.playerPos - player->GetHeightOffset();
        // Seperate the position by slashes per coordinate in this format: `X / Y / Z`
        // also round them to the nearest whole number
        info.Pos.y -= ImGui::GetFontSize() + 2;
        info.Text = "XYZ: " + std::to_string((int)round(pos.x)) + " / " + std::to_string((int)round(pos.y)) + " / " + std::to_string((int)round(pos.z));
        Render::RenderText(info);
    }


    if (ShowPearlCount.Enabled)
    {
        info.Pos.y -= ImGui::GetFontSize() + 2;
        info.Text = "Pearls: " + std::to_string(pearlCount);
        Render::RenderText(info);
    }

    if (ShowBps.Enabled)
    {
        float bps = Bps;
        if (AverageBps.Enabled) bps = AvgBps;
        info.Pos.y -= ImGui::GetFontSize() + 2;
        // Round the BPS to 2 decimal places
        std::string bpsStr = std::to_string(bps);
        bpsStr = bpsStr.substr(0, bpsStr.find(".") + 3);
        info.Text = "BPS: " + bpsStr;
        Render::RenderText(info);
    }

    if (ShowFps.Enabled)
    {
        info.Pos.y -= ImGui::GetFontSize() + 2;
        info.Text = "FPS: " + std::to_string((int)io.Framerate);
        Render::RenderText(info);
    }

    if (ShowPing.Enabled)
    {
        info.Pos.y -= ImGui::GetFontSize() + 2;
        info.Text = "Ping: " + std::to_string((int)RakPeerHooks::LastPing);
        Render::RenderText(info);
    }

    ImGui::PopFont();
}
