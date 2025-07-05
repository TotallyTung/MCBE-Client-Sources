//
// Created by Flash on 10/24/2023.
//

#include "ThemeManager.h"

#include <imgui.h>
#include "Notifications.h"

Theme* ThemeManager::CurrentTheme = nullptr;

void ThemeManager::LoadPreferences(std::string name, Theme& preferences) {
    try
    {
        // Load the settings from the json file
        std::ifstream i(FileSystem::Directory + "\\" + name + ".json");

        if (i.good()) {

            nlohmann::json j;
            i >> j;

            // Make sure the json object is valid
            if (j.is_null()) {
                Notifications::Notify("Theme", "Failed to load theme from " + std::string(name) + ".json (null)", NotificationType::Error);
                return;
            }


            // Load the settings from the json object

            //GlobalRGBInfo
            preferences.GlobalRGBInfo.Time = j["GlobalRGBInfo"]["Time"];
            preferences.GlobalRGBInfo.Saturation = j["GlobalRGBInfo"]["Saturation"];
            preferences.GlobalRGBInfo.Value = j["GlobalRGBInfo"]["Value"];
            preferences.GlobalRGBInfo.Separation = j["GlobalRGBInfo"]["Separation"];


            preferences.backgroundColor1.from_json(j["backgroundColor1"]);
            preferences.backgroundColor2.from_json(j["backgroundColor2"]);
            preferences.backgroundColor3.from_json(j["backgroundColor3"]);
            preferences.primaryColor.from_json(j["primaryColor"]);
            preferences.secondaryColor.from_json(j["secondaryColor"]);
            preferences.moduleTextColor.from_json(j["moduleTextColor"]);
            preferences.moduleDisabledTextColor.from_json(j["moduleDisabledTextColor"]);
            preferences.settingTextColor.from_json(j["settingTextColor"]);
            preferences.toggleColor.from_json(j["toggleColor"]);
            preferences.toggleDisabledColor.from_json(j["toggleDisabledColor"]);
            preferences.toggleHoverColor.from_json(j["toggleHoverColor"]);
            preferences.shadowColor.from_json(j["shadowColor"]);
            preferences.categoryTextColor.from_json(j["categoryTextColor"]);
            preferences.EyeIconOnColor.from_json(j["EyeIconOnColor"]);
            preferences.EyeIconOffColor.from_json(j["EyeIconOffColor"]);



            preferences.HeaderHeight = j["HeaderHeight"];
            preferences.ModuleHeight = j["ModuleHeight"];
            preferences.SettingHeight = j["SettingHeight"];

            preferences.CategoryFontSize = j["CategoryFontSize"];
            preferences.ModuleFontSize = j["ModuleFontSize"];
            preferences.SettingFontSize = j["SettingFontSize"];
            preferences.ToolTipFontSize = j["ToolTipFontSize"];
            preferences.IconFontSize = j["IconFontSize"];

            preferences.blur = j["blur"];
            preferences.gobalOpacity = j["gobalOpacity"];
            preferences.Rounding = j["Rounding"];
            preferences.BorderSize = j["BorderSize"];
            preferences.MaxWindowHeightPercent = j["MaxWindowHeightPercent"];

            // ImVec2
            preferences.ItemSpacing = ImVec2(j["ItemSpacing"][0], j["ItemSpacing"][1]);
            preferences.InnerItemSpacing = ImVec2(j["InnerItemSpacing"][0], j["InnerItemSpacing"][1]);
            preferences.ItemPadding = ImVec2(j["ItemPadding"][0], j["ItemPadding"][1]);

            preferences.ButtonTextAlign = ImVec2(j["ButtonTextAlign"][0], j["ButtonTextAlign"][1]);
            preferences.WindowTitleTextAlign = ImVec2(j["WindowTitleTextAlign"][0], j["WindowTitleTextAlign"][1]);



            Notifications::Notify("Theme", "Loaded Theme from " + std::string(name) + ".json", NotificationType::Info);
        }
        else {
            Notifications::Notify("Theme", "Failed to load theme from " + std::string(name) + ".json! (not good)", NotificationType::Error);
        }
        i.close();
    } catch (std::exception& e) {
        Notifications::Notify("Theme", "Failed to load theme from " + std::string(name) + ".json! (caught: " + e.what() + ")", NotificationType::Error);
    }
}

