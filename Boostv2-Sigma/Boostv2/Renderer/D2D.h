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

namespace D2D {
    extern float deltaTime;

    void NewFrame(IDXGISwapChain3* swapChain, ID3D11Device* d3d11Device, float fxdpi);
    void EndFrame();
    void Render();
    void Clean();
    void Flush();

    Vec2<float> getWindowSize();
    void drawNamePng(float x, float y, float width, float height);
    void drawText(const Vec2<float>& textPos, const std::string& textStr, const UIColor& color, float textSize = 1.f, bool storeTextLayout = true);
    float getTextWidth(const std::string& textStr, float textSize = 1.f, bool storeTextLayout = true);
    float getTextHeight(const std::string& textStr, float textSize = 1.f, bool storeTextLayout = true);
    void drawLine(const Vec2<float>& startPos, const Vec2<float>& endPos, const UIColor& color, float width = 1.f);
    void drawRectangle(const Vec4<float>& rect, const UIColor& color, float width = 1.f);
    void fillRectangle(const Vec4<float>& rect, const UIColor& color);
    void fillRoundingRectangle(const Vec4<float>& rect, const UIColor& color, float radius);
    void drawBox(const AABB& box, const UIColor& fillColor, const UIColor& outlineColor, float thickness, bool fill, bool outline);
    void drawBoxTest(const Vec3<float>& lower, const Vec3<float>& upper, float lineWidth, const UIColor& fillColor, const UIColor& outlineColor, bool fill, bool outline, float rotationAngle, const Vec3<float>& rotationAxis);
    void drawBoxOutline(const Vec3<float>& lower, const Vec3<float>& upper, float lineWidth, const UIColor& outlineColor, float rotationAngle, const Vec3<float>& rotationAxis);
    void fillGlowRectangle(const Vec4<float>& rect, const UIColor& color, float glowStrength);
    void drawCircle(const Vec2<float>& centerPos, const UIColor& color, float radius, float width = 1.f);
    void fillCircle(const Vec2<float>& centerPos, const UIColor& color, float radius);
    void addBlur(const Vec4<float>& rect, float strength, bool flush = true);
    void drawColorSB(const Vec4<float>& rect, float hue);
    void drawColorHueBar(const Vec4<float>& rect);
    void lookAt(float viewMatrix[4][4], const Vec3<float>& eye, const Vec3<float>& center, const Vec3<float>& up);
    void perspective(float projectionMatrix[4][4], float fov, float aspect, float zNear, float zFar);
};