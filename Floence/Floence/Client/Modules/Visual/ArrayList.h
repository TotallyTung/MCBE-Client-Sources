#pragma once
#include <algorithm>

class ArrayList : public Module
{
public:
    ArrayList(int keybind = 7, bool enabled = false) :
        Module("Arraylist", "Visual", "Display list of modules that are enabled.", keybind, enabled)
    {
        addEnum("Style", "Change the look of the arraylist", { "Outline", "Bar", "None" }, &style);
        addEnum("Animation", "The animation of enabling and disabling", { "Lerp", "Expo", "Bounce", "Elastic", "None" }, &animationMode);
        //addBool("Blur", "Blurs the ArrayList's background", &blur);
        addSlider("Blur Strength", "The strength of the blur", &blurStrength, 0, 15);
        //addSlider("Size", "The size of the arraylist", &textSize, 5, 25);
        //addSlider("Padding", "The padding of the arraylist", &padding, 5, 15);
        //addBool("Text Shadow", "Render shadows for the texts", &TextShadow);
        //addBool("Shadow", "Render shadows for the arraylist's rectangle", &shadow);
        addBool("Fill Shadow", "Fill the rectangle with shadow", &shadowfilled);
        addBool("Modes", "Fill the rectangle with shadow", &modes);
        addSlider("Shadow strenght", "The strenght of the shadow", &shadowstrenght, 0, 200);
        addSlider("Shadow opacity", "The opacity of the shadow", &shadowopacity, 0, 100);
        addSlider("Bar opacity", "The opacity of the bar shadow", &baropacity, 0, 100);
        //addSlider("Opacity", "The opacity of the arraylist", &opacity, 0, 100);
        addSlider("Rounding", "The opacity of the arraylist", &rounding, 0, 10);
        addSlider("Line Width", "Line Width", &lineWidth, 0, 10);
        addSlider("Line Space", "Line Space", &lineSpace, 0, 5);
        addSlider("Offset", "Offset.ahh!", &Offset, -10.f, 70.f);
        addBool("Fill Background", "Fill the rectangle with background", &Background);
        addBool("wavetext", "wave and text and nigger and chicken wing", &wavetext);
    }

    int style = 0;
    int animationMode = 0;
    bool blur = true;
    static inline float blurStrength = 0;
    static inline float textSize = 14;
    static inline float padding = 10;
    bool TextShadow = true;
    bool shadow = true;
    bool shadowfilled = true;
    bool modes = true;
    float shadowstrenght = 0;
    float shadowopacity = 0;
    float baropacity = 100;
    float opacity = 40;
    float rounding = 0;
    int FontMode = 0;
    float lineWidth = 1.f;
    float lineSpace = 1.f;
    float Offset = 1.f;
    bool Background = true;
    bool wavetext = false;
    bool calmtext = false;

