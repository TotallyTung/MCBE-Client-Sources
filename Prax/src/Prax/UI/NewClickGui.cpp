//
// Created by Flash on 12/24/2023.
//

#include "NewClickGui.h"

#include <imgui_toggle.h>
#include <magic_enum.hpp>
#include <Prax/Module/Modules/Visual/ClickGui.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/Utils/System/KeyManager.h>

#include "GUIManager.h"
#include "ImTween.h"

std::vector<ClickGuiWindow*> NewClickGui::Windows = std::vector<ClickGuiWindow*>();
ImGuiStyle NewClickGui::StyleGUI()
{
	// Fork of Deep Dark style from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2();
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2();
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(6.0f, 0.0f);
	style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
	style.CellPadding = ImVec2();
	style.IndentSpacing = 25.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 1.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImColor(30, 30, 30, 255).Value;
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 0.9200000166893005f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 0.2899999916553497f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.239999994635582f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0470588244497776f, 0.0470588244497776f, 0.0470588244497776f, 0.5400000214576721f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 0.5400000214576721f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2274509817361832f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImColor(12, 12, 12, 255).Value;
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(12, 12, 12, 255).Value;
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.0470588244497776f, 0.0470588244497776f, 0.5400000214576721f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3372549116611481f, 0.3372549116611481f, 0.3372549116611481f, 0.5400000214576721f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4000000059604645f, 0.4000000059604645f, 0.4000000059604645f, 0.5400000214576721f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5568627715110779f, 0.5568627715110779f, 0.5568627715110779f, 0.5400000214576721f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5293611288070679f, 0.3294117450714111f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImColor(132, 84, 250, 255).Value;
	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(145, 85, 255).Value;
	style.Colors[ImGuiCol_Button] = ImColor(30, 30, 30, 255).Value;
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.0470588244497776f, 0.0470588244497776f, 0.0470588244497776f, 0.5400000214576721f);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(132, 84, 250, 255).Value;
	style.Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.0f, 0.0f, 0.5199999809265137f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.3600000143051147f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2274509817361832f, 0.3300000131130219f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 0.2899999916553497f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4392156898975372f, 0.4392156898975372f, 0.4392156898975372f, 0.2899999916553497f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.4000000059604645f, 0.4392156898975372f, 0.4666666686534882f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 0.2899999916553497f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4392156898975372f, 0.4392156898975372f, 0.4392156898975372f, 0.2899999916553497f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4000000059604645f, 0.4392156898975372f, 0.4666666686534882f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.0f, 0.0f, 0.0f, 0.5199999809265137f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.3600000143051147f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.0f, 0.0f, 0.5199999809265137f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5199999809265137f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0f, 0.0f, 0.0f, 0.5199999809265137f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 0.2899999916553497f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2274509817361832f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.3294117748737335f, 0.6666666865348816f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.529411792755127f, 0.3294117748737335f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImColor().Value;
	style.Colors[ImGuiCol_WindowShadow] = ImColor().Value;

	return style;
}

void NewClickGui::Init() {
    for (auto module : ModuleManager::Modules) {

        bool categoryExists = false;
        // See if the category already exists if it does, add the module to the category
        for (const auto window : Windows) {
            if (String::ToLower(window->Name) == String::ToLower(module->Category)) {
                window->Modules.push_back(module);
                categoryExists = true;
            }
        }



        // If the category doesn't exist, create it
        if (!categoryExists) {
            auto window = new ClickGuiWindow(module->Category);
            window->Modules.push_back(module);
            Windows.push_back(window);
        }
    }

	SortCategoriesByLength(Windows);
}

void NewClickGui::SortModuleByLength(std::vector<Module*> &modules) {
	std::ranges::sort(modules, [](const Module* a, const Module* b) {
		const float aTextLength = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, 0, a->Name.c_str()).x;
		const float bTextLength = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, 0, b->Name.c_str()).x;

		return aTextLength > bTextLength;
	});
}

void NewClickGui::SortCategoriesByLength(std::vector<ClickGuiWindow*> &windows)
{
	std::ranges::sort(windows, [](const ClickGuiWindow* a, const ClickGuiWindow* b) {
		return a->Modules.size() > b->Modules.size();
	});
}

