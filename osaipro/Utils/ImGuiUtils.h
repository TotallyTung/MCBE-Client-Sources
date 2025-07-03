#pragma once
#include "HMath.h"
#include "../Library/ImGui/imgui.h"
#include "../Library/ImFX/imfx.h"
#include "ColorUtil.h"
#include <d3d11.h>

class ImGuiUtils {
public:

	static inline bool isLeftClickDown = false;
	static inline bool isRightClickDown = false;
	static inline bool shouldToggleLeftClick = false;
	static inline bool shouldToggleRightClick = false;

	static void onMouseClickUpdate(int key, bool isDown);
	static ImVec2 getScreenResolution();

	static float fontSize;
	static void setDrawList(ImDrawList* d);
	static void drawText(Vec2 pos, const char* text, UIColor color, float textSize = 1.f, bool shadow = true);
	static void drawText(Vec2 pos, std::string* textStr, UIColor color, float textSize, float alpha, bool shadow = false);
	static float getTextWidth(const char* textStr, float textSize = 1.f);
	static float getTextHeight(float textSize = 1.f);
	static float getTextHeight(const char* textStr, float textSize = 1.f);
	static void drawRect(const Vec4& pos, UIColor color, float thickness = 1.f, float rounding = 0.f);
	static void drawRectFilled(const Vec4& pos, UIColor color, float rounding = 0.f);
	
	static void dImage(ID3D11ShaderResourceView* textureView, ImVec2 position, ImVec2 size, ImDrawList* d);
	static void drawImage(const char* filename, ImVec2 pos, ImVec2 size);
	static void drawSliderFloat(const char* settingName, float min, float max, float defaultValue, float* currentValue, bool isDrag, ImVec2 pos);
	static void drawSliderInt(const char* settingName, int min, int max, int defaultValue, int* currentValue, bool isDrag, ImVec2 pos);
	static void drawToggle(const char* settingName, bool* current, ImVec2 pos, float* toggleVal);
	static ID3D11ShaderResourceView* LoadTextureFromFile(ID3D11Device* device, const char* filename);

	static bool isFullScreen() {
		RECT desktop;
		static HWND window = (HWND)FindWindowA(nullptr, "Minecraft");
		GetWindowRect(window, &desktop);
		if (desktop.top != 0 || desktop.left != 0)
			return false;
		return true;
	}

	static void drawShadowRect(const Vec4& pos, UIColor color, float shadowThickness = 50.f, bool filled = true, float rounding = 0.f);
};