    void onEvent(ImGuiRenderEvent* event) override {
        {
            if (Global::interfont == 0)
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);
            TextShadow = true;
        }
        if (Global::interfont == 1)  {
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
            TextShadow = false;
        }
        if (Global::interfont == 2) {
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[6]);
            TextShadow = true;
        }

        if (style == 1) {
            setRenderSetting("Line Width", true);
            setRenderSetting("Line Width", true);
        }
        else {
            setRenderSetting("Line Width", false);
            setRenderSetting("Line Width", false);
        }
        if (style == 1) {
            setRenderSetting("Rounding", true);
            setRenderSetting("Rounding", true);
        }
        else {
            setRenderSetting("Rounding", false);
            setRenderSetting("Rounding", false);
        }

        if (!ImGui::GetBackgroundDrawList() && Address::getClientInstance() == nullptr) return;

        // Create a copy of the modules list
        static size_t previousSize;
        static std::vector<Module*> sortedModules;
        static std::vector<Module*> currentModules;

        for (auto mod : modules) {
            // Hide modules that aren't enabled or visible
            if (!mod->isEnabled() && mod->animPercentage <= 0.01 || !mod->isVisible()) continue;
            sortedModules.push_back(mod);
        }

        // Sort the modules if the vector size has changed (this is so scuffed lmao)
        if (sortedModules.size() != previousSize) {
            sortModules(sortedModules);
            currentModules.clear();
            currentModules = sortedModules;
            previousSize = sortedModules.size();
        }

        // Define the textSize and module index
        const float textHeight = ImRenderUtil::getTextHeight((textSize / 10)) * (padding / 10);
        const float padding = 0.5f * (textSize / 10);
        float textPosY = +6 + 10.50;
        int index = 0; // The RGB Index

        float textWid = 0;
        float easeAnim = 0;
        Vector4<float> underline = Vector4<float>();

        std::vector<std::tuple<Vector4<float>, std::string, std::string, Vector2<float>, float, float>> shadowDraw;
        for (Module* mod : currentModules) {
            float targetAnim = mod->isEnabled() ? 1.f : 0.f;
            mod->animPercentage = Math::animate(targetAnim, mod->animPercentage, ImRenderUtil::getDeltaTime() * (animationMode == 0 ? 15 : 4));
            mod->animPercentage = Math::clamp(mod->animPercentage, 0.f, 1.f);

            if (animationMode == 0) {
                easeAnim = mod->animPercentage;
            }
            else if (animationMode == 1) {
                easeAnim = easeInOutExpo(mod->animPercentage);
            }
            else if (animationMode == 2) {
                easeAnim = easeInOutBounce(mod->animPercentage);
            }
            else if (animationMode == 3) {
                easeAnim = easeInOutElastic(mod->animPercentage);
            }
            else if (animationMode == 4) {
                easeAnim = easeInOutNone(mod->animPercentage);
            }

            std::string ModuleName = mod->getName();
            std::string ModeName = mod->getModeName();
            float modWidth = ImRenderUtil::getTextWidth(&ModuleName, (textSize / 10));
            float modeWidth = !ModeName.empty() ? ImRenderUtil::getTextWidth(&ModeName, (textSize / 10)) : 0.f;
            if (!modes) modeWidth = 0;
            float modulePosX = (ImGui::GetIO().DisplaySize.x - modWidth - modeWidth - padding - lineWidth) - 10.20;
            if (style != 1) modulePosX = (ImGui::GetIO().DisplaySize.x - modWidth - modeWidth - padding) - 10.20;
            mod->pos.x = Math::lerp(ImGui::GetIO().DisplaySize.x + 5.f, modulePosX, easeAnim);
            mod->pos.y = textPosY;

            UIColor arrayColor = ColorUtils::getClientColor(1.8, 1, 1, index * 40);

            // Normal
            Vector4<float> boxRect(mod->pos.x - padding - 2.f - Offset, textPosY + Offset, ImRenderUtil::getScreenSize().x - 6.5f - Offset, textPosY + textHeight + Offset);

            Vector4<float> rightSide(boxRect.z - lineWidth, boxRect.y + lineSpace, boxRect.z, boxRect.w - lineSpace);
            if (style != 1) rightSide = Vector4<float>(boxRect.z - 1.f, boxRect.y, boxRect.z, boxRect.w);
            Vector4<float> leftSide(boxRect.x - 1.f, boxRect.y, boxRect.x, boxRect.w + 1);
            if (style != 1) leftSide = Vector4<float>(boxRect.x - 1.f, boxRect.y, boxRect.x, boxRect.w + 1);

            underline = Vector4<float>(boxRect.z - textWid, boxRect.y, boxRect.x, boxRect.y + 1.f);
            if (style != 1) underline = Vector4<float>(boxRect.z - textWid, boxRect.y, boxRect.x, boxRect.y + 1.f);


            if (blur) {
                ImRenderUtil::Blur(boxRect, blurStrength, rounding / 10);
            }
            if (Background) {
                ImRenderUtil::fillRectangle(boxRect, UIColor(40, 40, 40), easeAnim * (opacity / 100), 0);
            }
            //ImRenderUtil::fillRectangle(boxRect, UIColor(0, 0, 0), easeAnim * (opacity / 100), rounding / 10);

//            if (style != 1) {
//                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);
//            }
//            if (style = 1) {
//                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
//            }
            //ImRenderUtil::Blur(boxRect, 6, rounding / 10);

            switch (style) {
            case 0: // Outline
                ImRenderUtil::fillRectangle(leftSide, arrayColor, easeAnim);
                ImRenderUtil::fillRectangle(underline, arrayColor, 1.f);
                ImRenderUtil::fillRectangle(rightSide, arrayColor, easeAnim, rounding / 10);
                break;
            case 1: // Bar
                ImRenderUtil::fillRectangle(rightSide, arrayColor, easeAnim, rounding * 10);
                break;
            }

            shadowDraw.push_back(std::make_tuple(boxRect, ModuleName, ModeName, Vector2<float>(mod->pos.x, mod->pos.y), modWidth, easeAnim));

            textWid = boxRect.z - boxRect.x + 1.f;
            textPosY = Math::lerp(textPosY, textPosY + textHeight, easeAnim); // Set a space between arraylist modules.
            underline = Vector4<float>(boxRect.x, boxRect.w, boxRect.z, boxRect.w + 1);

            ++index;
        }
        if (!style) ImRenderUtil::fillRectangle(Vector4<float>(underline), ColorUtils::getClientColor(1.8, 1, 1, index * 40), easeAnim);

        int colorIndex = 0;
        for (const auto& pos : shadowDraw) {
            Vector4<float> boxPos = std::get<0>(pos);
            std::string ModuleName = std::get<1>(pos);
            std::string ModeName = std::get<2>(pos);
            Vector2<float> ModulePos = std::get<3>(pos);
            float ModuleWidth = std::get<4>(pos);
            float anim = std::get<5>(pos);

            UIColor arrayColor = ColorUtils::getClientColor(1.8, 1, 1, colorIndex * 40);
            Vector4<float> rightSide(boxPos.z, boxPos.y, boxPos.z + lineWidth, boxPos.w);

            if (shadow) {
                switch (style) {
                case 0: // Outline
                    ImRenderUtil::fillShadowRectangle(boxPos, arrayColor, (shadowopacity / 100) * anim, shadowstrenght, shadowfilled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground, rounding / 10);
                    break;
                case 1: // Bar
                    ImRenderUtil::fillShadowRectangle(boxPos, UIColor(0, 0, 0), (shadowopacity / 100) * anim, shadowstrenght, shadowfilled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground, rounding / 10);
                    ImRenderUtil::fillShadowRectangle(rightSide, arrayColor, (baropacity / 100) * anim, shadowstrenght, shadowfilled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground, rounding / 10);
                    break;
                case 2: // None
                    ImRenderUtil::fillShadowRectangle(boxPos, arrayColor, (shadowopacity / 100) * anim, shadowstrenght, shadowfilled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground, rounding / 10);
                    break;
                }
            }

            if (wavetext) {
                ImRenderUtil::drawGradientText(Vector2<float>(ModulePos.x - Offset, ModulePos.y + Offset), &ModuleName, (textSize / 10), anim, TextShadow, 2.5, 100);
                if (modes) ImRenderUtil::drawText(Vector2<float>(ModulePos.x + ModuleWidth - Offset, ModulePos.y + Offset), &ModeName, UIColor(127, 127, 127), (textSize / 10), anim, TextShadow);
            }
            else if (calmtext) {
                ImRenderUtil::drawGradientText(Vector2<float>(ModulePos.x - Offset, ModulePos.y + Offset), &ModuleName, (textSize / 10), anim, TextShadow, 5, 50);
                if (modes) ImRenderUtil::drawText(Vector2<float>(ModulePos.x + ModuleWidth - Offset, ModulePos.y + Offset), &ModeName, UIColor(127, 127, 127), (textSize / 10), anim, TextShadow);
            }
            else {
                ImRenderUtil::drawText(Vector2<float>(ModulePos.x - Offset, ModulePos.y + Offset), &ModuleName, arrayColor, (textSize / 10), anim, TextShadow);
                if (modes) ImRenderUtil::drawText(Vector2<float>(ModulePos.x + ModuleWidth - Offset, ModulePos.y + Offset), &ModeName, UIColor(127, 127, 127), (textSize / 10), anim, TextShadow);
            }

            colorIndex++;
        }

        sortedModules.clear(); // Clear sortedModules
    }
