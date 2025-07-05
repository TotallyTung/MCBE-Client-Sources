#pragma once

#include "ClickGui.h"

#include <imgui.h>
#include <imgui_toggle.h>
#include <Prax/Utils/Math.h>
#include <Prax/UI/ThemeManager.h>
#include <Prax/Utils/Render/D2D.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/Utils/System/KeyManager.h>
#include <Prax/Module/Modules/Visual/ClickGui.h>

#include "GUIManager.h"

std::vector<std::pair<std::string, CategoryUIElement*>> categoryList;

int initIndex = 0;

bool ClickGui::Binding = false;

void ClickGui::ShowToolTip(const std::string& text) {
    ImGui::PopFont();
    ImGui::PushFont(Font::Fonts["greycliff_demibold"]);
    ImGui::SetItemTooltip("%s", text.c_str());
    ImGui::PopFont();
    ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);
}

static bool ShowAllEyeIcons = false;

static float newGUIScale = 0.5f;

void ClickGui::RenderCategoryBoxImGui(CategoryUIElement* categoryCtx) {
    if (!GUIManager::Visible) return;
    categoryCtx->settingIndex = 0;
    categoryCtx->moduleIndex = 0;

    ImGui::PopFont();
    ImGui::PushFont(Font::Fonts["inter"]);
    ImGui::PopFont();
    ImGui::PushFont(Font::Fonts["inter_large"]);

    if (Minecraft::ClientInstance->guiData->guiScale == 3.0f)
        newGUIScale = 1.0f;
    else if (Minecraft::ClientInstance->guiData->guiScale == 2.0f)
        newGUIScale = 0.85f;
    else
        newGUIScale = 1.0f;

    // Clamp the minimum gui scale to 0.85
    if (newGUIScale < 0.95f)
        newGUIScale = 0.95f;

    categoryCtx->GuiScale = newGUIScale;

    categoryCtx->Size = ImVec2(categoryCtx->GetLength(), categoryCtx->GetFullHeight(false));

    ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
    ImGui::GetStyle().DisplaySafeAreaPadding = ImVec2(0, 0);
    ImGui::GetStyle().DisplayWindowPadding = ImVec2(0, 0);
    ImGui::GetStyle().TouchExtraPadding = ImVec2(0, 0);
    ImGui::GetStyle().WindowBorderSize = 0;

    ImGui::Begin(std::string("##CategoryBox" + categoryCtx->Name).c_str(), nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoBackground |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoFocusOnAppearing |
                 ImGuiWindowFlags_NoResize);




    // Set the window shadow to the shadow color
    ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] = ThemeManager::CurrentTheme->shadowColor;
    ImGui::GetStyle().Colors[ImGuiCol_WindowShadow] = ThemeManager::CurrentTheme->shadowColor;



    if (categoryCtx->firstRender) {
        categoryCtx->firstRender = false;
        ImGui::SetWindowPos(categoryCtx->Position);
    }
    categoryCtx->Position = ImGui::GetWindowPos();
    ImGui::SetWindowSize(categoryCtx->Size);

    std::string categoryText = categoryCtx->Name;

    // Get ClickGui module to get sorting mode
    static auto* clickGuiModule = ModuleManager::GetModule<ClickGuiMod>();

    if (clickGuiModule->SortingMode.SelectedIndex == 0) {
        // Sort the modules by name
        std::sort(categoryCtx->Modules.begin(), categoryCtx->Modules.end(), [](const std::pair<Module*, ModuleUIElement*>& a,
                                                                               const std::pair<Module*, ModuleUIElement*>& b) {
            return a.first->Name < b.first->Name;
        });
    } else {
        // Sort the modules by name length
        std::sort(categoryCtx->Modules.begin(), categoryCtx->Modules.end(), [](const std::pair<Module*, ModuleUIElement*>& a,
                                                                               const std::pair<Module*, ModuleUIElement*>& b) {
            return ImGui::GetFont()->CalcTextSizeA(20, FLT_MAX, 0, a.first->Name.c_str()).x >
                   ImGui::GetFont()->CalcTextSizeA(20, FLT_MAX, 0, b.first->Name.c_str()).x;
        });
    }
    std::vector<std::pair<Module*, ModuleUIElement*>> modules = categoryCtx->Modules;


    auto windowDrawList = ImGui::GetWindowDrawList();

    // Calculate the size of the category box with all the modules
    categoryCtx->CurrentHeight = categoryCtx->GetFullHeight();

    ImVec2 categoryBoxBoundsEnd(categoryCtx->Position.x + categoryCtx->GetLength(),
                                categoryCtx->GetFullHeight());
    ImVec4 categoryBoxBounds(categoryCtx->Position.x, categoryCtx->Position.y, categoryBoxBoundsEnd.x,
                             categoryBoxBoundsEnd.y);

    // Create a rectangle to house the modules
    D2D::AddBlur(windowDrawList, ThemeManager::CurrentTheme->blur, categoryBoxBounds); // best blur
    windowDrawList->AddRectFilled(categoryCtx->Position, categoryBoxBoundsEnd, ThemeManager::CurrentTheme->backgroundColor1); // best rect}]


    // Render category header
    ImVec4 headerBounds(categoryCtx->Position.x, categoryCtx->Position.y,
                        categoryCtx->Position.x + categoryCtx->GetLength(),
                        categoryCtx->Position.y + categoryCtx->GetHeaderHeight());
    windowDrawList->AddRectFilled(categoryCtx->Position, {headerBounds.z, headerBounds.w}, (ImU32)ThemeManager::CurrentTheme->backgroundColor1); // best rect}]

    ImGui::PopFont();
    ImGui::PushFont(Font::Fonts["greycliff_bold_large"]);
    // Render the category text
    // Get the center of the category categoryCtx->Position
    ImVec2 categoryTextSize = ImGui::GetFont()->CalcTextSizeA(categoryCtx->GetCategoryFontSize(), FLT_MAX, 0,
                                                              categoryText.c_str());
    ImVec2 categoryTextPos((categoryCtx->Position.x + categoryCtx->GetLength() / 2) - (categoryTextSize.x / 2),
                           categoryCtx->Position.y + categoryCtx->GetHeaderHeight() / 2 - categoryTextSize.y / 2);


    // Add shadow
    windowDrawList->AddText(ImGui::GetFont(), categoryCtx->GetCategoryFontSize(), {categoryTextPos.x + 1, categoryTextPos.y + 1},
                            static_cast<ImU32>(ThemeManager::CurrentTheme->categoryTextColor.GetShadowColor()), categoryText.c_str());

    windowDrawList->AddText(ImGui::GetFont(), categoryCtx->GetCategoryFontSize(), categoryTextPos, static_cast<ImU32>(ThemeManager::CurrentTheme->categoryTextColor),
                            categoryText.c_str());


    ImGui::PopFont();
    ImGui::PushFont(Font::Fonts["greycliff_demibold"]);
    for (auto modulePair: modules) {
        Module *module = modulePair.first;
        auto moduleUIElement = modulePair.second;

        std::string moduleText = module->Binding ? "Binding..." : module->Name;

        // If any module is binding then Binding is true
        // if none of the modules are binding then Binding is false
        if (module->Binding) {
            Binding = true;
        }


        if (module->Binding) {
            if (auto key = KeyManager::GetFirstDown(); key != -1 && key != 'W' && key != 'A' && key != 'S' && key != 'D' && key != 0) {
                Minecraft::ClientInstance->playUI("random.orb", 0.75f, 1.0f);
                module->KeyId = key;
                module->Binding = false;
                Binding = false;

            } else if (key == 0){
                // Play the breaking sound
                Minecraft::ClientInstance->playUI("random.break", 0.75f, 1.0f);
                module->Binding = false;
                module->KeyId = 0;
                Binding = false;
            }
        }


        // Get the center of the module button with the text in it
        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(categoryCtx->GetModuleFontSize(), FLT_MAX,
                                                          0, moduleText.c_str());
        ImGui::SetCursorPos({0, categoryCtx->GetCurrentRenderY(false)});


        ImGui::InvisibleButton(std::string("##ModuleButton" + module->Name + std::to_string(
                                       categoryCtx->moduleIndex)).c_str(),
                               ImVec2(categoryCtx->GetLength(), categoryCtx->GetModuleHeight()), ImGuiButtonFlags_MouseButtonMiddle);
        ShowToolTip(module->Description);

        ImGui::PushFont(Font::Fonts["jetbrains_mono_large"]);

        ImVec2 expandTextSize = { 0, 0 };
        if (!module->Settings.empty()) {
            expandTextSize = ImGui::GetFont()->CalcTextSizeA(categoryCtx->GetModuleFontSize() + 5, FLT_MAX,
                                                             0, module->Expanded ? "-" : "+");
            ImVec2 expandTextPos = ImVec2(categoryCtx->Position.x + categoryCtx->GetLength() - expandTextSize.x - categoryCtx->GetPadding(),
                                          categoryCtx->GetCurrentRenderY() + categoryCtx->GetModuleHeight() / 2 - expandTextSize.y / 2);

            // Add shadow
            windowDrawList->AddText(ImGui::GetFont(), categoryCtx->GetModuleFontSize() + 5, {expandTextPos.x + 1, expandTextPos.y + 1},
                                    ThemeManager::CurrentTheme->categoryTextColor.GetShadowColor(), module->Expanded ? "-" : "+");
            windowDrawList->AddText(ImGui::GetFont(), categoryCtx->GetModuleFontSize() + 5, expandTextPos, static_cast<ImU32>(ThemeManager::CurrentTheme->moduleTextColor),
                                    module->Expanded ? "-" : "+");

        }

        ImGui::PopFont();


        ClickGuiColor moduleButtonTextColor = module->Enabled ? ThemeManager::CurrentTheme->moduleTextColor : ThemeManager::CurrentTheme->moduleDisabledTextColor;
        ImVec2 moduleButtonCenter(categoryCtx->Position.x + categoryCtx->GetLength() / 2 - textSize.x / 2,
                                  categoryCtx->GetCurrentRenderY() + categoryCtx->GetModuleHeight() / 2 -
                                  textSize.y / 2);


        // Add shadow
        windowDrawList->AddText(ImGui::GetFont(), categoryCtx->GetModuleFontSize(), {moduleButtonCenter.x + 1, moduleButtonCenter.y + 1},
                                static_cast<ImU32>(ThemeManager::CurrentTheme->moduleTextColor.GetShadowColor()), moduleText.c_str());

        windowDrawList->AddText(ImGui::GetFont(), categoryCtx->GetModuleFontSize(), moduleButtonCenter,
                                moduleButtonTextColor, moduleText.c_str());



        bool moduleButtonHovered = ImGui::IsItemHovered();

        if (moduleButtonHovered || ShowAllEyeIcons) {
            // Render eye icon for arraylist
            ImGui::PushFont(Font::Fonts["icons_v1"]);

            ImVec2 eyeIconSize = ImGui::GetFont()->CalcTextSizeA(categoryCtx->GetIconFontSize(), FLT_MAX,
                                                                 0, module->HideInArraylist ? Font::IconFontToString(IconFont::EyeOpen) : Font::IconFontToString(IconFont::EyeClosed));

            ImVec2 iconPos = ImVec2(categoryCtx->Position.x + categoryCtx->GetPadding(),
                                    categoryCtx->GetCurrentRenderY() + (categoryCtx->GetModuleHeight() / 2) - (eyeIconSize.y / 2));

            ClickGuiColor eyeIconColor = module->HideInArraylist ? ThemeManager::CurrentTheme->EyeIconOffColor : ThemeManager::CurrentTheme->EyeIconOnColor;

            // Add shadow
            windowDrawList->AddText(ImGui::GetFont(), categoryCtx->GetIconFontSize(), {iconPos.x + 1, iconPos.y + 1},
                                    (ImColor)ThemeManager::CurrentTheme->moduleTextColor.GetShadowColor(),  module->HideInArraylist ? Font::IconFontToString(IconFont::EyeOpen) : Font::IconFontToString(IconFont::EyeClosed));

            windowDrawList->AddText(ImGui::GetFont(), categoryCtx->GetIconFontSize(), iconPos, eyeIconColor,
                                    module->HideInArraylist ? Font::IconFontToString(IconFont::EyeOpen) : Font::IconFontToString(IconFont::EyeClosed));


            // This is dumb
            ImVec2 iconButtonPos = ImVec2(categoryCtx->GetPadding(),
                                          categoryCtx->GetCurrentRenderY(false) + (categoryCtx->GetModuleHeight() / 2) - (eyeIconSize.y / 2));

            ImGui::SetCursorPos(iconButtonPos);

            ImGui::InvisibleButton(std::string("##HideInArrayList" + module->Name + std::to_string(
                                           categoryCtx->moduleIndex)).c_str(),
                                   eyeIconSize);

            if (ImGui::IsItemHovered()) {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    if (KeyManager::Keys[VK_MENU]) {
                        ShowAllEyeIcons = !ShowAllEyeIcons;
                    } else {
                        module->HideInArraylist = !module->HideInArraylist;
                    }
                }
            } else {

                if (moduleButtonHovered) {
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {

                        if (KeyManager::Keys[VK_CONTROL] && !KeyManager::Keys['W'] && !KeyManager::Keys['S'] && !KeyManager::Keys['A'] && !KeyManager::Keys['D']) {
                            module->Binding = true;
                            Minecraft::ClientInstance->playUI("random.pop", 0.75f, 1.0f);
                        } else {
                            module->Toggle();
                        }
                    }else if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
                        module->Binding = true;
                        Minecraft::ClientInstance->playUI("random.pop", 0.75f, 1.0f);
                    } else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                        module->Expanded = !module->Expanded;
                    }
                }
            }

            ImGui::PopFont();
        }

        // Set setting font scale
        ImGui::SetWindowFontScale(Minecraft::ClientInstance->guiData->guiScale / 2);

        if (!module->Settings.empty() && module->Expanded) {

            ImGui::PopFont();
            ImGui::PushFont(Font::Fonts["greycliff_demibold"]);

            ImVec4 settingRect = {
                    categoryCtx->Position.x + categoryCtx->GetPadding(),
                    categoryCtx->GetCurrentRenderY() + categoryCtx->GetModuleHeight(),
                    categoryCtx->Position.x + categoryCtx->GetLength() - categoryCtx->GetPadding(),
                    categoryCtx->GetCurrentRenderY() +
                    (categoryCtx->GetSettingHeight() * ((int) module->GetVisibleSettingCount() + 1)) -
                    (categoryCtx->GetPadding() * 2)
            };

            windowDrawList->AddRectFilled({settingRect.x, settingRect.y}, {settingRect.z, settingRect.w},
                                          (ImU32)ThemeManager::CurrentTheme->backgroundColor1, 0);


            for (auto setting: module->Settings) {
                if (!setting->IsVisible()) continue;

                ImGui::SetCursorPos(
                        {0, settingRect.y + (categoryCtx->GetSettingHeight() * (float) moduleUIElement->settingIndex)});


                auto *boolSetting = (BoolSetting *) setting;
                auto *modeSetting = (ModeSetting *) setting;
                auto *numberSetting = (NumberSetting *) setting;


                std::string items;


                if (setting->Type == SettingType::Mode) {
                    for (const std::string &mode: modeSetting->Modes) {
                        items += mode + '\0';
                    }
                }

                float currentSettingY =
                        categoryCtx->GetCurrentRenderY() + categoryCtx->GetModuleHeight() +
                        (categoryCtx->GetSettingHeight() * (float) moduleUIElement->settingIndex);

                ImVec4 buttonRect = {
                        categoryCtx->Position.x + categoryCtx->GetPadding(),
                        currentSettingY,
                        categoryCtx->Position.x + categoryCtx->GetLength() - (categoryCtx->GetPadding() * 2) / 2,
                        currentSettingY + categoryCtx->GetSettingHeight()
                };

                textSize = Font::Fonts["greycliff_demibold_large"]->CalcTextSizeA(categoryCtx->GetSettingFontSize(), FLT_MAX,
                                                                                  0, setting->GetDisplayText().c_str());

                ImVec2 textPos = ImVec2(buttonRect.x + (buttonRect.z - buttonRect.x) / 2 - textSize.x / 2,
                                        buttonRect.y + (buttonRect.w - buttonRect.y) / 2 - textSize.y / 2);

                // Add an imgui element for the setting
                switch (setting->Type) {
                    case SettingType::Bool:
                        // Check if this setting  has a
                        // Add a checkbox

                        textPos = ImVec2(categoryCtx->Position.x + categoryCtx->GetPadding() * 3, textPos.y);

                        // Add shadow
                        windowDrawList->AddText(Font::Fonts["greycliff_demibold_large"], categoryCtx->GetSettingFontSize(), {textPos.x + 1, textPos.y + 1},
                                                (ImU32)ThemeManager::CurrentTheme->moduleTextColor.GetShadowColor(), std::string(setting->Name + ":").c_str());

                        windowDrawList->AddText(Font::Fonts["greycliff_demibold_large"], categoryCtx->GetSettingFontSize(), textPos,
                                                (ImU32)ThemeManager::CurrentTheme->moduleTextColor, std::string(setting->Name + ":").c_str());


                        ImGui::SetCursorPos({categoryCtx->GetLength() - 35 - (categoryCtx->GetPadding() * 2),
                                             (categoryCtx->GetCurrentRenderY(false) + categoryCtx->GetModuleHeight() +
                                              (categoryCtx->GetSettingHeight() * (float) moduleUIElement->settingIndex) +
                                              (categoryCtx->GetPadding() * 2))});

                        // Push frame size
                        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(ThemeManager::CurrentTheme->toggleHoverColor.color));
                        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(ThemeManager::CurrentTheme->toggleDisabledColor.color));
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(ThemeManager::CurrentTheme->toggleDisabledColor.color));
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ThemeManager::CurrentTheme->toggleHoverColor.color));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ThemeManager::CurrentTheme->toggleHoverColor.color));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ThemeManager::CurrentTheme->toggleHoverColor.color));
                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(newGUIScale, newGUIScale));
                        ImGui::Toggle(std::string("##" + boolSetting->Name +
                                                  std::to_string(moduleUIElement->settingIndex)).c_str(),
                                      &boolSetting->Enabled, ImGuiToggleFlags_Animated);
                        ImGui::PopStyleVar();
                        ImGui::PopStyleColor(6);
                        break;
                    case SettingType::Mode:
                        textPos = ImVec2(categoryCtx->Position.x + categoryCtx->GetPadding() * 3, textPos.y);

                        // Add shadow
                        windowDrawList->AddText(Font::Fonts["greycliff_demibold_large"], categoryCtx->GetSettingFontSize(), {textPos.x + 1, textPos.y + 1},
                                                (ImU32)ThemeManager::CurrentTheme->moduleTextColor.GetShadowColor(), numberSetting->GetDisplayText().c_str());

                        windowDrawList->AddText(Font::Fonts["greycliff_demibold_large"], categoryCtx->GetSettingFontSize(), textPos,
                                                (ImU32)ThemeManager::CurrentTheme->moduleTextColor, numberSetting->GetDisplayText().c_str());


                        ImGui::SetCursorPos({0,
                                             (categoryCtx->GetCurrentRenderY(false) + categoryCtx->GetModuleHeight() + (moduleUIElement->settingIndex) * categoryCtx->GetSettingHeight())});

                        if (ImGui::InvisibleButton(std::string("##ModuleButton" + module->Name + setting->Name).c_str(),
                                                   ImVec2(categoryCtx->GetLength(), categoryCtx->GetSettingHeight()))) {}

                        if (ImGui::IsItemHovered()) {
                            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                                modeSetting->HandleClick(true);
                            } else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                                modeSetting->HandleClick(false);
                            }
                        }

                        break;
                    case SettingType::Number:
                        textPos = ImVec2(categoryCtx->Position.x + categoryCtx->GetPadding() * 3, textPos.y - 6);

                        // Add shadow
                        windowDrawList->AddText(Font::Fonts["greycliff_demibold"], categoryCtx->GetSettingFontSize(), {textPos.x + 1, textPos.y + 1},
                                                (ImU32)ThemeManager::CurrentTheme->moduleTextColor.GetShadowColor(), numberSetting->GetDisplayText().c_str());

                        windowDrawList->AddText(Font::Fonts["greycliff_demibold"], categoryCtx->GetSettingFontSize(), textPos,
                                                (ImU32)ThemeManager::CurrentTheme->moduleTextColor, numberSetting->GetDisplayText().c_str());


                        ImGui::SetCursorPos({(categoryCtx->GetPadding() * 3),
                                             (categoryCtx->GetCurrentRenderY(false) + categoryCtx->GetModuleHeight() +
                                              (categoryCtx->GetSettingHeight() * (float) moduleUIElement->settingIndex) +
                                              (categoryCtx->GetPadding() * 5))});

                        ImGui::PushItemWidth(categoryCtx->GetLength() - (categoryCtx->GetPadding() * 2) - 20);

                        // change ImGuiStyleVar_FramePadding
                        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(255, 255, 255, 255));
                        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(255, 255, 255, 255));
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(255, 255, 255, 255));
                        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(255, 255, 255, 255));
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(255, 255, 255, 255));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 255));
                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(newGUIScale * 2, newGUIScale * 3));



                        ImGui::SliderFloatCustom(std::string(
                                                         "##" + numberSetting->Name + std::to_string(categoryCtx->settingIndex)).c_str(),
                                                 &numberSetting->Value, numberSetting->Min, numberSetting->Max, "%.2f",
                                                 ImGuiSliderFlags_None);

                        if (ImGui::IsItemHovered()) {
                            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                                modeSetting->HandleClick(false);
                                ImGui::SetKeyboardFocusHere(-1);
                            }
                        }

                        ImGui::PopStyleVar();
                        ImGui::PopStyleColor(6);
                        ImGui::PopItemWidth();

                        break;
                }
                categoryCtx->settingIndex++;
                moduleUIElement->settingIndex++;
            }
            moduleUIElement->settingIndex = 0;
            ImGui::PopFont();
            ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);
        }
        categoryCtx->moduleIndex++;
    }
    ImGui::End();
    GUIManager::Binding = Binding;
}

