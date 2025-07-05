//
// Created by Flash on 7/28/2023.
//

#pragma once


#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include "Prax/Module/ModuleManager.h"
#include "Prax/Utils/Render/Font.h"
#include "Prax/Utils/Render/D2D.h"

class ArrayList : public ModuleBase<ArrayList> {
public:
    enum class ColorMode {
        Themed,
        Static
    };

    ModeSetting PositionMode = ModeSetting("Mode", "Position to render", {"Top Left", "Top Right", "Bottom Left", "Bottom Right" }, 1);
    static inline BoolSetting Lowercase = BoolSetting("Lowercase", "Render the text in lowercase", false);
    BoolSetting RenderBGSetting = BoolSetting("Render BG", "Render a background behind the text", true);
    BoolSetting RenderLine = BoolSetting("Render Line", "Render a line next to the text", false);
    BoolSetting RenderFullOutline = BoolSetting("Full Outline (DEBUG)", "Render a full outline around the box", true);
    BoolSetting RenderMode = BoolSetting("Render Mode", "Render the mode in the arraylist", true);
//
    ArrayList() : ModuleBase("ArrayList", "Shows enabled modules", "Visual", 0, true) {
        AddSetting(&PositionMode);
        AddSetting(&Lowercase);
        AddSetting(&RenderBGSetting);
        AddSetting(&RenderLine);
        AddSetting(&RenderFullOutline);
        AddSetting(&RenderMode);
    }
    static ImVec2 ScreenSize;
    static ImDrawList* DrawList;
    static float FontSize;

    enum Position {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    void OnTick() override;
    ImColor GetColor(int index);
    void OnRender() override;
    void OnEnable() override;
    void Sort();

};
