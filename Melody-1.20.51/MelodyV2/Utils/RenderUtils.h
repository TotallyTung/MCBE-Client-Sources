// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../SDK/Classes/ScreenView.h"
#include "../SDK/Classes/MinecraftUIRenderContext.h"
#include "../SDK/Classes/ScreenContext.h"
#include "../SDK/Classes/MaterialPtr.h"
#include "../Utils/ColorUtils.h"

class RenderUtils {
private:
	static inline bool init = false;
public:
	enum Type
	{
		INTERNAL = 0,
		EXTERNAL = 1,
	};
	static inline MinecraftUIRenderContext* renderCtx = nullptr;
	static inline ScreenContext* screenContext2D = nullptr;
	static inline ScreenContext* screenContext3D = nullptr;
	static inline Tessellator* Tessellator2D = nullptr;
	static inline float* colorHolder = nullptr;
	static inline FontBitmap* mcFont = nullptr;
	static inline MaterialPtr* uiMaterial = nullptr;
	static inline MaterialPtr* blendMaterial = nullptr;
	static inline float deltaTime = 0.016f;

	static void SetUp(ScreenView* screenView, MinecraftUIRenderContext* ctx);
	static void setGameRenderContext(ScreenContext* screenContext);
	static void setColor(const MC_Color& color);
	static void setColor(float r, float g, float b, float a);
	static Tessellator* getTessellator3D();
	static void drawText(const Vec2<float>& textPos, std::string textStr, MC_Color color, float textSize = 1.f, float alpha = 1.f, bool shadow = true);
	static float getTextWidth(const std::string& textStr, float textSize);
	static float getFontHeight(float textSize);
	static void fillRectangle(Vec4<float> pos, MC_Color col, float alpha);
	static void drawQuad(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Vec2<float>& p4, const MC_Color& color);
	static void drawQuad(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Vec2<float>& p4);
	static void Flush();
	static void drawLine2D(const Vec2<float>& start, const Vec2<float>& end, const MC_Color& color, float lineWidth = 1.f);
	static void drawBox(const AABB& blockAABB, UIColor color, UIColor lineColor, float lineWidth, bool fill, bool outline);
	static void drawBox(const Vec3<float>& blockPos, UIColor color, UIColor lineColor, float lineWidth, bool fill, bool outline);
	static bool DrawAABB(const AABB& aabb, ImU32 espCol, float thickness = 2.0f);
	static bool DrawBlock(Vec3<int>& blockPos, ImU32 color, float thickness = 2.0f);
	static bool worldToScreen(const Vec3<float>& worldPos, Vec2<float>& screenPos);
	static void renderImage(std::string filePath, Vec4<float> rectPosition, Vec2<float> uvPos = Vec2<float>(0.f, 0.f), Vec2<float> uvSize = Vec2<float>(1.f, 1.f), Type type = EXTERNAL);
	static void flushImage(MC_Color color = MC_Color(255, 255, 255), float alpha = 1.f);
};