ImGuiMouseButton NewClickGui::GetMouseDown()
{
	const ImGuiContext& g = *GImGui;
	for (int n = 0; n < IM_ARRAYSIZE(g.IO.MouseClicked); n++)
	{
		if (g.IO.MouseClicked[n])
		{
			return n;
		}

	}
	return -1;
}

void NewClickGui::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	constexpr ImGuiDataType data_type = ImGuiDataType_Float;
	const void* p_min = &v_min;
	const void* p_max = &v_max;
	void* p_data = v;


	ImGuiWindow* window = ImGui::GetCurrentWindow();

	if (window->SkipItems)
		return;

	if (const ImDrawList* drawList = ImGui::GetWindowDrawList(); drawList == nullptr)
		return;

	constexpr float rounding = 10;

	const ImGuiStyle& style = ImGui::GetStyle();

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    const float w = ImGui::CalcItemWidth();

    const ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
	ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
        return;

    // Default format string when passing NULL
    if (format == nullptr)
        format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ImGui::ItemHoverable(frame_bb, id, g.LastItemData.InFlags);
    bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive(id);
    if (!temp_input_is_active)
    {
        // Tabbing or CTRL-clicking on Slider turns it into an input box
        const bool input_requested_by_tabbing = temp_input_allowed && (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
        const bool clicked = hovered && ImGui::IsMouseClicked(0, id);
        const bool make_active = (input_requested_by_tabbing || clicked || g.NavActivateId == id);
        if (make_active && clicked)
            ImGui::SetKeyOwner(ImGuiKey_MouseLeft, id);
        if (make_active && temp_input_allowed)
            if (input_requested_by_tabbing || (clicked && g.IO.KeyCtrl) || (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
                temp_input_is_active = true;

        if (make_active && !temp_input_is_active)
        {
	        ImGui::SetActiveID(id, window);
	        ImGui::SetFocusID(id, window);
	        ImGui::FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }
    }

    if (temp_input_is_active)
    {
        // Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
        const bool is_clamp_input = (flags & ImGuiSliderFlags_AlwaysClamp) != 0;
        ImGui::TempInputScalar(frame_bb, id, label, data_type, p_data, format, is_clamp_input ? p_min : nullptr, is_clamp_input ? p_max : nullptr);
    	return;
    }

    // Draw frame
    const ImU32 frame_col = ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    ImGui::RenderNavHighlight(frame_bb, id);
    ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, rounding);

    // Slider behavior
    ImRect grab_bb;
    if (ImGui::SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb))
        ImGui::MarkItemEdited(id);

    // Render grab
    if (grab_bb.Max.x > grab_bb.Min.x)
    {
    	window->DrawList->AddRectFilled(frame_bb.Min, grab_bb.Max, ImGui::GetColorU32(ImGuiCol_SliderGrabActive), rounding);
    }

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    if (g.LogEnabled)
	    ImGui::LogSetNextTextDecoration("{", "}");
	ImGui::RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, nullptr, ImVec2(0.5f, 0.5f));

    if (label_size.x > 0.0f)
	    ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
}

bool IsFirstRender = true;
ImFont* lastFont = nullptr;

void ShowToolTip(const std::string& text)
{
	if (ImGui::IsItemHovered())
	{

		float fontSize = ImGui::GetFontSize();
		ImFont* font = ImGui::GetFont();
		// Push theme font size
		ImGui::GetCurrentContext()->Font = Font::Fonts["inter_light"];
		ImGui::BeginTooltip();
		ImGui::GetCurrentContext()->FontSize = ThemeManager::CurrentTheme->ToolTipFontSize;
		ImGui::Text(text.c_str());
		ImGui::EndTooltip();

		ImGui::GetCurrentContext()->FontSize = fontSize;
		ImGui::GetCurrentContext()->Font = font;
	}
}

ClickGuiMod::SortingModeE sortingMode = ClickGuiMod::SortingModeE::None;

float clickguiAnimationPercent = 0.0f;

