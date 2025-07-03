// Created by Tony on 2024-10-10 10:01:54

#include "ConfigMenu.h"
#include "../../../Client.h"
#include <filesystem>
#include <algorithm>
float saveAnimationTime = 0.f;
ConfigMenu::ConfigMenu() : Module("ConfigMenu", "Manage configuration files", Category::CLIENT, VK_F7) {
    windowPos = Vec2<float>(100.f, 100.f);
    newConfigName = "";
    updateConfigList();
}

ConfigMenu::~ConfigMenu() {}

void ConfigMenu::onEnable() {}

void ConfigMenu::onDisable() {}

bool ConfigMenu::isVisible() {
    return true;
}

void ConfigMenu::updateConfigList() {
    configList.clear();
    std::string configPath = Utils::getClientPath() + "Configs\\";
    for (const auto& entry : std::filesystem::directory_iterator(configPath)) {
        if (entry.path().extension() == ".json") {
            configList.push_back(entry.path().stem().string());
        }
    }
    std::sort(configList.begin(), configList.end());
}

void ConfigMenu::Render(ImDrawList* drawlist) {
    if (!client->isInitialized()) return;

    static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");
    ClientInstance* ci = mc.getClientInstance();
    ImGuiIO& io = ImGui::GetIO();
    Vec2<float> mousePos = ci->getMousePos();

    static const float textSize = 1.f;
    const float textHeight = ImGuiUtils::getTextHeight(textSize);
    static const float textPadding = 2.f * textSize;

    float windowWidth = 600.f;
    float windowHeight = 400.f;
    float titleBarHeight = 50.f;

    Vec4<float> windowRect = Vec4<float>(windowPos.x, windowPos.y, windowPos.x + windowWidth, windowPos.y + windowHeight);
    Vec4<float> titleBarRect = Vec4<float>(windowPos.x, windowPos.y, windowPos.x + windowWidth, windowPos.y + titleBarHeight);

    // Handle dragging (only from title bar)
    static bool isDragging = false;
    static Vec2<float> dragOffset;

    if (titleBarRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
        isDragging = true;
        dragOffset = mousePos - windowPos;
    }

    if (isDragging) {
        if (ImGui::IsMouseDown(0)) {
            windowPos = mousePos - dragOffset;
        }
        else {
            isDragging = false;
        }
    }

    // Background with blur effect
    ImGuiUtils::fillRectangle(windowRect, UIColor(255, 255, 255, 200), 15.f);
    drawlist->AddRectFilled(ImVec2(windowRect.x, windowRect.y), ImVec2(windowRect.z, windowRect.w), IM_COL32(255, 255, 255, 100), 15.f);

    // Title bar
    ImGuiUtils::fillRectangle(titleBarRect, UIColor(240, 240, 240, 255), 15.f, ImDrawFlags_RoundCornersTop);
    std::string title = "Configuration Manager";
    float titleWidth = ImGuiUtils::getTextWidth(title, textSize * 1.5f);
    ImGuiUtils::drawText(Vec2<float>(windowPos.x + (windowWidth - titleWidth) / 2, windowPos.y + (titleBarHeight - textHeight) / 2),
        title, UIColor(50, 50, 50, 255), textSize * 1.5f);

    // Content area
    Vec4<float> contentRect = Vec4<float>(windowPos.x + 20, windowPos.y + titleBarHeight + 20,
        windowPos.x + windowWidth - 20, windowPos.y + windowHeight - 20);

    if (isViewingConfig) {
        renderJsonViewer(drawlist);
    }
    else {
        // Render config list
        renderConfigList(drawlist, contentRect);

        // Config actions area
        Vec4<float> actionsRect = Vec4<float>(contentRect.x, contentRect.w - 60,
            contentRect.z, contentRect.w);
        renderConfigActions(drawlist, actionsRect);
    }

    // Handle input for new config name
    handleInput();
}

