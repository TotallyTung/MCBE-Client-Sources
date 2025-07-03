#pragma once
#include <algorithm>

int sorting = 1;

class ArrayList : public Module
{
public:
    ArrayList(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Array List", "Visual", "Display enabled modules in a list", keybind, enabled)
    {
        addEnum("Style", "Change the look of the arraylist", { "Outline", "Flux", "Simple", "Rise" }, &style);
        addEnum("Sorting", "Change the sorting of the arraylist", { "Lenght", "Random" }, &sorting);
        addBool("LowerCase", "Change the arraylist text to lower", &lowercase);
        addSlider("Opacity", "The rectangle's opacity.", &opacity, 0, 1);
    }

    bool orientation = false;
    bool fancy = false;
    int style = 1;

    bool lowercase = false;

    float opacity = 0.5f;
    const float textSize = 1.35f;

    void onEvent(ImGUIRenderEvent* event) override
    {
        //ImRenderUtil::drawText(Vector2<float>(6, 6), &GGwha, UIColor(255, 255, 255), 5, 1, true);
        ImDrawList* d = ImGui::GetBackgroundDrawList();
        if (/*getModuleByName("clickgui")->isEnabled() or */!d && Game::GetInstance() == nullptr) return;
        //if (Game::GetInstance()->minecraftGame->canMoveKey)
        //std::string GGwha = std::to_string(Position->x) + " " + std::to_string(Position->y) + " " + std::to_string(Position->z);

        // Create a copy of the modules list
        static size_t previousSize;
        static std::vector<Module*> sortedModules;
        static std::vector<Module*> currentModules;

        for (auto mod : modules)
        {
            // Hide modules that aren't enabled or visible
            if (!mod->isEnabled() && mod->animPercentage <= 0.01 || !mod->isVisible()) continue;
            sortedModules.push_back(mod);
        }

        // Sort the modules if the vector size has changed (this is so scuffed lmao)
        if (sortedModules.size() != previousSize)
        {
            sortModules(sortedModules);
            currentModules.clear();
            currentModules = sortedModules;
            previousSize = sortedModules.size();
        }

        // Define the textSize and module index
        const float textHeight = ImRenderUtil::getTextHeight(textSize);
        const float padding = 0.5f * textSize;
        float textPosY = -1 + 10.20;
        int index = 0;

        Vector4<float> underline = Vector4<float>();
        float textWid = 0;
        float easeAnim = 0;
        float black;
        black = 0;
        if (style == 0) {
            black = style == 0 ? 4 : 0;
        }
        else if (style == 1) {
            black = style == 1 ? 4 : 0;
        }
        else if (style == 3) {
            black = style == 3 ? 4 : 0;
        }

        std::vector<std::tuple<Vector4<float>, std::string, std::string, Vector2<float>, float, float>> glowDraw;
        std::vector<std::tuple<Vector4<float>, std::string, std::string, Vector2<float>, float, float>> glowBarDraw;
        for (Module* mod : currentModules)
        {
            float targetAnim = mod->isEnabled() ? 1.f : 0.f;
            mod->animPercentage = Math::animate(targetAnim, mod->animPercentage, ImRenderUtil::getDeltaTime() * 10);
            mod->animPercentage = Math::clamp(mod->animPercentage, 0.f, 1.f);
            easeAnim = mod->animPercentage;

            std::string modName = mod->getName();
            std::string modMode = mod->getModeName();
            if (lowercase) {
                std::transform(modName.begin(), modName.end(), modName.begin(), ::tolower);
                std::transform(modMode.begin(), modMode.end(), modMode.begin(), ::tolower);
            }
            float modWidth = ImRenderUtil::getTextWidth(&modName, textSize);
            float modeWidth = !modMode.empty() ? ImRenderUtil::getTextWidth(&modMode, textSize) : 0.f;
            float modulePosX = (ImGui::GetIO().DisplaySize.x - modWidth - modeWidth - padding - black) - 10.20;
            mod->pos.x = Math::lerp(ImGui::GetIO().DisplaySize.x + 5.f, modulePosX, easeAnim);
            mod->pos.y = textPosY;

            UIColor arrayColor = ColorUtil::rainbowColor(4, 1, 1, index * 100, 255);
            Vector4<float> boxRect(mod->pos.x - padding - 2.f, textPosY, ImRenderUtil::getScreenSize().x, textPosY + textHeight);
            Vector4<float> rightSide(boxRect.x + modWidth + modeWidth + 1.f + black, boxRect.y, boxRect.x + modWidth + modeWidth + 3 + black, boxRect.w);
            Vector4<float> rightSide2(boxRect.x + modWidth + modeWidth + 1.f + black, boxRect.y + 1, boxRect.x + modWidth + modeWidth + 3 + black, boxRect.w - 1);
            Vector4<float> leftSide(boxRect.x - 1.f, boxRect.y, boxRect.x, boxRect.w + 1);
            underline = Vector4<float>(boxRect.z - textWid, boxRect.y, boxRect.x, boxRect.y + 1.f);

            ImRenderUtil::fillRectangle(boxRect, UIColor(0, 0, 0), easeAnim * opacity);

            switch (style)
            {
            case 0:
                ImRenderUtil::fillRectangle(leftSide, arrayColor, easeAnim);
                ImRenderUtil::fillRectangle(underline, arrayColor, 1.f);
                ImRenderUtil::fillRectangle(rightSide, arrayColor, easeAnim, 3.f);
                break;
            case 1:
                ImRenderUtil::fillRectangle(rightSide, arrayColor, easeAnim, 3.f);
                break;
            case 3:
                ImRenderUtil::fillRectangle(rightSide2, arrayColor, easeAnim, 3.f);
                break;
            }

            glowDraw.push_back(std::make_tuple(boxRect, modName, modMode, Vector2<float>(mod->pos.x, mod->pos.y), modWidth, easeAnim));
            glowBarDraw.push_back(std::make_tuple(rightSide, modName, modMode, Vector2<float>(mod->pos.x, mod->pos.y), modWidth, easeAnim));

            textWid = boxRect.z - boxRect.x + 1.f;
            textPosY = Math::lerp(textPosY, textPosY + textHeight, easeAnim);
            underline = Vector4<float>(boxRect.x, boxRect.w, boxRect.z + 1.f, boxRect.w + 1.f);

            ++index;
        }
        if (!style) ImRenderUtil::fillRectangle(Vector4<float>(underline), ColorUtil::rainbowColor(4, 1, 1, index * 100, 255), easeAnim);

        int colorIndex = 0;
        for (const auto& pos : glowDraw) {
            Vector4<float> boxPos = std::get<0>(pos);
            std::string modName = std::get<1>(pos);
            std::string modMode = std::get<2>(pos);
            Vector2<float> modPos = std::get<3>(pos);
            float modWidth = std::get<4>(pos);
            float anim = std::get<5>(pos);

            UIColor arrayColor = ColorUtil::rainbowColor(4, 1, 1, colorIndex * 100, 255);

            ImRenderUtil::fillShadowRectangle(boxPos, UIColor(0, 0, 0), anim * 1, 45, 0);

            ImRenderUtil::drawText(modPos, &modName, arrayColor, textSize, anim, true);
            ImRenderUtil::drawText(Vector2<float>(modPos.x + modWidth, modPos.y), &modMode, UIColor(170, 170, 170), textSize, anim, true);
            //ImRenderUtil::drawText(Vector2<float>(modPos.x + modWidth, modPos.y), &modMode, UIColor(170, 170, 170), textSize, anim, true);

            

            colorIndex++;
        }

        for (const auto& pos : glowBarDraw) {
            Vector4<float> boxPos = std::get<0>(pos);
            std::string modName = std::get<1>(pos);
            std::string modMode = std::get<2>(pos);
            Vector2<float> modPos = std::get<3>(pos);
            float modWidth = std::get<4>(pos);
            float anim = std::get<5>(pos);

            UIColor arrayColor = ColorUtil::rainbowColor(4, 1, 1, colorIndex * 100, 255);

            ImRenderUtil::fillShadowRectangle(boxPos, arrayColor, anim * 1, 60, 0);

            colorIndex++;
        }

        sortedModules.clear();
    }

private:
    void sortModules(std::vector<Module*>& sortedModules)
    {
        std::sort(sortedModules.begin(), sortedModules.end(), [](Module* a, Module* b) {
            std::string nameA = a->getName() + a->getModeName();
            std::string nameB = b->getName() + b->getModeName();
            float textSizeA = ImRenderUtil::getTextWidth(&nameA, 1.10);
            float textSizeB = ImRenderUtil::getTextWidth(&nameB, 1.10);
            if (sorting == 0) {
                return textSizeA > textSizeB;
            }
            if (sorting == 1) {
                return textSizeA > textSizeA;
            }
            });
    }

};