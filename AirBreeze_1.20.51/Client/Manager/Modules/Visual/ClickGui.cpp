#include "ClickGui.h"
#include "../../Manager.h"
#include "../../Client.h"
#include "../../../Include/imgui/imgui.h"
#include "../../../Utils/Render/ImGuiUtils.h"
#include "../../../Utils/Render/RenderUtils.h"
#include "../../../Include/imgui/imgui_internal.h"
#include <glm/glm.hpp>

#include <ctime> // Include the header for time functions

static bool showModules = true;
char searchQuery[256];
char h[2] = { 0 };
static bool enableSnowflakes = true;
bool colorPickerOpen = false;
ImVec4 guiBackgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 0.85f);
ImVec4 rainbowBorderColor;


float animationStartTime = -1.0f;
float animationDuration = 0.5f; // Adjust the animation duration
ImVec2 targetSize; // The final size of the GUI window

bool guiOpen = false; // Variable to control GUI opening

struct Snowflake {
    float x;
    float y;
    float speed;
};

enum class AnimationType {
    None,
    Snowflakes,
};

AnimationType currentAnimation = AnimationType::None;
static constexpr int MaxSnowflakes = 100;
std::mt19937 gen;
std::uniform_real_distribution<float> distX;
std::uniform_real_distribution<float> distSpeed;
Snowflake snowflakes[MaxSnowflakes];

void ClickGui::onKey(uint64_t key, bool isDown, bool* cancelSend) {
    static std::chrono::steady_clock::time_point lastBackspaceTime = std::chrono::steady_clock::now();

    if (key == VK_ESCAPE) {
        guiOpen = false; // Close the GUI when 'ESC' is pressed
        return;
    }

    if (!isDown)
        return;

    if (key != VK_BACK && (key < 65 || key > 90)) // Limit range to A -> Z and check backspace
        return;

    *cancelSend = true;

    if (key == VK_BACK) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastBackspaceTime).count();

        if (elapsedTime >= 100) { // Change 100 to your desired delay in milliseconds
            int len = strlen(searchQuery);
            if (len > 0) {
                searchQuery[len - 1] = '\0';
            }
            lastBackspaceTime = currentTime;
        }
    }
    else {
        h[0] = (char)key;
        if (h[0] != '\0') {
            strcat_s(searchQuery, h);
        }
    }
}

void ClickGui::onEnable() {



    auto instance = Minecraft::getClientInstance();
    instance->grabMouse();
    instance->setDisableInput(true);

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    animationStartTime = ImGui::GetTime();
    targetSize = ImVec2(200, 500); // Adjust the target size

    if (enableSnowflakes) {
        float snowflakeSpacing = displaySize.x / MaxSnowflakes;
        for (int i = 0; i < MaxSnowflakes; ++i) {
            snowflakes[i].x = snowflakeSpacing * i;
            snowflakes[i].y = -distX(gen);
            snowflakes[i].speed = distSpeed(gen);
        }
    }

    if (enableSnowflakes) {
        currentAnimation = AnimationType::Snowflakes;
    }
    else {
        currentAnimation = AnimationType::None;
    }
}

void ClickGui::onDisable() {
    auto instance = Minecraft::getClientInstance();
    instance->releaseMouse();
    instance->setDisableInput(false);

    searchQuery[0] = '\0'; //empty search on disable
}

