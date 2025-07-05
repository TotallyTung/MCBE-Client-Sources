#pragma once

#include "BPSDisplay.h"

#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/D2D.h>
#include <Prax/Utils/Render/Font.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/SDK/Game/Network/Network.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/UI/ThemeManager.h>

void BPSDisplay::OnEnable() {

}

float BPSDisplay::bps = 0;
float BPSDisplay::lastBps = 0;
float BPSDisplay::avgBps = 0;

Vector3 posPrev = Vector3(0, 0, 0);

std::pair<uint64_t, float> bpsHistory[200];
std::vector<float> bpsHistoryVector = std::vector<float>();

void BPSDisplay::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    Vector3 pos = *player->getPos();

    Vector2 posxz = Vector2(pos.x, pos.z);
    Vector2 posPrevxz = Vector2(posPrev.x, posPrev.z);
    lastBps = bps;
    bps = posxz.Distance(posPrevxz) * (Minecraft::ClientInstance->GetTimer() * Minecraft::ClientInstance->GetTimerMultiplier());

    if (bpsHistoryVector.size() > 100)
        bpsHistoryVector.erase(bpsHistoryVector.begin());

    bpsHistoryVector.push_back(bps);

    uint64_t now = Misc::GetCurrentMs();
    auto newPair = std::make_pair(now, bps + 0.0000001f);
    // Add the new pair
    for (int i = 0; i < 100; i++) {
        if (bpsHistory[i].first == 0) {
            bpsHistory[i] = newPair;
            break;
        }
    }
    // Remove pairs older than 1 second
    for (int i = 0; i < 100; i++) {
        if (bpsHistory[i].first != 0 && now - bpsHistory[i].first > 1000) {
            bpsHistory[i] = std::make_pair((uint64_t)0, 0.f);
        }
    }

    // Average the pairs
    float total = 0;
    int count = 0;
    for (int i = 0; i < 100; i++) {
        if (bpsHistory[i].first != 0) {
            total += bpsHistory[i].second;
            count++;
        }
    }

    avgBps = total / (float)count;



    posPrev = pos;
}

void BPSDisplay::OnDisable() {
    bpsHistoryVector.clear();
}

static float f1 = 1.0f;
static float f2 = 1.0f;
static float f3 = 1.0f;
static float f4 = 1.0f;
static float f5 = 1.0f;
static float f6 = 1.0f;
void BPSDisplay::DrawSpeedometer(ImVec2 pos, float radius, float speed, float maxSpeed)
{

    ImGui::Begin("Debug");

    ImGui::SliderFloat("f1", &f1, 0, 360);
    ImGui::SliderFloat("f2", &f2, 0, 360);
    ImGui::SliderFloat("f3", &f3, 0, 360);
    ImGui::SliderFloat("f4", &f4, 0, 360);
    ImGui::SliderFloat("f5", &f5, 0, 360);
    ImGui::SliderFloat("f6", &f6, 0, 360);

    ImGui::End();


    ImGui::PushFont(Font::Fonts["greycliff_demibold"]);


    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    // Draw outer circle
    drawList->AddCircleFilled(pos, radius, IM_COL32(20, 20, 20, 255), 100);
    drawList->AddCircle(pos, radius, IM_COL32(10, 10, 10, 255), 100, 2.f);

    // Calculate angle based on speed and maxSpeed
    float angle = (speed / maxSpeed) * f1;



    // Calculate position of the needle
    ImVec2 needleEnd;
    needleEnd.x = pos.x + (radius * 0.9) * cos(angle * 3.14159265f / -f3);
    needleEnd.y = pos.y + (radius * 0.9) * sin(angle * 3.14159265f / f3);


    // Draw center circle
    drawList->AddCircleFilled(pos, radius * 0.25f, IM_COL32(30, 30, 30, 255), 100);

    // For each 10 km/h, draw a tick with text
    for (int i = 0; i <= maxSpeed; i++)
    {
        float tickAngle = (i / maxSpeed) * f4 - f4;

        // Calculate position of the tick
        ImVec2 tickEnd;
        tickEnd.x = pos.x + radius * cos(tickAngle * 3.14159265f / -f5);
        tickEnd.y = pos.y + radius * sin(tickAngle * 3.14159265f / f5);

        ImVec2 tickStart;
        tickStart.x = pos.x + (radius * 0.9f) * cos(tickAngle * 3.14159265f / -f5);
        tickStart.y = pos.y + (radius * 0.9f) * sin(tickAngle * 3.14159265f / f5);

        // Draw tick
        drawList->AddLine(tickStart, tickEnd, IM_COL32(255, 255, 255, 255), 2.0f);

        // Draw tick glow
        ImVec2* points = new ImVec2[4];
        points[0] = tickStart;
        points[1] = tickEnd;
        points[2] = tickStart;
        points[3] = tickEnd;

        drawList->AddShadowConvexPoly(points, 4, IM_COL32(255, 255, 255, 255), 25.0f, ImVec2(0, 0));


        // Draw text closer to the middle of the circle
        ImVec2 textPos;
        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, 0.0f, std::to_string(i).c_str());
        // Calculate position of the text
        textPos.x = pos.x + (radius * 0.8f) * cos(tickAngle * 3.14159265f / -f5) - textSize.x / 2;
        textPos.y = pos.y + (radius * 0.8f) * sin(tickAngle * 3.14159265f / f5) - textSize.y / 2;

        // Draw text using util
        TextRenderInfo textInfo = TextRenderInfo(std::to_string(i), textPos, 20, IM_COL32(60, 255, 50, 255));
        textInfo.RenderShadow = true;


        Render::DrawGlowText(textInfo);
        Render::DrawShadowText(textInfo, false);
    }


    // Draw needle
    drawList->AddLine(pos, needleEnd, IM_COL32(255, 0, 0, 255), 2.0f);

    // Add glow to the needle
    ImVec2* points = new ImVec2[3];
    points[0] = pos;
    points[1] = needleEnd;
    points[2] = ImVec2(pos.x, pos.y + 1);
    drawList->AddShadowConvexPoly(points, 3, IM_COL32(255, 0, 0, 255), 25.0f, ImVec2(0, 0));

    ImGui::PopFont();
}


