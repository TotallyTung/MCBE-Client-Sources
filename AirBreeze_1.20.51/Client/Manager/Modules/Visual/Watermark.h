#pragma once

#include "../../Module.h"
#include "../../Manager.h"
#include "../../Client.h"
#include "../../../Include/imgui/imgui.h"
#include "../../../Utils/Render/ImGuiUtils.h"
#include "../../../Include/imgui/imgui_notify.h"

class Watermark : public Module {
public:
    Watermark(Category* c) : Module(c, "Watermark", "just read it genius.") {
        this->isEnabled = true;
    }

    auto onRender(MinecraftUIRenderContext* ctx) -> void {
        Vec2<float> Pos = Vec2<float>(1, -12);
        Vec2<float> ImageDimension(80.0f, 80.0f); // Original dimensions of the image (1920x1080)
        Vec2<float> uvPos(0.0f, 0.0f);
        Vec2<float> uvSize(1.0f, 1.0f);
        RenderUtils::drawImage(Utils::getResourcePath() + "\\logo.png", Pos, ImageDimension, uvPos, uvSize);
        RenderUtils::flushImage();
    }

    auto onImGuiRender() -> void {
        //ImGuiUtils::RenderGlowText(Vec2<float>(6, 6), "Air", 2);
    }
};
