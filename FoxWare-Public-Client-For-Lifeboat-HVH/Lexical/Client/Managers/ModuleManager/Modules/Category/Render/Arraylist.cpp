#include "Arraylist.h"
#include "../../../../../../Utils/TimerUtil.h"
#include "../../../ModuleManager.h"
bool doblur;
Arraylist::Arraylist() : Module("Arraylist", "Display module enabled", Category::RENDER) {
    registerSetting(new BoolSetting("Bottom", "Fat", &bottom, true));
    registerSetting(new BoolSetting("Show Modes", "Show Modes", &showModes, true));
    registerSetting(new BoolSetting("Blur", "bg blur", &doblur, true));
    // Slider para controlar o espaçamento
    registerSetting(new SliderSetting<float>("Spacing X", "Adjust spacing X", &spacing, 0, -5, 5));
    registerSetting(new SliderSetting<float>("Spacing Y", "Adjust spacing Y", &spacing2, 0, -5, 5));

    // Slider para controlar o tamanho dos módulos
    registerSetting(new SliderSetting<float>("Size", "Control the size of modules", &size, 1.0f, 0.1f, 2.0f)); // Tamanho dos módulos

    registerSetting(new BoolSetting("Outline", "NULL", &outline, false));
 //   registerSetting(new ColorSetting("MainColor", "NULL", &color, color, false));
    registerSetting(new BoolSetting("Rainbow", "Enable Gay Mode For You", &rainbow, false));

    registerSetting(new EnumSetting("ModeOutline", "NULL", { "Horizontal", "Vertical" }, &Enum2, 0));

    registerSetting(new SliderSetting<int>("Opacity", "NULL", &opacity, 130, 0, 255));
    registerSetting(new SliderSetting<int>("Offset", "NULL", &offset, 0, 0, 30));
}

bool Arraylist::sortByLength(Module* lhs, Module* rhs) {
    float textSize = 0.7;

    float textWidth1 = D2D::getTextWidth(lhs->getModuleName(), textSize);
    float textWidth2 = D2D::getTextWidth(rhs->getModuleName(), textSize);

    static Arraylist* arralistMod = ModuleManager::getModule<Arraylist>();

    if (arralistMod->showModes) {
        if (lhs->getModeText() != "NULL")
            textWidth1 += D2D::getTextWidth(" " + lhs->getModeText(), textSize);
        if (rhs->getModeText() != "NULL")
            textWidth2 += D2D::getTextWidth(" " + rhs->getModeText(), textSize);
    }

    // Garantir que a keybind é válida antes de adicionar
    std::string keybindText1 = "[" + std::string(KeyNames[lhs->getKeybind()]) + "]";
    std::string keybindText2 = "[" + std::string(KeyNames[rhs->getKeybind()]) + "]";

    if (keybindText1 != "[Unknown]")
        textWidth1 += D2D::getTextWidth(" " + keybindText1, textSize);

    if (keybindText2 != "[Unknown]")
        textWidth2 += D2D::getTextWidth(" " + keybindText2, textSize);

    return (textWidth1 > textWidth2);
}