private:
    inline float easeInOutExpo(float x) {
        return x == 0
            ? 0
            : x == 1
            ? 1
            : x < 0.5 ? pow(2, 20 * x - 10) / 2
            : (2 - pow(2, -20 * x + 10)) / 2;
    };

    inline float easeInOutBounce(float x) {
        if (x < 0.5) {
            return 8 * pow(2, 8 * (x - 1)) * abs(sin(x * PI * 7));
        }
        else {
            return 1 - 8 * pow(2, -8 * x) * abs(sin(x * PI * 7));
        }
    };

    __forceinline double easeInOutElastic(float x) {
        double t2;
        if (x < 0.45) {
            t2 = x * x;
            return 8 * t2 * t2 * sin(x * PI * 9);
        }
        else if (x < 0.55) {
            return 0.5 + 0.75 * sin(x * PI * 4);
        }
        else {
            t2 = (x - 1) * (x - 1);
            return 1 - 8 * t2 * t2 * sin(x * PI * 9);
        }
    }
    inline float easeInOutNone(float x) {
        return x == 0
            ? 0
            : x == 1
            ? 1
            : x < 0.5 ? 0
            : 1;
    }


    void sortModules(std::vector<Module*>& sortedModules) { // Sort the modules by length.
        bool modes2 = false;
        if (modes) modes2 = true;

        std::sort(sortedModules.begin(), sortedModules.end(), [modes2](Module* a, Module* b) {
            std::string nameA = a->getName() + a->getModeName(); // First module.
            std::string nameB = b->getName() + b->getModeName(); // Last module.
            if (!modes2) {
                nameA = a->getName();
                nameB = b->getName();
            }
            float textSizeA = ImRenderUtil::getTextWidth(&nameA, (textSize / 10)); // First module's Length.
            float textSizeB = ImRenderUtil::getTextWidth(&nameB, (textSize / 10)); // Last module's Lenght.
            return textSizeA > textSizeB; // Sort by the most lenght to lower.
            });
    }

};