void BPSDisplay::OnRender() {
    //DrawSpeedometer(ImVec2( 110, ImGui::GetIO().DisplaySize.y - 110), 100, avgBps, f1);

    // Check if we should render
    if (!Minecraft::ClientInstance->getmcGame()->canUseKeys()) return;


    if (displayMode.SelectedIndex == 1 || displayMode.SelectedIndex == 2) {

        float width = 150;
        float height = 50;
        float position = positionSetting.SelectedIndex == 0 ? 0.01f : 0.8f;
        // Try and get center of the screen
        ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y * position);
        center.x -= width / 2;

        ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
        ImGui::GetStyle().DisplaySafeAreaPadding = ImVec2(0, 0);
        ImGui::GetStyle().DisplayWindowPadding = ImVec2(0, 0);
        ImGui::GetStyle().TouchExtraPadding = ImVec2(0, 0);
        ImGui::GetStyle().WindowBorderSize = 0;

        ImGui::Begin(std::string("##BPSGraph").c_str(), nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse |
                     ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_NoInputs |
                     ImGuiWindowFlags_NoResize);

        ImGui::SetWindowPos(ImVec2(center.x, center.y), ImGuiCond_Always);


        // Set the window shadow to the shadow color
        ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] = ImColor(0, 0, 0, 255);
        ImGui::GetStyle().Colors[ImGuiCol_WindowShadow] = ImColor(0, 0, 0, 255);
        ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImColor(0, 0, 0, 150);
        ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] = ImColor(0, 0, 0, 150);
        ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = ImColor(0, 0, 0, 150);

        ImColor color = rgb.Enabled ?  ColorUtil::GetDefaultRainbow(1, ColorUtil::GlobalRGBInfo.Time,
                                                                                    ColorUtil::GlobalRGBInfo.Saturation,
                                                                                    ColorUtil::GlobalRGBInfo.Value) : ThemeManager::CurrentTheme->primaryColor.color;

        // Push line color to be the rgb of the rainbow
        ImGui::GetStyle().Colors[ImGuiCol_PlotLines] = color;

        // Plot the bps history
        ImGui::PlotLines("",
                         bpsHistoryVector.data(),
                         bpsHistoryVector.size(),
                         0, "", 0,
                         25, ImVec2(width, height));

        ImGui::End();
    }

    if (displayMode.SelectedIndex == 0 || displayMode.SelectedIndex == 2)
    {
        ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);

        std::string bpsStr = "BPS: ";
        if(boolSetting.Enabled) bpsStr += std::to_string(avgBps);
        else bpsStr += std::to_string(bps);

        // limit the float to 2 decimal places
        bpsStr = bpsStr.substr(0, bpsStr.find(".") + 3);


        // Render the string in the bottom-left hand corner of the screen using ImGui
        ImVec2 bottomLeftPos = ImVec2(5, ImGui::GetIO().DisplaySize.y - 32);
        // Draw a transparent background for the text

        auto color = rgb.Enabled ? ColorUtil::GetDefaultRainbow(1, ColorUtil::GlobalRGBInfo.Time, ColorUtil::GlobalRGBInfo.Saturation, ColorUtil::GlobalRGBInfo.Value) : ImColor(255, 255, 255, 255);


        ImVec2 bottomLeft = ImVec2(bottomLeftPos.x - 5, bottomLeftPos.y);
        ImVec2 topRight = ImVec2(bottomLeftPos.x + 5 + ImGui::GetFont()->CalcTextSizeA(28, FLT_MAX, 0.0f, bpsStr.c_str()).x,
                                 bottomLeftPos.y + 5 + 28);


        Math::DrawList->AddShadowRect(bottomLeft,
                                      topRight,
                                color,
                                200.0f, ImVec2(), ImDrawFlags_ShadowCutOutShapeBackground);

        Math::DrawList->AddRectFilled(bottomLeft,
                                      topRight, ImColor(0, 0, 0, 255 / 2));

        bottomLeftPos.y += 4;


        if (rgb.Enabled)
            Render::DrawRainbowText(bpsStr, bottomLeftPos, 28, ColorUtil::GlobalRGBInfo.Time, ColorUtil::GlobalRGBInfo.Saturation, ColorUtil::GlobalRGBInfo.Value, ColorUtil::GlobalRGBInfo.Separation);
        else
            Math::DrawList->AddText(Font::Fonts["greycliff_demibold_large"], 28, bottomLeftPos, color, bpsStr.c_str());
        ImGui::PopFont();
    }



}