void Arraylist::onD2DRender() {
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (localPlayer == nullptr)
        return;
    static Colors* cmod = ModuleManager::getModule<Colors>();
    Vec2<float> windowSize = Game::clientInstance->guiData->windowSizeReal;
    Vec2<float> windowSizeScaled = Game::clientInstance->guiData->windowSizeScaled;

    // Usando o tamanho do módulo controlado pelo slider
    float textSize = size; // Tamanho do módulo controlado pelo slider
    float textPaddingX = std::max(1.f, ((float)spacing + 3.f)) * textSize;
    float textPaddingY = (float)spacing2 * textSize;
    float textHeight = D2D::getTextHeight("", textSize);

    float lineWidth = 1.5f * textSize;

    Vec2<float> Pos = Vec2<float>(windowSize.x - (float)offset, (float)offset);
    if (bottom)
        Pos.y = windowSize.y - (textHeight + textPaddingY * 2.f) - (float)offset;

    UIColor mainColor = UIColor(cmod->getColor());
    UIColor modeColor = UIColor(255, 255, 255);



    static std::vector<Module*> moduleList = ModuleManager::moduleList;
    std::sort(moduleList.begin(), moduleList.end(), sortByLength);

    int index = 0;
    Vec4<float> lastRect;

    for (auto& mod : moduleList) {
        std::string moduleName = mod->getModuleName();
        std::string modeText = mod->getModeText();
        bool shouldRender = (mod->isEnabled() && mod->isVisible());
        std::string keybindText = "[" + std::string(KeyNames[mod->getKeybind()]) + "]";
        if (!showModes)
            modeText = "NULL";

        if (keybindText == "[Unknown]")
            keybindText = "";

        if (rainbow) {
            // Ajustando a saturação com o valor de rainbowSaturation
            float saturation = 127.5f / 255.0f;  // Mapeando de 0 a 255 para 0 a 1

            // Usando a saturação ajustada ao gerar a cor
            mainColor = ColorUtil::getRainbowColor(7.f, saturation, 1.f, (int)(index * 200 * (bottom ? -1.f : 1.f)));
        }

            modeColor = UIColor(mainColor);



        //mod->arraylistAnim = Math::lerp(mod->arraylistAnim, shouldRender ? 1.05f : -0.1f, D2D::deltaTime * 9.f);
        mod->arraylistAnim = Math::moveTowards(mod->arraylistAnim, shouldRender ? 1.05f : -0.1f, D2D::deltaTime * 10.f);


        if (mod->arraylistAnim > 1.f)
            mod->arraylistAnim = 1.f;
        if (mod->arraylistAnim < 0.f)
            mod->arraylistAnim = 0.f;
        slider = (mod->arraylistAnim);
        if (mod->arraylistAnim > 0.f) {
            float fullTextWidth = D2D::getTextWidth(moduleName, textSize);
            if (modeText != "NULL")
                fullTextWidth += D2D::getTextWidth(" " + modeText, textSize);
            if (keybindText == "") {
                fullTextWidth += D2D::getTextWidth(keybindText, textSize);
            }
            else {
                fullTextWidth += D2D::getTextWidth(" " + keybindText, textSize);
            }

            Vec4<float> rectPos = Vec4<float>(Pos.x - textPaddingX - fullTextWidth - textPaddingX,
                Pos.y,
                Pos.x,
                Pos.y + textPaddingY + textHeight + textPaddingY);




            float animRect = (windowSize.x + 25.f - rectPos.x) * (1.f - mod->arraylistAnim);
            rectPos.x += animRect;
            rectPos.z += animRect;
            // Definir o valor de 1 cm em pixels (aproximadamente)
            const float cmInPixels = 2.0f;
            const float cmInPixels2 = 0.8f;

            // Calcular o valor para subir 1 cm
            float offsetY = cmInPixels;
            float offsetX = cmInPixels2;

            // Ajustar a posição do texto, movendo 1 cm para cima
            Vec2<float> textPos = Vec2<float>(rectPos.x + (textPaddingX + offsetX),  rectPos.y + textPaddingY - offsetY);
            if (doblur) {
                D2D::addBlur(rectPos, 10.f);
            }
            D2D::fillRectangle(rectPos, UIColor(10, 10, 10, opacity));
            Vec4<float> rectPos22 = Vec4<float>(rectPos.x,
                rectPos.y - 1.0f,
                rectPos.z,
                rectPos.w);
            D2D::fillRectangle(rectPos22, UIColor(10, 10, 10, opacity));
            if (outline) {
                // Ajuste para esconder as linhas horizontais (não desenhá-las)
                float offsetY = 0.f;  // Ajuste vertical
                float offsetX = 0.f;    // Ajuste horizontal
                // Ocultando ou empurrando as linhas horizontais para fora da tela
                if (Enum2 == 1) {
                    // Aumente o valor de lineWidth aqui para aumentar a espessura das outlines horizontais
                    float increasedLineWidth = lineWidth * 1.7f; // Exemplo: dobra a espessura

                    // Desenhando as linhas verticais com a outline mais grossa
                    D2D::fillRectangle(Vec4<float>(rectPos.x + offsetX - increasedLineWidth, rectPos.y + offsetY, rectPos.x + offsetX, rectPos.w + offsetY), mainColor);
                    D2D::fillRectangle(Vec4<float>(rectPos.z + offsetX, rectPos.y + offsetY, rectPos.z + offsetX + increasedLineWidth, rectPos.w + offsetY), mainColor);

                    if (index == 0) {
                        // Se você não quiser desenhar as linhas horizontais, pode comentar ou remover as próximas linhas
                        // Desenhando as linhas horizontais fora da tela
                        D2D::fillRectangle(Vec4<float>(rectPos.x + offsetX - increasedLineWidth, offScreenPosition, rectPos.z + offsetX + increasedLineWidth, offScreenPosition + increasedLineWidth), mainColor);
                    }
                    else {
                        // Desenhando as linhas horizontais fora da tela
                        D2D::fillRectangle(Vec4<float>(lastRect.x + offsetX - increasedLineWidth, offScreenPosition, rectPos.x + offsetX, offScreenPosition + increasedLineWidth), mainColor);
                    }
                    offScreenPosition = 999999.f; // Valor para empurrar as linhas horizontais para fora da tela
                }
                else {
                    // Aumente o valor de lineWidth aqui para aumentar a espessura das outlines verticais
                    float increasedLineWidth = lineWidth * 1.7f; // Exemplo: dobra a espessura

                    if (outline) {
                        // Desenhando as linhas verticais com a outline mais grossa
                        D2D::fillRectangle(Vec4<float>(rectPos.x - increasedLineWidth, rectPos.y, rectPos.x, rectPos.w), mainColor);
                        D2D::fillRectangle(Vec4<float>(rectPos.z, rectPos.y, rectPos.z + increasedLineWidth, rectPos.w), mainColor);

                        if (index == 0) {
                            if (bottom)
                                D2D::fillRectangle(Vec4<float>(rectPos.x - increasedLineWidth, rectPos.w, rectPos.z + increasedLineWidth, rectPos.w + increasedLineWidth), mainColor);
                            else
                                D2D::fillRectangle(Vec4<float>(rectPos.x - increasedLineWidth, rectPos.y - increasedLineWidth, rectPos.z + increasedLineWidth, rectPos.y), mainColor);
                        }
                        else {
                            if (bottom)
                                D2D::fillRectangle(Vec4<float>(lastRect.x - increasedLineWidth, rectPos.w - increasedLineWidth, rectPos.x, rectPos.w), mainColor);
                            else
                                D2D::fillRectangle(Vec4<float>(lastRect.x - increasedLineWidth, rectPos.y, rectPos.x, rectPos.y + increasedLineWidth), mainColor);
                        }
                    }
                    offScreenPosition = 0.f;
                }
               
            }
            D2D::drawText(textPos, moduleName, mainColor, textSize, true);
            textPos.x += D2D::getTextWidth(moduleName, textSize);

            if (modeText != "NULL") {
                textPos.x += D2D::getTextWidth(" ", textSize);
                D2D::drawText(textPos, modeText, modeColor, textSize, true);
                textPos.x += D2D::getTextWidth(modeText, textSize);
            }

            textPos.x += D2D::getTextWidth(" ", textSize);
            D2D::drawText(textPos, keybindText, modeColor, textSize, true);

            lastRect = rectPos;
        }

        float yAnim = mod->arraylistAnim * 979.25f;
        if (yAnim > 1.f)
            yAnim = 1.f;

        Pos.y += (textHeight + textPaddingY * 2.f) * yAnim * (bottom ? -1.f : 1.f);

        if (shouldRender)
            index++;
    }
    if (Enum2 == 0) {
        if (outline) {
            // Aumente o valor de lineWidth aqui para aumentar a espessura das outlines
            float increasedLineWidth = lineWidth * 1.7f; // Exemplo: dobra a espessura

            if (bottom)
                D2D::fillRectangle(Vec4<float>(lastRect.x - increasedLineWidth, lastRect.y - increasedLineWidth, lastRect.z + increasedLineWidth, lastRect.y), mainColor);
            else
                D2D::fillRectangle(Vec4<float>(lastRect.x - increasedLineWidth, lastRect.w, lastRect.z + increasedLineWidth, lastRect.w + increasedLineWidth), mainColor);
        }

    }
}
