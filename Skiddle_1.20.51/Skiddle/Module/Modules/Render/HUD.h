#pragma once

class HUD : public Module
{
public:
    int colortype = 2;
    bool showFPS = true;
    bool showBPS = true;
    float scale = 2.6f;

    HUD(int keybind = Keyboard::NONE, bool enabled = true) :
        Module("HUD", "Render", "Display info on the screen", keybind, enabled)
    {
        registerEnumSetting("Mode", "The type of mode", { "Gay", "Custom", "Astolfo", "Trans", "Weather", "Bubblegum", "Cherry", "Fadeaway", "Xextreame", "Zipppo" }, &colortype);
        registerFloatSetting("R", "", &r, 0, 255);
        registerFloatSetting("G", "", &g, 0, 255);
        registerFloatSetting("B", "", &b, 0, 255);
        registerFloatSetting("R2", "", &r2, 0, 255);
        registerFloatSetting("G2", "", &g2, 0, 255);
        registerFloatSetting("B2", "", &b2, 0, 255);
        registerBoolSetting("Show FPS", "", &showFPS);
        registerBoolSetting("Show BPS", "", &showBPS);
        registerFloatSetting("Scale", "", &scale, 0.5, 4);
    }

    float r = 120, g = 35, b = 255;
    float r2 = 0, g2 = 0, b2 = 0;

    void onEvent(UpdateTickEvent* event) override
    {
        switch (colortype) {
        case 0: // Gay
            ColorUtil::setClientColorType(0);
            ColorUtil::setClientCustomColor(255, 255, 255);
            ColorUtil::setClientCustomColor2(255, 255, 255);
            break;
        case 1: // Custom
            ColorUtil::setClientColorType(1);
            ColorUtil::setClientCustomWaveColor(r, g, b);
            ColorUtil::setClientCustomWaveColor2(r2, g2, b2);
            break;
        case 2: // Astolfo
            ColorUtil::setClientColorType(2);
            break;
        case 3: // Trans Colors
            ColorUtil::setClientColorType(1);
            ColorUtil::setClientCustomWaveColor(255, 255, 255);
            ColorUtil::setClientCustomWaveColor2(128, 128, 255);
            break;
        case 4: // Weather Colors
            ColorUtil::setClientColorType(1);
            ColorUtil::setClientCustomWaveColor(255, 255, 255);
            ColorUtil::setClientCustomWaveColor2(0, 191, 255);
            break;
        case 5: // Bubblegum
            ColorUtil::setClientColorType(1);
            ColorUtil::setClientCustomWaveColor(255, 182, 193);
            ColorUtil::setClientCustomWaveColor2(255, 182, 193);
            break;
        case 6: // Cherry
            ColorUtil::setClientColorType(1);
            ColorUtil::setClientCustomWaveColor(178, 34, 34);
            ColorUtil::setClientCustomWaveColor2(178, 34, 34);
            break;
        case 7: // Fadeaway
            ColorUtil::setClientColorType(1);
            ColorUtil::setClientCustomWaveColor(148, 35, 217);
            ColorUtil::setClientCustomWaveColor2(148, 35, 217);
            break;
        case 8: // Xextreame
            ColorUtil::setClientColorType(1);
            ColorUtil::setClientCustomWaveColor(230, 0, 0);
            ColorUtil::setClientCustomWaveColor2(230, 0, 0);
            break;
        case 9: // Zipppo
            ColorUtil::setClientColorType(1);
            ColorUtil::setClientCustomWaveColor(253, 223, 231);
            ColorUtil::setClientCustomWaveColor2(253, 223, 231);
            break;
        case 10: // Suggestion
            ColorUtil::setClientColorType(1);
            ColorUtil::setClientCustomWaveColor(150, 45, 255);
            ColorUtil::setClientCustomWaveColor2(150, 45, 255);
            break;
        }
    }

    void onEvent(ImGUIRenderEvent* event) override
    {
        Player* player = Game::GetLocalPlayer();

        if (!player)
            return;

        std::ostringstream oss;
        oss.precision(2);
        oss << std::fixed << player->getHorizontalSpeed();

        if (Game::Core::showMenu)
        {
            if (showFPS)
                drawTile(combine("FPS: ", Game::gameFps), 1, colortype);

            if (showBPS)
                drawTile(combine("BPS: ", oss.str().c_str()), 0, colortype);
        }
    }

    void drawTile(std::string tileDescr, int index, int colortype)
    {
        Vector2<float> tilePos = Vector2<float>(5, (Game::Core::ClientInstance->getGuiData()->WindowResolution2.y - 25) - (index * 16.f));
        Vector4<float> rectPos = Vector4<float>(5, (Game::Core::ClientInstance->getGuiData()->WindowResolution2.y - 15) - (index * 16.f), 60, (Game::Core::ClientInstance->getGuiData()->WindowResolution2.y - 15) - (index * 16.f));

        UIColor textColor = ColorUtil::getClientColor(2.1, 1, 1, colortype * 80);

        ImRenderUtil::fillShadowRectangle(rectPos, UIColor(0, 0, 0), 0.8f, 50.f, 0);
        ImRenderUtil::drawText(tilePos, &tileDescr, textColor, 1.2f, 1, true);

        float width = ImRenderUtil::getTextWidth(&tileDescr, 1);

        if (size.x < width)
            size.x = width;

        location = tilePos;
        size.y = (index + 1) * 10;
    }

    void onDisabled()
    {
        setEnabled(true);
    }
};
