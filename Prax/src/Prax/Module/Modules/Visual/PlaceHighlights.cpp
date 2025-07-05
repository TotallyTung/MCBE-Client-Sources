//
// Created by vastrakai on 9/28/2023.
//

#include <Prax/Utils/Render/Render.h>
#include "PlaceHighlights.h"

std::map<uint64_t, Vector3> PlaceHighlights::LastPlaceMap = std::map<uint64_t, Vector3>();

void PlaceHighlights::OnTick() {

}

void PlaceHighlights::OnDisable() {

}

void PlaceHighlights::OnEnable() {

}

void PlaceHighlights::OnRender() {
    if (LastPlaceMap.empty()) return;

    if (!Minecraft::ClientInstance->levelRenderer) return;

    try {
        std::vector<uint64_t> toRemove = std::vector<uint64_t>();
        for (auto &it: LastPlaceMap) {
            Vector3 blockPos = it.second;
            float alpha = 1.f - ((float) (Misc::GetCurrentMs() - it.first) / (SecondsToRender.Value * 1000));
            int alphai = (int) (alpha * 200);

            int totalBlocks =
                    Minecraft::ClientInstance->ThePlayer()->supplies->GetAllPlaceableBlocks(false);
            // Decide color based on the amount of blocks

            ImColor color;

            if (totalBlocks < 32) color = ImColor(249, 98, 94, alphai);
            else if (totalBlocks < 64) color = ImColor(255, 229, 61, alphai);
            else color = ImColor(61, 245, 1, alphai);

            ImColor filledColor = ImColor(color.Value.x, color.Value.y, color.Value.z, color.Value.w / 2);
            if (alpha <= 0) toRemove.push_back(it.first);
            else {
                if (DrawMethod.SelectedIndex == 0) Render::DrawBoxAroundBlockOutline(blockPos, color);
                else Render::DrawBoxAroundBlock(blockPos, color, filledColor);
            }

            for (auto &it: toRemove) {
                LastPlaceMap.erase(it);
            }

            toRemove.clear();
        }
    } catch (std::exception &e) {
        Logger::Write("PlaceHighlights", "Failed to render: " + std::string(e.what()), Logger::LogType::Error);
    }
}