void NewClickGui::Render(bool visible)
{



	if (!visible || !GUIManager::Visible)
		return;

	if (IsFirstRender)
	{
		Init();

		StyleGUI();
	}

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	// Render each category window
	int windowIndex = 1;
	for (const auto window : Windows) {

		ClickGuiMod::SortingModeE currentSortingMode = (ClickGuiMod::SortingModeE)ClickGuiMod::SortingMode.SelectedIndex;
		// Sort modules in category if needed
		if (IsFirstRender || lastFont != ImGui::GetFont() || sortingMode != currentSortingMode)
		{

			if (currentSortingMode == ClickGuiMod::SortingModeE::Alphabetical)
				std::ranges::sort(window->Modules, [](const Module* a, const Module* b) {
					return a->Name < b->Name;
				});
			else if (currentSortingMode == ClickGuiMod::SortingModeE::Length)
				SortModuleByLength(window->Modules);
			sortingMode = currentSortingMode;
		}

		Theme* theme = ThemeManager::CurrentTheme;
		ImGui::SetNextWindowPos(ImVec2(windowIndex * (window->windowSize.x + 10), 50), ImGuiCond_Once);
		ImGui::SetNextWindowSize(window->windowSize, ImGuiCond_Once);
		ImGui::SetNextWindowSizeConstraints(ImVec2(window->windowSize.x, ThemeManager::CurrentTheme->HeaderHeight), ImVec2(window->windowSize.x,
			                                    theme->MaxWindowHeightPercent * ImGui::GetIO().DisplaySize.y));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));


		ImGui::Begin(window->Name.c_str(), nullptr,
		             ImGuiWindowFlags_NoScrollbar |
		             ImGuiWindowFlags_NoResize |
		             ImGuiWindowFlags_NoCollapse |
		             ImGuiWindowFlags_AlwaysAutoResize |
		             ImGuiWindowFlags_NoBackground |
		             ImGuiWindowFlags_NoTitleBar);
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const ImVec2 windowPos = ImGui::GetWindowPos();
		const ImVec2 windowSize = ImGui::GetWindowSize();

		// Render our own background
		ImRect windowRect(windowPos, windowPos + windowSize);

		drawList->AddRectFilled(windowRect.Min, windowRect.Max, theme->backgroundColor1, theme->Rounding);
		drawList->AddRect(windowRect.Min, windowRect.Max, theme->BorderColor,
		                  theme->Rounding, ImDrawCornerFlags_All, theme->BorderSize);

		// Render category header
		{
			// TODO: Add configurability
			ImGui::PushFont(Font::Fonts["inter_light_large"]);
			ImGui::GetCurrentContext()->FontSize = ThemeManager::CurrentTheme->CategoryFontSize;

			const ImVec2 titleSize(ImGui::GetWindowWidth(), ThemeManager::CurrentTheme->HeaderHeight);

			// Switch to moving the window after mouse is moved beyond the initial drag threshold
			ImGui::Dummy(titleSize);


			const ImVec2 titleStart = ImGui::GetItemRectMin();
			const ImVec2 titleEnd = ImGui::GetItemRectMax();


			// Render title box
			drawList->AddRectFilled(
				ImVec2(titleStart.x, titleStart.y),
				ImVec2(titleEnd.x, titleEnd.y),
				ThemeManager::CurrentTheme->backgroundColor2, theme->Rounding, ImDrawCornerFlags_Top);


			std::string titleText = window->Name;
			const ImVec2 titleTextSize = ImGui::GetFont()->CalcTextSizeA(theme->CategoryFontSize, FLT_MAX, 0, titleText.c_str());

			ImVec2 titleTextPos =
			{
				titleStart.x + (titleSize.x * theme->WindowTitleTextAlign.x) - (titleTextSize.x / 2),
				titleStart.y + (titleSize.y * theme->WindowTitleTextAlign.y) - (titleTextSize.y / 2),
			};

			drawList->AddText(ImGui::GetFont(), theme->CategoryFontSize, titleTextPos + 1, theme->categoryTextColor.GetShadowColor(), titleText.c_str());
			drawList->AddText(ImGui::GetFont(), theme->CategoryFontSize, titleTextPos, theme->categoryTextColor, titleText.c_str());


			ImGui::PopFont();
		}

		// For each module in the category, render it
		for (const auto module : window->Modules)
		{
			// Render button for module
			{
				ImGui::PushFont(Font::Fonts["inter_extralight_large"]);
				ImGui::GetCurrentContext()->FontSize = ThemeManager::CurrentTheme->ModuleFontSize;

				std::string text = module->Binding ? "..." : module->Name;

				ImVec2 buttonSize = {
					windowSize.x,
					theme->ModuleHeight
				};

				ImGui::InvisibleButton(("##moduleButton" + module->Name).c_str(), buttonSize, ImGuiButtonFlags_None);
				ShowToolTip(module->Description);

				ImVec2 buttonStart(ImGui::GetItemRectMin());
				ImVec2 buttonEnd(ImGui::GetItemRectMax());

				const ImColor col = module->Enabled ? theme->primaryColor : ImGui::IsItemHovered() ? theme->backgroundColor2 : theme->backgroundColor1;

				theme->primaryColor.useRgb ? module->UIInfo.color = col : module->UIInfo.color = module->UIInfo.color.Lerp(col, ImGui::GetIO().DeltaTime * 5.0f);


				drawList->AddRectFilled(buttonStart, buttonEnd, module->UIInfo.color, 0);

				const ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(theme->ModuleFontSize, FLT_MAX, 0, text.c_str());
				ImVec2 textPos = {
					buttonStart.x + (buttonSize.x * theme->ButtonTextAlign.x) - (textSize.x / 2),
					buttonStart.y + (buttonSize.y * theme->ButtonTextAlign.y) - (textSize.y / 2)
				};


				ClickGuiColor moduleTextColor = module->Enabled ? theme->moduleTextColor : theme->moduleDisabledTextColor;

				drawList->AddText(ImGui::GetFont(), theme->ModuleFontSize, textPos + 1, moduleTextColor.GetShadowColor(), text.c_str());
				drawList->AddText(ImGui::GetFont(), theme->ModuleFontSize, textPos, moduleTextColor, text.c_str());


				// Handle clicking logic
				{

					if (ImGui::IsItemHovered())
					{
						switch (GetMouseDown())
						{
						case ImGuiMouseButton_Left:
							if (KeyManager::Keys[VK_CONTROL])
							{
								module->Binding = true;
								GUIManager::Binding = true;
							} else
							{
								module->Toggle();
							}
							break;
						case ImGuiMouseButton_Right:
							module->Expanded = !module->Expanded;
							break;
						case ImGuiMouseButton_Middle:
							module->Binding = true;
							GUIManager::Binding = true;
							break;
						default:
							break;
						}
					}

					if (module->Binding)
					{
						if (const auto key = KeyManager::GetFirstDown(); key == VK_ESCAPE)
						{
							module->Binding = false;
							GUIManager::Binding = false;
						}
						else if (key != -1)
						{
							module->KeyId = static_cast<int>(key);
							module->Binding = false;
							GUIManager::Binding = false;
						}

					}
				}

				ImGui::PopFont();
			}

			module->UIInfo.expandingAnimationPercent = Math::Lerp(module->UIInfo.expandingAnimationPercent, module->Expanded ? 1.0f : 0.0f, ImGui::GetIO().DeltaTime * 5.0f);

			if (ClickGuiMod::SettingDisplayMode.SelectedIndex == (int)ClickGuiMod::SettingDisplayModeE::Integrated)
			{
				// Adjust to padding
				ImVec2 controlPanelSize = {
					windowSize.x - 3,
					30
				};

				const float height = theme->SettingFontSize + theme->ItemSpacing.y * 2 + ImGui::GetStyle().FramePadding.y * 2;

				// Calculate the max height of the settings

				int index = 0;
				for (auto setting : module->Settings)
				{
					switch (setting->Type)
					{
					case SettingType::Number:
						{
							index += 2;
							break;
						}
					default:
						{
							index++;
							break;
						}
					}
				}

				float maxHeight = (height * index) + ImGui::GetStyle().WindowPadding.y * 2 + controlPanelSize.y + 3;

				ImTween<float>::Tween(
													std::tuple { 0.01f, maxHeight, &module->expandedHeight })
												.OfType(ImTween<>::TweenType::Lerped)
												.Speed(5.0f * ImGui::GetIO().DeltaTime)
												.When(
													[module]()
													{
														return module->Expanded;
													}).Tick();


				ImGui::BeginChild(("##Settings" + module->Name).c_str(),
					ImVec2(ImGui::GetWindowWidth() - (theme->ItemPadding.x * 2), module->expandedHeight),
					false,
					ImGuiWindowFlags_NoScrollbar);


				// Render control panel
				{
					ImGui::PushFont(Font::Fonts["inter_light"]);
					ImGui::Dummy(controlPanelSize);

					ImVec2 controlPanelStart(ImGui::GetItemRectMin() + ImVec2(3, 3));
					ImVec2 controlPanelEnd(ImGui::GetItemRectMax());

					drawList->AddRectFilled(controlPanelStart, controlPanelEnd, theme->backgroundColor3, theme->Rounding);


					std::string text = module->Binding ? "Awaiting key..." : "Keybind: " + module->GetKeyText();

					ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(theme->SettingFontSize, FLT_MAX, 0, text.c_str());

					// Get the center of the control panel to render what the module is bound to
					ImVec2 textPos  = {
						controlPanelStart.x + theme->InnerItemSpacing.x,
						controlPanelStart.y + (controlPanelSize.y / 2) - (textSize.y / 2)
					};

					drawList->AddText(ImGui::GetFont(), theme->SettingFontSize, textPos + 1, theme->settingTextColor.GetShadowColor(), text.c_str());
					drawList->AddText(ImGui::GetFont(), theme->SettingFontSize, textPos, theme->settingTextColor, text.c_str());

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - controlPanelSize.y + 3);

					ImGui::InvisibleButton(std::string("##" + module->Name + "controlPanel").c_str(), ImVec2(controlPanelSize.x / 2, controlPanelSize.y / 2), ImGuiButtonFlags_None);
					ShowToolTip("Bind");
					if (ImGui::IsItemHovered())
					{
						if (GetMouseDown() == ImGuiMouseButton_Left)
						{
							module->Binding = !module->Binding;
							GUIManager::Binding = module->Binding;
						}
					}

					ImGui::PopFont();

					// Render button for hiding in arraylist
					{

						ImGui::PushFont(Font::Fonts["icons_v1"]);

						text = Font::IconFontToString(module->HideInArraylist ? EyeClosed : EyeOpen);

						ImVec2 buttonSize = {
							controlPanelSize.x / 4,
							controlPanelSize.y
						};

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + windowSize.x - buttonSize.x);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (controlPanelSize.y / 2) + (theme->ItemPadding.y / 2));

						ImGui::InvisibleButton(("##hideInArraylist" + module->Name).c_str(), buttonSize, ImGuiButtonFlags_None);
						ShowToolTip("Hide in arraylist");


						ImVec2 buttonStart(ImGui::GetItemRectMin());
						ImVec2 buttonEnd(ImGui::GetItemRectMax());

						ClickGuiColor eyeColor = module->HideInArraylist ? theme->EyeIconOnColor : theme->EyeIconOffColor;

						textSize = ImGui::GetFont()->CalcTextSizeA(theme->IconFontSize, FLT_MAX, 0, text.c_str());
						textPos = {
							buttonStart.x + (buttonSize.x / 2) - (textSize.x / 2),
							buttonStart.y + (buttonSize.y / 2) - (textSize.y / 2)
						};

						drawList->AddText(ImGui::GetFont(), theme->IconFontSize, textPos + 1, eyeColor.GetShadowColor(), text.c_str());
						drawList->AddText(ImGui::GetFont(), theme->IconFontSize, textPos, eyeColor, text.c_str());

						if (ImGui::IsItemHovered())
						{
							if (GetMouseDown() == ImGuiMouseButton_Left)
							{
								module->HideInArraylist = !module->HideInArraylist;
							}
						}

						ImGui::PopFont();
					}
				}

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, theme->ItemSpacing);
				ImGui::GetCurrentContext()->FontSize = theme->SettingFontSize;


				// Set text color to setting text color
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)theme->settingTextColor);

				// If the module is expanded, render the settings
				for (const auto setting : module->Settings)
				{
					switch (setting->Type)
					{
					case SettingType::Bool:
						{

							// TODO: Right alright switch and left align text as well as change color
							const auto boolSetting = reinterpret_cast<BoolSetting*>(setting);

							ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(ThemeManager::CurrentTheme->toggleHoverColor));
							ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(ThemeManager::CurrentTheme->toggleDisabledColor));
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(ThemeManager::CurrentTheme->toggleDisabledColor));
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ThemeManager::CurrentTheme->toggleHoverColor));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ThemeManager::CurrentTheme->toggleHoverColor));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ThemeManager::CurrentTheme->toggleHoverColor));
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + theme->ItemSpacing.x * 2);

							ImGui::Text((boolSetting->Name + ":").c_str());

							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetItemRectSize().y - theme->ItemSpacing.y);
							// Align toggle to the right
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + windowSize.x - (ImGuiToggleConstants::WidthRatioDefault * ImGui::GetItemRectSize().y) - ImGui::GetStyle().ItemSpacing.x * 2);
							ImGui::Toggle(std::string("##" + module->Name + boolSetting->Name).c_str(), &boolSetting->Enabled, ImGuiToggleFlags_Animated);

							ImGui::PopStyleColor(6);
							break;
						}
					case SettingType::Number:
						{
							// Show text above


							const auto numberSetting = reinterpret_cast<NumberSetting*>(setting);
							// Sliders are weird and take up a lot of space so we need to make them smaller
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + theme->ItemSpacing.x * 2);
							ImGui::Text((numberSetting->Name + ":").c_str());
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + theme->ItemSpacing.x * 2);
							ImGui::SetNextItemWidth(windowSize.x - theme->ItemSpacing.x * 4);

							ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(ThemeManager::CurrentTheme->primaryColor));
							SliderFloat(std::string("##" + module->Name + numberSetting->Name).c_str(), &numberSetting->Value, numberSetting->Min, numberSetting->Max);
							ImGui::PopStyleColor(1);

							break;
						}
					case SettingType::Mode:
						{
							// Style to look like concept and right align drop down
							const auto modeSetting = reinterpret_cast<ModeSetting*>(setting);

							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + theme->ItemSpacing.x * 2);
							ImGui::Text((modeSetting->Name + ":").c_str());
							ImGui::SameLine();

							float width = ImGui::GetWindowWidth() - ImGui::GetCursorPosX() - theme->ItemSpacing.x * 2;
							ImGui::SetNextItemWidth(width);

							ImGui::Combo(std::string("##" + module->Name + modeSetting->Name).c_str(), &modeSetting->SelectedIndex, modeSetting->GetImGuiComboText().c_str(), static_cast<int>(modeSetting->Modes.size()));

							break;
						}
					default: break;
					}
					ShowToolTip(setting->Description);
				}
				ImGui::PopStyleColor(1);
				ImGui::PopStyleVar(1);

				ImGui::EndChild();
			}
			else
			{

				if (module->Expanded)
				{
					for (auto mod : ModuleManager::Modules)
					{
						if (mod != module)
						{
							mod->Expanded = false;
						}
					}

				}

				ImVec2 settingWindowSize = {
					displaySize.x * 0.5f,
					displaySize.y * 0.3f
				};
				ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2 - settingWindowSize.x / 2, (displaySize.y / 2 - settingWindowSize.y / 2) * (module->UIInfo.expandingAnimationPercent) - settingWindowSize.y * (1 - module->UIInfo.expandingAnimationPercent)));
				ImGui::SetNextWindowSize(settingWindowSize);
				ImGui::Begin(std::string(module->Name + " Settings").c_str(), &module->Expanded);

				if (ImGui::Button(("Key: " + module->GetKeyText()).c_str()))
				{
					module->Binding = true;
					GUIManager::Binding = true;
				}

				for (auto setting : module->Settings)
				{
					switch (setting->Type)
					{
						case SettingType::Bool:
						{
							const auto boolSetting = reinterpret_cast<BoolSetting*>(setting);
							ImGui::Checkbox(boolSetting->Name.c_str(), &boolSetting->Enabled);
							break;
						}
						case SettingType::Number:
						{
							const auto numberSetting = reinterpret_cast<NumberSetting*>(setting);
							ImGui::SliderFloat(numberSetting->Name.c_str(), &numberSetting->Value, numberSetting->Min, numberSetting->Max);
							break;
						}
						case SettingType::Mode:
						{
							const auto modeSetting = reinterpret_cast<ModeSetting*>(setting);
							ImGui::Combo(modeSetting->Name.c_str(), &modeSetting->SelectedIndex, modeSetting->GetImGuiComboText().c_str(), static_cast<int>(modeSetting->Modes.size()));
							break;
						}
					}
				}

				ImGui::End();
			}
		}

		ImGui::End();
		ImGui::PopStyleVar(1);

		windowIndex++;
	}


	if (lastFont != ImGui::GetFont())
		lastFont = ImGui::GetFont();

	IsFirstRender = false;
}