void ConfigMenu::renderConfigList(ImDrawList* drawlist, const Vec4<float>& contentRect) {
    static const float textSize = 1.f;
    const float textHeight = ImGuiUtils::getTextHeight(textSize);
    static const float textPadding = 2.f * textSize;

    float yOffset = contentRect.y + textPadding;
    float contentHeight = (textHeight + textPadding * 3) * configList.size();
    float visibleHeight = contentRect.w - contentRect.y - 70;  // Adjust for actions area
    float maxScroll = std::max(0.f, contentHeight - visibleHeight);

    // Scrollbar logic
    float scrollbarWidth = 8.f;
    float scrollbarHeight = (visibleHeight / contentHeight) * visibleHeight;
    float scrollbarY = contentRect.y + (scrollOffset / maxScroll) * (visibleHeight - scrollbarHeight);
    Vec4<float> scrollbarRect = Vec4<float>(contentRect.z - scrollbarWidth, scrollbarY, contentRect.z, scrollbarY + scrollbarHeight);

    // Handle scrolling
    ClientInstance* ci = mc.getClientInstance();
    ImGuiIO& io = ImGui::GetIO();
    Vec2<float> mousePos = ci->getMousePos();

    if (Vec4<float>(contentRect.x, contentRect.y, contentRect.z, contentRect.w - 70).contains(mousePos)) {
        scrollOffset -= io.MouseWheel * 20;
        scrollOffset = std::clamp(scrollOffset, 0.f, maxScroll);
    }

    drawlist->PushClipRect(ImVec2(contentRect.x, contentRect.y), ImVec2(contentRect.z, contentRect.w - 70), true);

    for (const auto& configName : configList) {
        Vec4<float> configRect = Vec4<float>(
            contentRect.x,
            yOffset - scrollOffset,
            contentRect.z - scrollbarWidth - 10,
            yOffset + textHeight + textPadding * 2 - scrollOffset
        );

        if (configRect.y + configRect.w > contentRect.y && configRect.y < contentRect.w - 70) {
            // Config item background
            ImGuiUtils::fillRectangle(configRect, UIColor(245, 245, 245, 255), 10.f);

            // Config name
            ImGuiUtils::drawText(Vec2<float>(configRect.x + textPadding, configRect.y + textPadding),
                configName, UIColor(50, 50, 50, 255), textSize);

            // Load button
            std::string loadText = "Load";
            float buttonWidth = ImGuiUtils::getTextWidth(loadText, textSize) + textPadding * 4;
            Vec4<float> loadButtonRect = Vec4<float>(
                configRect.z - buttonWidth - textPadding - buttonWidth - textPadding,
                configRect.y + textPadding,
                configRect.z - buttonWidth - textPadding,
                configRect.w - textPadding
            );

            ImGuiUtils::fillRectangle(loadButtonRect, UIColor(0, 122, 255, 255), 8.f);
            ImGuiUtils::drawText(Vec2<float>(loadButtonRect.x + textPadding * 2, loadButtonRect.y + textPadding),
                loadText, UIColor(255, 255, 255, 255), textSize);

            if (loadButtonRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
                client->configMgr->loadConfig(configName);
                mc.DisplayClientMessage("[%sMelody%s] %sLoaded config: %s", DARK_PURPLE, WHITE, GREEN, configName.c_str());
            }

            // Add View button
            std::string viewText = "View";
            float viewButtonWidth = ImGuiUtils::getTextWidth(viewText, textSize) + textPadding * 4;
            Vec4<float> viewButtonRect = Vec4<float>(
                loadButtonRect.x - viewButtonWidth - textPadding,
                configRect.y + textPadding,
                loadButtonRect.x - textPadding,
                configRect.w - textPadding
            );

            ImGuiUtils::fillRectangle(viewButtonRect, UIColor(255, 204, 0, 255), 8.f);
            ImGuiUtils::drawText(Vec2<float>(viewButtonRect.x + textPadding * 2, viewButtonRect.y + textPadding),
                viewText, UIColor(255, 255, 255, 255), textSize);

            if (viewButtonRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
                selectedConfig = configName;
                isViewingConfig = true;
            }
            // Delete button
            std::string deleteText = "Delete";
            Vec4<float> deleteButtonRect = Vec4<float>(
                configRect.z - buttonWidth - textPadding,
                configRect.y + textPadding,
                configRect.z - textPadding,
                configRect.w - textPadding
            );

            ImGuiUtils::fillRectangle(deleteButtonRect, UIColor(255, 59, 48, 255), 8.f);
            ImGuiUtils::drawText(Vec2<float>(deleteButtonRect.x + textPadding * 2, deleteButtonRect.y + textPadding),
                deleteText, UIColor(255, 255, 255, 255), textSize);

            if (deleteButtonRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
                client->configMgr->deleteConfig(configName);
                updateConfigList();
                mc.DisplayClientMessage("[%sMelody%s] %sDeleted config: %s", DARK_PURPLE, WHITE, GREEN, configName.c_str());
            }
        }

        yOffset += textHeight + textPadding * 3;
    }

    drawlist->PopClipRect();

    // Draw scrollbar
    if (scrollbarHeight < visibleHeight) {
        ImGuiUtils::fillRectangle(scrollbarRect, UIColor(200, 200, 200, 200), 4.f);
    }
}
float jsonScrollOffset = 0.f;
void ConfigMenu::renderJsonViewer(ImDrawList* drawlist) {
    if (!isViewingConfig) return;
    // Check if the selected config is the active one
    if (selectedConfig == activeConfig) {
        // Create a backup config
        backupConfig = activeConfig + "_backup";
        std::string originalPath = Utils::getClientPath() + "Configs\\" + activeConfig + ".json";
        std::string backupPath = Utils::getClientPath() + "Configs\\" + backupConfig + ".json";

        // Copy the active config to the backup
        std::ifstream src(originalPath, std::ios::binary);
        std::ofstream dst(backupPath, std::ios::binary);
        dst << src.rdbuf();

        // Switch to editing the backup
        selectedConfig = backupConfig;
    }
    static const float textSize = 1.f;
    const float textHeight = ImGuiUtils::getTextHeight(textSize);
    static const float textPadding = 2.f * textSize;

    Vec4<float> viewerRect = Vec4<float>(windowPos.x + 20, windowPos.y + 70,
        windowPos.x + 580, windowPos.y + 380);

    ImGuiUtils::fillRectangle(viewerRect, UIColor(240, 240, 240, 255), 10.f);

    // Title bar
    float titleBarHeight = 40.f;
    Vec4<float> titleBarRect = Vec4<float>(viewerRect.x, viewerRect.y, viewerRect.z, viewerRect.y + titleBarHeight);
    ImGuiUtils::fillRectangle(titleBarRect, UIColor(220, 220, 220, 255), 10.f, ImDrawFlags_RoundCornersTop);

    std::string title = "Editing: " + selectedConfig;
    float titleWidth = ImGuiUtils::getTextWidth(title, textSize * 1.2f);
    ImGuiUtils::drawText(Vec2<float>(titleBarRect.x + textPadding, titleBarRect.y + (titleBarHeight - textHeight) / 2),
        title, UIColor(50, 50, 50, 255), textSize * 1.2f);

    // Save button
    std::string saveText = "Save";
    float buttonWidth = ImGuiUtils::getTextWidth(saveText, textSize) + textPadding * 4;
    Vec4<float> saveButtonRect = Vec4<float>(
        titleBarRect.z - buttonWidth * 2 - textPadding * 3,
        titleBarRect.y + (titleBarHeight - textHeight) / 2 - textPadding,
        titleBarRect.z - buttonWidth - textPadding * 2,
        titleBarRect.y + (titleBarHeight + textHeight) / 2 + textPadding
    );

    UIColor saveButtonColor = UIColor(52, 199, 89, 255);
    if (saveAnimationTime > 0) {
        saveAnimationTime -= ImGui::GetIO().DeltaTime;
        float t = saveAnimationTime / 0.5f; // Normalize time
        saveButtonColor = UIColor(
            52 + (int)((255 - 52) * (1 - t)),
            199 + (int)((255 - 199) * (1 - t)),
            89 + (int)((255 - 89) * (1 - t)),
            255
        );
    }

    ImGuiUtils::fillRectangle(saveButtonRect, saveButtonColor, 8.f);
    ImGuiUtils::drawText(Vec2<float>(saveButtonRect.x + textPadding * 2, saveButtonRect.y + textPadding),
        saveText, UIColor(255, 255, 255, 255), textSize);

    // Close button
    std::string closeText = "Close";
    Vec4<float> closeButtonRect = Vec4<float>(
        titleBarRect.z - buttonWidth - textPadding,
        titleBarRect.y + (titleBarHeight - textHeight) / 2 - textPadding,
        titleBarRect.z - textPadding,
        titleBarRect.y + (titleBarHeight + textHeight) / 2 + textPadding
    );

    ImGuiUtils::fillRectangle(closeButtonRect, UIColor(255, 59, 48, 255), 8.f);
    ImGuiUtils::drawText(Vec2<float>(closeButtonRect.x + textPadding * 2, closeButtonRect.y + textPadding),
        closeText, UIColor(255, 255, 255, 255), textSize);

    // Handle button clicks
    ClientInstance* ci = mc.getClientInstance();
    Vec2<float> mousePos = ci->getMousePos();

    if (saveButtonRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
        saveJsonConfig();
        if (backupConfig.empty()) {
            // Normal save for non-active configs
            mc.DisplayClientMessage("[%sMelody%s] %sConfig saved successfully", DARK_PURPLE, WHITE, GREEN);
        }
        else {
            // For active config, apply changes and switch back
            applyBackupChanges();
            mc.DisplayClientMessage("[%sMelody%s] %sChanges applied to active config", DARK_PURPLE, WHITE, GREEN);
        }
    }

    // Modify the Close button click handler
    if (closeButtonRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
        if (!backupConfig.empty()) {
            // If we were editing the active config, discard changes and switch back
            selectedConfig = activeConfig;
            std::string backupPath = Utils::getClientPath() + "Configs\\" + backupConfig + ".json";
            std::filesystem::remove(backupPath);
            backupConfig.clear();
        }
        isViewingConfig = false;
        jsonScrollOffset = 0;
        editedContent.clear();
    }

    // Content area
    Vec4<float> contentRect = Vec4<float>(viewerRect.x, viewerRect.y + titleBarHeight,
        viewerRect.z, viewerRect.w);

    std::string configPath = Utils::getClientPath() + "Configs\\" + selectedConfig + ".json";

    if (editedContent.empty()) {
        std::ifstream configFile(configPath);
        if (configFile.is_open()) {
            std::stringstream buffer;
            buffer << configFile.rdbuf();
            editedContent = buffer.str();
            configFile.close();
        }
    }

    // Calculate content height
    float contentHeight = 0;
    renderJsonNode(drawlist, editedContent, 0, contentRect.x + textPadding, contentHeight, contentRect, true);

    // Scrollbar logic
    float visibleHeight = contentRect.w - contentRect.y;
    float maxScroll = std::max(0.f, contentHeight - visibleHeight);
    float scrollbarWidth = 8.f;
    float scrollbarHeight = (visibleHeight / contentHeight) * visibleHeight;
    float scrollbarY = contentRect.y + (jsonScrollOffset / maxScroll) * (visibleHeight - scrollbarHeight);
    Vec4<float> scrollbarRect = Vec4<float>(contentRect.z - scrollbarWidth, scrollbarY, contentRect.z, scrollbarY + scrollbarHeight);

    // Handle scrolling
    // Draggable scrollbar
    static bool isDraggingScrollbar = false;
    if (scrollbarRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
        isDraggingScrollbar = true;
    }
    if (isDraggingScrollbar) {
        if (ImGui::IsMouseDown(0)) {
            float scrollDelta = (mousePos.y - scrollbarY) / (visibleHeight - scrollbarHeight) * maxScroll;
            jsonScrollOffset = std::clamp(scrollDelta, 0.f, maxScroll);
        }
        else {
            isDraggingScrollbar = false;
        }
    }

    if (contentRect.contains(mousePos) && !isDraggingScrollbar) {
        jsonScrollOffset -= ImGui::GetIO().MouseWheel * 20;
        jsonScrollOffset = std::clamp(jsonScrollOffset, 0.f, maxScroll);
    }

    // Render JSON content
    float yOffset = contentRect.y + textPadding - jsonScrollOffset;
    drawlist->PushClipRect(ImVec2(contentRect.x, contentRect.y), ImVec2(contentRect.z - scrollbarWidth, contentRect.w), true);
    renderJsonNode(drawlist, editedContent, 0, contentRect.x + textPadding, yOffset, contentRect, false);
    drawlist->PopClipRect();

    // Draw scrollbar
    if (scrollbarHeight < visibleHeight) {
        ImGuiUtils::fillRectangle(scrollbarRect, UIColor(200, 200, 200, 200), 4.f);
    }
}

