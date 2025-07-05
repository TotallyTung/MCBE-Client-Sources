//
// Created by Flash on 1/2/2024.
//


#pragma once
#include <Prax/Module/Modules/Visual/ClickGui.h>

enum class Tab
{
    ClickGui = 0,
    Theme,
    Friends,
    Keybinds,
    Config,
    Settings,
};
class GUIManager {
    public:
    static ClickGuiMod* Instance;

    static bool Visible;
    static bool Binding;
    static int SelectedTab;

    static void RenderTabs();
    static void Render(ClickGuiMod* instanc);
};
