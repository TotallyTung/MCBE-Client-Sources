//
// Created by vastrakai on 6/29/2024.
//

#include "ClickGui.hpp"

#include <Features/Events/MouseEvent.hpp>
#include <Features/Events/KeyEvent.hpp>
#include <Features/GUI/ModernDropdown.hpp>
#include <Features/GUI/ScriptingGui.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>

static bool lastMouseState = false;
static bool isPressingShift = false;
static ModernGui modernGui = ModernGui();


void ClickGui::onEnable()
{
    auto ci = ClientInstance::get();
    lastMouseState = !ci->getMouseGrabbed();

    ci->releaseMouse();

    gFeatureManager->mDispatcher->listen<MouseEvent, &ClickGui::onMouseEvent>(this);
    gFeatureManager->mDispatcher->listen<KeyEvent, &ClickGui::onKeyEvent, nes::event_priority::FIRST>(this);
}

void ClickGui::onDisable()
{
    gFeatureManager->mDispatcher->deafen<MouseEvent, &ClickGui::onMouseEvent>(this);
    gFeatureManager->mDispatcher->deafen<KeyEvent, &ClickGui::onKeyEvent>(this);

    if (lastMouseState) {
        ClientInstance::get()->grabMouse();
    }
}

void ClickGui::onWindowResizeEvent(WindowResizeEvent& event)
{
    modernGui.onWindowResizeEvent(event); // are you okay in the head 😭
}


void ClickGui::onMouseEvent(MouseEvent& event)
{
    event.mCancelled = true;
}

void ClickGui::onKeyEvent(KeyEvent& event)
{
    if (event.mKey == VK_ESCAPE) {
        if (!modernGui.isBinding && event.mPressed) this->toggle();
        event.mCancelled = true;
    }

    // Consume all key events to prevent unintended module toggles
    if (modernGui.isBinding) {
        event.mCancelled = true;
        return;
    }

    if (event.mKey == VK_SHIFT && event.mPressed) {
        isPressingShift = true;
        event.mCancelled = true;
    }
    else {
        isPressingShift = false;
    }
}

float ClickGui::getEaseAnim(EasingUtil ease, int mode) {
    switch (mode) {
    case 0: return ease.easeOutExpo(); break;
    case 1: return mEnabled ? ease.easeOutElastic() : ease.easeOutBack(); break;
    default: return ease.easeOutExpo(); break;
    }

}

enum class Tab
{
    ClickGui,
    HudEditor,
    Scripting
};