void ConfigMenu::renderJsonNode(ImDrawList* drawlist, std::string& content, int depth, float x, float& y, const Vec4<float>& clipRect, bool measureOnly) {
    static const float textSize = 1.f;
    const float textHeight = ImGuiUtils::getTextHeight(textSize);
    static const float textPadding = 2.f * textSize;
    const float indentWidth = 20.f;
    const float lineNumberWidth = 40.f;

    if (!measureOnly) {
        drawlist->PushClipRect(ImVec2(clipRect.x, clipRect.y), ImVec2(clipRect.z, clipRect.w), true);
    }

    int lineNumber = 0;
    std::istringstream iss(content);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        lineNumber++;
        if (!measureOnly) {
            // Render line number
            ImGuiUtils::drawText(Vec2<float>(x - lineNumberWidth, y), std::to_string(lineNumber), UIColor(150, 150, 150, 255), textSize);

            // Render content
            if (lineNumber == this->selectedLine && this->isEditing) {
                // Highlight the entire line
                drawlist->AddRectFilled(ImVec2(x - lineNumberWidth, y), ImVec2(clipRect.z, y + textHeight), IM_COL32(173, 214, 255, 100));

                // Render the editable text
                ImGuiUtils::drawText(Vec2<float>(x + depth * indentWidth, y), this->editBuffer, UIColor(0, 0, 0, 255), textSize);

                // Render cursor
                float cursorX = x + depth * indentWidth + ImGuiUtils::getTextWidth(this->editBuffer.substr(0, this->cursorPosition), textSize);
                drawlist->AddLine(ImVec2(cursorX, y), ImVec2(cursorX, y + textHeight), IM_COL32(0, 0, 0, 255));

                // Handle keyboard input
                for (int key = 0; key < 256; key++) {
                    if (mc.isKeyDown(key) && !this->keyStates[key]) {
                        this->keyStates[key] = true;
                        handleKeyPress(key);
                    }
                    else if (!mc.isKeyDown(key) && this->keyStates[key]) {
                        this->keyStates[key] = false;
                    }
                }
            }
            else {
                ImGuiUtils::drawText(Vec2<float>(x + depth * indentWidth, y), lines[i], UIColor(0, 0, 0, 255), textSize);
            }

            // Handle line selection and cursor positioning
            Vec4<float> lineRect(x - lineNumberWidth, y, clipRect.z, y + textHeight);
            Vec2<float> mousePos(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
            if (lineRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
                this->selectedLine = lineNumber;
                this->editBuffer = lines[i];
                this->isEditing = true;

                // Calculate cursor position based on click position
                float clickX = mousePos.x - (x + depth * indentWidth);
                this->cursorPosition = 0;
                float accumulatedWidth = 0;
                for (size_t j = 0; j < this->editBuffer.length(); ++j) {
                    float charWidth = ImGuiUtils::getTextWidth(this->editBuffer.substr(j, 1), textSize);
                    if (accumulatedWidth + charWidth / 2 > clickX) break;
                    accumulatedWidth += charWidth;
                    this->cursorPosition++;
                }
            }
        }
        y += textHeight + textPadding;
    }

    if (!measureOnly) {
        drawlist->PopClipRect();
    }
}

