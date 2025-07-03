#include "ImGuiUtils.h"
#include "../Nuke.pl/Module/ModuleManager.h"
#include "../Library/ImGui/imgui_internal.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "../Memory/Hooks.h"

ImDrawList* drawList;
float ImGuiUtils::fontSize = 22.f;

void ImGuiUtils::setDrawList(ImDrawList* d) {
	drawList = d;
}

void ImGuiUtils::drawText(Vec2 pos, const char* text, UIColor color, float textSize, bool shadow) {
	/*
	if (shadow) {
		drawList->AddText(nullptr, 22.f * textSize, pos.add(2.f * textSize).toImVec2(), ImColor((int)(color.r / 5.f), (int)(color.g / 5.f), (int)(color.b / 5.f), color.a), text);
	}
	*/
	static CustomFont* customFontMod = moduleMgr->getModule<CustomFont>();
	if (shadow) {
		if (customFontMod->shadowMode.getSelectedValue() == 1) {
			drawList->AddText(nullptr, fontSize * textSize, pos.add(2.f * textSize).toImVec2(), ImColor((int)(color.r / 5.f), (int)(color.g / 5.f), (int)(color.b / 5.f), color.a), text);
		} else if (customFontMod->shadowMode.getSelectedValue() == 2) {
			drawList->AddText(nullptr, fontSize * textSize, pos.add(1.f * textSize).toImVec2(), ImColor((int)(color.r / 5.f), (int)(color.g / 5.f), (int)(color.b / 5.f), color.a), text);
		}
	}
	drawList->AddText(nullptr, fontSize * textSize, pos.toImVec2(), color.toImColor(), text);
}

void ImGuiUtils::drawText(Vec2 pos, std::string* textStr, UIColor color, float textSize, float alpha, bool shadow) {
    if (!ImGui::GetCurrentContext())
        return;

    ImVec2 textPos = (pos).toImVec2();
    constexpr ImVec2 shadowOffset = ImVec2(1.f, 1.f);
    drawList->AddText(nullptr, (textSize * 18), ImVec2(textPos.x + shadowOffset.x, textPos.y + shadowOffset.y), ImColor(color.r * 0.2f, color.g * 0.2f, color.b * 0.2f, alpha * 0.5f), textStr->c_str());
    color = UIColor(color.r, color.g, color.b, alpha);
    drawList->AddText(nullptr, (textSize * 18), textPos, color.toImColor(), textStr->c_str());
}

float ImGuiUtils::getTextWidth(const char* textStr, float textSize) {
	return ImGui::GetFont()->CalcTextSizeA(fontSize * textSize, FLT_MAX, -1, textStr).x;
}

float ImGuiUtils::getTextHeight(const char* textStr, float textSize) {
    return ImGui::GetFont()->CalcTextSizeA(fontSize * textSize, FLT_MAX, -1, textStr).y;
}

float ImGuiUtils::getTextHeight(float textSize) {
    return ImGui::GetFont()->CalcTextSizeA(fontSize * textSize, FLT_MAX, -1, "").y;
}

void ImGuiUtils::drawRect(const Vec4& pos, UIColor color, float thickness, float rounding) {
	drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color.toImColor(), rounding, 0, thickness);
}

void ImGuiUtils::drawRectFilled(const Vec4& pos, UIColor color, float rounding) {
	drawList->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color.toImColor(), rounding, 0);
}

void ImGuiUtils::drawShadowRect(const Vec4& pos, UIColor color, float shadowThickness, bool filled, float rounding) {
	drawList->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color.toImColor(), shadowThickness, ImVec2(0.f, 0.f), filled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground, rounding);
}

#define PI 3.1415926535

float TORAD(float degree) {
    return static_cast<float>(degree * (PI / 180));
}

ID3D11ShaderResourceView* ImGuiUtils::LoadTextureFromFile(ID3D11Device* device, const char* filename)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    if (!image)
    {
        return nullptr;
    }

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(initData));
    initData.pSysMem = image;
    initData.SysMemPitch = desc.Width * 4;
    initData.SysMemSlicePitch = 0;

    ID3D11Texture2D* texture;
    if (FAILED(device->CreateTexture2D(&desc, &initData, &texture)))
    {
        stbi_image_free(image);
        return nullptr;
    }

    stbi_image_free(image);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;

    ID3D11ShaderResourceView* textureView;
    if (FAILED(device->CreateShaderResourceView(texture, &srvDesc, &textureView)))
    {
        texture->Release();
        return nullptr;
    }

    return textureView;
}

void ImGuiUtils::dImage(ID3D11ShaderResourceView* textureView, ImVec2 position, ImVec2 size, ImDrawList* d)
{
    d->AddImage(textureView, ImVec2(position.x, position.y), ImVec2(position.x + size.x, position.y + size.y));
}

