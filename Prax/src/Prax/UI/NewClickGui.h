//
// Created by Flash on 12/24/2023.
//


#pragma once


#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <Prax/Module/Module.h>

#include "imgui_internal.h"
#include "UIElement.h"

// These are individual windows rendered for each category
class ClickGuiWindow : ImGuiWindow {
public:

    std::string Name;
    std::vector<Module*> Modules;

    // Custom ui stuff
    ImVec2 windowSize = { 225, 500 };


    // Constructor
    ClickGuiWindow(std::string name): ImGuiWindow(ImGui::GetCurrentContext(), name.c_str())
    {
        this->Name = name;
    }
};

class NewClickGui {
public:
    static std::vector<ClickGuiWindow*> Windows;

    static ImGuiStyle StyleGUI();
    static void SortModuleByLength(std::vector<Module*>& modules);
    static void SortCategoriesByLength(std::vector<ClickGuiWindow*>& windows);
    static ImGuiMouseButton GetMouseDown();
    static void SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    static void Init();
    static void Render(bool visible);
};

