#include "CreditsModule.h"
#include "../../Manager.h"
#include "../../Client.h"
#include "../../../Include/imgui/imgui.h"

void CreditsModule::onImGuiRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    // Define cube position, size, and shadow parameters
    ImVec2 cubePosition = ImVec2(10, 10);   // Adjust position as needed
    ImVec2 cubeSize = ImVec2(200, 200);     // Adjust cube size as needed
    ImVec2 shadowOffset = ImVec2(10, 10);   // Adjust shadow offset as needed
    float shadowThickness = 100.0f;        // Adjust shadow thickness as needed
    ImVec4 shadowColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Adjust shadow color as needed

    // Draw the cube of shadows using AddShadowRect
    ImVec2 r1(cubePosition.x + shadowOffset.x, cubePosition.y + shadowOffset.y);
    ImVec2 r2(cubePosition.x + cubeSize.x + shadowOffset.x, cubePosition.y + cubeSize.y + shadowOffset.y);
    ImDrawFlags drawFlags = ImDrawFlags_ShadowCutOutShapeBackground;

    drawList->AddShadowRect(r1, r2, ImGui::GetColorU32(shadowColor), shadowThickness, shadowOffset, drawFlags, 0.0f);
}