void ThemeManager::SavePreferences(std::string name, Theme& preferences) {
    // Convert all the settings to a json object

    nlohmann::json j;

    std::ofstream o(FileSystem::Directory + "\\" + name + ".json");

    //GlobalRGBInfo
    j["GlobalRGBInfo"]["Time"] = preferences.GlobalRGBInfo.Time;
    j["GlobalRGBInfo"]["Saturation"] = preferences.GlobalRGBInfo.Saturation;
    j["GlobalRGBInfo"]["Value"] = preferences.GlobalRGBInfo.Value;
    j["GlobalRGBInfo"]["Separation"] = preferences.GlobalRGBInfo.Separation;

    preferences.backgroundColor1.to_json(j["backgroundColor1"]);
    preferences.backgroundColor2.to_json(j["backgroundColor2"]);
    preferences.backgroundColor3.to_json(j["backgroundColor3"]);
    preferences.primaryColor.to_json(j["primaryColor"]);
    preferences.secondaryColor.to_json(j["secondaryColor"]);
    preferences.moduleTextColor.to_json(j["moduleTextColor"]);
    preferences.moduleDisabledTextColor.to_json(j["moduleDisabledTextColor"]);
    preferences.settingTextColor.to_json(j["settingTextColor"]);
    preferences.toggleColor.to_json(j["toggleColor"]);
    preferences.toggleDisabledColor.to_json(j["toggleDisabledColor"]);
    preferences.toggleHoverColor.to_json(j["toggleHoverColor"]);
    preferences.shadowColor.to_json(j["shadowColor"]);
    preferences.categoryTextColor.to_json(j["categoryTextColor"]);
    preferences.EyeIconOnColor.to_json(j["EyeIconOnColor"]);
    preferences.EyeIconOffColor.to_json(j["EyeIconOffColor"]);

    j["HeaderHeight"] = preferences.HeaderHeight;
    j["ModuleHeight"] = preferences.ModuleHeight;
    j["SettingHeight"] = preferences.SettingHeight;

    j["CategoryFontSize"] = preferences.CategoryFontSize;
    j["ModuleFontSize"] = preferences.ModuleFontSize;
    j["SettingFontSize"] = preferences.SettingFontSize;
    j["ToolTipFontSize"] = preferences.ToolTipFontSize;
    j["IconFontSize"] = preferences.IconFontSize;

    j["blur"] = preferences.blur;
    j["gobalOpacity"] = preferences.gobalOpacity;
    j["Rounding"] = preferences.Rounding;
    j["BorderSize"] = preferences.BorderSize;
    j["MaxWindowHeightPercent"] = preferences.MaxWindowHeightPercent;


    j["ItemSpacing"] = {preferences.ItemSpacing.x, preferences.ItemSpacing.y};
    j["InnerItemSpacing"] = {preferences.InnerItemSpacing.x, preferences.InnerItemSpacing.y};
    j["ItemPadding"] = {preferences.ItemPadding.x, preferences.ItemPadding.y};

    j["ButtonTextAlign"] = {preferences.ButtonTextAlign.x, preferences.ButtonTextAlign.y};
    j["WindowTitleTextAlign"] = {preferences.WindowTitleTextAlign.x, preferences.WindowTitleTextAlign.y};



    o << j.dump() << std::endl;

    o.close();

    Notifications::Notify("Theme", "Saved Theme to " + std::string(name) + ".json", NotificationType::Info);

}


