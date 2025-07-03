//
// Created by vastrakai on 6/29/2024.
//

#include "ImRenderUtils.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>
#include <Utils/ProcUtils.hpp>

#include "D2D.hpp"

void ImRenderUtils::addBlur(const ImVec4& pos, float strength, float radius, ImDrawList* list, bool optimized)
{
    if (!ImGui::GetCurrentContext())
        return;

	if (optimized) D2D::addBlurOptimized(list, strength, pos, radius);
    else D2D::addBlur(list, strength, pos, radius);
}

void ImRenderUtils::drawText(ImVec2 pos, const std::string& textStr, const ImColor& color, float textSize, float alpha,
    bool shadow, int index, ImDrawList* d)
{
    if (!ImGui::GetCurrentContext())
        return;
    ImFont* font = ImGui::GetFont();



    ImVec2 textPos = ImVec2(pos.x, pos.y);
    constexpr ImVec2 shadowOffset = ImVec2(1.f, 1.f);
    constexpr ImVec2 shadowOffsetMinecraft = ImVec2(1.45f, 1.45f);

    if (shadow)
    {
        ImVec2 added = textPos;
        added.x = added.x + shadowOffset.x;
        added.y = added.y + shadowOffset.y;
        /*if (font == ImGui::GetIO().Fonts->Fonts[4]) {
				d->AddText(font, (textSize * 18), textPos + shadowOffsetMinecraft, ImColor(color.Value.x * 0.2f, color.Value.y * 0.2f, color.Value.z * 0.2f, alpha * 0.7f), textStr->c_str());
			}
			else {*/
        d->AddText(font, (textSize * 18), added, ImColor(color.Value.x * 0.25f, color.Value.y * 0.25f, color.Value.z * 0.25f, 1.f * alpha), textStr.c_str());
        //}
    }

    d->AddText(font, (textSize * 18), textPos, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), textStr.c_str());
}

void ImRenderUtils::drawShadowText(ImDrawList* drawList, const std::string& text, ImVec2 pos, ImColor color, float fontSize, bool specializedMultiplier)
{
    ImVec2 shadowPos = pos;
    ImVec2 charSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, -1, text.c_str());
	// Needed for ItemESP because of dynamic font size, but it causes issues with other text
	float shadowPosMultiplier = specializedMultiplier ? fontSize / 24.f : 1.f;

    shadowPos.x += 1.f * shadowPosMultiplier;
    shadowPos.y += 1.f * shadowPosMultiplier;
    drawList->AddText(ImGui::GetFont(), fontSize, shadowPos, ImColor(color.Value.x * 0.35f, color.Value.y * 0.35f, color.Value.z * 0.35f, 1.f * color.Value.w), text.c_str());
    drawList->AddText(ImGui::GetFont(), fontSize, pos, color, text.c_str());
}

void ImRenderUtils::fillRectangle(ImVec4 pos, const ImColor& color, float alpha, float radius, ImDrawList* list, ImDrawFlags flags)
{
    if (!ImGui::GetCurrentContext())
        return;

    list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), radius, flags);
}

void ImRenderUtils::fillShadowRectangle(ImVec4 pos, const ImColor& color, float alpha, float thickness,
    ImDrawFlags flags, float radius, ImDrawList* list)
{
    if (!ImGui::GetCurrentContext())
        return;

    ImVec2 offset = ImVec2(0, 0);
    list->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), thickness, offset, flags, radius);
}

void ImRenderUtils::fillShadowCircle(ImVec2 pos, float radius, const ImColor& color, float alpha, float thickness,
    ImDrawFlags flags, float segments)
{
    if (!ImGui::GetCurrentContext())
        return;

    ImDrawList* list = ImGui::GetBackgroundDrawList();
    ImVec2 offset = ImVec2(0, 0);
    list->AddShadowCircle(ImVec2(pos.x, pos.y), radius, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), thickness, offset, flags, radius);
}

void ImRenderUtils::drawRoundRect(ImVec4 pos, const ImDrawFlags& flags, float radius, const ImColor& color, float alpha,
    float lineWidth)
{
    if (!ImGui::GetCurrentContext())
        return;
    const auto d = ImGui::GetBackgroundDrawList();
    d->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), radius, flags, lineWidth);
}

