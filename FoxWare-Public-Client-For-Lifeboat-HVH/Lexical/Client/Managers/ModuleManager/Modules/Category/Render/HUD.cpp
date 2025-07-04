#include "HUD.h"
#include "../../../ModuleManager.h"
float watermarksizeH = false;
float SizeH = 1.f;
bool backgroundBlur = false;
bool watermarkH;
HUD::HUD() : Module("HUD", "Displays HUD Elements", Category::RENDER) {
    registerSetting(new BoolSetting("Position", "NULL", &showPosition, true));
    registerSetting(new BoolSetting("Direction", "NULL", &showDirection, false));
    registerSetting(new BoolSetting("Watermark", "NULL", &watermarkH, false));
    registerSetting(new BoolSetting("Effects", "NULL", &showEffects, true));
    registerSetting(new BoolSetting("BG Blur", "NULL", &backgroundBlur, false));
    registerSetting(new SliderSetting<int>("Opacity", "NULL", &opacity, 130, 0, 255));
    registerSetting(new SliderSetting<int>("Spacing", "NULL", &spacing, 0, -3, 3));
    registerSetting(new SliderSetting<int>("Offset", "NULL", &offset, 0, 0, 30));
    registerSetting(new SliderSetting<float>("Size", "NULL", &SizeH, 1.f, 0.1f, 2.f));
    registerSetting(new SliderSetting<float>("Watermark Size", "NULL", &watermarksizeH, 1.f, 0.1f, 5.f));
}

HUD::~HUD() {
}

