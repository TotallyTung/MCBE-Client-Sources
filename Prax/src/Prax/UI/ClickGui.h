#pragma once

#include <Prax/Utils/Render/ColorUtil.h>
#include "UIElement.h"

class ClickGui {
public:
    static bool Binding;

    static void Init();
    static void Render();
    static void Shutdown();

    static void RenderCategoryBoxImGui(CategoryUIElement *categoryCtx);
    static void RenderCategoryBoxImGui2(CategoryUIElement *categoryCtx);
    static void ShowToolTip(const std::string& text);
    static void RenderControls();
};