void ImRenderUtils::fillGradientOpaqueRectangle(ImVec4 pos, const ImColor& firstColor, const ImColor& secondColor,
    float firstAlpha, float secondAlpha)
{
    if (!ImGui::GetCurrentContext())
        return;

    ImDrawList* list = ImGui::GetBackgroundDrawList();

    ImVec2 topLeft = ImVec2(pos.x, pos.y);
    ImVec2 bottomRight = ImVec2(pos.z, pos.w);

    list->AddRectFilledMultiColor(topLeft, bottomRight,
                                  ImColor(firstColor.Value.x, firstColor.Value.y, firstColor.Value.z, secondAlpha),
                                  ImColor(secondColor.Value.x, secondColor.Value.y, secondColor.Value.z, secondAlpha),
                                  ImColor(secondColor.Value.x, secondColor.Value.y, secondColor.Value.z, firstAlpha),
                                  ImColor(firstColor.Value.x, firstColor.Value.y, firstColor.Value.z, firstAlpha));
}

void ImRenderUtils::fillRoundedGradientRectangle(ImVec4 pos, const ImColor& firstColor, const ImColor& secondColor,
    float radius, float firstAlpha, float secondAlpha, ImDrawFlags flags)
{
	if (!ImGui::GetCurrentContext())
		return;

	ImDrawList* list = ImGui::GetBackgroundDrawList();

	ImVec2 topLeft = ImVec2(pos.x, pos.y);
	ImVec2 bottomRight = ImVec2(pos.z, pos.w);

	int startBufferSize = list->VtxBuffer.Size;
	list->AddRectFilled(topLeft, bottomRight, ImColor(firstColor.Value.x, firstColor.Value.y, firstColor.Value.z, firstAlpha), radius, flags);
	int endBufferSize = list->VtxBuffer.Size;
	list->AddRectFilled(topLeft, bottomRight, ImColor(firstColor.Value.x, firstColor.Value.y, firstColor.Value.z, firstAlpha), radius, flags);
	int endBufferSize2 = list->VtxBuffer.Size;

	ImGui::ShadeVertsLinearColorGradientKeepAlpha(list, startBufferSize, endBufferSize, topLeft, bottomRight, ImColor(firstColor.Value.x, firstColor.Value.y, firstColor.Value.z, firstAlpha), ImColor(secondColor.Value.x, secondColor.Value.y, secondColor.Value.z, secondAlpha));
	ImGui::ShadeVertsLinearColorGradientKeepAlpha(list, endBufferSize, endBufferSize2, topLeft, bottomRight, ImColor(firstColor.Value.x, firstColor.Value.y, firstColor.Value.z, firstAlpha), ImColor(secondColor.Value.x, secondColor.Value.y, secondColor.Value.z, secondAlpha));
}

void ImRenderUtils::drawCheckMark(ImVec2 pos, float size, const ImColor& color, float alpha, float thickness)
{
    ImVec2 end1 = ImVec2(pos.x + (3 * size), pos.y + (3 * size));
    ImVec2 end2 = ImVec2(pos.x + (7 * size), pos.y - (3 * size));

    ImGui::GetForegroundDrawList()->AddLine(ImVec2(pos.x, pos.y), end1, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), thickness);
    ImGui::GetForegroundDrawList()->AddLine(end1, end2, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), thickness);
}

