#pragma once
#include <d3d11.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <dxgi1_4.h>
#include <dwrite_3.h>

#include <string>

#include "../Utils/Maths.h"
#include "../Utils/ColorUtil.h"
#include "../Utils/FileUtil.h"

namespace D2D {
	extern float deltaTime;
	void drawWaterMark(float x, float y, float width, float height);
	void NewFrame(IDXGISwapChain3* swapChain, ID3D11Device* d3d11Device, float fxdpi);
	void EndFrame();
	void Render();
	void Clean();
	void Flush();
	void drawNamePng(float x, float y, float width, float height);
	Vec2<float> getWindowSize();
	void drawText(const Vec2<float>& textPos, const std::string& textStr, const UIColor& color, float textSize = 1.f, bool storeTextLayout = true);
	float getTextWidth(const std::string& textStr, float textSize = 1.f, bool storeTextLayout = true);
	float getTextHeight(const std::string& textStr, float textSize = 1.f, bool storeTextLayout = true);
	void drawLine(const Vec2<float>& startPos, const Vec2<float>& endPos, const UIColor& color, float width = 1.f);
	void drawRectangle(const Vec4<float>& rect, const UIColor& color, float width = 1.f);
	void fillRectangle(const Vec4<float>& rect, const UIColor& color);
	void drawCircle(const Vec2<float>& centerPos, const UIColor& color, float radius, float width = 1.f);
	void fillCircle(const Vec2<float>& centerPos, const UIColor& color, float radius);
	void addBlurWTINT(const Vec4<float>& rect, float strength, const UIColor& tintColor, bool flush);
	void addBlur(const Vec4<float>& rect, float strength, bool flush);
	void drawGlowingText(const Vec2<float>& textPos, const std::string& textStr, const UIColor& color, float textSize, float glowAmount, bool storeTextLayout);
	void lookAt(float viewMatrix[4][4], const Vec3<float>& eye, const Vec3<float>& center, const Vec3<float>& up);
	void perspective(float projectionMatrix[4][4], float fov, float aspect, float zNear, float zFar);
	void draw3DBox(const Vec2<float>& pos, const Vec2<float>& size, float depthOffset, const UIColor& fillColor, const UIColor& outlineColor, bool drawOutline);
	void drawQuad(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Vec2<float>& p4, const UIColor& color);
	void fillRoundedRectangle(const Vec4<float>& rect, const UIColor& color, float radius, float rotationAngle);
	void fillTriangle(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const UIColor& color, float rotationAngle);
	void fillRoundedRectangleGradient(const Vec4<float>& rect, const UIColor& color1, const UIColor& color2,
		const UIColor& color3, const UIColor& color4, float radius, float rotationAngle);
	void fillHexagon(const Vec2<float>& center, float size, const UIColor& color, float rotationAngle);
};
