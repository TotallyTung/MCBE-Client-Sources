#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

#include "../ImGui/imgui.h"
#include "../Utils/Utils.h"
#include "../ImGui/imgui_internal.h"



#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <intrin.h>

#include <thread>
#include <unordered_map>

#include "../Horion/Command/CommandMgr.h"
#include "../Horion/Config/ConfigManager.h"
#include "../Horion/DrawUtils.h"
#include "../Horion/Menu/ClickGui.h"
#include "../Horion/Module/ModuleManager.h"
#include "../SDK/BlockLegacy.h"
#include "../SDK/Camera.h"
#include "../SDK/ChestBlockActor.h"
#include "../SDK/GameMode.h"
#include "../SDK/MinecraftUIRenderContext.h"
#include "../SDK/MoveInputHandler.h"
#include "../SDK/RakNetInstance.h"
#include "../SDK/TextHolder.h"
#include "../SDK/UIScene.h"
#include "../SDK/ScreenView.h"
#include "../SDK/MobEffectInstance.h"
#include "../Utils/Logger.h"
#include "../Utils/SkinUtil.h"
#include "../Utils/TextFormat.h"
#include "../resource.h"
#include "../Memory/GameData.h"
#include "../Memory/MinHook.h"

#include <dxgi.h>
#include <dxgi1_4.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d2d1_3.h>
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_dx12.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../Kiero/Kiero.h"

#include "../ImFX/imfx.h"
#include "../Utils/HMath.h"
#include "../Utils/ColorUtil.h"
#include <iostream>
#include <optional>
#include <vector>

std::string base64_encode(uint8_t const* buf, unsigned int bufLen);
std::vector<uint8_t> base64_decode(std::string const&);

struct ID3D11Device;
struct IDXGISurface;
struct ID2D1Bitmap1;

namespace ImFX {
    bool NewFrame(ID3D11Device* d3d11Device, IDXGISurface* backBuffer, float dpi);
    bool EndFrame();
    bool CleanupFX();
    bool Begin(ImDrawList* drawList, bool dontCopy = false);
    bool End(bool composite = false);
    bool PushLayer(std::optional<ImVec4> clipRect = std::optional<ImVec4>());
    bool PopLayer();
    bool AddBlur(float strength, std::optional<ImVec4> clipRect = std::optional<ImVec4>(), float rounding = ImGui::GetStyle().FrameRounding);
    bool AddColor(ImVec4 color, float rounding, std::optional<ImVec4> clipRect);
    bool AddColor(ImVec4 color, std::optional<ImVec4> rounding, std::optional<ImVec4> clipRect);
    bool AddColor(ImVec4 color, std::optional<ImVec4> clipRect = std::optional<ImVec4>());
    bool AddColor(ImGuiCol color, std::optional<ImVec4> clipRect = std::optional<ImVec4>());
    bool AddShadow(float strength = 8.0f, ImVec4 color = ImVec4(0, 0, 0, 1), std::optional<ImVec4> clipRect = std::optional<ImVec4>());
    bool AddDisplacementMap();
};