//
// Created by vastrakai on 9/25/2024.
//

#include "ScriptingGui.hpp"

void ScriptingGui::render(float inScale, float animation, ImVec2 screen, float blurStrength)
{
    FontHelper::pushPrefFont();

    auto drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilled(ImVec2(0, 0), ImVec2(screen.x, screen.y), IM_COL32(0, 0, 0, 255 * animation * 0.38f));
    ImRenderUtils::addBlur(ImVec4(0.f, 0.f, screen.x, screen.y),
                           animation * blurStrength, 0);

    ImColor shadowRectColor = ColorUtils::getThemedColor(0);
    shadowRectColor.Value.w = 0.5f * animation; // Adjust rect alpha with animation

    float firstheight = (screen.y - screen.y / 3);

    firstheight = MathUtils::lerp(screen.y, firstheight, inScale);
    ImRenderUtils::fillGradientOpaqueRectangle(
        ImVec4(0, firstheight, screen.x, screen.y),
        shadowRectColor, shadowRectColor, 0.4f * inScale, 0.0f);


    ImVec2 windowSize = ImVec2(800.0f * inScale, 600.0f * inScale);  // Scale window size
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::SetNextWindowBgAlpha(0.8f * inScale);  // Control opacity using animation

    ImGui::Begin("Scripting");

    auto& sm = gFeatureManager->mScriptManager;
    auto& scripts = sm->getScripts();

    ImGui::Text("NOTE: This GUI is NOT finished and should absolutely not be used for release builds.");

    ImGui::Text("Scripts:");

    if (sm->mLoadingScripts)
    {
        ImGui::Text("Loading scripts...");
        ImGui::End();
        FontHelper::popPrefFont();
        return;
    }



    int i = 0;
    for (auto& psc : scripts) {
        if (psc->type != SolsticeScriptType::MODULE) continue;
        i++;

        std::shared_ptr<ModuleScript> script = std::reinterpret_pointer_cast<ModuleScript>(psc);

        bool enabled = script->enabled;
        std::string name = "##enabled" + std::to_string(i);
        if (ImGui::Checkbox(name.c_str(), &enabled)) {
            script->enabled = enabled;
            script->triggerStateChangeEvent();
        }
        ImGui::SameLine();
        ImGui::Text(script->moduleName.c_str());
        ImGui::SameLine();
        std::string desc = "- " + script->moduleDescription + (script->moduleAuthor == "" ? "" : " -");
        ImGui::Text(desc.c_str());
        ImGui::SameLine();
        ImGui::TextColored(ImColor(1.0f, 0.84f, 0.0f, 1.0f), "made by");
        // blue
        ImGui::SameLine();
        ImGui::TextColored(ImColor(0.0f, 0.5f, 1.0f, 1.0f), script->moduleAuthor.c_str());
    }

    FontHelper::popPrefFont();
    ImGui::End();
}