void ClickGui::RenderCategoryBoxImGui2(CategoryUIElement* categoryCtx)
{
    ImFont* greycliff_semi_large = Font::Fonts["greycliff_demibold_large"];
    ImFont* greycliff_semi = Font::Fonts["greycliff_demibold"];
    ImFont* greycliff_bold_large = Font::Fonts["greycliff_bold_large"];
    ImFont* greycliff_bold = Font::Fonts["greycliff_bold"];


    ImGui::Begin(std::string("##CategoryBox" + categoryCtx->Name).c_str(), nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoBackground |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoFocusOnAppearing |
                 ImGuiWindowFlags_NoResize);

    ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

    // Create a centered text at the top of the category box
    {
        const ImVec4 headerBounds(categoryCtx->Position.x, categoryCtx->Position.y,
                                  categoryCtx->Position.x + categoryCtx->GetLength(),
                                  categoryCtx->Position.y + categoryCtx->GetHeaderHeight());

        // Draw slightly transparent background for the header
        const ImColor headerColor = ThemeManager::CurrentTheme->backgroundColor2;
        windowDrawList->AddRectFilled(ImVec2(headerBounds.x, headerBounds.y), ImVec2(headerBounds.z, headerBounds.w), headerColor);

        ImVec2 categoryTextSize = greycliff_bold_large->CalcTextSizeA(categoryCtx->GetCategoryFontSize(), FLT_MAX, 0, categoryCtx->Name.c_str());
        ImVec2 categoryTextPos((categoryCtx->Position.x + categoryCtx->GetLength() / 2) - (categoryTextSize.x / 2),
                               categoryCtx->Position.y + categoryCtx->GetHeaderHeight() / 2 - categoryTextSize.y / 2);

        windowDrawList->AddText(greycliff_bold_large,
                                categoryCtx->GetCategoryFontSize(),
                                categoryTextPos,
                                static_cast<ImU32>(ThemeManager::CurrentTheme->categoryTextColor),
                                categoryCtx->Name.c_str());
    }


    // Start iterating through the modules
    for (auto modulePair: categoryCtx->Modules)
    {
        Module* module = modulePair.first;
        ModuleUIElement* moduleUIElement = modulePair.second;

        // Draw the modules and icons
        {
            // Create an invisible button for the module
            // This is used to detect if the user is hovering over the module
        }
    }


    ImGui::End();
}

