#pragma once
//
// Created by vastrakai on 9/14/2024.
//

class HudElement
{
public:
    enum class Anchor {
        TopLeft,
        TopMiddle,
        TopRight,
        MiddleLeft,
        Middle,
        MiddleRight,
        BottomLeft,
        BottomMiddle,
        BottomRight
    };

    // Gets the anchor position based on the display size
    static ImVec2 getAnchorPos(Anchor anchor)
    {
        auto display = ImGui::GetIO().DisplaySize;
        ImVec2 pos = { 0, 0 };

        switch (anchor)
        {
        case Anchor::TopLeft:
            break;
        case Anchor::TopMiddle:
            pos.x = display.x / 2;
            break;
        case Anchor::TopRight:
            pos.x = display.x;
            break;
        case Anchor::MiddleLeft:
            pos.y = display.y / 2;
            break;
        case Anchor::Middle:
            pos.x = display.x / 2;
            pos.y = display.y / 2;
            break;
        case Anchor::MiddleRight:
            pos.x = display.x;
            pos.y = display.y / 2;
            break;
        case Anchor::BottomLeft:
            pos.y = display.y;
            break;
        case Anchor::BottomMiddle:
            pos.x = display.x / 2;
            pos.y = display.y;
            break;
        case Anchor::BottomRight:
            pos.x = display.x;
            pos.y = display.y;
            break;
        }

        return pos;
    }

    glm::vec2 mPos = { 0, 0 };  // Relative position to the anchor
    glm::vec2 mSize = { 0, 0 }; // Size of the element (readonly for editor)
    bool mVisible = false;
    bool mSampleMode = false; // Show the element in the editor
    char* mParentTypeIdentifier = nullptr;
    bool mCentered = false;
    Anchor mAnchor = Anchor::TopLeft;
    bool mCustom = false;

    HudElement() = default;
    HudElement(char* parentTypeIdentifier) : mParentTypeIdentifier(parentTypeIdentifier) {}

    // Set position based on normalized position and find the closest anchor
    void setFromPos(glm::vec2 normalPos) {
        // Find the closest anchor to the given position
        Anchor closestAnchor = Anchor::TopLeft;
        float minDistance = std::numeric_limits<float>::max();

        for (int i = 0; i < 9; i++) {
            ImVec2 anchorPos = getAnchorPos(static_cast<Anchor>(i));
            float dist = glm::distance(glm::vec2(anchorPos.x, anchorPos.y), normalPos);

            if (dist < minDistance) {
                minDistance = dist;
                closestAnchor = static_cast<Anchor>(i);
            }
        }

        mAnchor = closestAnchor;

        // Set the relative position based on the anchor
        ImVec2 anchorPos = getAnchorPos(mAnchor);
        mPos = { normalPos.x - anchorPos.x, normalPos.y - anchorPos.y };
    }

    // Get the absolute position based on the anchor and the relative position
    ImVec2 getPos() const {
        ImVec2 anchorPos = getAnchorPos(mAnchor);
        return { mPos.x + anchorPos.x, mPos.y + anchorPos.y };
    }
};

class CustomHudElement : public HudElement
{
public:
    enum class Type {
        Text,
        /*Image,
        Line,
        Rect,
        Circle,
        Triangle*/
    };
    std::string mDisplayName;
    Type mType;
    std::string mText;
    // Text input
    char mInputBuffer[256] = { 0 };
    float mFontSize = 24.f;
    bool mBold = false;
    std::string mIdentifier;
    bool mUseThemeColor = false;
    ImColor mColor = ImColor(255, 255, 255, 255);

    CustomHudElement(std::string identifier, std::string displayName, Type type, std::string text, bool useThemeColor, ImColor color) : mDisplayName(displayName), mText(text), mType(type), mUseThemeColor(useThemeColor), mColor(color) {
        mCustom = true;
        mIdentifier = identifier;
        mParentTypeIdentifier = mIdentifier.data();
    }
};


class HudEditor : public ModuleBase<HudEditor> {
public:
    static inline HudEditor* gInstance = nullptr;

    HudEditor() : ModuleBase("HudEditor", "Shows the hud editor on screen", ModuleCategory::Visual, VK_HOME, false) {

        mNames = {
            {Lowercase, "hudeditor"},
            {LowercaseSpaced, "hud editor"},
            {Normal, "HudEditor"},
            {NormalSpaced, "Hud Editor"}
        };

        gInstance = this;


        gFeatureManager->mDispatcher->listen<RenderEvent, &HudEditor::onRenderEvent, nes::event_priority::FIRST>(this);
        gFeatureManager->mDispatcher->listen<RenderEvent, &HudEditor::onCustomRenderEvent, nes::event_priority::LAST>(this);
    }

    const float mSnapPointDist = 10.f;

    int mSnapDistance = 0.f;
    std::vector<HudElement*> mElements = {};

    std::vector<std::unique_ptr<CustomHudElement>> mCustomElements = {};

    bool mCustomGuiOpen = false;

    void registerElement(HudElement* element) { mElements.emplace_back(element); }

    void showAllElements();
    void hideAllElements();
    void saveToFile();
    void loadFromFile();

    void onInit() override;
    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
    void onCustomRenderEvent(class RenderEvent& event);
    void onKeyEvent(class KeyEvent& event);
    void onMouseEvent(MouseEvent& event);
};

