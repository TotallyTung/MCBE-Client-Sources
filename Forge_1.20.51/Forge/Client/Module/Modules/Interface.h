#pragma once

class Interface : public Module
{
private:
    Vector2<float> logoPos = Vector2<float>(-20, -20); //lmao

public:
    bool watermark = true;

    Interface(int keybind = Keyboard::NONE, bool enabled = true) :
        Module("Interface", "Render", "Display info on the screen", keybind, enabled)
    {
        addBool("Watermark", "displays the client name", &watermark);
    }

    void onEvent(ImGUIRenderEvent* event) override
    {
        float fontSize = 2.5;

        Vector2<float> logoPos = Vector2<float>(6.f, 6.f);

        Vector2<float> pos2 = logoPos;
        int ind = 0;
        int ind2 = 0;
        std::string name = "Forge Client";

        name = "Forge Client";

        for (char c : (std::string)name)
        {
            std::string string = combine(c, "");
            int colorIndex = ind;

            float charWidth = ImRenderUtil::getTextWidth(&string, fontSize);
            float charHeight = ImRenderUtil::getTextHeight(fontSize);
            Vector4<float> rect(pos2.x, pos2.y, pos2.x + 20, pos2.y + 20);
            ImRenderUtil::drawShadowSquare(Vector2<float>(pos2.x + charWidth / 2, pos2.y + charHeight / 1.2), 15.f, ColorUtil::rainbowColor(1.5, 1, 1, colorIndex * 80, 255), 1.f, 70.f, 0);

            ImRenderUtil::drawText(Vector2<float>(pos2.x + 11.5, pos2.y + 10), &string, UIColor(0, 0, 0), fontSize, 1, false);
            ImRenderUtil::drawText(Vector2<float>(pos2.x + 10, pos2.y + 10), &string, ColorUtil::rainbowColor(1.5, 1, 1, colorIndex * 80, 255), fontSize, 1, false);
            ImRenderUtil::drawText(Vector2<float>(pos2.x + 10.5, pos2.y + 10), &string, ColorUtil::rainbowColor(1.5, 1, 1, colorIndex * 80, 255), fontSize, 1, false);
            ImRenderUtil::drawText(Vector2<float>(pos2.x + 11, pos2.y + 10), &string, ColorUtil::rainbowColor(1.5, 1, 1, colorIndex * 80, 255), fontSize, 1, false);

            pos2.x += charWidth;
            ++ind;
        }
    }


    /*void onImGuiRender() override
    {
        float fontSize = 2.5;

        logoPos = logoPos.animate(Vector2<float>(6.f, 6.f), logoPos, 0.016f * 10.f);

        Vector2<float> pos2 = logoPos;
        int ind = 0;
        int ind2 = 0;
        std::string name = "Sniper Client";

        name = "Sniper Client";

        for (char c : (std::string)name)
        {
            std::string string = ImRenderUtil::combine(c, "");
            int colorIndex = ind * 25;

            float charWidth = ImRenderUtil::getTextWidth(&string, fontSize);
            float charHeight = ImRenderUtil::getTextHeight(fontSize);
            Vector4<float> rect(pos2.x, pos2.y, pos2.x + 20, pos2.y + 20);
            ImRenderUtil::drawShadowSquare(Vector2<float>(pos2.x + charWidth / 2, pos2.y + charHeight / 1.2), 15.f, ColorUtil::waveColor(2, 69, 95, 255, 255, 255, colorIndex * 80, 255), 1.f, 70.f, 0);

            ImRenderUtil::drawText(Vector2<float>(pos2.x + 10, pos2.y + 10), &string, ColorUtil::waveColor(2, 69, 95, 255, 255, 255, colorIndex * 80, 255), fontSize, 1, false);
            ImRenderUtil::drawText(Vector2<float>(pos2.x + 10.5, pos2.y + 10), &string, ColorUtil::waveColor(2, 69, 95, 255, 255, 255, colorIndex * 80, 255), fontSize, 1, false);
            ImRenderUtil::drawText(Vector2<float>(pos2.x + 11, pos2.y + 10), &string, ColorUtil::waveColor(2, 69, 95, 255, 255, 255, colorIndex * 80, 255), fontSize, 1, false);

            pos2.x += charWidth;
            ++ind;
        }
    }

    void onEnabled() {
        logoPos = Vector2<float>(-20, -20);
    }*/
};