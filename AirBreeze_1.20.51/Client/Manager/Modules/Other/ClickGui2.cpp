#include "ClickGui2.h"
#include "../../Manager.h"
#include "../../Client.h"
#include "../../Hooks/SwapChain/ImGui/imgui.h"
#include "../../../Utils/Render/ImGuiUtils.h"
#include "../../../Utils/Render/RenderUtils.h"
#include "../../Hooks/SwapChain/ImGui/imgui_internal.h"



bool colorPickerOpen = false;
ImVec4 guiBackgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 0.85f);
ImVec4 rainbowBorderColor;

void DrawRainbowBorder() {
    ImVec2 min = ImGui::GetWindowPos();
    ImVec2 max = ImVec2(min.x + ImGui::GetWindowSize().x, min.y); // Corrected line

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    const int numSegments = 48;
    for (int i = 0; i < numSegments; ++i) {
        float t0 = static_cast<float>(i) / numSegments;
        float t1 = static_cast<float>(i + 1) / numSegments;

        ImVec2 p0 = ImVec2(ImLerp(min.x, max.x, t0), min.y);
        ImVec2 p1 = ImVec2(ImLerp(min.x, max.x, t1), min.y);

        ImU32 color = ImColor::HSV(t0, 1.0f, 1.0f);

        drawList->AddLine(p0, p1, color, 1.0f);
    }
}

auto ClickGui2::onImGuiRender(void) -> void {
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 10.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.9f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);


    // Change button colors
    style.Colors[ImGuiCol_Button] = ImVec4(0.3f, 0.5f, 0.8f, 1.0f);       // Button background color when idle
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4f, 0.6f, 0.9f, 1.0f);  // Button background color when hovered
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2f, 0.4f, 0.7f, 1.0f);   // Button background color when pressed

    // Change button rounding
    style.FrameRounding = 6.0f;

    // Change button border size and rounding (optional)
    style.GrabMinSize = 40.0f; // Minimum size for "drag box" in a button
    style.GrabRounding = 4.0f; // Rounding for "drag box" in a button



    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 6.0f)); // Adjust frame padding for all elements
    //ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // Use a larger font for improved readability

    auto& categories = this->category->mgr->categories;

    static int selectedCategory = 0;
    static int selectedModule = 0;

    const float contentWidth = 400.0f;
    const float windowWidth = 150.0f + 200.0f + contentWidth + 16.0f;

    ImGuiIO& op = ImGui::GetIO();


    static float alpha = 0.0f;
    if (alpha < 1.0f) {
        alpha += 0.01f;
        style.Alpha = alpha;
    }
 

    if (!this->setWinAttr) {
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImVec2 windowSize = ImVec2(800, 300);
        ImVec2 displayCenter = ImVec2(displaySize.x * 0.5f, displaySize.y * 0.5f);
        ImVec2 windowPos = ImVec2(displayCenter.x - windowSize.x * 0.5f, displayCenter.y - windowSize.y * 0.5f);
        ImGui::SetNextWindowPos(windowPos);
        ImGui::SetNextWindowSize(windowSize);

        this->setWinAttr = true;
    }

    if (ImGui::Begin(this->category->mgr->client->name.c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar ))
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();


        static bool setWindowSize = false;

        if (!setWindowSize) {
            setWindowSize = true;
            ImGui::SetWindowSize(ImVec2(windowWidth, 600.0f));
        };

        if (ImGui::BeginChild("Categories", ImVec2(150, 0), true)) {

            for (int i = 0; i < categories.size(); i++) {

                if (ImGui::Selectable(categories[i]->name.c_str(), selectedCategory == i)) {

                    if (selectedCategory != i)
                        selectedModule = 0;

                    selectedCategory = i;

                };

            };

            ImGui::EndChild();

        }

        ImGui::SameLine();

        if (ImGui::BeginChild("Modules", ImVec2(200, 0), true)) {

            auto category = categories.at(selectedCategory);
            auto& modules = category->modules;

            for (int i = 0; i < modules.size(); i++) {

                if (ImGui::Selectable(modules.at(i)->name.c_str(), selectedModule == i)) {

                    selectedModule = i;

                };

            };

            ImGui::EndChild();

        };

        ImGui::SameLine();

        if (ImGui::BeginChild("Content", ImVec2(0, 0), true)) {

            auto& modules = categories.at(selectedCategory)->modules;
            auto module = (modules.empty() ? nullptr : modules.at(selectedModule));

            if (module) {

                ImGui::Text(module->name.c_str());

                ImGui::PushStyleColor(ImGuiCol_Button, module->isEnabled ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

                if (ImGui::Button(module->name.c_str(), ImVec2(0.f, 0.f)))
                    module->isEnabled = !module->isEnabled;

                ImGui::PopStyleColor();

                module->onImGuiOptions();

                //ImGui::EndChild();

            };

            ImGui::EndChild();

        };

        ImGui::End();

    };

    ImGui::PopStyleVar();
    //ImGui::PopFont();

};

auto ClickGui2::onEnable() -> void {
    ClientInstance* CI = Minecraft::getClientInstance();

    ImGuiIO& io = ImGui::GetIO();

    if (CI == nullptr) {
        return;
    }
    CI->acquireCursor();
}

auto ClickGui2::onDisable() -> void {
    ImGuiIO& io = ImGui::GetIO();
    io.WantCaptureMouse = false;
    ClientInstance* CI = Minecraft::getClientInstance();
    if (CI == nullptr) {
        return;
    }
    if (CI->isInGame()) {
        CI->dropCursor();
    }
}