void ImRenderUtils::drawColorPicker(ImVec4& color, ImVec4 canvas_pos) // Hard coded sorry emily
{
	static float hue = 0.0f;
	static float saturation = 0.0f;
	static float value = 0.0f;

	// Color to HSV shit
	ImGui::ColorConvertRGBtoHSV(color.x, color.y, color.z, hue, saturation, value);

	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	//ImVec2 canvas_size = ImVec2(70, 40);
	float hue_picker_thickness = 14.0f;

	ImVec2 sv_picker_pos = ImVec2(canvas_pos.x, canvas_pos.y);
	ImVec2 hue_picker_pos = ImVec2(canvas_pos.x, canvas_pos.y + canvas_pos.w + 10.0f);

	// Draw SV Square
	for (int y = 0; y < canvas_pos.w; y++)
	{
		for (int x = 0; x < canvas_pos.z; x++)
		{
			float s = (float)x / canvas_pos.z;
			float v = 1.0f - (float)y / canvas_pos.w;
			ImU32 col = ImColor::HSV(hue, s, v);
			draw_list->AddRectFilled(
				ImVec2(sv_picker_pos.x + x, sv_picker_pos.y + y),
				ImVec2(sv_picker_pos.x + x + 1, sv_picker_pos.y + y + 1),
				col
			);
		}
	}

	// Draw the circle at the current saturation and value
	ImVec2 sv_cursor_pos = ImVec2(
		sv_picker_pos.x + saturation * canvas_pos.z,
		sv_picker_pos.y + (1.0f - value) * canvas_pos.w
	);
	draw_list->AddCircle(sv_cursor_pos, 5.0f, IM_COL32(255, 255, 255, 255), 12, 2.0f);

	// Draw Hue Bar
	for (int i = 0; i < canvas_pos.z; i++)
	{
		float h = (float)i / canvas_pos.z;
		ImU32 col = ImColor::HSV(h, 1.0f, 1.0f);
		draw_list->AddRectFilled(
			ImVec2(hue_picker_pos.x + i, hue_picker_pos.y),
			ImVec2(hue_picker_pos.x + i + 1, hue_picker_pos.y + hue_picker_thickness),
			col
		);
	}

	// renders the line on the hue(colour) selecter
	ImVec2 hue_cursor_pos = ImVec2(hue_picker_pos.x + hue * canvas_pos.z, hue_picker_pos.y);
	draw_list->AddLine(
		ImVec2(hue_cursor_pos.x, hue_cursor_pos.y),
		ImVec2(hue_cursor_pos.x, hue_cursor_pos.y + hue_picker_thickness),
		IM_COL32(255, 255, 255, 255),
		2.0f
	);

	// Handle Mouse Input for SV Square 😎
	ImGuiIO& io = ImGui::GetIO();
	if (ImRenderUtils::isMouseOver(ImVec4(sv_picker_pos.x, sv_picker_pos.y, sv_picker_pos.x + canvas_pos.z, sv_picker_pos.y + canvas_pos.w)))
	{
		if (ImGui::IsMouseDown(0))
		{
			ImVec2 mouse_pos_in_canvas = ImVec2(io.MousePos.x - sv_picker_pos.x, io.MousePos.y - sv_picker_pos.y);
			saturation = mouse_pos_in_canvas.x / canvas_pos.z;
			value = 1.0f - (mouse_pos_in_canvas.y / canvas_pos.w);
			color = ImColor::HSV(hue, saturation, value);
		}
	}

	// Handle Mouse Input for Hue Bar :s
	if (ImRenderUtils::isMouseOver(ImVec4(hue_picker_pos.x, hue_picker_pos.y, hue_picker_pos.x + canvas_pos.z, hue_picker_pos.y + hue_picker_thickness)))
	{
		if (ImGui::IsMouseDown(0))
		{
			ImVec2 mouse_pos_in_canvas = ImVec2(io.MousePos.x - hue_picker_pos.x, io.MousePos.y - hue_picker_pos.y);
			hue = mouse_pos_in_canvas.x / canvas_pos.z;
			color = ImColor::HSV(hue, saturation, value);
		}
	}

	// Show the selected color don't mind shit frost code :P
	//ImVec2 color_preview_pos = ImVec2(canvas_pos.x, hue_picker_pos.y + hue_picker_thickness + 10.0f);
	//draw_list->AddRectFilled(color_preview_pos, ImVec2(color_preview_pos.x + canvas_pos.z, color_preview_pos.y + 20.0f), ImColor(color));
}


void ImRenderUtils::fillCircle(ImVec2 center, float radius, const ImColor& color, float alpha, int segments)
{
    if (!ImGui::GetCurrentContext())
        return;

    ImDrawList* list = ImGui::GetBackgroundDrawList();
    list->AddCircleFilled(ImVec2(center.x , center.y), radius, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), segments);
}

ImVec2 ImRenderUtils::getScreenSize()
{
    glm::vec2 resolution = ClientInstance::get()->getGuiData()->mResolution;
    return ImVec2(resolution.x, resolution.y);
}

bool ImRenderUtils::isFullScreen()
{
    RECT desktop;

    GetWindowRect(ProcUtils::getMinecraftWindow(), &desktop);
    if (desktop.top != 0 || desktop.left != 0)
        return false;
    return true;
}

float ImRenderUtils::getTextHeightStr(std::string* textStr, float textSize)
{
    return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, textStr->c_str()).y;
}

float ImRenderUtils::getTextWidth(std::string* textStr, float textSize)
{
    return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, textStr->c_str()).x;
}

float ImRenderUtils::getTextHeight(float textSize)
{
    return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, "").y;
}

float ImRenderUtils::getDeltaTime()
{
    return ImGui::GetIO().DeltaTime;
}

ImVec2 ImRenderUtils::getMousePos()
{
    return ImGui::GetIO().MousePos;
}

bool ImRenderUtils::isMouseOver(ImVec4 pos)
{
    ImVec2 mousePos = getMousePos();
    return mousePos.x >= pos.x && mousePos.y >= pos.y && mousePos.x < pos.z && mousePos.y < pos.w;
}

bool ImRenderUtils::isPointInRect(ImVec2 point, ImVec2 rectMin, ImVec2 rectMax)
{
	return point.x >= rectMin.x && point.y >= rectMin.y && point.x <= rectMax.x && point.y <= rectMax.y;
}