void ClickGui::onImGuiRender() {

    ImGui::SetNextWindowPos(ImVec2(1800, 1), ImGuiCond_FirstUseEver);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
    std::time_t currentTime = std::time(nullptr);
    std::tm timeInfo;
    if (localtime_s(&timeInfo, &currentTime) == 0) {
        char timeString[80];
        if (std::strftime(timeString, sizeof(timeString), "%I:%M:%S %p", &timeInfo) > 0) {
            std::string timeStr(timeString);
            ImGui::Text(timeString);
        }
    }
    //its so easy

    ImGuiIO& io = ImGui::GetIO();
    static bool is24HourFormat = false; // Toggle variable
    const char* format = is24HourFormat ? "%H:%M" : "%I:%M %p";
    ImVec2 textSize = ImGui::CalcTextSize(format);
    ImVec2 padding(20, 20);
    ImVec2 buttonSize(textSize.x + padding.x, textSize.y + padding.y);
    ImVec2 buttonPos((io.DisplaySize.x - buttonSize.x) / 2, 0); // Centered at the top

    ImGui::SetNextWindowSize(buttonSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(buttonPos, ImGuiCond_Always);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1));

    if (ImGui::Begin("timeshit",
        nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoScrollWithMouse)) {

        std::time_t currentTime = std::time(nullptr);
        std::tm timeInfo;
        if (localtime_s(&timeInfo, &currentTime) == 0) {
            char timeString[80];
            if (is24HourFormat) {
                if (std::strftime(timeString, sizeof(timeString), "%H:%M", &timeInfo) > 0) {
                    format = timeString;
                }
            }
            else {
                if (std::strftime(timeString, sizeof(timeString), "%I:%M %p", &timeInfo) > 0) {
                    format = timeString;
                }
            }

            if (ImGui::Button(format, ImVec2(buttonSize.x - padding.x, buttonSize.y - padding.y))) {
                is24HourFormat = !is24HourFormat;
            }
        }
    }


    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    //its so fucking easy and mine is better

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 18.0f;

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilled(ImVec2(0, 0), displaySize, ImColor(0.f, 0.f, 0.f, 0.30f));

    ImVec4 gradientStartColor = ImVec4(0.0f, 0.0f, 1.0f, 0.4f);
    ImVec4 gradientEndColor = ImVec4(1.0f, 0.0f, 1.0f, 0.80f);


    for (int i = 0; i < MaxSnowflakes; ++i) {
        snowflakes[i].y += snowflakes[i].speed;

        if (snowflakes[i].y > displaySize.y || snowflakes[i].x < 0.0f || snowflakes[i].x > displaySize.x) {
            snowflakes[i].x = distX(gen);
            snowflakes[i].y = -distX(gen);
            snowflakes[i].x = distX(gen) * displaySize.x;
        }

        ImVec2 circleCenter(snowflakes[i].x, snowflakes[i].y);
        float circleRadius = 4.0f;
        ImU32 circleColor = IM_COL32(255, 255, 255, 255);
        drawList->AddCircleFilled(circleCenter, circleRadius, circleColor);
    }

    drawList->AddRectFilledMultiColor(
        ImVec2(0, 0), ImVec2(displaySize.x, displaySize.y),
        ImColor(gradientStartColor), ImColor(gradientStartColor),
        ImColor(gradientEndColor), ImColor(gradientEndColor));

    float elapsed = ImGui::GetTime() - animationStartTime;
    float alpha = glm::smoothstep(0.0f, 1.0f, glm::clamp(elapsed / animationDuration, 0.0f, 1.0f));

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.85f);
    style.Colors[ImGuiCol_WindowBg] = guiBackgroundColor;
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_Button] = guiBackgroundColor;
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(guiBackgroundColor.x + 0.1f, guiBackgroundColor.y + 0.1f, guiBackgroundColor.z + 0.1f, guiBackgroundColor.w);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(guiBackgroundColor.x + 0.05f, guiBackgroundColor.y + 0.05f, guiBackgroundColor.z + 0.05f, guiBackgroundColor.w);

    ImVec4 greyColor = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);

    style.Colors[ImGuiCol_CheckMark] = greyColor;
    style.Colors[ImGuiCol_FrameBg] = guiBackgroundColor;
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(guiBackgroundColor.x + 0.1f, guiBackgroundColor.y + 0.1f, guiBackgroundColor.z + 0.1f, guiBackgroundColor.w);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(guiBackgroundColor.x + 0.05f, guiBackgroundColor.y + 0.05f, guiBackgroundColor.z + 0.05f, guiBackgroundColor.w);

    style.Colors[ImGuiCol_SliderGrab] = greyColor;  // Color of the grabber
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);  // Color of the grabber when active


    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    style.WindowPadding = ImVec2(10, 10);

    ImGui::SetNextWindowSize(ImVec2(200, 50));
    ImGui::Begin("Color", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    if (ImGui::Button("Pick Color", ImVec2(-1, 30))) {
        colorPickerOpen = true;
    }

    ImVec2 currentSize = ImVec2(targetSize.x, targetSize.y * alpha);
    ImVec2 currentPosition = ImVec2(
        ImGui::GetIO().DisplaySize.x - currentSize.x,
        ImGui::GetIO().DisplaySize.y - currentSize.y
    );

    ImGui::End();

    float categoryWindowWidth = 200.0f;
    float categoryWindowHeight = 500.0f;
    float horizontalSpacing = 60.0f;

    for (size_t i = 0; i < this->category->mgr->categories.size(); i++) {
        Category* category = this->category->mgr->categories[i];

        // Calculate the horizontal position based on category width and spacing
        float xPos = i * (categoryWindowWidth + horizontalSpacing);

        // For the first category, no need to add spacing before it
        if (i > 0) {
            // Add spacing after the previous category
            xPos += i * horizontalSpacing;
        }

        // Set the window position and size
        ImGui::SetNextWindowSize(ImVec2(categoryWindowWidth, categoryWindowHeight));
        ImGui::SetNextWindowPos(ImVec2(xPos, 115.0f), ImGuiCond_FirstUseEver);
        ImGui::Begin(category->name.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

        ImGui::SetWindowFontScale(0.6f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

        float textWidth = ImGui::CalcTextSize(category->name.c_str()).x;
        float windowWidth = ImGui::GetWindowSize().x;
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text(category->name.c_str());

        ImGui::SetWindowFontScale(0.6f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 5));
        for (Module* module : category->modules) {
            bool isEnabled = module->isEnabled;
            bool isExtending = module->isExtending;
            ImVec4 buttonColor = isEnabled ? ImVec4(0.3f, 0.3f, 0.3f, 0.80f) : ImVec4(0.0f, 0.0f, 0.0f, 0.80f);
            ImVec4 hoverColor = isEnabled ? ImVec4(0.25f, 0.25f, 0.25f, 0.85f) : ImVec4(0.1f, 0.1f, 0.1f, 0.85f);
            ImVec4 activeColor = isEnabled ? ImVec4(0.2f, 0.2f, 0.2f, 0.85f) : ImVec4(0.1f, 0.1f, 0.1f, 0.85f);

            ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);

            if (ImGui::Button(module->name.c_str(), ImVec2(-1, 30))) {
                module->isEnabled = !module->isEnabled;
            }

            if (ImGui::IsItemHovered()) {
                // Adjust the Y offset to position the tooltip closer to the button
                ImGui::SetTooltip("%s", module->description.c_str());
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                    module->isExtending = !module->isExtending;
                }
            }



            // MODULE SETTING SHIT


            if (isExtending) {
                for (Setting* setting : module->settingList) {
                    switch (setting->settingType) {
                    case (SettingType::BOOL):
                        ImGui::Checkbox(setting->name.c_str(), &setting->valuePtr->_bool);
                        break;

                    case (SettingType::FLOAT):
                        ImGui::SliderFloat(setting->name.c_str(), &setting->valuePtr->_float, setting->minValue->_float, setting->maxValue->_float);
                        break;
                    case (SettingType::INT):
                        ImGui::SliderInt(setting->name.c_str(), &setting->valuePtr->_int, setting->minValue->_int, setting->maxValue->_int);
                        break;
                    }
                }
            }

            ImGui::PopStyleColor(3);
        }

        ImGui::PopStyleVar();

        ImGui::End();
    }







    // SEARCH SHIT







    // Get the screen dimensions
    int screenWidth = ImGui::GetIO().DisplaySize.x;
    int screenHeight = ImGui::GetIO().DisplaySize.y;

    // Calculate the position to center the window slightly higher from the bottom center
    float windowWidth = 220.0f; // Adjust as needed
    float windowHeight = 150.0f; // Adjust as needed
    float yOffset = 50.0f; // Adjust as needed
    float windowX = (screenWidth - windowWidth) * 0.5f;
    float windowY = screenHeight - windowHeight - yOffset; // Subtract yOffset

    ImGui::SetNextWindowPos(ImVec2(windowX, windowY));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
    ImGui::Begin("Search Window", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    // Center the title
    ImGui::SetWindowFontScale(0.6f); // Smaller title font
    float titleOffsetX = (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Search").x) * 0.5f;
    ImGui::SetCursorPosX(titleOffsetX);
    ImGui::Text("Search");

    // Style the search input box with a dark gray border
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Dark gray background
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); // Slightly lighter gray on hover
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Even lighter gray when active
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Dark gray border
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Dark gray border shadow

    // Calculate the center position for the search bar
    float searchBoxWidth = 200.0f; // Adjust as needed
    float paddingX = (windowWidth - searchBoxWidth) * 0.5f; // Adjust this value to move it right
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(paddingX, 0.0f));

    // Make the search bar non-clickable (read-only)
    ImGui::PushItemWidth(searchBoxWidth);
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    if (ImGui::InputText("##SearchQuery", searchQuery, sizeof(searchQuery), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
        // Perform the search when Enter is pressed
    }
    ImGui::PopItemFlag();
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(5); // Pop the style changes

    // Flag to control whether to render modules
    bool showModules = strlen(searchQuery) == 0 ? false : true;

    if (showModules) {
        for (size_t i = 0; i < this->category->mgr->categories.size(); i++) {
            Category* category = this->category->mgr->categories[i];

            // Iterate through modules in the category
            for (Module* module : category->modules) {
                // Convert module name to uppercase as searchQuery is in uppercase
                std::string moduleName = module->name;
                std::transform(moduleName.begin(), moduleName.end(), moduleName.begin(), [](unsigned char c) {
                    return std::toupper(c);
                    });

                // Check if the module name contains the search query
                if (strlen(searchQuery) == 0 || strstr(moduleName.c_str(), searchQuery)) {
                    // Style to remove borders around modules
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));

                    // Button for each module
                    if (ImGui::Button(module->name.c_str(), ImVec2(-1, 30))) {
                        module->isEnabled = !module->isEnabled;
                    }

                    ImGui::PopStyleVar(2); // Pop the style changes

                    // Display tooltip when hovering
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("%s", module->description.c_str());
                    }
                }
            }
        }
    }








    //more colorpicker shit









    ImGui::End();

    // Pop the styling changes
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(3);


    if (colorPickerOpen) {
        ImGui::Begin("Pick a Theme", &colorPickerOpen, ImGuiWindowFlags_NoCollapse);


        // add more themes if needed
        static bool useBlackTheme = true;
        static bool useWhiteTheme = false;
        static ImVec4 blackThemeBg = ImVec4(0.0f, 0.0f, 0.0f, 0.85f);
        static ImVec4 whiteThemeBg = ImVec4(1.0f, 1.0f, 1.0f, 0.85f);
        static float rainbowSpeed = 0.1f;
        ImVec4 buttonColor = useBlackTheme ? ImVec4(0.3f, 0.3f, 0.3f, 0.80f) : ImVec4(1.0f, 1.0f, 1.0f, 0.80f);
        ImVec4 buttonHoveredColor = useBlackTheme ? ImVec4(0.25f, 0.25f, 0.25f, 0.85f) : ImVec4(0.95f, 0.95f, 0.95f, 0.85f);
        ImVec4 buttonActiveColor = useBlackTheme ? ImVec4(0.2f, 0.2f, 0.2f, 0.85f) : ImVec4(0.9f, 0.9f, 0.9f, 0.85f);
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoveredColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActiveColor);
        //


        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(20, 20);

        // Color themes
        static ImVec4 pickedColor = useBlackTheme ? blackThemeBg : whiteThemeBg;
        ImVec4 themeColor = useBlackTheme ? blackThemeBg : whiteThemeBg;
        ImVec4 textColor = useBlackTheme ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        style.Colors[ImGuiCol_Text] = textColor;
        bool colorChanged = ImGui::ColorPicker3("##ThemePicker", (float*)&pickedColor);

        // Check for changes in the theme color
        if (colorChanged) {
            useBlackTheme = false; // Set the custom theme flags
            useWhiteTheme = false;
        }

        static bool enableSnowflakes = true;
        ImGui::Checkbox("Enable Snowflakes", &enableSnowflakes);

        static float rainbowHue = 0.0f;
        rainbowHue += rainbowSpeed * ImGui::GetIO().DeltaTime;
        if (rainbowHue > 1.0f)
            rainbowHue -= 1.0f;
        ImVec4 rainbowColor = ImColor::HSV(rainbowHue, 0.7f, 0.7f, 0.85f);

        // Color theme selection
        ImGui::Text("Select a Theme:");
        ImGui::Checkbox("Black Theme", &useBlackTheme);
        ImGui::Checkbox("White Theme", &useWhiteTheme);

        // Update the selected theme color based on user choice
        if (useBlackTheme) {
            pickedColor = blackThemeBg;
            themeColor = blackThemeBg;
        }
        else if (useWhiteTheme) {
            pickedColor = whiteThemeBg;
            themeColor = whiteThemeBg;
        }

        if (useBlackTheme) {
            textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
        else {
            textColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        }

        // Apply the selected theme color to the entire GUI
        guiBackgroundColor = themeColor;
        style.Colors[ImGuiCol_WindowBg] = themeColor;

        ImGui::End();
    }

    else {

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(10, 10);

        style.Colors[ImGuiCol_WindowBg] = guiBackgroundColor;
        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_Button] = guiBackgroundColor;
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(guiBackgroundColor.x + 0.1f, guiBackgroundColor.y + 0.1f, guiBackgroundColor.z + 0.1f, guiBackgroundColor.w);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(guiBackgroundColor.x + 0.05f, guiBackgroundColor.y + 0.05f, guiBackgroundColor.z + 0.05f, guiBackgroundColor.w);

        ImGui::End();
    }
}
