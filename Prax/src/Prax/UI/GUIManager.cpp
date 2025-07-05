//
// Created by Flash on 1/2/2024.
//
#define IMGUI_DEFINE_MATH_OPERATORS

#include "GUIManager.h"

#include <Prax/Config/ConfigManager.h>
#include <Prax/Utils/Render/Render.h>

#include "ClickGui.h"
#include "NewClickGui.h"
bool GUIManager::Binding = false;
bool GUIManager::Visible = false;

#define TAB_AMOUNT 5
#define DRAWLIST ImGui::GetForegroundDrawList()
#define FONTSIZE 40
#define PADDING 10

ClickGuiMod* GUIManager::Instance = nullptr;

class TabData
{
public:
    float animationPercentage;
    std::string_view name;
    char icon;
    ImColor color;
    ImVec2 position;

    // Constructor
    TabData(std::string_view name, const char icon)
    {
        this->name = name;
        this->icon = icon;
        this->color =  ImColor(255, 255, 255, 100);
        this->animationPercentage = 0.f;
    }
};

int GUIManager::SelectedTab = 0;




void ClickGuiRender()
{
#ifdef __DEBUG__
    if (ClickGuiMod::ClickGuiMode.SelectedIndex == (int)ClickGuiMod::ClickGuiModeE::New) {
        NewClickGui::Render(GUIManager::SelectedTab == (int)Tab::ClickGui);
    } else {
        ClickGui::Render();
    }
#else
    ClickGui::Render();
#endif
}

void ThemeRender()
{
    ThemeManager::OnRender(GUIManager::SelectedTab == (int)Tab::Theme && GUIManager::Visible);

}

void FriendsRender()
{
    bool visible = GUIManager::SelectedTab == (int)Tab::Friends && GUIManager::Visible;

    float delta = ImGui::GetIO().DeltaTime;

    ImVec2 windowSize = {ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2};
    ImGui::SetNextWindowSize(windowSize);

    static ImVec2 windowPos = {ImGui::GetIO().DisplaySize.x / 2 - windowSize.x / 2, ImGui::GetIO().DisplaySize.y + 10};
    windowPos.x = ImGui::GetIO().DisplaySize.x / 2 - windowSize.x / 2;
    windowPos.y = Math::Animate(windowPos.y, visible ? ImGui::GetIO().DisplaySize.y / 2 - windowSize.y / 2 :
                                             ImGui::GetIO().DisplaySize.y + 30, delta * 10);
    ImGui::SetNextWindowPos(windowPos);

    ImGui::Begin("Friends");

    for (auto friend_ : Prax::Preferences->Module.Friends)
    {
        ImGui::Text(friend_.c_str());
        ImGui::SameLine();
        if (ImGui::Button(std::string("Remove##Remove" + friend_).c_str()))
        {
            Prax::Preferences->Module.Friends.erase(std::remove(Prax::Preferences->Module.Friends.begin(), Prax::Preferences->Module.Friends.end(), friend_), Prax::Preferences->Module.Friends.end());
            PrefManager::Save(Prax::Preferences);
        }
    }

    // Create string for the input
    static char input[64] = "";

    ImGui::InputText("##FriendInput", input, 64);

    if (ImGui::Button("Add"))
    {
        if (strlen(input) == 0)
            return;

        Prax::Preferences->Module.Friends.push_back(input);
        // Clear the input
        memset(input, 0, sizeof(input));
        PrefManager::Save(Prax::Preferences);
    }



    ImGui::End();
}

