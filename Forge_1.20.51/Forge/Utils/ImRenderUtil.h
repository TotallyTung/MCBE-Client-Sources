#pragma once

class ImRenderUtil
{
public:
	inline static void drawText(Vector2<float> pos, std::string* textStr, const UIColor& color, float textSize, float alpha, bool shadow = false, int index = 0, ImDrawList* d = ImGui::GetBackgroundDrawList())
	{
		if (!ImGui::GetCurrentContext())
			return;
		ImFont* font = ImGui::GetFont();

		ImVec2 textPos = (pos).ToImVec();
		constexpr ImVec2 shadowOffset = ImVec2(1.f, 1.f);

		if (shadow)
		{
			d->AddText(font, (textSize * 18), textPos + shadowOffset, ImColor(color.r * 0.2f, color.g * 0.2f, color.b * 0.2f, alpha * 0.5f), textStr->c_str());
		}

		d->AddText(font, (textSize * 18), textPos, ImColor(color.r, color.g, color.b, alpha), textStr->c_str());
	};

	template <typename T>
	static void fillRectangle(Vector4<T> pos, const UIColor& color, float alpha, float radius = 0.f, ImDrawList* list = ImGui::GetBackgroundDrawList())
	{
		if (!ImGui::GetCurrentContext())
			return;

		list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, alpha), radius);
	}

	static void fillShadowRectangle(Vector4<float> pos, const UIColor& color, float alpha, float thickness, ImDrawFlags flags, float radius = 0.f, ImDrawList* list = ImGui::GetBackgroundDrawList())
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImVec2 offset = ImVec2(0, 0);
		list->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, alpha), thickness, offset, flags, radius);
	}

	static void fillShadowCircle(Vector2<float> pos, float radius, const UIColor& color, float alpha, float thickness, ImDrawFlags flags, float segments = 12.f)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		ImVec2 offset = ImVec2(0, 0);
		list->AddShadowCircle(ImVec2(pos.x, pos.y), radius, ImColor(color.r, color.g, color.b, alpha), thickness, offset, flags, radius);
	}

	static void drawShadowSquare(Vector2<float> center, float size, const UIColor& color, float alpha, float thickness, ImDrawFlags flags)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		ImVec2 offset = ImVec2(0, 0);

		// Define the four corners of the square
		ImVec2 points[4];
		points[0] = ImVec2(center.x - size / 2.f, center.y - size / 2.f);
		points[1] = ImVec2(center.x + size / 2.f, center.y - size / 2.f);
		points[2] = ImVec2(center.x + size / 2.f, center.y + size / 2.f);
		points[3] = ImVec2(center.x - size / 2.f, center.y + size / 2.f);

		list->AddShadowConvexPoly(points, 4, ImColor(color.r, color.g, color.b, alpha), thickness, offset, flags);
	}

	static void drawRoundRect(Vector4<float> pos, const ImDrawFlags& flags, float radius, const UIColor& color, float alpha, float lineWidth)
	{
		if (!ImGui::GetCurrentContext())
			return;
		const auto d = ImGui::GetBackgroundDrawList();
		d->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, alpha), radius, flags, lineWidth);
	}

	static void fillGradientOpaqueRectangle(Vector4<float> pos, const UIColor& firstColor, const UIColor& secondColor, float firstAlpha, float secondAlpha)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		list->AddRectFilledMultiColor(topLeft, bottomRight,
			ImColor(firstColor.r, firstColor.g, firstColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, firstAlpha),
			ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha));
	}

	static void fillRoundedGradientRectangle(Vector4<float> pos, const UIColor& firstColor, const UIColor& secondColor, float radius, float firstAlpha, float secondAlpha)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		list->AddRectFilledMultiColor(topLeft, bottomRight,
			ImColor(firstColor.r, firstColor.g, firstColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, firstAlpha),
			ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha));
	}

	static void fillCircle(Vector2<float> center, float radius, const UIColor& color, float alpha, int segments)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		list->AddCircleFilled(center.ToImVec(), radius, ImColor(color.r, color.g, color.b, alpha), segments);
	}

	static inline Vector2<float> getScreenSize() {
		RECT desktop;
		static HWND window = (HWND)FindWindowA(nullptr, "Minecraft");
		GetWindowRect(window, &desktop);
		int w = desktop.right - desktop.left;
		int h = desktop.bottom - desktop.top;
		// not in fullscreen
		if (desktop.top != 0 || desktop.left != 0) {
			w -= 30;
			h -= 45;
		}
		return Vector2<float>(w, h);
	}


	static inline float getTextWidth(std::string* textStr, float textSize)
	{
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, textStr->c_str()).x;
	}

	static inline float getTextHeight(float textSize)
	{
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, "").y;
	}

	static inline float getDeltaTime() {
		return 0.016f;
	}
};