void ConfigMenu::handleKeyPress(int key) {
    if (key == VK_BACK) {
        if (cursorPosition > 0) {
            editBuffer.erase(cursorPosition - 1, 1);
            cursorPosition--;
        }
    }
    else if (key == VK_DELETE) {
        if (cursorPosition < editBuffer.length()) {
            editBuffer.erase(cursorPosition, 1);
        }
    }
    else if (key == VK_LEFT) {
        if (cursorPosition > 0) cursorPosition--;
    }
    else if (key == VK_RIGHT) {
        if (cursorPosition < editBuffer.length()) cursorPosition++;
    }
    else if (key == VK_HOME) {
        cursorPosition = 0;
    }
    else if (key == VK_END) {
        cursorPosition = editBuffer.length();
    }
    else if (key == VK_RETURN) {
        applyEdit(selectedLine, editBuffer);
        isEditing = false;
    }
    else if (key == VK_ESCAPE) {
        isEditing = false;
    }
    else if (isprint(key)) {
        char c = static_cast<char>(key);
        bool isShiftDown = mc.isKeyDown(VK_SHIFT);

        if (isShiftDown) {
            if (c >= 'a' && c <= 'z') c = toupper(c);
            else {
                switch (c) {
                case '0': c = ')'; break;
                case '1': c = '!'; break;
                case '2': c = '@'; break;
                case '3': c = '#'; break;
                case '4': c = '$'; break;
                case '5': c = '%'; break;
                case '6': c = '^'; break;
                case '7': c = '&'; break;
                case '8': c = '*'; break;
                case '9': c = '('; break;
                case '-': c = '_'; break;
                case '=': c = '+'; break;
                case '[': c = '{'; break;
                case ']': c = '}'; break;
                case '\\': c = '|'; break;
                case ';': c = ':'; break;
                case '\'': c = '"'; break;
                case ',': c = '<'; break;
                case '.': c = '>'; break;
                case '/': c = '?'; break;
                case '`': c = '~'; break;
                }
            }
        }

        editBuffer.insert(cursorPosition, 1, c);
        cursorPosition++;
    }
}
json ConfigMenu::parseJsonContent() {
    json result;
    std::istringstream iss(editedContent);
    std::string line;
    std::vector<json*> stack = { &result };

    while (std::getline(iss, line)) {
        size_t pos = line.find_first_not_of(" \t");
        if (pos == std::string::npos) continue;

        line = line.substr(pos);
        if (line.empty()) continue;

        if (line[0] == '{') {
            *stack.back() = json::object();
            stack.push_back(&(*stack.back()));
        }
        else if (line[0] == '[') {
            *stack.back() = json::array();
            stack.push_back(&(*stack.back()));
        }
        else if (line[0] == '}' || line[0] == ']') {
            stack.pop_back();
        }
        else {
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                if (key.front() == '"' && key.back() == '"') {
                    key = key.substr(1, key.length() - 2);
                }

                try {
                    (*stack.back())[key] = json::parse(value);
                }
                catch (json::parse_error&) {
                    (*stack.back())[key] = value;
                }
            }
        }
    }

    return result;
}

