// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"
#include "../../../ConfigManager/ConfigManager.h"
#include <vector>
#include "../../../../Libs/Json.hpp"

class ConfigMenu : public Module {
public:
    ConfigMenu();
    ~ConfigMenu();
    void Render(ImDrawList* drawlist);
    virtual bool isVisible() override;
    virtual void onEnable() override;
    virtual void onDisable() override;

private:
    Vec2<float> windowPos;
    float scrollOffset = 0.f;
    std::string newConfigName;
    bool isInputFocused = false;
    std::vector<std::string> configList;
    std::string selectedConfig;
    int selectedLine = -1;
    bool isEditing = false;
    std::string editBuffer;
    int cursorPosition = 0;
    bool keyStates[256] = { false };
    bool isViewingConfig = false;
    std::string editedContent;

    void saveJsonConfig();
    json parseJsonContent();
    void applyEdit(int lineNumber, const std::string& newContent);
    std::string activeConfig;
    std::string backupConfig;

    void applyBackupChanges();
    void setActiveConfig(const std::string& configName);
    void loadConfig(const std::string& configName);
    void renderJsonViewer(ImDrawList* drawlist);
    void renderJsonNode(ImDrawList* drawlist, std::string& content, int depth, float x, float& y, const Vec4<float>& clipRect, bool measureOnly);
    void updateConfigList();
    void renderConfigList(ImDrawList* drawlist, const Vec4<float>& contentRect);
    void renderConfigActions(ImDrawList* drawlist, const Vec4<float>& actionsRect);
    void handleInput();
    void handleKeyPress(int key);
};