void ImGuiUtils::drawImage(const char* filename, ImVec2 pos, ImVec2 size)
{
    ID3D11Device* device = Hooks::d3d11Device;
    ID3D11DeviceContext* context = Hooks::dContext;

    char* value;
    size_t size2;
    _dupenv_s(&value, &size2, "APPDATA");
    std::string path = value + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\" + Utils::getClientName() + "\\Assets\\") + filename;

    ID3D11ShaderResourceView* textureView = ImGuiUtils::LoadTextureFromFile(device, path.c_str());
    if (textureView)
    {
        ImGuiUtils::dImage(textureView, pos, size, drawList);
    }
}
ImVec2 ImGuiUtils::getScreenResolution() {
    if (!Game.getClientInstance()->getGuiData()) {
        RECT desktop;
        const HWND hDesktop = GetDesktopWindow();
        GetWindowRect(hDesktop, &desktop);
        return ImVec2(static_cast<float>(desktop.right), static_cast<float>(desktop.bottom));
    }
    return ImVec2(Game.getClientInstance()->getGuiData()->widthReal, Game.getClientInstance()->getGuiData()->heightReal);
}
void ImGuiUtils::onMouseClickUpdate(int key, bool isDown) {
    switch (key) {
    case 1:  // Left Click
        isLeftClickDown = isDown;
        if (isDown)
            shouldToggleLeftClick = true;
        else
            isLeftClickDown = false;
        break;
    case 2:  // Right Click
        isRightClickDown = isDown;
        if (!isDown)
            shouldToggleRightClick = true;
        else
            isRightClickDown = false;
    }

}/*

void ImGuiUtils::drawSliderInt(const char* settingName, int min, int max, int defaultValue, int* currentValue, bool isDrag, ImVec2 pos) {
    auto clickGui = moduleMgr->getModule<ClickGuiMod>();
    auto io = ImGui::GetIO();

    ImRect sliderImRect(pos.x, pos.y, pos.x + 300.f, pos.y + ImGuiUtils::getTextHeight(1.2f) + 17.5f);
    ImVec4 sliderRect(sliderImRect.Min.x, sliderImRect.Min.y, sliderImRect.Max.x, sliderImRect.Max.y);

    Vec2 mousePos = Vec2(io.MousePos.x, io.MousePos.y);

    const bool areWeFocused = ImRect(sliderImRect.Min.x, sliderImRect.Min.y, sliderImRect.Max.x + 5.f, sliderImRect.Max.y).Contains(mousePos.toImVec2());

    if (io.MouseDown[0] && areWeFocused) {
        float value = (mousePos.x - sliderRect.x) / (sliderRect.z - sliderRect.x);
        value = fmax(0.0f, fmin(1.0f, value));
        *currentValue = static_cast<int>(value * (max - min) + min);
    }

    float range = static_cast<float>(max - min);
    float normalizedValue = range > 0 ? static_cast<float>(*currentValue - min) / range : 0.0f;
    float circlePosX = sliderRect.x + normalizedValue * (sliderRect.z - sliderRect.x);

    drawList->AddText(nullptr, fontSize * 1.2f, ImVec2(pos.x, pos.y),
        UIColor(255, 255, 255, static_cast<int>(255 * clickGui->percent)).toImColor(), settingName);
    drawList->AddRectFilled(ImVec2(sliderRect.x, sliderRect.y + ImGuiUtils::getTextHeight(1.2f) + 5.f),
        ImVec2(sliderRect.z, sliderRect.y + ImGuiUtils::getTextHeight(1.2f) + 17.5f),
        UIColor(128, 132, 142, static_cast<int>(255 * clickGui->percent)).toImColor(),
        50.f, ImDrawFlags_RoundCornersAll);
    drawList->AddRectFilled(ImVec2(sliderRect.x, sliderRect.y + ImGuiUtils::getTextHeight(1.2f) + 5.f),
        ImVec2(circlePosX, sliderRect.y + ImGuiUtils::getTextHeight(1.2f) + 17.5f),
        UIColor(44, 41, 230, static_cast<int>(255 * clickGui->percent)).toImColor(),
        50.f, ImDrawFlags_RoundCornersLeft);
    drawList->AddCircleFilled(ImVec2(circlePosX, sliderRect.y + ImGuiUtils::getTextHeight(1.2f) + 11.4f), 10.5f,
        UIColor(255, 255, 255, static_cast<int>(255 * clickGui->percent)).toImColor());

    char str[50];
    sprintf_s(str, 50, "%d", *currentValue);
    drawList->AddText(nullptr, fontSize * 1.2f, ImVec2((pos.x + 300.f) - getTextWidth(str, 1.2f) - 6.f, pos.y),
        UIColor(255, 255, 255, static_cast<int>(255 * clickGui->percent)).toImColor(), str);
}

void ImGuiUtils::drawSliderFloat(const char* settingName, float min, float max, float defaultValue, float* currentValue, bool isDrag, ImVec2 pos) {
    auto clickGui = moduleMgr->getModule<ClickGuiMod>();
    auto io = ImGui::GetIO();

    ImRect sliderImRect(pos.x, pos.y, pos.x + 300.f, pos.y + ImGuiUtils::getTextHeight(1.2f) + 17.5f);
    ImVec4 sliderRect(sliderImRect.Min.x, sliderImRect.Min.y, sliderImRect.Max.x, sliderImRect.Max.y);

    Vec2 mousePos = Vec2(io.MousePos.x, io.MousePos.y);

    const bool areWeFocused = ImRect(sliderImRect.Min.x, sliderImRect.Min.y, sliderImRect.Max.x + 5.f, sliderImRect.Max.y).Contains(mousePos.toImVec2());

    if (io.MouseDown[0] && areWeFocused) {
        float value = (mousePos.x - sliderRect.x) / (sliderRect.z - sliderRect.x);
        value = fmax(0.0f, fmin(1.0f, value));
        *currentValue = value * (max - min) + min;
    }

    float range = max - min;
    float normalizedValue = range > 0 ? (*currentValue - min) / range : 0.0f;
    float circlePosX = sliderRect.x + normalizedValue * (sliderRect.z - sliderRect.x);

    drawList->AddText(nullptr, fontSize * 1.2f, ImVec2(pos.x, pos.y),
        UIColor(255, 255, 255, static_cast<int>(255 * clickGui->percent)).toImColor(), settingName);
    drawList->AddRectFilled(ImVec2(sliderRect.x, sliderRect.y + ImGuiUtils::getTextHeight(1.2f) + 5.f),
        ImVec2(sliderRect.z, sliderRect.y + ImGuiUtils::getTextHeight(1.2f) + 17.5f),
        UIColor(128, 132, 142, static_cast<int>(255 * clickGui->percent)).toImColor(),
        50.f, ImDrawFlags_RoundCornersAll);
    drawList->AddRectFilled(ImVec2(sliderRect.x, sliderRect.y + ImGuiUtils::getTextHeight(1.2f) + 5.f),
        ImVec2(circlePosX, sliderRect.y + ImGuiUtils::getTextHeight(1.2f) + 17.5f),
        UIColor(44, 41, 230, static_cast<int>(255 * clickGui->percent)).toImColor(),
        50.f, ImDrawFlags_RoundCornersLeft);
    drawList->AddCircleFilled(ImVec2(circlePosX, sliderRect.y + ImGuiUtils::getTextHeight(1.2f) + 11.4f), 10.5f,
        UIColor(255, 255, 255, static_cast<int>(255 * clickGui->percent)).toImColor());

    char str[50];
    sprintf_s(str, 50, "%.2f", *currentValue);
    drawList->AddText(nullptr, fontSize * 1.2f, ImVec2((pos.x + 300.f) - getTextWidth(str, 1.2f) - 6.f, pos.y),
        UIColor(255, 255, 255, static_cast<int>(255 * clickGui->percent)).toImColor(), str);
}

void ImGuiUtils::drawToggle(const char* settingName, bool* current, ImVec2 pos, float* toggleVal) {
    float theight = ImGuiUtils::getTextHeight(1.2f) + 6.f;
    Vec2 mousePos = Vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);

    auto percent = moduleMgr->getModule<ClickGuiMod>()->percent;

    float circleX = *toggleVal;

    float start = *current ? (pos.x + 12.4f) : (pos.x + 41.6f);
    float end = *current ? (pos.x + 41.6f) : (pos.x + 12.4f);

    *toggleVal = static_cast<float>(smoothLerp(end, *toggleVal, 0.45f));

    circleX = *toggleVal;

    UIColor boolColor(*current ? UIColor(44, 41, 230, static_cast<int>(255 * percent)) : UIColor(128, 132, 142, static_cast<int>(255 * percent)));

    drawList->AddRectFilled(ImVec2(pos.x, pos.y + theight), ImVec2(pos.x + 55.f, pos.y + 25.f + theight), UIColor(boolColor).toImColor(), 50.f, ImDrawFlags_RoundCornersAll);

    float circlePosY = pos.y + 12.3f + theight + (25.f - 12.3f) / 2.0f;

    drawList->AddCircleFilled(ImVec2(circleX, circlePosY), 9.15f, UIColor(255, 255, 255, static_cast<int>(255 * percent)).toImColor());

    if (Vec4(pos.x, pos.y + theight, pos.x + 55.f, pos.y + 25.f + theight).contains(&mousePos) && shouldToggleLeftClick) {
        ImGuiUtils::drawText(Vec2(pos.x + 260, pos.y), *current ? "auaua" : "not auau", UIColor(255, 255, 255, 255 * percent), 1.2f);
        shouldToggleLeftClick = false;
        *current = !(*current);
    }

    shouldToggleLeftClick = false;
    shouldToggleRightClick = false;
}*/