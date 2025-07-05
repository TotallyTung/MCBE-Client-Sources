//
// Created by Flash on 7/28/2023.
//

#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/Module/ModuleManager.h>
#include "ArrayList.h"

#include <Prax/UI/ThemeManager.h>

#include "HUD.h"


ImVec2 ArrayList::ScreenSize = ImVec2(0, 0);
ImDrawList* ArrayList::DrawList = nullptr;
float ArrayList::FontSize = 25.0f;
float Height = 25.f;

auto SortedModules = std::vector<Module*>();


struct TempRenderInfo {
    std::string correspondingModule;
    ImVec2 start;
    ImVec2 end;
    ImColor color;
    Module* modulePtr;
};

// TODO: Finish full arraylist outline

void ArrayList::Sort() {
    // Sort modules by calcTextSize
    // ImGui::CalcTextSize(str.c_str());

    SortedModules = ModuleManager::Modules;
    ImFont* font = Font::Fonts["greycliff_medium_large"];

    bool renderSettings = RenderMode.Enabled;

    std::ranges::sort(SortedModules, [font, renderSettings](Module* a,   Module* b) {


        float aSize = font->CalcTextSizeA(FontSize, FLT_MAX, 0.0f, Lowercase.Enabled ? String::ToLower(a->Name).c_str() : a->Name.c_str()).x;
        float bSize = font->CalcTextSizeA(FontSize, FLT_MAX, 0.0f, Lowercase.Enabled ? String::ToLower(b->Name).c_str() : b->Name.c_str()).x;

        if (renderSettings) {

            std::string aSettingText = a->GetSettingDisplayText();
            std::string bSettingText = b->GetSettingDisplayText();

            if (Lowercase.Enabled)
            {
                std::transform(aSettingText.begin(), aSettingText.end(), aSettingText.begin(), ::tolower);
                std::transform(bSettingText.begin(), bSettingText.end(), bSettingText.begin(), ::tolower);
            }

            if (!aSettingText.empty())
            {
                aSize +=  font->CalcTextSizeA(FontSize,
                                                      FLT_MAX,
                                                      0,
                                                      (" " + aSettingText).c_str()).x;
            }

            if (!bSettingText.empty())
            {
                bSize += font->CalcTextSizeA(FontSize,
                                                     FLT_MAX,
                                                     0,
                                                     (" " + bSettingText).c_str()).x;
            }

        }



        return aSize > bSize;
    });

}


void ArrayList::OnEnable() {
    Sort();
}


struct RenderInfo {
    Module* module;
    ImVec2 position;
    ImVec2 maxPosition;
    float delta;
    ImVec2 offset;
    int x;
    int y;
};

void ArrayList::OnTick() {
}

ImColor ArrayList::GetColor(int index)
{
    return HUD::GetColor(index);
}


std::vector<RenderInfo> renderInfos = std::vector<RenderInfo>();

