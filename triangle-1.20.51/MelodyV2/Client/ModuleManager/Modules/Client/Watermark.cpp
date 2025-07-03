#include "Watermark.h"
#include "../../../Client.h"
bool fillrect = false;
Watermark::Watermark() : Module("Watermark", "Shows Watermark", Category::CLIENT) {
	slider<float>("Size", "Changes watermark size", ValueType::FLOAT_T, &size, 0.5f, 1.5f);
    boolsetting("Rectangle", "NULL", &fillrect);
}

Watermark::~Watermark() {
}

void Watermark::onNormalTick(Actor* actor) {
}

void Watermark::onImGuiRender(ImDrawList* drawlist) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    static ClientManager* colorsMod = (ClientManager*)client->moduleMgr->getModule("ClientManager");
    UIColor mainColor(colorsMod->getColor());
    mainColor.r = (int)(mainColor.r);
    mainColor.g = (int)(mainColor.g);
    mainColor.b = (int)(mainColor.b);
    const Vec2<float> textPadding(15, 15);
    const char* text1 = "triangle";
    const char* text2 = " developer";
    float textScale = 1.f * size;
    Vec2<float> cTextPos(textPadding.x, textPadding.y);
    float text1Width = ImGuiUtils::getTextWidth(text1) * textScale;
    float text2Width = ImGuiUtils::getTextWidth(text2) * textScale;
    float totalTextWidth = text1Width + text2Width;
    float measuredHeight = ImGuiUtils::getTextHeight(1.0f);
    float textHeight = measuredHeight * textScale;
    float extraPadding = 3.f * size;
    Vec4<float> rect(
        cTextPos.x - extraPadding,
        cTextPos.y - extraPadding,
        cTextPos.x + totalTextWidth + extraPadding,
        cTextPos.y + textHeight + extraPadding
    );
    if (fillrect) {
        ImGuiUtils::fillRectangle(rect, UIColor(0, 0, 0, 255));
    }
    ImGuiUtils::drawText(cTextPos, text1, UIColor(mainColor), textScale, true);
    Vec2<float> text2Pos(cTextPos.x + text1Width, cTextPos.y);
    ImGuiUtils::drawText(text2Pos, text2, UIColor(255, 255, 255, 255), textScale, true);
    float underlineThickness = 1.0f * size;
    Vec4<float> underlineRect(
        rect.x,
        rect.w - underlineThickness,
        rect.x + totalTextWidth,
        rect.w
    );
    if (fillrect) {
        ImGuiUtils::fillRectangle(underlineRect, UIColor(mainColor));
    }
}