void ClickGui::onRenderEvent(RenderEvent& event)
{
    if (mEnabled) ClientInstance::get()->releaseMouse();
    static float animation = 0;
    static int styleMode = 0; // Ease enum
    static int scrollDirection = 0;
    static char h[2] = { 0 };
    static EasingUtil inEase = EasingUtil();

    float delta = ImGui::GetIO().DeltaTime;

    this->mEnabled ? inEase.incrementPercentage(delta * mEaseSpeed.mValue / 10)
    : inEase.decrementPercentage(delta * 2 * mEaseSpeed.mValue / 10);
    float inScale = getEaseAnim(inEase, mAnimation.as<int>());
    if (inEase.isPercentageMax()) inScale = 0.996;
    if (mAnimation.mValue == ClickGuiAnimation::Zoom) inScale = MathUtils::clamp(inScale, 0.0f, 0.996);
    animation = MathUtils::lerp(0, 1, inEase.easeOutExpo());

    if (animation < 0.0001f) {
        return;
    }

    // Use IO to determine the scroll direction
    if (ImGui::GetIO().MouseWheel > 0) {
        scrollDirection = -1;
    }
    else if (ImGui::GetIO().MouseWheel < 0) {
        scrollDirection = 1;
    }
    else {
        scrollDirection = 0;
    }

    static Tab currentTab = Tab::ClickGui;


    auto drawList = ImGui::GetForegroundDrawList(); // because we need to draw in front of everything else

    switch (currentTab)
    {
    case Tab::ClickGui:
        if (mStyle.mValue == ClickGuiStyle::Modern)
            modernGui.render(animation, inScale, scrollDirection, h, mBlurStrength.mValue, mMidclickRounding.mValue, isPressingShift);
        break;
    case Tab::HudEditor:
        {
            // draw red text in the middle of the screen saying "Not implemented yet!"
            ImVec2 screen = ImRenderUtils::getScreenSize();
            drawList->AddRectFilled(ImVec2(0, 0), ImVec2(screen.x, screen.y), IM_COL32(0, 0, 0, 255 * animation * 0.38f));
            ImRenderUtils::addBlur(ImVec4(0.f, 0.f, screen.x, screen.y),
                               animation * mBlurStrength.mValue, 0);
            ImColor shadowRectColor = ColorUtils::getThemedColor(0);
            shadowRectColor.Value.w = 0.5f * animation; // Adjust rect alpha with animation

            float firstheight = (screen.y - screen.y / 3);

            firstheight = MathUtils::lerp(screen.y, firstheight, inScale);
            ImRenderUtils::fillGradientOpaqueRectangle(
                ImVec4(0, firstheight, screen.x, screen.y),
                shadowRectColor, shadowRectColor, 0.4f * inScale, 0.0f);

            FontHelper::pushPrefFont(true, false, true);
            float fontSize = 25.f;
            float fontX = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, "Not implemented yet!").x;
            drawList->AddText(ImVec2(ImRenderUtils::getScreenSize().x / 2 - (fontX), ImRenderUtils::getScreenSize().y / 2), IM_COL32(255, 0, 0, 255), "Not implemented yet!");
            FontHelper::popPrefFont();
            //HudEditor::render(inScale, animation, ImRenderUtils::getScreenSize());
            break;
        }
    case Tab::Scripting:
        ScriptingGui::render(inScale, animation, ImRenderUtils::getScreenSize(), mBlurStrength.mValue);
        break;
    }

    //ScriptingGui::render(inScale, animation, ImRenderUtils::getScreenSize());

    FontHelper::pushPrefFont(false, false, true);

    // Render tabs at the top
    std::vector<std::pair<Tab, std::string>> tabs = {
        {Tab::ClickGui, "ClickGui"},
        {Tab::HudEditor, "HudEditor"},
        {Tab::Scripting, "Scripting"}
    };

    float paddingBetween = 20.f;  // Increased padding for a better look
    float fontSize = 25.f;
    static ImVec2 underlinePos = ImVec2(0, 0);
    static ImVec2 underlineSize = ImVec2(0, 0);

    // Calculate the text sizes for each tab
    std::map<Tab, ImVec2> tabTextSizes;
    for (auto& tab : tabs)
    {
        tabTextSizes[tab.first] = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, tab.second.c_str());
    }

    // Calculate the entire size of each tab including where their padding is
    std::map<Tab, ImVec2> tabSizes;
    for (auto& tab : tabs)
    {
        tabSizes[tab.first] = ImVec2(tabTextSizes[tab.first].x + paddingBetween, tabTextSizes[tab.first].y);
    }

    // Calculate total width of all tabs + padding
    auto windowSize = ImGui::GetIO().DisplaySize;
    float totalWidth = paddingBetween * (tabs.size() - 1);
    for (auto& tab : tabs)
    {
        totalWidth += tabTextSizes[tab.first].x;
    }

    float anim = inScale;
    float startY = -35; // Start the tabs off screen
    float targetY = 10;

    // Calculate the starting X position to center tabs horizontally
    float x = (windowSize.x - totalWidth) / 2;
    float y = MathUtils::lerp(startY, targetY, anim);

    // Render background behind the tabs
    ImVec4 bg = ImVec4(x - paddingBetween, y, x + totalWidth, y + tabTextSizes[currentTab].y + 5);
    drawList->AddRectFilled(ImVec2(bg.x, bg.y), ImVec2(bg.z, bg.w), IM_COL32(30, 30, 30, 180), 5.f);

    std::map<Tab, ImVec2> tabPositions;

    for (auto& tab : tabs)
    {
        ImVec2 textSize = tabTextSizes[tab.first];
        ImVec2 textPos = ImVec2(x, y);

        // Center text vertically
        textPos.y += (bg.w - bg.y - textSize.y) / 2;

        // Center text horizontally by adjusting x for the tab's text size
        float centeredX = x + (textSize.x / 2) - (tabTextSizes[tab.first].x / 2);

        // Update position to center horizontally
        textPos.x = centeredX;

        // Check if the mouse is hovering over the tab
        if (ImRenderUtils::isMouseOver(ImVec4(x, y, x + textSize.x, y + textSize.y)))
        {
            if (ImGui::IsMouseClicked(0))
            {
                currentTab = tab.first;
            }
        }

        tabPositions[tab.first] = textPos;

        // Render the tab text
        drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), tab.second.c_str());

        // Move to the next tab position
        x += textSize.x + paddingBetween;
    }

    // Animate the underline position and size
    ImVec2 underlineTargetPos = ImVec2(tabPositions[currentTab].x - paddingBetween, tabPositions[currentTab].y + tabSizes[currentTab].y + 2);
    underlinePos.x = MathUtils::lerp(underlinePos.x, underlineTargetPos.x, ImGui::GetIO().DeltaTime * 10);
    underlinePos.y = underlineTargetPos.y;

    ImVec2 underlineTargetSize = ImVec2(tabSizes[currentTab].x, 2);
    underlineSize = MathUtils::lerp(underlineSize, underlineTargetSize, ImGui::GetIO().DeltaTime * 10);

    // Render the underline
    drawList->AddLine(underlinePos, ImVec2(underlinePos.x + underlineSize.x, underlinePos.y), IM_COL32(255, 255, 255, 255), underlineSize.y);

    FontHelper::popPrefFont();

}
