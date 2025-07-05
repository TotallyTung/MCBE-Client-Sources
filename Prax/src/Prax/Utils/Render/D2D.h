#pragma once
#include <d2d1_3.h>
#include <d3d11on12.h>
#include <imgui.h>
#include <optional>

class D2D {
public:
    static void Init(IDXGISwapChain* pSwapChain, ID3D11Device* pDevice);
    static void Shutdown();

    static void BeginRender(IDXGISurface* surface, float fxdpi);
    static void EndRender();
    static void DrawFilledRect(ImVec4 rect, D2D1::ColorF color);

    static bool AddBlur(ImDrawList *drawList, float strength, std::optional<ImVec4> clipRect, float rounding = ImGui::GetStyle().FrameRounding);
    static void BlurCallback(const ImDrawList *parent_list, const ImDrawCmd *cmd);
    ID3D11ShaderResourceView* LoadImageA(const std::string& string);
};