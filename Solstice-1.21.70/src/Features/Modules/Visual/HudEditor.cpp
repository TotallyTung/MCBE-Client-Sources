//
// Created by vastrakai on 9/14/2024.
//

#include "HudEditor.hpp"

#include <Features/Events/KeyEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftGame.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

#include "ClickGui.hpp"

bool lastCscState = false;

void HudEditor::showAllElements()
{
    for (auto element : mElements) {
        element->mSampleMode = true;
    }
}

void HudEditor::hideAllElements()
{
    for (auto element : mElements) {
        element->mSampleMode = false;
    }
}

void HudEditor::saveToFile()
{
    try
    {
        static std::string path = FileUtils::getSolsticeDir() + "hud.json";
        nlohmann::json j;
        for (auto element : mElements)
        {
            j[element->mParentTypeIdentifier] = {
                {"pos", {element->mPos.x, element->mPos.y}},
                {"size", {element->mSize.x, element->mSize.y}},
                {"anchor", element->mAnchor}
            };

            spdlog::info("Saved element: {}", element->mParentTypeIdentifier);
        }

        for (auto& element : mCustomElements)
        {
            j[element->mParentTypeIdentifier] = {
                {"pos", {element->mPos.x, element->mPos.y}},
                {"size", {element->mSize.x, element->mSize.y}},
                {"anchor", element->mAnchor},
                {"text", element->mText},
                {"fontSize", element->mFontSize},
                {"bold", element->mBold},
                {"useThemeColor", element->mUseThemeColor},
                {"color", {element->mColor.Value.x, element->mColor.Value.y, element->mColor.Value.z, element->mColor.Value.w}} // ImColor as array
            };

            spdlog::info("Saved custom element: {}", element->mText);
        }

        j["snapDistance"] = mSnapDistance;

        std::ofstream file(path);
        file << j.dump(4);
        file.close();

        spdlog::info("Saved hud elements to file!");
    } catch (const std::exception& e)
    {
        spdlog::error("Failed to save hud elements to file: {}", e.what());
    } catch (const nlohmann::json::exception& e)
    {
        spdlog::error("Failed to save hud elements to file: {}", e.what());
    } catch (...)
    {
        spdlog::error("Failed to save hud elements to file: unknown error");
    }
}

void HudEditor::loadFromFile()
{
    try
    {
        static std::string path = FileUtils::getSolsticeDir() + "hud.json";
        if (!FileUtils::fileExists(path))
        {
            spdlog::warn("No hud elements file found, creating one!");
            saveToFile();
            return;
        }
        nlohmann::json j;
        std::ifstream file(path);
        file >> j;
        file.close();

        for (auto element : mElements)
        {
            if (j.contains(element->mParentTypeIdentifier))
            {
                auto& data = j[element->mParentTypeIdentifier];
                element->mPos = { data["pos"][0], data["pos"][1] };
                element->mSize = { data["size"][0], data["size"][1] };
                element->mAnchor = data["anchor"];

                j.erase(element->mParentTypeIdentifier);
            }
        }

        if (j.contains("snapDistance"))
        {
            mSnapDistance = j["snapDistance"];
            j.erase("snapDistance");
        }

        // For the rest of the keys, they are custom elements
        for (auto& [key, value] : j.items())
        {
            auto element = std::make_unique<CustomHudElement>(key.c_str(), value["text"], CustomHudElement::Type::Text, value["text"], value["useThemeColor"], ImColor(static_cast<float>(value["color"][0]), value["color"][1], value["color"][2], value["color"][3]));
            element->mPos = { value["pos"][0], value["pos"][1] };
            element->mSize = { value["size"][0], value["size"][1] };
            element->mAnchor = value["anchor"];
            element->mFontSize = value["fontSize"];
            element->mBold = value["bold"];
            element->mUseThemeColor = value.contains("useThemeColor") ? static_cast<bool>(value["useThemeColor"]) : false;

            // ImColor uses RGBA values, so load from array
            auto colorArray = value["color"];
            element->mColor = ImColor(static_cast<float>(colorArray[0]), colorArray[1], colorArray[2], colorArray[3]);

            // Copy the mText to the input buffer
            strcpy(element->mInputBuffer, element->mText.c_str());

            // if the key is not in the elements, add it to the custom elements
            bool found = false;
            for (auto& customElement : mCustomElements)
            {
                if (customElement->mParentTypeIdentifier == key)
                {
                    customElement->mPos = { value["pos"][0], value["pos"][1] };
                    customElement->mSize = { value["size"][0], value["size"][1] };
                    customElement->mAnchor = value["anchor"];
                    customElement->mFontSize = value["fontSize"];
                    customElement->mText = value["text"];
                    customElement->mUseThemeColor = value["useThemeColor"];
                    customElement->mColor = ImColor(static_cast<float>(colorArray[0]), colorArray[1], colorArray[2], colorArray[3]);
                    customElement->mVisible = true;
                    customElement->mBold = value["bold"];
                    strcpy(customElement->mInputBuffer, customElement->mText.c_str());
                    found = true;
                }
            }

            if (!found)
                mCustomElements.push_back(std::move(element));
        }

        spdlog::info("Loaded hud elements from file!");
    } catch (const std::exception& e)
    {
        spdlog::error("Failed to load hud elements from file: {}", e.what());
    } catch (const nlohmann::json::exception& e)
    {
        spdlog::error("Failed to load hud elements from file: {}", e.what());
        saveToFile();
    } catch (...)
    {
        spdlog::error("Failed to load hud elements from file: unknown error");
    }
}