void HUD::onD2DRender() {
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (localPlayer == nullptr)
        return;
    Vec2<float> windowSize = Game::clientInstance->guiData->windowSizeReal;
    Vec2<float> windowSizeScaled = Game::clientInstance->guiData->windowSizeScaled;
    float textSize = SizeH;
    float textPaddingX = std::max(1.f, ((float)spacing + 3.f)) * textSize;
    float textPaddingY = (float)spacing * textSize;
    float textHeight = D2D::getTextHeight("", textSize);
    float posX = 0.f + (float)offset;
    float posY = windowSize.y - (textHeight + textPaddingY * 2.f) - (float)offset;
    static Colors* coie = ModuleManager::getModule<Colors>();
    UIColor mainColor = UIColor(coie->getColor());

    if (watermarkH) {
        Vec2<float> watermarkPos(offset, offset);
        std::string watermarkText = "FOXY";
        float textWidth = D2D::getTextWidth(watermarkText, watermarksizeH, false);
        float textHeight = D2D::getTextHeight(watermarkText, watermarksizeH, false);
        Vec4<float> watermarkRect(
            watermarkPos.x,
            watermarkPos.y - spacing,
            watermarkPos.x + textWidth + spacing,
            watermarkPos.y + textHeight
        );
        D2D::fillRectangle(watermarkRect, UIColor(0, 0, 0, opacity));
        if (backgroundBlur)
            D2D::addBlur(watermarkRect, 10.f);
        D2D::drawText(watermarkPos, watermarkText, UIColor(mainColor), watermarksizeH, false);
    }

    if (showPosition) {
        Vec3<float> lpPos = localPlayer->getPos();
        int dimensionId = localPlayer->getDimensionTypeComponent()->type;
        if (dimensionId == 0) {
            // Overworld
            char netherPosText[50];
            sprintf_s(netherPosText, 50, "Nether: %.1f, %.1f, %.1f", (lpPos.x / 8.f), (lpPos.y - 1.6f), (lpPos.z / 8.f));
            Vec4<float> netherRect(posX, posY, posX + D2D::getTextWidth(std::string(netherPosText), textSize, false) + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f);
            D2D::fillRectangle(netherRect, UIColor(0, 0, 0, opacity));
            if (backgroundBlur)
                D2D::addBlur(netherRect, 10.f);
            D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), std::string(netherPosText), UIColor(mainColor), textSize, false);
            posY -= textHeight + (textPaddingY * 2.f);
        }
        else if (dimensionId == 1) {
            // Nether
            char overworldPosText[50];
            sprintf_s(overworldPosText, 50, "Overworld: %.1f, %.1f, %.1f", (lpPos.x * 8.f), (lpPos.y - 1.6f), (lpPos.z * 8.f));
            Vec4<float> overworldRect(posX, posY, posX + D2D::getTextWidth(std::string(overworldPosText), textSize, false) + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f);
            D2D::fillRectangle(overworldRect, UIColor(0, 0, 0, opacity));
            if (backgroundBlur)
                D2D::addBlur(overworldRect, 10.f);
            D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), std::string(overworldPosText), UIColor(mainColor), textSize, false);
            posY -= textHeight + (textPaddingY * 2.f);
        }

        char posText[50];
        sprintf_s(posText, 50, "Position: %.1f, %.1f, %.1f", lpPos.x, (lpPos.y - 1.6f), lpPos.z);
        Vec4<float> posRect(posX, posY, posX + D2D::getTextWidth(std::string(posText), textSize, false) + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f);
        D2D::fillRectangle(posRect, UIColor(0, 0, 0, opacity));
        if (backgroundBlur)
            D2D::addBlur(posRect, 10.f);
        D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), std::string(posText), UIColor(mainColor), textSize, false);
        posY -= textHeight + (textPaddingY * 2.f);
    }

    if (showDirection) {
        float rotation = localPlayer->rotation->presentRot.y;
        if (rotation < 0) {
            rotation += 360.0;
        }
        std::string directionText;
        if ((0 <= rotation && rotation < 45) || (315 <= rotation && rotation < 360)) {
            directionText = "South (+Z)";
        }
        else if (45 <= rotation && rotation < 135) {
            directionText = "West (-X)";
        }
        else if (135 <= rotation && rotation < 225) {
            directionText = "North (-Z)";
        }
        else if (225 <= rotation && rotation < 315) {
            directionText = "East (+X)";
        }
        else {
            directionText = "NULL";
        }
        char finalText[25];
        sprintf_s(finalText, 25, "Direction: %s", directionText.c_str());

        Vec4<float> dirRect(posX, posY, posX + D2D::getTextWidth(std::string(finalText), textSize) + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f);
        D2D::fillRectangle(dirRect, UIColor(0, 0, 0, opacity));
        if (backgroundBlur)
            D2D::addBlur(dirRect, 10.f);
        D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), std::string(finalText), UIColor(mainColor), textSize);

        posY -= textHeight + (textPaddingY * 2.f);
    }

    if (showEffects) {
        static Arraylist* arrayListMod = ModuleManager::getModule<Arraylist>();

        Vec2<float> EffectPos = Vec2<float>(windowSize.x - (float)offset, (float)offset);
        if (!arrayListMod->bottom)
            EffectPos.y = windowSize.y - (textHeight + textPaddingY * 2.f) - (float)offset;

        for (uint32_t effectId = 1; effectId < 37; effectId++) {
            MobEffect* mEffect = MobEffect::getById(effectId);
            if (mEffect == nullptr)
                continue;
            if (localPlayer->hasEffect(mEffect)) {
                MobEffectInstance* mEffectInstance = localPlayer->getEffect(mEffect);
                if (mEffectInstance == nullptr)
                    continue;
                std::string text1 = mEffectInstance->getDisplayName() + " ";
                std::string text2 = getEffectTimeLeftStr(mEffectInstance);
                std::string fullText = text1 + text2;

                Vec4<float> eRectPos = Vec4<float>(EffectPos.x - D2D::getTextWidth(fullText, textSize, false) - (textPaddingX * 2.f),
                    EffectPos.y,
                    EffectPos.x,
                    EffectPos.y + textHeight + (textPaddingY * 2.f));

                Vec2<float> eTextPos = Vec2<float>(eRectPos.x + textPaddingX, eRectPos.y + textPaddingY);
                Vec2<float> eDurationTextPos = Vec2<float>(eTextPos.x + D2D::getTextWidth(text1, textSize, true), eTextPos.y);

                D2D::fillRectangle(eRectPos, UIColor(0, 0, 0, opacity));
                if (backgroundBlur)
                    D2D::addBlur(eRectPos, 10.f);
                D2D::drawText(eTextPos, text1, mEffect->color, textSize, true);
                D2D::drawText(eDurationTextPos, text2, UIColor(mainColor), textSize, false);

                EffectPos.y -= (textHeight + (textPaddingY * 2.f)) * (arrayListMod->bottom ? -1.f : 1.f);
            }
        }
    }
}