void SetupImGuiStyle()
{
    // Fork of Deep Dark style from ImThemes
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.6000000238418579f;
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.WindowRounding = 0.0f;
    //style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 0.0f;
    //style.ChildBorderSize = 1.0f;
    style.PopupRounding = 0.0f;
    //style.PopupBorderSize = 1.0f;
    //style.FramePadding = ImVec2(5.0f, 2.0f);
    style.FrameRounding = 0.0f;
    style.ItemSpacing = ImVec2(6.0f, 6.0f);
    style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
    style.CellPadding = ImVec2(6.0f, 6.0f);
    style.IndentSpacing = 25.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabMinSize = 10.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.TabBorderSize = 1.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);


    /*style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.3372549116611481f, 0.3372549116611481f, 0.3372549116611481f, 0.5400000214576721f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5568627715110779f, 0.5568627715110779f, 0.5568627715110779f, 0.5400000214576721f);
    */

    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 0.9200000166893005f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 0.2899999916553497f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.239999994635582f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0470588244497776f, 0.0470588244497776f, 0.0470588244497776f, 0.5400000214576721f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 0.5400000214576721f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2274509817361832f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.0470588244497776f, 0.0470588244497776f, 0.5400000214576721f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3372549116611481f, 0.3372549116611481f, 0.3372549116611481f, 0.5400000214576721f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4000000059604645f, 0.4000000059604645f, 0.4000000059604645f, 0.5400000214576721f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5568627715110779f, 0.5568627715110779f, 0.5568627715110779f, 0.5400000214576721f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5293611288070679f, 0.3294117450714111f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.0470588244497776f, 0.0470588244497776f, 0.0470588244497776f, 0.5400000214576721f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 0.5400000214576721f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2274509817361832f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.0f, 0.0f, 0.5199999809265137f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.3600000143051147f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2274509817361832f, 0.3300000131130219f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 0.2899999916553497f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4392156898975372f, 0.4392156898975372f, 0.4392156898975372f, 0.2899999916553497f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.4000000059604645f, 0.4392156898975372f, 0.4666666686534882f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 0.2899999916553497f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4392156898975372f, 0.4392156898975372f, 0.4392156898975372f, 0.2899999916553497f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4000000059604645f, 0.4392156898975372f, 0.4666666686534882f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.0f, 0.0f, 0.0f, 0.5199999809265137f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.3600000143051147f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.0f, 0.0f, 0.5199999809265137f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5199999809265137f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0f, 0.0f, 0.0f, 0.5199999809265137f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 0.2899999916553497f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2274509817361832f, 1.0f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.3294117748737335f, 0.6666666865348816f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
}