void RenderButtonForColor(ClickGuiColor* color, std::string name) {
    if (ImGui::Button(name.c_str())) {
        ImGui::OpenPopup(std::string("##ColorPickerPopup" + name).c_str());
    }

    if (ImGui::BeginPopup(std::string("##ColorPickerPopup" + name).c_str())) {
        ImGui::Text("Pick a color:");

        ImGui::Checkbox(std::string("Rainbow").c_str(), &color->useRgb);


        ImGui::ColorPicker4("Color", (float*)color);

        if (ImGui::Button("OK")) {
            // You can use the 'color' value here.
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}


void ThemeManager::OnRender(bool render) {
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    float size = 50;

    screenSize.x -= size;
    screenSize.y -= size;

    float delta = ImGui::GetIO().DeltaTime;

    ImVec2 windowSize = {ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2};
    ImGui::SetNextWindowSize(windowSize);

    static ImVec2 windowPos = {ImGui::GetIO().DisplaySize.x / 2 - windowSize.x / 2, ImGui::GetIO().DisplaySize.y + 10};
    windowPos.x = ImGui::GetIO().DisplaySize.x / 2 - windowSize.x / 2;
    windowPos.y = Math::Animate(windowPos.y, render ? ImGui::GetIO().DisplaySize.y / 2 - windowSize.y / 2 :
                                             ImGui::GetIO().DisplaySize.y + 30, delta * 10);

    ImGui::SetNextWindowPos(windowPos);

    ImGui::GetStyle().WindowPadding = ImVec2(10, 10);
    ImGui::Begin("ClickGui Settings", nullptr,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoResize);

    if (ImGui::CollapsingHeader("Client"))
    {
        ImGui::Checkbox("D3D11 fallback", &Prax::Preferences->d3d11Fallback);
        ImGui::SetItemTooltip("Uses d3d11 like older version of Prax, may fix some issues with rendering or improve performance.\nRequires restart to take effect.");

    }

    if (ImGui::CollapsingHeader("RGB")) {
        ImGui::SliderFloat("Time", &ColorUtil::GlobalRGBInfo.Time, 0.f, 20.f);
        ImGui::SliderFloat("Saturation", &ColorUtil::GlobalRGBInfo.Saturation, 0.f, 1.f);
        ImGui::SliderFloat("Value", &ColorUtil::GlobalRGBInfo.Value, 0.f, 1.f);
        ImGui::SliderFloat("Separation", &ColorUtil::GlobalRGBInfo.Separation, 0.f, 20.f);
    }

    if (ImGui::CollapsingHeader("Misc")) {
        ImGui::SliderFloat("Blur", &CurrentTheme->blur, 0.f, 10.f);
        ImGui::SliderFloat("Opacity", &CurrentTheme->gobalOpacity, 0.f, 1.f);
        ImGui::SliderFloat("Rounding", &CurrentTheme->Rounding, 0.f, 20.f);
        ImGui::SliderFloat("BorderSize", &CurrentTheme->BorderSize, 0.f, 10.f);
        ImGui::SliderFloat("Max Window Height", &CurrentTheme->MaxWindowHeightPercent, 0.f, 1.f);

        // Show ImVec2 as a slider
        ImGui::SliderFloat2("Spacing", (float*)&CurrentTheme->ItemSpacing, 0.f, 20.f);
        ImGui::SliderFloat2("Inner Spacing", (float*)&CurrentTheme->InnerItemSpacing, 0.f, 20.f);
        ImGui::SliderFloat2("Padding", (float*)&CurrentTheme->ItemPadding, 0.f, 20.f);
        ImGui::SliderFloat2("Window Title Text Align", (float*)&CurrentTheme->WindowTitleTextAlign, 0.f, 1.f);
        ImGui::SliderFloat2("Button Text Align", (float*)&CurrentTheme->ButtonTextAlign, 0.f, 1.f);

    }
    if (ImGui::CollapsingHeader("Size")) {
        // Font Size
        ImGui::SliderFloat("Header Height", &CurrentTheme->HeaderHeight, 0.1f, 50.f);
        ImGui::SliderFloat("Module Height", &CurrentTheme->ModuleHeight, 0.1f, 50.f);
        ImGui::SliderFloat("Setting Height", &CurrentTheme->SettingHeight, 0.1f, 50.f);
    }


    if (ImGui::CollapsingHeader("Font")) {
        // Font Size
        ImGui::SliderFloat("Header Font Size", &CurrentTheme->CategoryFontSize, 0.f, 50.f);
        ImGui::SliderFloat("Module Font Size", &CurrentTheme->ModuleFontSize, 0.f, 50.f);
        ImGui::SliderFloat("Setting Font Size", &CurrentTheme->SettingFontSize, 0.f, 50.f);
        ImGui::SliderFloat("Tooltip Font Size", &CurrentTheme->ToolTipFontSize, 0.f, 50.f);
        ImGui::SliderFloat("Icon Font Size", &CurrentTheme->IconFontSize, 0.f, 50.f);
    }


    if (ImGui::CollapsingHeader("Colors")) {

        RenderButtonForColor(&CurrentTheme->backgroundColor1, "Background Color");
        RenderButtonForColor(&CurrentTheme->backgroundColor2, "Background Color 2");
        RenderButtonForColor(&CurrentTheme->backgroundColor3, "Background Color 3");
        RenderButtonForColor(&CurrentTheme->primaryColor, "Primary Color");
        RenderButtonForColor(&CurrentTheme->secondaryColor, "Secondary Color");
        RenderButtonForColor(&CurrentTheme->moduleTextColor, "Module Text Color");
        RenderButtonForColor(&CurrentTheme->moduleDisabledTextColor, "Module Disabled Text Color");
        RenderButtonForColor(&CurrentTheme->settingTextColor, "Setting Text Color");
        RenderButtonForColor(&CurrentTheme->categoryTextColor, "Category Text Color");
        RenderButtonForColor(&CurrentTheme->shadowColor, "Shadow Color");
        RenderButtonForColor(&CurrentTheme->EyeIconOnColor, "Eye Icon On Color");
        RenderButtonForColor(&CurrentTheme->EyeIconOffColor, "Eye Icon Off Color");
        RenderButtonForColor(&CurrentTheme->toggleColor, "Toggle Color");
        RenderButtonForColor(&CurrentTheme->toggleDisabledColor, "Toggle Disabled Color");
        RenderButtonForColor(&CurrentTheme->toggleHoverColor, "Toggle Hover Color");
        RenderButtonForColor(&CurrentTheme->BorderColor, "Border Color");


    }


    /*// Tick rgb's
    ImColor rgb = ColorUtil::GetDefaultRainbow(0, ColorUtil::GlobalRGBInfo.Time, ColorUtil::GlobalRGBInfo.Saturation,
                                               ColorUtil::GlobalRGBInfo.Value, ColorUtil::GlobalRGBInfo.Separation);

    auto updateColor = [&](ClickGuiColor& color) {
        if (color.useRgb) {
            ImU32 alpha = color.color.Value.w;
            color = ImColor(rgb.Value.x, rgb.Value.y, rgb.Value.z, alpha);
        }
    };

    updateColor(CurrentTheme->backgroundColor1);
    updateColor(CurrentTheme->backgroundColor2);
    updateColor(CurrentTheme->secondaryColor);
    updateColor(CurrentTheme->moduleTextColor);
    updateColor(CurrentTheme->toggleColor);
    updateColor(CurrentTheme->toggleDisabledColor);
    updateColor(CurrentTheme->toggleHoverColor);
    updateColor(CurrentTheme->shadowColor);
    updateColor(CurrentTheme->textShadowColor);
    updateColor(CurrentTheme->categoryTextColor);
    updateColor(CurrentTheme->EyeIconOnColor);
    updateColor(CurrentTheme->EyeIconOffColor);*/


    ImGui::SetCursorPos({ImGui::GetWindowWidth() - 210, ImGui::GetWindowHeight() + ImGui::GetScrollY() - 30});

    ImGui::SetNextItemWidth(100);

    // create text input for file name to save as
    static char fileName[128] = "default";
    ImGui::InputText("##FileInput", fileName, IM_ARRAYSIZE(fileName));


    ImGui::SetCursorPos({ImGui::GetWindowWidth() - 50, ImGui::GetWindowHeight() + ImGui::GetScrollY() - 30});

    // Do logic for saving the settings
    if (ImGui::Button("Save")) {
        SavePreferences(fileName, *CurrentTheme);
        PrefManager::Save(Prax::Preferences);
    }
    ImGui::SetCursorPos({ImGui::GetWindowWidth() - 100, ImGui::GetWindowHeight() + ImGui::GetScrollY() - 30});

    if (ImGui::Button("Load")) {
        LoadPreferences(fileName, *CurrentTheme);
        Prax::Preferences = PrefManager::Load();
    }


    ImGui::End();
}

void ThemeManager::Init() {
    if (CurrentTheme == nullptr) {
        CurrentTheme = new Theme();
    }

    // If the default theme file exists, load it
    std::ifstream i(FileSystem::Directory + "\\default.json");

    if (i.good()) {
        LoadPreferences("default", *CurrentTheme);
    }
    else {
        SavePreferences("default", *CurrentTheme);
    }
}