void ConfigMenu::applyEdit(int lineNumber, const std::string& newContent) {
    std::vector<std::string> lines;
    std::istringstream iss(editedContent);
    std::string line;

    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    if (lineNumber > 0 && lineNumber <= lines.size()) {
        lines[lineNumber - 1] = newContent;
    }

    editedContent.clear();
    for (const auto& l : lines) {
        editedContent += l + "\n";
    }
}
void ConfigMenu::saveJsonConfig() {
    try {
        std::string configPath = Utils::getClientPath() + "Configs\\" + selectedConfig + ".json";
        std::ofstream configFile(configPath);
        if (configFile.is_open()) {
            configFile << editedContent;
            configFile.close();
            saveAnimationTime = 0.5f; // Start animation for 0.5 seconds
        }
        else {
            mc.DisplayClientMessage("[%sMelody%s] %sError: Unable to open config file for writing", DARK_PURPLE, WHITE, RED);
        }
    }
    catch (const std::exception& e) {
        mc.DisplayClientMessage("[%sMelody%s] %sError saving config: %s", DARK_PURPLE, WHITE, RED, e.what());
    }
}
void ConfigMenu::applyBackupChanges() {
    std::string backupPath = Utils::getClientPath() + "Configs\\" + backupConfig + ".json";
    std::string originalPath = Utils::getClientPath() + "Configs\\" + activeConfig + ".json";

    // Copy the backup to the original
    std::ifstream src(backupPath, std::ios::binary);
    std::ofstream dst(originalPath, std::ios::binary);
    dst << src.rdbuf();

    // Remove the backup file
    std::filesystem::remove(backupPath);

    // Switch back to the original config
    selectedConfig = activeConfig;
    backupConfig.clear();

    // Reload the config
    client->configMgr->loadConfig(activeConfig);
}

