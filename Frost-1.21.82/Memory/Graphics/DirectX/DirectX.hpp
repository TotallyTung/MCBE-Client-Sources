#pragma once

#include <wrl/client.h>

#include "Fonts/Icons.hpp"
#include "Fonts/ProductSans Bold.hpp"
#include "Fonts/ProductSans Medium.hpp"
#include "Fonts/ProductSans Regular.hpp"

void* oPresent;
void* oResize;

std::atomic<bool> imguiInit{ false };

Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device = nullptr;
Microsoft::WRL::ComPtr<ID3D12Device> d3d12Device = nullptr;

void DrawImGuiOverlay()
{
    static bool mInitialized = false;
    if (!mInitialized) {
        ImGui::GetIO().FontGlobalScale = 0.5f;
        mInitialized = true;
    }

    ImGuiIO& io = ImGui::GetIO();
    float fps = io.Framerate;
    float ms_per_frame = 1000.0f / fps;

    GlobalVariables::mFPS = fps;

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Frame Time: %.3f ms/frame", ms_per_frame);

    ImRenderEvent event{};
    EventManager::registerEvent(&event);
}

void ApplyUIFonts()
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryTTF(const_cast<uint8_t*>(ProductSans_Regular_ttf), sizeof(ProductSans_Regular_ttf), 48);
    io.Fonts->AddFontFromMemoryTTF(const_cast<uint8_t*>(ProductSans_Medium_ttf), sizeof(ProductSans_Medium_ttf), 48);
    io.Fonts->AddFontFromMemoryTTF(const_cast<uint8_t*>(ProductSans_Bold_ttf), sizeof(ProductSans_Bold_ttf), 48);
    io.Fonts->AddFontFromMemoryTTF(const_cast<uint8_t*>(Icon_ttf), sizeof(Icon_ttf), 48);

}

HRESULT D3D12_PresentDetour(IDXGISwapChain3* swapchain, UINT syncInterval, UINT flags)
{
    if (!EngineWindow) {
        return Memory::CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);
    }

    if (!SUCCEEDED(swapchain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
        if (SUCCEEDED(swapchain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
            swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
            static_cast<ID3D12Device5*>(d3d12Device.Get())->RemoveDevice();
        }
        return Memory::CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);
    }

    while (FrameUtil::mFrameTransforms.size() > FrameUtil::mTransformDelay)
    {
        FrameUtil::mTransform = FrameUtil::mFrameTransforms.front();
        FrameUtil::mFrameTransforms.pop();
    }

    Microsoft::WRL::ComPtr<ID3D11DeviceContext> ppContext;
    d3d11Device->GetImmediateContext(ppContext.GetAddressOf());

    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(pBackBuffer.GetAddressOf()));

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mainRenderTargetView;
    d3d11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, mainRenderTargetView.GetAddressOf());

    if (!imguiInit.load(std::memory_order_acquire)) {
        ImGui::CreateContext();
        ApplyUIFonts();
        ImGui_ImplWin32_Init(EngineWindow);
        ImGui_ImplDX11_Init(d3d11Device.Get(), ppContext.Get());
        imguiInit.store(true, std::memory_order_release);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    Frame::mFov = FrameUtil::mTransform.mFov;
    Frame::mDisplaySize = InstanceManager::get<ClientInstance>()->getGuiData()->mWindowResolution;
    Frame::mOrigin = FrameUtil::mTransform.mOrigin;

    DrawImGuiOverlay();

    ImGui::EndFrame();
    ImGui::Render();
    ppContext->OMSetRenderTargets(1, mainRenderTargetView.GetAddressOf(), nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return Memory::CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);
}

class DirectXHook {
public:
    static bool StartUp() 
    {
        winrt::Windows::UI::Core::CoreWindow cw = winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow();
        winrt::com_ptr<ICoreWindowInterop> interop;
        winrt::check_hresult(winrt::get_unknown(cw)->QueryInterface(interop.put()));
        winrt::check_hresult(interop->get_WindowHandle(&EngineWindow));

        if (kiero::init(kiero::RenderType::D3D12) != kiero::Status::Success && kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success) {
            log("Failed to initialize kiero hooking.");
            return false;
        }

        uint16_t PresentDetourIndex = (kiero::getRenderType() == kiero::RenderType::D3D12) ? 140 : 8;

        if (kiero::bind(PresentDetourIndex, reinterpret_cast<void**>(&oPresent), D3D12_PresentDetour) != kiero::Status::Success) {
            log("Failed to create a hook for Present Detour.");
            return false;
        }

        return true;
    }
};
