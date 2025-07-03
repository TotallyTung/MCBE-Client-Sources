#pragma once

class ImRenderUtil
{
public:
	static inline void drawText(Vector2<float> pos, std::string* textStr, const Color& color, float textSize, float alpha, bool shadow = false)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImVec2 textPos = ImVec2(pos.x, pos.y);
		float offset = 1;
		ImVec2 shadowOffset = ImVec2(textPos.x + offset, textPos.y + offset);

		if (shadow)
			ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), (textSize * 18), shadowOffset, ImColor(color.r * 0.2f, color.g * 0.2f, color.b * 0.2f, alpha * 0.6f), textStr->c_str());

		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), (textSize * 18), textPos, ImColor(color.r, color.g, color.b, alpha), textStr->c_str());
	};

	static inline void drawColoredText(Vector2<float> mPos, std::string* textStr, Color mColor, float mTextSize, float mAlpha, bool mShadow = false)
	{
		if (!ImGui::GetCurrentContext())
			return;

		std::string mMessage = *textStr;

		Vector2<float> mTextPos = mPos;

		Color mCurrentColor = mColor;

		for (size_t j = 0; j < mMessage.length(); ++j) {
			char c = mMessage[j];

			if (c == '§' && j + 1 < mMessage.length()) {
				char colorCode = mMessage[j + 1];
				if (mColorMap.find(colorCode) != mColorMap.end()) {
					mCurrentColor = mColorMap[colorCode];
					j++;
				}

				if (colorCode == 'r') {
					mCurrentColor = mColor;
				}
				continue;
			}

			if (c == '\n') {
				mTextPos.x = mPos.x;
				mTextPos.y += ImGui::GetFont()->CalcTextSizeA(mTextSize * 18, FLT_MAX, 0, "\n").y;
			}

			if (!std::isprint(c)) {
				if (c != '»') {
					continue;
				}
			}

			std::string mString = Util::combine(c, "");

			ImRenderUtil::drawText(mTextPos, &mString, mCurrentColor, mTextSize, mAlpha, mShadow);

			mTextPos.x += ImGui::GetFont()->CalcTextSizeA(mTextSize * 18, FLT_MAX, -1, mString.c_str()).x;
		}
	};

	template <typename T>
	static inline void fillRectangle(Vector4<T> pos, const Color& color, float alpha, float radius = 0.f, const ImDrawFlags& flags = ImDrawFlags_None)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, alpha), radius, flags);
	}

	static inline void fillTriangle(Vector2<float> p1, Vector2<float> p2, Vector2<float> p3, const Color& color, float alpha)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImGui::GetBackgroundDrawList()->AddTriangleFilled(ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y), ImVec2(p3.x, p3.y), ImColor(color.r, color.g, color.b, alpha));
	}

	static void drawShadowSquare(Vector2<float> center, float size, const Color& color, float alpha, float thickness, ImDrawFlags flags)
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

	static void fillShadowRectangle(Vector4<float> pos, const Color& color, float alpha, float thickness, ImDrawFlags flags, float rounding = 0.f)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImVec2 offset = ImVec2(0, 0);
		ImGui::GetBackgroundDrawList()->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, alpha), thickness, offset, flags, rounding);
	}

	static void drawRoundRect(Vector4<float> pos, const ImDrawFlags& flags, float rounding, const Color& color, float alpha, float lineWidth)
	{
		if (!ImGui::GetCurrentContext())
			return;
		const auto d = ImGui::GetBackgroundDrawList();
		d->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, alpha), rounding, flags, lineWidth);
	}

	static void drawGradientRectangle(Vector4<float> pos, const Color& firstColor, const Color& secondColor, float rounding = 0.f, float firstAlpha = 1.f, float secondAlpha = 1.f, float outlineThickness = 1.0f)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		ImVec2 shrinkedTopLeft = ImVec2(pos.x + outlineThickness, pos.y + outlineThickness);
		ImVec2 shrinkedBottomRight = ImVec2(pos.z - outlineThickness, pos.w - outlineThickness);

		int startBufferSize = list->VtxBuffer.Size;
		list->AddRect(shrinkedTopLeft, shrinkedBottomRight, ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha), rounding, 0, outlineThickness);
		int endBufferSize = list->VtxBuffer.Size;

		ImGui::ShadeVertsLinearColorGradientKeepAlpha(list, startBufferSize, endBufferSize, topLeft, bottomRight, ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha), ImColor(secondColor.r, secondColor.g, secondColor.b, secondAlpha));
	}

	static void fillGradientRectangleFlag(Vector4<float> pos, const Color& firstColor, const Color& secondColor, float rounding = 0.f, float firstAlpha = 1.f, float secondAlpha = 1.f, ImDrawFlags flags = ImDrawFlags_None)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		int startBufferSize = list->VtxBuffer.Size;
		list->AddRectFilled(topLeft, bottomRight, ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha), rounding, flags);
		int endBufferSize = list->VtxBuffer.Size;
		list->AddRectFilled(topLeft, bottomRight, ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha), rounding, flags);
		int endBufferSize2 = list->VtxBuffer.Size;

		ImGui::ShadeVertsLinearColorGradientKeepAlpha(list, startBufferSize, endBufferSize, topLeft, bottomRight, ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha), ImColor(secondColor.r, secondColor.g, secondColor.b, secondAlpha));
		ImGui::ShadeVertsLinearColorGradientKeepAlpha(list, endBufferSize, endBufferSize2, topLeft, bottomRight, ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha), ImColor(secondColor.r, secondColor.g, secondColor.b, secondAlpha));
	}

	static void fillGradientRectangle(Vector4<float> pos, const Color& firstColor, const Color& secondColor, float firstAlpha, float secondAlpha, float rounding = 0)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(topLeft, bottomRight,
			ImColor(firstColor.r, firstColor.g, firstColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, firstAlpha),
			ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha), rounding, 0);
	}
	
	static void fillGradientRectangleHorizontal(Vector4<float> pos, const Color& firstColor, const Color& secondColor, float firstAlpha, float secondAlpha, float rounding = 0)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(topLeft, bottomRight,
			ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha),
			ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, secondAlpha), rounding, 0);
	}

	static void fillCircle(Vector2<float> center, float radius, const Color& color, float alpha, int segments)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		list->AddCircleFilled(ImVec2(center.x, center.y), radius, ImColor(color.r, color.g, color.b, alpha), segments);
	}

	static void drawCircle(Vector2<float> center, float radius, const Color& color, float alpha, int segments, float thickness = 1.f)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		list->AddCircle(ImVec2(center.x, center.y), radius, ImColor(color.r, color.g, color.b, alpha), segments, thickness);
	}

	static void fillShadowCircle(Vector2<float> pos, float rounding, const Color& color, float alpha, float thickness, ImDrawFlags flags = ImDrawFlags_RoundCornersAll, float segments = 12.f)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		ImVec2 offset = ImVec2(0, 0);
		list->AddShadowCircle(ImVec2(pos.x, pos.y), rounding, ImColor(color.r, color.g, color.b, alpha), thickness, offset, flags, rounding);
	}

	static inline float getTextWidth(std::string* textStr, float textSize)
	{
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, textStr->c_str()).x;
	}

	static inline float getTextHeight(float textSize)
	{
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, "").y;
	}

	static inline Vector2<float> getScreenSize()
	{
		RECT desktop;

		GetWindowRect(EngineWindow, &desktop);
		int w = desktop.right - desktop.left;
		int h = desktop.bottom - desktop.top;
		return Vector2<float>(w, h);
	}

	static inline Vector2<float> getMousePos() {
		if (!ImGui::GetCurrentContext()) return Vector2<float>(0, 0);
		return Vector2<float>(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	}

	inline static bool isMouseOver(Vector4<float>(pos))
	{
		Vector2<float> mousePos = getMousePos();
		return mousePos.x >= pos.x && mousePos.y >= pos.y && mousePos.x < pos.z && mousePos.y < pos.w;
	}

	static inline void drawLine(Vector2<float> start, Vector2<float> end, Color color, float lineWidth)
	{
		if (!ImGui::GetCurrentContext()) return;

		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImColor(color.r, color.g, color.b, color.a), lineWidth);
	}

	static inline void drawCheckMark(Vector2<float> pos, float size, Color color, float alpha, float thickness = 2.0f)
	{
		ImVec2 end1 = ImVec2(pos.x + (3 * size), pos.y + (3 * size));
		ImVec2 end2 = ImVec2(pos.x + (7 * size), pos.y - (3 * size));

		ImGui::GetForegroundDrawList()->AddLine(ImVec2(pos.x, pos.y), end1, ImColor(color.r, color.g, color.b, alpha), thickness);
		ImGui::GetForegroundDrawList()->AddLine(end1, end2, ImColor(color.r, color.g, color.b, alpha), thickness);
	}

	static inline float getDeltaTime()
	{
		return 0.016f;
	}
};