void KeybindsRender()
{
    bool visible = GUIManager::SelectedTab == (int)Tab::Keybinds && GUIManager::Visible;

    float delta = ImGui::GetIO().DeltaTime;

    ImVec2 windowSize = {ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2};
    ImGui::SetNextWindowSize(windowSize);

    static ImVec2 windowPos = {ImGui::GetIO().DisplaySize.x / 2 - windowSize.x / 2, ImGui::GetIO().DisplaySize.y + 10};
    windowPos.x = ImGui::GetIO().DisplaySize.x / 2 - windowSize.x / 2;
    windowPos.y = Math::Animate(windowPos.y, visible ? ImGui::GetIO().DisplaySize.y / 2 - windowSize.y / 2 :
                                             ImGui::GetIO().DisplaySize.y + 30, delta * 10);
    ImGui::SetNextWindowPos(windowPos);

    ImGui::Begin("Keybinds");

    for (auto& module : ModuleManager::Modules)
    {
        if (module->KeyId == 0)
            continue;

        ImGui::Text("%s Key: %s", module->Name, module->GetKeyText().c_str());
    }

    ImGui::End();
}

void ConfigRender()
{

    bool visible = GUIManager::SelectedTab == (int)Tab::Config && GUIManager::Visible;

    float delta = ImGui::GetIO().DeltaTime;

    ImVec2 windowSize = {ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2};
    ImGui::SetNextWindowSize(windowSize);

    static ImVec2 windowPos = {ImGui::GetIO().DisplaySize.x / 2 - windowSize.x / 2, ImGui::GetIO().DisplaySize.y + 10};
    windowPos.x = ImGui::GetIO().DisplaySize.x / 2 - windowSize.x / 2;
    windowPos.y = Math::Animate(windowPos.y, visible ? ImGui::GetIO().DisplaySize.y / 2 - windowSize.y / 2 :
                                             ImGui::GetIO().DisplaySize.y + 30, delta * 10);
    ImGui::SetNextWindowPos(windowPos);

    ImGui::Begin("Config");

    auto configs = FileSystem::GetAllFilesInDir(FileSystem::ConfigDirectory);

    for (auto& config : configs)
    {
        if (config.find(".json") == std::string::npos)
            continue;

        ImGui::BeginGroup();

        // Set name to everything after the last slash and before the .json
        std::string name = config.substr(config.find_last_of("\\") + 1);

        // Remove the .json
        name = name.substr(0, name.find(".json"));

        // If the name is blank or just ".json", skip it
        if (name.empty() || name == ".json") continue;

        if (ImGui::Button(name.c_str()))
        {
            ConfigManager::Load(name);
        }

        ImGui::EndGroup();
    }

    ImGui::End();
}

float percent = 0.f;

void TabsOnRender()
{


    // Prevent initializing too quick
    {
        if (!ModuleManager::initialized)
            return;

        float delta = ImGui::GetIO().DeltaTime;

        percent = Math::Animate(percent, GUIManager::Visible ? 1.f : 0.f,
                                delta * 8.f);
        percent = std::clamp(percent, 0.f, 1.f);

        if (!ThemeManager::CurrentTheme)
            return;
        Render::FillScreenWithColor(ImColor(0.f, 0.f, 0.f, ThemeManager::CurrentTheme->gobalOpacity * percent));
        Render::FillScreenWithBlur(percent * ThemeManager::CurrentTheme->blur);
    }

    if (!GUIManager::Visible) return;

    ClickGuiRender();
    ThemeRender();
    FriendsRender();
    KeybindsRender();
    ConfigRender();

}

std::vector<TabData*> tabs = std::vector<TabData*>();