void HudEditor::onInit()
{
    loadFromFile();
}

void HudEditor::onEnable()
{
    gFeatureManager->mDispatcher->listen<KeyEvent, &HudEditor::onKeyEvent>(this);
    gFeatureManager->mDispatcher->listen<MouseEvent, &HudEditor::onMouseEvent>(this);

    auto ci = ClientInstance::get();
    lastCscState = !ci->getMouseGrabbed();
    ci->releaseMouse();

    loadFromFile();
    showAllElements();

    static auto clickGuiModule = gFeatureManager->mModuleManager->getModule<ClickGui>();
    if (clickGuiModule) clickGuiModule->setEnabled(false);
}

void HudEditor::onDisable()
{
    gFeatureManager->mDispatcher->deafen<KeyEvent, &HudEditor::onKeyEvent>(this);
    gFeatureManager->mDispatcher->deafen<MouseEvent, &HudEditor::onMouseEvent>(this);

    auto ci = ClientInstance::get();
    if (lastCscState) ci->grabMouse();
    else ci->releaseMouse();

    saveToFile();
    hideAllElements();
}

void HudEditor::onRenderEvent(RenderEvent& event)
{
    if (!mEnabled) return;

    // Draw grid lines
    auto drawList = ImGui::GetBackgroundDrawList();
    ImVec2 display = ImGui::GetIO().DisplaySize;

    auto ci = ClientInstance::get();

    ci->releaseMouse();

    // Draw background
    drawList->AddRectFilled(ImVec2(0, 0), display, IM_COL32(0, 0, 0, 50));

    const int snapDistance = mSnapDistance; // Copy to prevent race conditions


    static bool dragging = false;
    static ImVec2 dragStart;
    static ImVec2 dragOffset;
    static HudElement* draggedElement = nullptr;



    if (snapDistance > 0 && !mCustomGuiOpen)
    {
        for (int i = 0; i < display.x; i += snapDistance)
            drawList->AddLine(ImVec2(i, 0), ImVec2(i, display.y), IM_COL32(255, 255, 255, 100));
        for (int i = 0; i < display.y; i += snapDistance)
            drawList->AddLine(ImVec2(0, i), ImVec2(display.x, i), IM_COL32(255, 255, 255, 100));
    }


    if (ImGui::IsMouseClicked(0) && !mCustomGuiOpen)
    {
        dragStart = ImGui::GetMousePos();
        dragOffset = { 0, 0 };

        for (auto element : mElements)
        {
            if (!element->mVisible) continue;

            ImVec2 pos = element->getPos();
            ImVec2 size = element->mSize;

            if (element->mCentered)
            {
                pos.x -= size.x / 2;
                pos.y -= size.y / 2;
            }

            if (dragStart.x > pos.x && dragStart.x < pos.x + size.x &&
                dragStart.y > pos.y && dragStart.y < pos.y + size.y)
            {
                dragging = true;
                draggedElement = element;
                dragOffset = { dragStart.x - pos.x, dragStart.y - pos.y };
                break;
            }
        }

        for (auto& element : mCustomElements)
        {
            if (!element->mVisible) continue;

            ImVec2 pos = element->getPos();
            ImVec2 size = element->mSize;

            if (element->mCentered)
            {
                pos.x -= size.x / 2;
                pos.y -= size.y / 2;
            }

            if (dragStart.x > pos.x && dragStart.x < pos.x + size.x &&
                dragStart.y > pos.y && dragStart.y < pos.y + size.y)
            {
                dragging = true;
                draggedElement = element.get();
                dragOffset = { dragStart.x - pos.x, dragStart.y - pos.y };
                break;
            }
        }
    }

    if (dragging && draggedElement != nullptr)
    {
        ImVec2 dragEnd = ImGui::GetMousePos();

        glm::ivec2 delta = { dragEnd.x - dragStart.x, dragEnd.y - dragStart.y };
        glm::ivec2 newPos = { dragEnd.x - dragOffset.x, dragEnd.y - dragOffset.y };

        if (mSnapDistance != 0)
        {
            newPos.x = std::round(newPos.x / mSnapDistance) * mSnapDistance;
            newPos.y = std::round(newPos.y / mSnapDistance) * mSnapDistance;
        }

        if (draggedElement->mCentered)
        {
            newPos.x += draggedElement->mSize.x / 2.f;
            newPos.y += draggedElement->mSize.y / 2.f;
        }

        draggedElement->setFromPos(newPos);

        if (!ImGui::IsMouseDown(0))
        {
            dragging = false;
            draggedElement = nullptr;
        }
    }

    // Loop through all of the positions and clamp them to the screen, including the size+pos
    for (auto element : mElements)
    {
        if (!element->mVisible) continue;

        auto pos = element->getPos();
        glm::vec2 size = element->mSize;

        if (element->mCentered)
        {
            pos.x -= size.x / 2;
            pos.y -= size.y / 2;
        }

        pos.x = std::clamp(pos.x, 0.f, display.x - element->mSize.x);
        pos.y = std::clamp(pos.y, 0.f, display.y - element->mSize.y);
        if (element->mSize.x + pos.x > display.x) element->mSize.x = display.x - pos.x;
        if (element->mSize.y + pos.y > display.y) element->mSize.y = display.y - pos.y;

        auto newPos = pos;

        if (element->mCentered)
        {
            newPos.x += size.x / 2;
            newPos.y += size.y / 2;
        }

        element->setFromPos(glm::vec2(newPos.x, newPos.y));
    }

    for (auto& element : mCustomElements)
    {
        if (!element->mVisible) continue;

        auto pos = element->getPos();
        glm::vec2 size = element->mSize;

        if (element->mCentered)
        {
            pos.x -= size.x / 2;
            pos.y -= size.y / 2;
        }

        pos.x = std::clamp(pos.x, 0.f, display.x - element->mSize.x);
        pos.y = std::clamp(pos.y, 0.f, display.y - element->mSize.y);
        if (element->mSize.x + pos.x > display.x) element->mSize.x = display.x - pos.x;
        if (element->mSize.y + pos.y > display.y) element->mSize.y = display.y - pos.y;

        auto newPos = pos;

        if (element->mCentered)
        {
            newPos.x += size.x / 2;
            newPos.y += size.y / 2;
        }

        element->setFromPos(glm::vec2(newPos.x, newPos.y));
    }

    // Draw elements
    for (auto element : mElements)
    {
        if (!element->mVisible) continue;

        ImVec2 pos = element->getPos();
        ImVec2 size = element->mSize;
        if (element->mCentered)
        {
            pos.x -= size.x / 2;
            pos.y -= size.y / 2;
        }

        drawList->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(255, 255, 255, 125));
    }

    for (auto& element : mCustomElements)
    {
        if (!element->mVisible) continue;

        ImVec2 pos = element->getPos();
        ImVec2 size = element->mSize;

        if (element->mCentered)
        {
            pos.x -= size.x / 2;
            pos.y -= size.y / 2;
        }

        drawList->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(255, 255, 255, 125));
    }

    // Draw a circle with a + in the bottom left (add new element button)
    float radius = 15;
    ImVec2 buttonPos = { 30, display.y - 30 };
    ImVec4 buttonRect = { buttonPos.x - radius, buttonPos.y - radius, buttonPos.x + radius, buttonPos.y + radius };

    static ImColor butColor = ImColor(255, 255, 255, 125);
    ImColor targetColor = mCustomGuiOpen ? ImColor(0, 255, 0, 200) : ImRenderUtils::isMouseOver(buttonRect) ? ImColor(255, 255, 255, 255) : ImColor(255, 255, 255, 125);
    butColor.Value = MathUtils::lerp(butColor.Value, targetColor.Value, ImGui::GetIO().DeltaTime * 10);

    drawList->AddCircleFilled(buttonPos, radius, butColor);
    drawList->AddLine(ImVec2(buttonRect.x + 5, buttonRect.y + radius), ImVec2(buttonRect.z - 5, buttonRect.y + radius), IM_COL32(0, 0, 0, 255), 2);
    drawList->AddLine(ImVec2(buttonRect.x + radius, buttonRect.y + 5), ImVec2(buttonRect.x + radius, buttonRect.w - 5), IM_COL32(0, 0, 0, 255), 2);


    if (ImGui::IsMouseClicked(0)) {
        if (ImRenderUtils::isMouseOver(buttonRect)) {
            mCustomGuiOpen = !mCustomGuiOpen;
        }
    }

    if (!mCustomGuiOpen) return;

    // Draw in center
    ImVec2 center = { display.x / 2, display.y / 2 };
    ImVec2 size = { 700, 700 };
    ImVec2 pos = { center.x - size.x / 2, center.y - size.y / 2 };

    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Hud Editor", &mCustomGuiOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

    ImGui::Text("Edit custom hud elements here!");

    // Add text button
    if (ImGui::Button("Add Text"))
    {
        auto element = std::make_unique<CustomHudElement>(StringUtils::randomString(32).c_str(), "Sample Text", CustomHudElement::Type::Text, "Sample Text", false, ImColor(255, 255, 255, 255));
        element->mVisible = true;
        // Copy the mText to the input buffer
        strcpy(element->mInputBuffer, element->mText.c_str());

        mCustomElements.push_back(std::move(element));
        NotifyUtils::notify("Added new text element!", 1.f, Notification::Type::Info);
    }

    for (int i = 0; i < mCustomElements.size(); i++)
    {
        auto& element = mCustomElements[i];
        if (!element->mVisible) continue;

        // Make the label for each InputText unique by appending the index
        std::string inputLabel = "Text##" + std::to_string(i);
        ImGui::InputText(inputLabel.c_str(), element->mInputBuffer, 256);

        ImGui::SameLine();

        std::string fontSizeLabel = "Font Size##" + std::to_string(i);
        ImGui::InputFloat(fontSizeLabel.c_str(), &element->mFontSize);



        std::string boldLabel = "Bold##" + std::to_string(i);
        ImGui::Checkbox(boldLabel.c_str(), &element->mBold);

        ImGui::SameLine();

        // UseThemeColor toggle
        ImGui::Checkbox(("Use Theme Color##" + std::to_string(i)).c_str(), &element->mUseThemeColor);

        // Color picker
        if (!element->mUseThemeColor)
        {
            ImGui::ColorEdit4(("Color##" + std::to_string(i)).c_str(), &element->mColor.Value.x);
        }

        if (ImGui::Button(("Delete##" + std::to_string(i)).c_str()))
        {
            NotifyUtils::notify("Deleted element!", 1.f, Notification::Type::Info);

            mCustomElements.erase(mCustomElements.begin() + i);
            i--;
        }

        ImGui::SameLine();

        if (ImGui::Button(("Set##" + std::to_string(i)).c_str()))
        {
            element->mText = element->mInputBuffer;
            NotifyUtils::notify("Set text!", 1.f, Notification::Type::Info);
        }

        ImGui::SameLine();

        if (ImGui::Button(("Clear##" + std::to_string(i)).c_str()))
        {
            element->mText = "";
            strcpy(element->mInputBuffer, "");
            NotifyUtils::notify("Cleared text!", 1.f, Notification::Type::Info);
        }
    }


    ImGui::End();

}

