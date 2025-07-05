//
// Created by Flash on 9/12/2023.
//

#include "Watermark.h"

#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/Utils/Render/Font.h>
#include <Prax/Utils/Render/Render.h>
#include <xorstr.hpp>

#include "ArrayList.h"
#include "HUD.h"


void Watermark::OnRender() {
    if (Minecraft::ClientInstance->GetScreenName() == "start_screen") return;
    ImGui::PushFont(Font::Fonts["greycliff_bold_large"]);
    // Get top left corner of screen
    int posFromCorner = PositionFromCorner.Value;
    ImVec2 renderPosition = ImVec2(posFromCorner, posFromCorner);

    std::string watermarkText = Prax::ClientName;

    float watermarkSize = 50.f;

    auto drawList = ImGui::GetBackgroundDrawList();

    ImVec2 textSize = ImGui::CalcTextSize(watermarkText.c_str());

    float time = ColorUtil::GlobalRGBInfo.Time * 2.f;

    std::vector<ImColor> colors;

    // Draw the text
    Render::DrawGradientText(watermarkText, renderPosition, 50, time, &colors, true);


    Render::DrawGlowText(watermarkText, renderPosition, 50, colors, true);

    ImGui::PopFont();

}