void ClickGui::Init() {
    SetupImGuiStyle();
    // Get a list of the categories
    for (auto mod : ModuleManager::Modules) {
        bool categoryFound = false;
        for (auto& category : categoryList) {
            if (category.first == mod->Category) {
                category.second->Modules.push_back(std::make_pair(mod, new ModuleUIElement(mod)));
                categoryFound = true;
                break;
            }
        }

        if (!categoryFound) {
            initIndex++;
            auto category = new CategoryUIElement(mod->Category, ImVec2(initIndex * 250, 50), std::vector<std::pair<Module*, ModuleUIElement*>>());
            category->Modules.push_back(std::make_pair(mod, new ModuleUIElement(mod)));
            categoryList.emplace_back(std::make_pair(mod->Category, category));
        }
    }

    // Sort the vector based on a.second->Modules.size()
    std::sort(categoryList.begin(), categoryList.end(),
              [](const std::pair<std::string, CategoryUIElement*>& a, const std::pair<std::string, CategoryUIElement*>& b) {
                  return a.second->Modules.size() > b.second->Modules.size();
              }
    );

    // Set the position of the categories
    int index = 0;
    for (auto& category : categoryList) {
        category.second->Position = ImVec2(index * 250 + 75, 50);
        index++;
    }

}

void ClickGui::Render() {
    if (!Minecraft::ClientInstance)
        return;

    if (!GUIManager::Visible || GUIManager::SelectedTab != (int)Tab::ClickGui)
        return;

    static bool firstRender = true;
    if (firstRender && ModuleManager::initialized) {
        Init();
        firstRender = false;
    }

    static ClickGuiMod* ClickGuiModule;
    if (ClickGuiModule == nullptr) {
        ClickGuiModule = ModuleManager::GetModule<ClickGuiMod>();
    }

    if (ClickGuiModule == nullptr)
        return;


    float delta = ImGui::GetIO().DeltaTime;

    if (!ClickGuiModule->Enabled)
        return;

    ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);

    int index = 0;

    static std::map<std::string, ImVec2> targetCategoryPositions;

    for (const auto& [fst, snd]: categoryList) {
        bool isScrollingDown = ImGui::GetIO().MouseWheel < 0;
        bool isScrollingUp = ImGui::GetIO().MouseWheel > 0;

        bool isScrolling = isScrollingDown || isScrollingUp;

        bool isShiftHeld = KeyManager::Keys[VK_SHIFT];

        // Make sure the category's target pos is in the map. if it's not, set it to the current position
        if (targetCategoryPositions.find(snd->Name) == targetCategoryPositions.end()) {
            targetCategoryPositions[snd->Name] = snd->Position;
        }

        bool leftClickHeld = ImGui::IsMouseDown(0);

        // Honestly flash fuck you
        if (isScrolling && !leftClickHeld) {
            if (isShiftHeld) {
                if (isScrollingDown) {
                    targetCategoryPositions[snd->Name] = ImVec2(snd->Position.x - 100, snd->Position.y);
                } else if (isScrollingUp) {
                    targetCategoryPositions[snd->Name] = ImVec2(snd->Position.x + 100, snd->Position.y);
                }
            } else {
                if (isScrollingDown) {
                    targetCategoryPositions[snd->Name] = ImVec2(snd->Position.x, snd->Position.y - 100);
                } else if (isScrollingUp) {
                    targetCategoryPositions[snd->Name] = ImVec2(snd->Position.x, snd->Position.y + 100);
                }
            }

            snd->Position = Math::Lerp(snd->Position, targetCategoryPositions[snd->Name], delta * 10);
            ImGui::SetNextWindowPos(ImVec2(snd->Position.x, snd->Position.y));

        } else if (!leftClickHeld)
        {
            snd->Position = Math::Lerp(snd->Position, targetCategoryPositions[snd->Name], delta * 10);
            ImGui::SetNextWindowPos(ImVec2(snd->Position.x, snd->Position.y));
        } else targetCategoryPositions[snd->Name] = snd->Position;



        RenderCategoryBoxImGui(snd);

        index++;
    }


    ImGui::PopFont();
}


void ClickGui::Shutdown() {

}