void ArrayList::OnRender() {
    if (!Prax::ShouldUIRender || !Prax::Initialized || ImGui::GetBackgroundDrawList() == nullptr)
        return;

    if (DrawList == nullptr) {
        DrawList = ImGui::GetBackgroundDrawList();
    }

    Sort();


    ImGui::PushFont(Font::Fonts["greycliff_medium_large"]);

    // Get position to render
    const auto positionToRender = static_cast<Position>(PositionMode.SelectedIndex);

    // Get screen size
    ScreenSize = Minecraft::ClientInstance->guiData->resolution;

    float x = 0;
    float y = 0;
    float endX = 0;
    int index = 0;
    constexpr float padding = 2.0f;

    ImVec2 mousePos = ImGui::GetMousePos();

    if (positionToRender == BottomRight || positionToRender == BottomLeft) {
        y = ScreenSize.y - Height;
    }

    renderInfos.clear();

    for (const auto module: SortedModules) {
        if (module->HideInArraylist) continue;
        const float delta = ImGui::GetIO().DeltaTime;

        if (!module->Enabled && module->UIInfo.arrayListPercent <= 0.001f)
            continue;



        ImVec2 textSize =
                ImGui::GetFont()->CalcTextSizeA(FontSize,
                                                FLT_MAX,
                                                0,
                                                Lowercase.Enabled ? String::ToLower(module->Name).c_str() : module->Name.c_str());


        // If we are then add space for a space + the setting text
        if (RenderMode.Enabled && !module->GetSettingDisplayText().empty()) {
            std::string settingText = module->GetSettingDisplayText();
            if (Lowercase.Enabled) {
                std::transform(settingText.begin(), settingText.end(), settingText.begin(), ::tolower);
            }
            textSize.x += ImGui::GetFont()->CalcTextSizeA(FontSize,
                                                           FLT_MAX,
                                                           0,
                                                          (" " + settingText).c_str()).x;
        }

        module->UIInfo.arrayListPercent = Math::Animate(module->UIInfo.arrayListPercent, module->Enabled ? 1.f : 0.f,
                                                        delta * 12.f);
        module->UIInfo.arrayListPercent = std::clamp(module->UIInfo.arrayListPercent, 0.f, 1.f);

        // end x

        if (positionToRender == TopRight || positionToRender == BottomRight) {

            endX = ScreenSize.x - textSize.x - padding * 3;

            x = Math::Lerp(ScreenSize.x, endX, module->UIInfo.arrayListPercent);
        } else {
            endX = 0 - textSize.x - padding * 3;

            x = Math::Lerp(endX, 0, module->UIInfo.arrayListPercent);
        }


        if (RenderLine.Enabled)
        {
            // Move the position over if we are rendering a line
            if (positionToRender == TopRight || positionToRender == BottomRight) {
                x -= padding * 2;
            } else {
                x += padding * 1;
            }
        }

        const ImVec2 position = ImVec2(x, y);
        const ImVec2 maxPosition = ImVec2(position.x + textSize.x + padding * 4, position.y + Height + padding * 2);

        RenderInfo info = RenderInfo();
        info.module = module;
        info.position = position;
        info.maxPosition = maxPosition;
        info.delta = delta;
        info.offset = ImVec2(0, 0);
        info.x = x;
        info.y = y;
        renderInfos.push_back(info);

        if (positionToRender == BottomRight || positionToRender == BottomLeft) {
            y = Math::Lerp(y, y - (Height + padding * 2), module->UIInfo.arrayListPercent);
        } else {
            y = Math::Lerp(y, y + (Height + padding * 2), module->UIInfo.arrayListPercent);
        }

        index++;
    }

    index = 0;

    for (auto [module, position, maxPosition, delta, offset, x, y] : renderInfos) {

        auto color = GetColor(index);

        color.Value.w = 0.6f;

        DrawList->AddShadowRect(position,
                                maxPosition,
                                color,
                                125.0f, ImVec2(), ImDrawFlags_ShadowCutOutShapeBackground);
        // y
        index++;
    }

    index = 0;

    std::vector<TempRenderInfo> bgsToRender = std::vector<TempRenderInfo>();

    for (auto [module, position, maxPosition, delta, offset, x, y] : renderInfos) {

        // store all the strings needed here
        std::string moduleName = module->Name;
        std::string settingText = module->GetSettingDisplayText();
        if (Lowercase.Enabled) {
            std::transform(moduleName.begin(), moduleName.end(), moduleName.begin(), ::tolower);
            std::transform(settingText.begin(), settingText.end(), settingText.begin(), ::tolower);
        }

        ImVec2 textPosition = ImVec2(position.x + padding * 2, position.y + padding);

        bool isHovered = (mousePos.x > position.x && mousePos.x < maxPosition.x) &&
                         (mousePos.y > position.y && mousePos.y < maxPosition.y);

        if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            module->Toggle();
        }

        if (RenderBGSetting.Enabled)
            DrawList->AddRectFilled(position, maxPosition, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, isHovered ? 0.4f : 0.5f)));
        auto color = GetColor(index);

        bgsToRender.push_back(TempRenderInfo{ moduleName, position, maxPosition, color, module });


        DrawList->AddText(ImGui::GetFont(), FontSize, { textPosition.x + 2, textPosition.y + 2 }, ImColor(color.Value.x / 3, color.Value.y / 3, color.Value.z / 3, color.Value.w), moduleName.c_str());
        DrawList->AddText(ImGui::GetFont(), FontSize, textPosition, color, moduleName.c_str());


        textPosition.x += ImGui::GetFont()->CalcTextSizeA(FontSize,
                                                           FLT_MAX,
                                                           0,
                                                           moduleName.c_str()).x;

        if (RenderMode.Enabled)
        {
            ImColor modeSettingColor = ImColor(1.0f, 1.0f, 1.0f, 1.0f);

            DrawList->AddText(ImGui::GetFont(),
                              FontSize,
                              { textPosition.x + 2, textPosition.y + 2 },
                              ImGui::GetColorU32(ImVec4(modeSettingColor.Value.x / 3, modeSettingColor.Value.y / 3, modeSettingColor.Value.z / 3, modeSettingColor.Value.w / 3)),
                              (" " + settingText).c_str());
            DrawList->AddText(ImGui::GetFont(),
                              FontSize,
                              textPosition,
                              (ImU32)modeSettingColor,
                              (" " + settingText).c_str());
            textPosition.x += ImGui::GetFont()->CalcTextSizeA(FontSize,
                                                              FLT_MAX,
                                                              0,
                                                              settingText.c_str()).x;
        }

        if (RenderLine.Enabled) {
            // If its being rendered to the right then render the line after the text

            ImVec2 lineStart;
            ImVec2 lineEnd;

            if (PositionMode.SelectedIndex == BottomRight || PositionMode.SelectedIndex == TopRight)
            {
                lineStart = ImVec2(maxPosition.x, position.y);
                lineEnd = ImVec2(maxPosition.x + padding * 2, maxPosition.y);
                // Adjust the line thickness
                lineStart = ImVec2(lineStart.x - 2, lineStart.y);
            } else
            {
                lineStart = ImVec2(0, position.y);
                lineEnd = ImVec2(padding * 1, maxPosition.y);
            }



            DrawList->AddRectFilled(lineStart, lineEnd, color);

        }


        // y
        index++;
    }

    // Render the lines
    if (RenderFullOutline.Enabled)
    {
        std::vector<TempRenderInfo> linesToRender = std::vector<TempRenderInfo>();

        // based on the bg info, render the lines around the entire bg while using the color for the module
        // this should not have any lines that are behind the bg or any lines that are inside the bg
        // Sort the bgs by the y pos from lowest to highest
        std::ranges::sort(bgsToRender, [](TempRenderInfo a, TempRenderInfo b) {
            return a.start.y < b.start.y;
        });

        int bgi = 0;
        for (auto [moduleName, start, end, color, modulePtr] : bgsToRender)
        {
            float px = 0;

            if (PositionMode.SelectedIndex == TopRight)
            {
                // get all the bgs after this index
                std::vector<TempRenderInfo> bgsAfter = std::vector<TempRenderInfo>();
                for (int i = bgi + 1; i < bgsToRender.size(); i++)
                {
                    bgsAfter.push_back(bgsToRender[i]);
                }


                auto next = TempRenderInfo();

                if (!bgsAfter.empty())
                {
                    // Get the background with the lowest start x
                    std::ranges::sort(bgsAfter, [](TempRenderInfo a, TempRenderInfo b) {
                        return a.start.x < b.start.x;
                    });

                    next = bgsAfter.front();
                }


                float startxOffset = 0;
                float endxOffset = 0;
                float startyOffset = 0;
                float endyOffset = 1;

                if (!next.correspondingModule.empty() && next.start.x > start.x)
                {
                    // side: left
                    linesToRender.push_back(TempRenderInfo{ moduleName, ImVec2(start.x + startxOffset, start.y + startyOffset), ImVec2(start.x + endxOffset, end.y + endyOffset), color });
                    linesToRender.push_back(TempRenderInfo{ moduleName, ImVec2(end.x + startxOffset, start.y + startyOffset), ImVec2(end.x + endxOffset, end.y + endyOffset), color });
                } else if (next.correspondingModule.empty())
                {
                    // Push the lines anyways
                    linesToRender.push_back(TempRenderInfo{ moduleName, ImVec2(start.x + startxOffset, start.y + startyOffset), ImVec2(start.x + endxOffset, end.y + endyOffset), color });
                    linesToRender.push_back(TempRenderInfo{ moduleName, ImVec2(end.x + startxOffset, start.y + startyOffset), ImVec2(end.x + endxOffset, end.y + endyOffset), color });
                }

                float startxOffset2 = 0;
                float endxOffset2 = 0;
                float startyOffset2 = 0;
                float endyOffset2 = 0;
                // Top and bottom sides
                if (!next.correspondingModule.empty())
                {
                    // The end should be the next start
                    if (next.start.x < start.x) next.start.x = start.x;
                    linesToRender.push_back(TempRenderInfo{
                        //moduleName, ImVec2(start.x, end.y), ImVec2(next.start.x , end.y), color
                            moduleName, ImVec2(start.x + startxOffset2, end.y + startyOffset2), ImVec2(next.start.x + endxOffset2, end.y + endyOffset2), color
                    });
                } else {
                    // The end should be the end
                    if (next.start.x < start.x) next.start.x = start.x;
                    linesToRender.push_back(TempRenderInfo{
                            moduleName, ImVec2(start.x + startxOffset2, end.y + startyOffset2), ImVec2(end.x + endxOffset2, end.y + endyOffset2), color
                    });
                }
            }

            /*// If prev is empty and next isnt then render the top line
            if (prev.correspondingModule.empty() && !next.correspondingModule.empty())
            {
                linesToRender.push_back(TempRenderInfo{ moduleName, ImVec2(start.x, start.y), ImVec2(end.x, start.y), color });
            }*/

            bgi++;
        }

        // render the lines
        for (auto [_, start, end, color, mod] : linesToRender)
        {// start, end, color, thickness
            DrawList->AddLine(start, end, color, 2.f);

        }

        linesToRender.clear();

    }





    ImGui::PopFont();
}