// Add this function to update the active config when loading a new config
void ConfigMenu::setActiveConfig(const std::string& configName) {
    activeConfig = configName;
}

// Modify the existing function where you load a config
void ConfigMenu::loadConfig(const std::string& configName) {
    client->configMgr->loadConfig(configName);
    setActiveConfig(configName);
    mc.DisplayClientMessage("[%sMelody%s] %sLoaded config: %s", DARK_PURPLE, WHITE, GREEN, configName.c_str());
}
void ConfigMenu::renderConfigActions(ImDrawList* drawlist, const Vec4<float>& actionsRect) {
    static const float textSize = 1.f;
    const float textHeight = ImGuiUtils::getTextHeight(textSize);
    static const float textPadding = 2.f * textSize;

    // New config input
    Vec4<float> inputRect = Vec4<float>(
        actionsRect.x,
        actionsRect.y,
        actionsRect.z - 120,
        actionsRect.w
    );

    ImGuiUtils::fillRectangle(inputRect, UIColor(240, 240, 240, 255), 8.f);

    // Render text and blinking cursor
    std::string displayText = newConfigName.empty() ? "Enter new config name..." : newConfigName;
    UIColor textColor = newConfigName.empty() ? UIColor(150, 150, 150, 255) : UIColor(50, 50, 50, 255);

    ImGuiUtils::drawText(Vec2<float>(inputRect.x + textPadding, inputRect.y + textPadding),
        displayText, textColor, textSize);

    if (isInputFocused) {
        static float cursorBlinkTime = 0.0f;
        cursorBlinkTime += ImGui::GetIO().DeltaTime;
        if (fmodf(cursorBlinkTime, 1.0f) < 0.5f) {
            float cursorX = inputRect.x + textPadding + ImGuiUtils::getTextWidth(newConfigName, textSize);
            ImGuiUtils::fillRectangle(Vec4<float>(cursorX, inputRect.y + textPadding, cursorX + 1, inputRect.y + textPadding + textHeight), UIColor(0, 0, 0, 255));
        }
    }

    ClientInstance* ci = mc.getClientInstance();
    Vec2<float> mousePos = ci->getMousePos();

    if (inputRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
        isInputFocused = true;
    }
    else if (ImGui::IsMouseClicked(0)) {
        isInputFocused = false;
    }

    // Create button
    std::string createText = "Create";
    float buttonWidth = ImGuiUtils::getTextWidth(createText, textSize) + textPadding * 4;
    Vec4<float> createButtonRect = Vec4<float>(
        actionsRect.z - buttonWidth - textPadding,
        actionsRect.y,
        actionsRect.z,
        actionsRect.w
    );

    ImGuiUtils::fillRectangle(createButtonRect, UIColor(52, 199, 89, 255), 8.f);
    ImGuiUtils::drawText(Vec2<float>(createButtonRect.x + textPadding * 2, createButtonRect.y + textPadding),
        createText, UIColor(255, 255, 255, 255), textSize);

    if (createButtonRect.contains(mousePos) && ImGui::IsMouseClicked(0)) {
        if (!newConfigName.empty()) {
            client->configMgr->createConfig(newConfigName);
            updateConfigList();
            mc.DisplayClientMessage("[%sMelody%s] %sCreated new config: %s", DARK_PURPLE, WHITE, GREEN, newConfigName.c_str());
            newConfigName.clear();
        }
    }
}

