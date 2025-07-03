#pragma once

#include "../../Module.h"
#include "../../Manager.h"
#include "../../Client.h"
#include "../../../Include/imgui/imgui.h"
#include "../../../Utils/Render/ImGuiUtils.h"
#include "../../../Include/imgui/imgui_notify.h"

class ArrayList : public Module {
public:
    ArrayList(Category* c) : Module(c, "ArrayList", "Array.") {}


    auto onImGuiRender() -> void {
        this->isEnabled = true;
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 screenSize = io.DisplaySize;
        float imguiwindowsizex = 200;

        ImGui::SetNextWindowBgAlpha(0.0f);

        // Maximize the window size
        ImGui::SetNextWindowSize(ImVec2(screenSize.x, 400));
        ImGui::SetNextWindowPos(ImVec2(screenSize.x - 150, 0));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

        // Begin the window with an invisible border
        ImGui::Begin("Modules List", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize );

        // Add an invisible border
        ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0, 0, 0, 0)); // Set border shadow color to transparent
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // Set border size to 0

        ImGui::SetWindowFontScale(0.5f);

        auto& categories = this->category->mgr->categories;

        for (size_t i = 0; i < categories.size(); i++) {
            Category* category = categories[i];

            for (Module* module : category->modules) {
                if (module->isEnabled) {
                    // Calculate the width of the module name
                    float moduleNameWidth = ImGui::CalcTextSize(module->name.c_str()).x;

                    // Calculate the starting position for the module name
                    float rightSidePosX = screenSize.x - 50 - moduleNameWidth; // Fixed offset from the right side
                    if (rightSidePosX < 0) {
                        rightSidePosX = 0; // Ensure it doesn't go off the left side
                    }

                    // Set the cursor position for each module
                    //ImGui::SetCursorPosX(rightSidePosX);

                    ImVec2 letterSpacing(1.0f, 0.0f); // Adjust spacing between letters

                    for (int letterIndex = 0; letterIndex < module->name.length(); letterIndex++) {
                        ImVec4 rainbowColor = ImColor::HSV(
                            fmod(ImGui::GetTime() * 0.1f + letterIndex * 0.01f, 1.0f),
                            1.0f,
                            1.0f
                        );

                        //imguiwindowsizex = moduleNameWidth;

                        // Set the text color to rainbowColor for each letter
                        ImGui::PushStyleColor(ImGuiCol_Text, rainbowColor);
                        ImGui::Text("%c", module->name[letterIndex]);
                        ImGui::PopStyleColor();

                        // Add letter spacing
                        ImGui::SameLine(0, letterSpacing.x);
                    }

                    // Add spacing between modules
                    ImGui::Spacing();
                }
            }
        }

        // End the window
        ImGui::End();

        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }
};