void GUIManager::RenderTabs()
{
    if (!ThemeManager::CurrentTheme) return;


    TabsOnRender();

    constexpr ImVec2 windowSize = { TAB_AMOUNT * (FONTSIZE + PADDING) + PADDING * 2, FONTSIZE + PADDING * 2 };

    // Get center top of the screen
    ImVec2 center = ImGui::GetIO().DisplaySize / 2;
    center.y = (ImGui::GetIO().DisplaySize.y * 0.1f) + windowSize.y / 2;
    center.y *= percent;
    center.y -= windowSize.y;

    // Center the window
    ImVec2 windowStart = center - windowSize / 2;
    ImVec2 windowEnd = center + windowSize / 2;

    ImVec2 mousePos = ImGui::GetIO().MousePos;
    DRAWLIST->AddRectFilled(windowStart, windowEnd, ImColor(0, 0, 0, (int)(200 * percent)), ThemeManager::CurrentTheme->Rounding);

    bool toolBarHovered = mousePos.x > windowStart.x && mousePos.x < windowEnd.x && mousePos.y > windowStart.y && mousePos.y < windowEnd.y;

    if (Visible)
    {
        ImGui::SetNextWindowBgAlpha(0.f);
        ImGui::SetNextWindowPos(windowStart);
        ImGui::SetNextWindowSize(windowSize);
        if (toolBarHovered)
        {
            ImGui::SetNextWindowFocus();
        }
        ImGui::Begin("Tabs", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing);
        ImGui::End();
    }

    int i = 0;
    for (auto tab : tabs)
    {
        ImColor color = SelectedTab == i ? ImColor(255, 255, 255, (int)(200 * percent)) : ImColor(255, 255, 255, (int)(100 * percent));


        ImVec2 start = { windowStart.x + (i * (FONTSIZE + PADDING)) + PADDING, windowStart.y + (PADDING / 2)};
        ImVec2 end = { start.x + FONTSIZE + PADDING, start.y + FONTSIZE + PADDING };

        // Get the center of both points for the text
        ImVec2 textCenter = (start + end) / 2;


        bool hovered = mousePos.x > start.x && mousePos.x < end.x && mousePos.y > start.y && mousePos.y < end.y;

        tab->animationPercentage = Math::Lerp(tab->animationPercentage, hovered ? 1.0f : 0.0f, ImGui::GetIO().DeltaTime * 5.f);

        // If the mouse is hovering over the tab
        if (hovered)
        {
            color = ImColor(255, 255, 255, (int)(255 * percent));


            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                SelectedTab = i;
            }
        }
        tab->color = tab->color.LerpWithOpacity(color, tab->animationPercentage);

        auto text = std::string(1, tab->icon);

        // Calculate the text size
        ImVec2 textSize = Font::Fonts["icons_v1_large"]->CalcTextSizeA(FONTSIZE, FLT_MAX, 0, text.c_str());

        textCenter.x -= textSize.x / 2;
        textCenter.y -= textSize.y / 2 + (5 * tab->animationPercentage);

        DRAWLIST->AddText(Font::Fonts["icons_v1_large"], FONTSIZE, textCenter, tab->color, text.c_str(), NULL);


        i++;
    }

    // Get the selected tab text
    std::string selectedTabText = tabs[SelectedTab]->name.data();

    // Calculate the text size
    ImVec2 textSize = Font::Fonts["greycliff_medium_large"]->CalcTextSizeA(FONTSIZE / 2, FLT_MAX, 0, selectedTabText.c_str());


    ImVec2 textCenter = { windowStart.x + windowSize.x / 2 - textSize.x / 2, windowEnd.y + PADDING };

    float textHeight = textSize.y + PADDING;

    ImVec2 rectStart = textCenter - ImVec2(PADDING, textHeight / 2) + ImVec2(0, PADDING);
    ImVec2 rectEnd = textCenter + ImVec2(textSize.x, textHeight / 2) + ImVec2(PADDING, PADDING);


    DRAWLIST->AddRectFilled(rectStart, rectEnd, ImColor(0, 0, 0, (int)(200 * percent)), ThemeManager::CurrentTheme->Rounding);

    // Draw the selected tab text
    DRAWLIST->AddText(Font::Fonts["greycliff_medium_large"], FONTSIZE / 2, textCenter, ImColor(255, 255, 255, (int)(255 * percent)), selectedTabText.c_str(), NULL);


}

bool firstRender = false;

void GUIManager::Render(ClickGuiMod* instance)
{
    if (!instance) return;
    if (!ThemeManager::CurrentTheme) return;
    if (!Prax::Initialized) return;

    if (!firstRender)
    {
        firstRender = true;
        tabs.push_back(new TabData("ClickGui", GUI));
        tabs.push_back(new TabData("Theme", PaintBrush));
        tabs.push_back(new TabData("Friends", Person));
        tabs.push_back(new TabData("Keybinds", Keyboard));
        tabs.push_back(new TabData("Config", Config));
    }

    Instance = instance;
    RenderTabs();
}