void HudEditor::onCustomRenderEvent(RenderEvent& event)
{
    // TODO: Render custom elements here...


    for (auto& element : mCustomElements)
    {
        FontHelper::pushPrefFont(element->mFontSize > 30, element->mBold);

        ImVec2 pos = element->getPos();

        auto drawList = ImGui::GetBackgroundDrawList();

        ImColor color = element->mUseThemeColor ? ColorUtils::getThemedColor(0) : element->mColor;

        drawList->AddText(ImGui::GetFont(), element->mFontSize, pos, color, element->mText.c_str());
        ImVec2 size = ImGui::GetFont()->CalcTextSizeA(element->mFontSize, FLT_MAX, 0, element->mText.c_str());
        element->mSize = { size.x, size.y };

        FontHelper::popPrefFont();

    }

}

bool mShiftHeld = false;

void HudEditor::onKeyEvent(KeyEvent& event)
{
    // If imgui is capturing the keyboard, don't do anything
    if (ImGui::GetIO().WantCaptureKeyboard)
    {
        return;
    }

    if (event.mKey == mKey && event.mPressed)
    {
        setEnabled(false); // ????
        return;
    }

    if (event.mKey == VK_SHIFT)
    {
        mShiftHeld = event.mPressed;
        event.cancel();
        return;
    }

    if (event.mKey == VK_TAB)
    {
        event.cancel();

        if (event.mPressed)
        {
            if (mShiftHeld) mSnapDistance -= 5;
            else mSnapDistance += 5;

            if (mSnapDistance > 100) mSnapDistance = 0;
            if (mSnapDistance < 0) mSnapDistance = 100;

            NotifyUtils::notify("Snap distance: " + std::to_string(mSnapDistance), 2.f, Notification::Type::Info);
        }
    }

    if (event.mKey == VK_ESCAPE && event.mPressed)
    {
        event.cancel();
        if (mCustomGuiOpen) mCustomGuiOpen = false;
        else setEnabled(false);
    }
}

void HudEditor::onMouseEvent(MouseEvent& event)
{
    event.cancel();
}