class ImScaleUtil {
public:
	ImScaleUtil()
	{
		scale_start_index = 0.f;
	}

	static void ImScaleStart()
	{
		scale_start_index = ImGui::GetBackgroundDrawList()->VtxBuffer.Size;
	}

	static inline int scale_start_index;

	static ImVec2 ImScaleCenter()
	{
		ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

		const auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;
		for (int i = scale_start_index; i < buf.Size; i++)
			l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

		return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
	}

	static void ImScaleEnd(float scaleX, float scaleY, ImVec2 center = ImScaleCenter())
	{
		auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;

		for (int i = scale_start_index; i < buf.Size; i++)
		{
			ImVec2 pos = ImVec2(buf[i].pos.x - center.x, buf[i].pos.y - center.y);
			pos.x *= scaleX;
			pos.y *= scaleY;
			buf[i].pos = ImVec2(pos.x + center.x, pos.y + center.y);
		}
	}
};

class ImRotateUtil {
public:
	ImRotateUtil()
	{
		rotationStartIndex = 0.f;
	}

	static void startRotation()
	{
		rotationStartIndex = ImGui::GetBackgroundDrawList()->VtxBuffer.Size;
	}

	static ImVec2 getRotationCenter()
	{
		ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

		const auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;
		for (int i = rotationStartIndex; i < buf.Size; i++)
			l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

		return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
	}

	static void endRotation(float rad, ImVec2 center = getRotationCenter())
	{
		rad += PI * 0.5f;
		float s = sin(rad), c = cos(rad);
		center = ImVec2(ImRotate(center, s, c).x - center.x, ImRotate(center, s, c).y - center.y);

		auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;

		for (int i = rotationStartIndex; i < buf.Size; i++)
			buf[i].pos = ImVec2(ImRotate(buf[i].pos, s, c).x - center.x, ImRotate(buf[i].pos, s, c).y - center.y);
	}

private:
	inline static int rotationStartIndex = 0;
};

class ImClippingUtil {
public:
	static void beginClipping(Vector4<float> rect)
	{
		ImGui::GetBackgroundDrawList()->PushClipRect(ImVec2(rect.x, rect.y), ImVec2(rect.z, rect.w), true);
	}

	static void restoreClipping()
	{
		ImGui::GetBackgroundDrawList()->PopClipRect();
	}
};