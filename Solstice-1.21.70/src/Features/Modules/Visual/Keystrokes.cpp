//
// Created by vastrakai on 9/13/2024.
//

#include "Keystrokes.hpp"
#include "HudEditor.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

Keystrokes::Keystrokes(): ModuleBase("Keystrokes", "Shows keystrokes on screen", ModuleCategory::Visual, 0, false)
{
    addSetting(&mUseInterfaceColor);

    mNames = {
        {Lowercase, "keystrokes"},
        {LowercaseSpaced, "keystrokes"},
        {Normal, "Keystrokes"},
        {NormalSpaced, "Keystrokes"}
    };

    mElement = std::make_unique<HudElement>();
    mElement->mPos = { 100, 100 };
    const char* ModuleBaseType = ModuleBase<Keystrokes>::getTypeID();;
    mElement->mParentTypeIdentifier = const_cast<char*>(ModuleBaseType);
    HudEditor::gInstance->registerElement(mElement.get());
}

void Keystrokes::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderEvent, &Keystrokes::onRenderEvent>(this);
    mElement->mVisible = true;
}

void Keystrokes::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &Keystrokes::onRenderEvent>(this);
    mElement->mVisible = false;
}

void Keystrokes::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if(ClientInstance::get()->getScreenName() != "hud_screen") return;

    auto ci = ClientInstance::get();
    auto moveCo = player->getMoveInputComponent();

    bool forward = moveCo->mForward;
    bool back = moveCo->mBackward;
    bool left = moveCo->mLeft;
    bool right = moveCo->mRight;
    bool jump = moveCo->mIsJumping;

    float delta = ImGui::GetIO().DeltaTime;

    float rounding = 8;

    ImVec2 display = ImGui::GetIO().DisplaySize;
    ImVec2 boxSize = ImVec2(140, 140);
    mElement->mSize = glm::vec2(boxSize.x, boxSize.y);

    // Render in top left, with 100y offset
    ImVec2 renderPos = ImVec2(mElement->getPos().x, mElement->getPos().y);


    auto drawList = ImGui::GetBackgroundDrawList();

    ImVec2 keySize = ImVec2(40, 40);
    ImVec2 keyPadding = ImVec2(5, 5);



    ImVec2 spaceSize;
    // Calculate the spaceSize based on the space available
    {
        float spaceWidth = boxSize.x - keyPadding.x * 2;
        float spaceHeight = boxSize.y - keySize.y * 2 - keyPadding.y * 3 - keyPadding.y;
        spaceSize = ImVec2(spaceWidth, spaceHeight);
    }

    static float animSpeed = 10.f;

    // Render the background for the keys
    static std::map<int, float> keyAnimation = {
        {0, 0.f},
        {1, 0.f},
        {2, 0.f},
        {3, 0.f},
        {4, 0.f}
    };

    FontHelper::pushPrefFont(false, true);

    // Anims
    keyAnimation[0] = std::lerp(keyAnimation[0], forward ? 1.f : 0.f, delta * animSpeed);
    keyAnimation[1] = std::lerp(keyAnimation[1], back ? 1.f : 0.f, delta * animSpeed);
    keyAnimation[2] = std::lerp(keyAnimation[2], left ? 1.f : 0.f, delta * animSpeed);
    keyAnimation[3] = std::lerp(keyAnimation[3], right ? 1.f : 0.f, delta * animSpeed);
    keyAnimation[4] = std::lerp(keyAnimation[4], jump ? 1.f : 0.f, delta * animSpeed);

    // Format:
    //    W
    // A  S  D
    //  SPACE

    // Render the key backgrounds
    // W (first row)

    float size = 20.f;

    // Get W center pos (only center x between box)
    {
        ImVec2 wPos = ImVec2(renderPos.x + boxSize.x / 2 - keySize.x / 2, renderPos.y + keyPadding.y);
        ImVec2 wPosCenter = ImVec2(wPos.x + keySize.x / 2, wPos.y + keySize.y / 2);

        ImColor keyColor = mUseInterfaceColor.mValue ? ColorUtils::getThemedColor(wPos.x + wPos.y * 5) : ImColor(255, 255, 255, 200);
        if (mUseInterfaceColor.mValue) keyColor.Value.w = 200 / 255.f;
        keyColor.Value.w *= keyAnimation[0];

        // Draw the key background
        drawList->AddRectFilled(wPos, ImVec2(wPos.x + keySize.x, wPos.y + keySize.y), IM_COL32(0, 0, 0, 150), rounding);
        ImVec2 scaledW = ImVec2(keySize.x * keyAnimation[0], keySize.y * keyAnimation[0]);
        drawList->AddRectFilled(wPosCenter - scaledW / 2, wPosCenter + scaledW / 2, keyColor, rounding * keyAnimation[0]);

        // Draw the text
        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0, "W");
        drawList->AddText(ImGui::GetFont(), size, ImVec2(wPos.x + keySize.x / 2 - textSize.x / 2, wPos.y + keySize.y / 2 - textSize.y / 2), IM_COL32(255, 255, 255, 255), "W");
    }

    // A (second row)
    {
        ImVec2 aPos = ImVec2(renderPos.x + keyPadding.x, renderPos.y + keySize.y + keyPadding.y * 2);
        ImVec2 aPosCenter = ImVec2(aPos.x + keySize.x / 2, aPos.y + keySize.y / 2);

        ImColor keyColor = mUseInterfaceColor.mValue ? ColorUtils::getThemedColor(aPos.x + aPos.y * 5) : ImColor(255, 255, 255, 200);
        if (mUseInterfaceColor.mValue) keyColor.Value.w = 200 / 255.f;
        keyColor.Value.w *= keyAnimation[2];

        // Draw the key background
        drawList->AddRectFilled(aPos, ImVec2(aPos.x + keySize.x, aPos.y + keySize.y), IM_COL32(0, 0, 0, 150), rounding);
        ImVec2 scaledA = ImVec2(keySize.x * keyAnimation[2], keySize.y * keyAnimation[2]);
        drawList->AddRectFilled(aPosCenter - scaledA / 2, aPosCenter + scaledA / 2, keyColor, rounding * keyAnimation[2]);

        // Draw the text
        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0, "A");
        drawList->AddText(ImGui::GetFont(), size, ImVec2(aPos.x + keySize.x / 2 - textSize.x / 2, aPos.y + keySize.y / 2 - textSize.y / 2), IM_COL32(255, 255, 255, 255), "A");
    }

    // S (second row)
    {
        ImVec2 sPos = ImVec2(renderPos.x + boxSize.x / 2 - keySize.x / 2, renderPos.y + keySize.y + keyPadding.y * 2);
        ImVec2 sPosCenter = ImVec2(sPos.x + keySize.x / 2, sPos.y + keySize.y / 2);

        ImColor keyColor = mUseInterfaceColor.mValue ? ColorUtils::getThemedColor(sPos.x + sPos.y * 5) : ImColor(255, 255, 255, 200);
        if (mUseInterfaceColor.mValue) keyColor.Value.w = 200 / 255.f;
        keyColor.Value.w *= keyAnimation[1];

        // Draw the key background
        drawList->AddRectFilled(sPos, ImVec2(sPos.x + keySize.x, sPos.y + keySize.y), IM_COL32(0, 0, 0, 150), rounding);
        ImVec2 scaledS = ImVec2(keySize.x * keyAnimation[1], keySize.y * keyAnimation[1]);
        drawList->AddRectFilled(sPosCenter - scaledS / 2, sPosCenter + scaledS / 2, keyColor, rounding * keyAnimation[1]);

        // Draw the text
        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0, "S");
        drawList->AddText(ImGui::GetFont(), size, ImVec2(sPos.x + keySize.x / 2 - textSize.x / 2, sPos.y + keySize.y / 2 - textSize.y / 2), IM_COL32(255, 255, 255, 255), "S");
    }

    // D (second row)
    {
        ImVec2 dPos = ImVec2(renderPos.x + boxSize.x - keySize.x - keyPadding.x, renderPos.y + keySize.y + keyPadding.y * 2);
        ImVec2 dPosCenter = ImVec2(dPos.x + keySize.x / 2, dPos.y + keySize.y / 2);

        ImColor keyColor = mUseInterfaceColor.mValue ? ColorUtils::getThemedColor(dPos.x + dPos.y * 5) : ImColor(255, 255, 255, 200);
        if (mUseInterfaceColor.mValue) keyColor.Value.w = 200 / 255.f;
        keyColor.Value.w *= keyAnimation[3];

        // Draw the key background
        drawList->AddRectFilled(dPos, ImVec2(dPos.x + keySize.x, dPos.y + keySize.y), IM_COL32(0, 0, 0, 150), rounding);
        ImVec2 scaledD = ImVec2(keySize.x * keyAnimation[3], keySize.y * keyAnimation[3]);
        drawList->AddRectFilled(dPosCenter - scaledD / 2, dPosCenter + scaledD / 2, keyColor, rounding * keyAnimation[3]);

        // Draw the text
        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0, "D");
        drawList->AddText(ImGui::GetFont(), size, ImVec2(dPos.x + keySize.x / 2 - textSize.x / 2, dPos.y + keySize.y / 2 - textSize.y / 2), IM_COL32(255, 255, 255, 255), "D");
    }

    // SPACE (third row)
    {
        ImVec2 spacePos = ImVec2(renderPos.x + boxSize.x / 2 - spaceSize.x / 2, renderPos.y + keySize.y * 2 + keyPadding.y * 3);
        ImVec2 spacePosCenter = ImVec2(spacePos.x + spaceSize.x / 2, spacePos.y + spaceSize.y / 2);

        ImColor keyColor = mUseInterfaceColor.mValue ? ColorUtils::getThemedColor(spacePos.x + spacePos.y * 5) : ImColor(255, 255, 255, 200);
        if (mUseInterfaceColor.mValue) keyColor.Value.w = 200 / 255.f;
        keyColor.Value.w *= keyAnimation[4];

        // Draw the key background
        drawList->AddRectFilled(spacePos, ImVec2(spacePos.x + spaceSize.x, spacePos.y + spaceSize.y), IM_COL32(0, 0, 0, 150), rounding);
        ImVec2 scaledSpace = ImVec2(spaceSize.x * keyAnimation[4], spaceSize.y * keyAnimation[4]);
        drawList->AddRectFilled(spacePosCenter - scaledSpace / 2, spacePosCenter + scaledSpace / 2, keyColor, rounding * keyAnimation[4]);

    }

    FontHelper::popPrefFont();
}