void ConfigMenu::handleInput() {
    if (!isInputFocused) return;

    static bool keyStates[256] = { false };
    static float keyPressTime[256] = { 0.0f };
    static const float KEY_REPEAT_DELAY = 0.5f;
    static const float KEY_REPEAT_RATE = 0.03f;

    ImGuiIO& io = ImGui::GetIO();

    for (int i = 0; i < 256; i++) {
        bool isKeyDown = mc.isKeyDown(i);

        if (isKeyDown && !keyStates[i]) {
            // Key just pressed
            keyStates[i] = true;
            keyPressTime[i] = 0.0f;

            if (i == VK_BACK) {
                if (!newConfigName.empty()) newConfigName.pop_back();
            }
            else if (i == VK_RETURN) {
                isInputFocused = false;
            }
            else if (i == VK_ESCAPE) {
                isInputFocused = false;
                newConfigName.clear(); // Clear the input when pressing Escape
            }
            else if (isprint(i)) {
                char c = static_cast<char>(i);
                bool isShiftDown = mc.isKeyDown(VK_SHIFT);
                bool isCapsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

                if (isShiftDown ^ isCapsLockOn) {
                    c = toupper(c);
                }
                else {
                    c = tolower(c);
                }
                if (newConfigName.length() < 20) { // Limit config name length
                    newConfigName += c;
                }
            }
        }
        else if (isKeyDown && keyStates[i]) {
            // Key held down
            keyPressTime[i] += io.DeltaTime;
            if (keyPressTime[i] > KEY_REPEAT_DELAY) {
                float repeatDuration = keyPressTime[i] - KEY_REPEAT_DELAY;
                if (fmodf(repeatDuration, KEY_REPEAT_RATE) < io.DeltaTime) {
                    if (i == VK_BACK) {
                        if (!newConfigName.empty()) newConfigName.pop_back();
                    }
                    else if (isprint(i)) {
                        char c = static_cast<char>(i);
                        bool isShiftDown = mc.isKeyDown(VK_SHIFT);
                        bool isCapsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

                        if (isShiftDown ^ isCapsLockOn) {
                            c = toupper(c);
                        }
                        else {
                            c = tolower(c);
                        }
                        if (newConfigName.length() < 20) { // Limit config name length
                            newConfigName += c;
                        }
                    }
                }
            }
        }
        else if (!isKeyDown && keyStates[i]) {
            // Key released
            keyStates[i] = false;
        }
    }
}