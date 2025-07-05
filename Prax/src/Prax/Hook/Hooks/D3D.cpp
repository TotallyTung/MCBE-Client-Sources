#pragma once

#include "D3D.h"

#include <d3d12.h>
#include <d3d11.h>
#include <d2d1_3.h>
#include <dxgi1_4.h>
#include <d3d11on12.h>

#include <kiero.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>

#include <Prax/Utils/Render/D2D.h>
#include <Prax/Utils/Render/ImGuiHelper.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Misc/TestModule.h>
#include <Prax/Utils/Render/Render.h>

#include "stb_image.h"

#define BUFFER_COUNT 3

using winrt::com_ptr;

typedef HRESULT(__thiscall* presentD3D)(IDXGISwapChain3*, UINT, UINT);
presentD3D oPresent;

typedef HRESULT(__thiscall* resizeBuffers)(IDXGISwapChain3*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
resizeBuffers oResizeBuffers;

typedef HRESULT(__thiscall* drawIndexed)(struct ID3D11DeviceContext*, unsigned int, unsigned int, int);
drawIndexed oDrawIndexed;

typedef HRESULT(__stdcall* drawIndexedInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex);
drawIndexedInstanced oDrawIndexedInstanced;

typedef HRESULT(__stdcall* clearRenderTargetView)(ID3D11DeviceContext* dContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);
clearRenderTargetView oClearRenderTargetView;


static winrt::com_ptr<ID3D11Device> device11;
static winrt::com_ptr<ID3D11DeviceContext> context;
static winrt::com_ptr<ID3D12Device> device12;
static std::vector<winrt::com_ptr<ID3D11Texture2D>> mBackBuffer11Tex = std::vector<winrt::com_ptr<ID3D11Texture2D>>();
static std::vector<winrt::com_ptr<ID3D11RenderTargetView>> mBackBuffer11Rtv =
        std::vector<winrt::com_ptr<ID3D11RenderTargetView>>();

// in case we get D3D12 (stuff for D3D11on12)
static winrt::com_ptr<ID3D11DeviceContext> device_context11;
static winrt::com_ptr<ID3D11On12Device> device11on12;

static HWND wnd = NULL;
static WNDPROC oWndProc;
static bool runningD3D11Already = false;
static bool d3dInitImGui = false;

static void CHK(HRESULT hr) {
    if (FAILED(hr))
        throw std::runtime_error("HRESULT is failed value.");
}

#define NOW Misc::GetCurrentMs()


// Simple helper function to load an image into a DX11 texture with common settings
bool D3D::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, nullptr, 4);
    if (image_data == NULL)
    {
        Logger::Write("D3D", "Failed to load image [image_data == NULL]", Logger::Error);
        return false;
    }

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D *pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    device11->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    device11->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free(image_data);

    return true;
}

static IDXGISwapChain3* daSwapChain = nullptr;


HWND D3D::GetHWND() {
    Logger::Write("DirectX", "Getting Minecraft window...", Logger::Debug);
    static HWND cachedHwnd = nullptr;
    if (cachedHwnd) {
        Logger::Write("DirectX", "Found Minecraft window with HWND: " + std::to_string((uintptr_t) cachedHwnd) + " (cached)", Logger::Debug);
        return cachedHwnd;
    }

    Logger::Write("DirectX", "Searching for Minecraft window...", Logger::Debug);
    std::map<HWND, std::string> titles;
    auto callback = [](HWND hwnd, LPARAM lParam) -> BOOL {

        char title[256];
        GetWindowTextA(hwnd, title, sizeof(title));
        std::string titleStr = title;
        auto *titles = reinterpret_cast<std::map<HWND, std::string> *>(lParam);
        titles->insert({hwnd, titleStr});
        return TRUE;
    };

    EnumWindows(callback, reinterpret_cast<LPARAM>(&titles));

    for (auto& [hwnd, title] : titles) {
        // Log the title
        if (title.find("Minecraft") != std::string::npos && title.find("Catgirl") != std::string::npos) {
            Logger::Write("DirectX", "Found window, HWND: " + String::ToHex((uintptr_t) hwnd), Logger::Debug);
            cachedHwnd = hwnd;
            return hwnd;
        }
    }

    return nullptr;
};

HRESULT D3D::PresentDetour(IDXGISwapChain3 *pSwapchain, UINT sync_interval, UINT flags)
{
    daSwapChain = pSwapchain;
    static bool once = false;
    if (!once) {
        if (pSwapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void **>(&device11)) == S_OK) {
            Logger::Write("DirectX", "D3D11 device found", Logger::Debug);

            runningD3D11Already = true;
        }

        if (pSwapchain->GetDevice(__uuidof(ID3D12Device), reinterpret_cast<void **>(&device12)) == S_OK) {
            Logger::Write("DirectX", "D3D12 device found", Logger::Debug);

            if (Prax::Preferences->d3d11Fallback) {
                // Remove the d3d device
                ID3D12Device* bad_device;
                if (SUCCEEDED(pSwapchain->GetDevice(IID_PPV_ARGS(&bad_device))))
                {
                    Logger::Write("Fallback", "Removing D3D12 device...", Logger::Info);
                    reinterpret_cast<ID3D12Device5*>(bad_device)->RemoveDevice();
                    return oPresent(pSwapchain, sync_interval, flags);
                }
            }

            // some device flags for D3D11
            UINT device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED;

            ID3D12CommandQueue *possiblyACommandQueue =
                    reinterpret_cast<bgfx_d3d12_RendererContextD3D12 *>(ClientInstance::GetBGFX()->m_renderCtx)
                            ->m_commandQueue;
            ID3D12CommandQueue *definitelyACommandQueue{};

            CHK(possiblyACommandQueue->QueryInterface(IID_PPV_ARGS(&definitelyACommandQueue)));
            // that's about it, now we can just feed all this to D3D11on12
            // this function is a little confusing so i will comment it heavily
            CHK(D3D11On12CreateDevice(
                    device12.get(), // the D3D12 device
                    device_flags, // flags for the D3D11 device
                    nullptr, // array of feature levels (null is default, which is what the game uses i assume)
                    0, // number of feature levels in that array
                    reinterpret_cast<IUnknown **>(&possiblyACommandQueue), // array of command queues (we only have one)
                    1, // number of command queues in that array
                    0, // node mask (0 is a magic number i actually don't know what it does)
                    device11.put(), // the D3D11 device we get back
                    device_context11.put(), // the D3D11 device context we get back
                    nullptr // the feature level we get back (we don't care)
                    ));
        }
        once = true;
    }


    while(FrameTransforms.size() > transformDelay)
    {
        Render::Transform = FrameTransforms.front();
        FrameTransforms.pop();
    }

    int count = runningD3D11Already ? 1 : BUFFER_COUNT;

    if (!d3dInitImGui)
    {
        mBackBuffer11Tex.resize(count);
        mBackBuffer11Rtv.resize(count);
        if (runningD3D11Already) {
            device11->GetImmediateContext(context.put());
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapchain->GetDesc(&sd);
            pSwapchain->GetBuffer(0, IID_PPV_ARGS(mBackBuffer11Tex.at(0).put()));
            device11->CreateRenderTargetView(mBackBuffer11Tex.at(0).get(), NULL, mBackBuffer11Rtv.at(0).put());
        } else {
            device11->GetImmediateContext(context.put());
            DXGI_SWAP_CHAIN_DESC sd;
            CHK(pSwapchain->GetDesc(&sd));
            wnd = sd.OutputWindow;

            D3D11_RESOURCE_FLAGS backBuffer11Flags = {};
            backBuffer11Flags.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            for (int i = 0; i < BUFFER_COUNT; i++) {

                CHK(device11->QueryInterface(IID_PPV_ARGS(&device11on12)));
                winrt::com_ptr<ID3D12Resource> buffer = nullptr;

                CHK(pSwapchain->GetBuffer(i, IID_PPV_ARGS(buffer.put())));

                CHK(device11on12->CreateWrappedResource(buffer.get(), &backBuffer11Flags,
                                                        D3D12_RESOURCE_STATE_PRESENT,
                                                        D3D12_RESOURCE_STATE_PRESENT,
                                                        IID_PPV_ARGS(mBackBuffer11Tex.at(i).put())));

                D3D11_TEXTURE2D_DESC texDesc;
                mBackBuffer11Tex.at(i)->GetDesc(&texDesc);
                D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
                rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                rtvDesc.Format = texDesc.Format;
                rtvDesc.Texture2D.MipSlice = 0;

                CHK(device11->CreateRenderTargetView(mBackBuffer11Tex.at(i).get(), nullptr,
                                                     mBackBuffer11Rtv.at(i).put()));
            }
            oWndProc = (WNDPROC)SetWindowLongPtr(wnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);
        }
        d3dInitImGui = true;
    }

    float dpi = GetDpiForWindow(wnd);
    UINT index = reinterpret_cast<IDXGISwapChain3 *>(pSwapchain)->GetCurrentBackBufferIndex();
    index = runningD3D11Already ? 0 : index;


    ImGuiHelper::Init(pSwapchain, device11.get(), context.get());
    D2D::Init(pSwapchain, device11.get());

    ImGuiHelper::NewFrame();
    winrt::com_ptr<IDXGISurface> surface;
    mBackBuffer11Tex.at(index).try_as(surface);
    D2D::BeginRender(surface.get(), dpi);


    if (ModuleManager::initialized)
    {
        ImGuiHelper::OnRender();

        // draw stuff here
        bool isInDisconnectScreen = Minecraft::ClientInstance->isInDisconnectScreen();

        uint64_t startTime = NOW;
        for (auto& module : ModuleManager::Modules) {

            if (!module) continue;
            if (module->Enabled && !isInDisconnectScreen || module->RenderOnDisabled && !isInDisconnectScreen) {
                module->OnRender();
            }
        }
    }

    ImGuiHelper::EndFrame();

    if (!runningD3D11Already) {
        ID3D11Resource *resource = mBackBuffer11Tex.at(index).get();
        device11on12->AcquireWrappedResources(&resource, 1);
    }


    auto thing = mBackBuffer11Rtv.at(index).get();
    context->OMSetRenderTargets(1, &thing, NULL);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    D2D::EndRender();

    if (!runningD3D11Already) {
        ID3D11Resource *resource = mBackBuffer11Tex.at(index).get();
        device11on12->ReleaseWrappedResources(&resource, 1);

        context->Flush();
    }

    return oPresent(pSwapchain, sync_interval, flags);
}

HRESULT D3D::ResizeBuffers(IDXGISwapChain3* pSwapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {

    if (d3dInitImGui)
    {

        // release all the stuff we created

        D2D::Shutdown();
        ImGuiHelper::Shutdown();

        mBackBuffer11Rtv.clear();
        mBackBuffer11Tex.clear();
        context->Flush();

        d3dInitImGui = false;

    }

    return oResizeBuffers(pSwapChain, bufferCount, width, height, newFormat, swapChainFlags);
}

static bool isSecondPass = false;

HRESULT D3D::DrawIndexed(struct ID3D11DeviceContext* Context, unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation) {
    ID3D11Buffer* vertBuffer;
    UINT vertBufferOffset;
    UINT stride;

    Context->IAGetVertexBuffers(0, 1, &vertBuffer, &stride, &vertBufferOffset);
    if (stride == 24) {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        ID3D11DepthStencilState* depthStencil;
        UINT stencilRef;

        // get info about current depth stencil
        Context->OMGetDepthStencilState(&depthStencil, &stencilRef);
        depthStencil->GetDesc(&depthStencilDesc);
        depthStencilDesc.DepthEnable = false; // disable depth to ignore all other geometry
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // disable writing to depth buffer
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS; // always pass depth test
        depthStencilDesc.StencilEnable = false; // disable stencil
        depthStencilDesc.StencilReadMask = 0; // disable stencil


        // create a new depth stencil based on current drawn one but with the depth disabled as mentioned before
        device11->CreateDepthStencilState(&depthStencilDesc, &depthStencil);
        Context->OMSetDepthStencilState(depthStencil, stencilRef);

        // call original function
        oDrawIndexed(Context, IndexCount, StartIndexLocation, BaseVertexLocation);

        // release memory
        depthStencil->Release();

        // draw a glow effect

        return 0;
    }

    /*if (isSecondPass && daSwapChain)
    {
        isSecondPass = false;

        ImGuiHelper::Init(daSwapChain, device11.get(), context.get());
        D2D::Init(daSwapChain, device11.get());

        // Start a new imgui fram
        ImGuiHelper::NewFrame();

        // draw a rect
        ImGui::GetBackgroundDrawList()->AddRect(ImVec2(100, 100), ImVec2(1000, 600), IM_COL32(255, 0, 255, 255));

        ImGuiHelper::EndFrame();

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    }*/

    return oDrawIndexed(Context, IndexCount, StartIndexLocation, BaseVertexLocation);
}

// ClearRenderTargetView
HRESULT D3D::ClearRenderTargetView(ID3D11DeviceContext* dContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]) {
    return HRESULT();
    return oClearRenderTargetView(dContext, pRenderTargetView, ColorRGBA);
}

HRESULT D3D::DrawIndexedInstanced(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex) {
    dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

    dCommandList->IASetVertexBuffers(0, 1, nullptr);

    return oDrawIndexedInstanced(dCommandList, IndexCount, InstanceCount, StartIndex, BaseVertex);
}
void D3D::Init() {
    wnd = GetHWND();

    if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success) {
        // Present and ResizeBuffers live at indexes 140 and 145 respectively
        kiero::bind(140, (void**)&oPresent, PresentDetour);
        kiero::bind(145, (void**)&oResizeBuffers, ResizeBuffers);

        Logger::Write("DirectX", "Hooked D3D12.", Logger::Debug);
        return;
    } else {
        Logger::Write("DirectX", "Failed to hook D3D12!", Logger::Warning);
    }

    if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
        // indexes are 8 and 13 for D3D11 instead
        kiero::bind(8, (void**)&oPresent, PresentDetour);
        kiero::bind(13, (void**)&oResizeBuffers, ResizeBuffers);
        //kiero::bind(73, (void**)&oDrawIndexed, DrawIndexed);
        /*kiero::bind(111, (void**)&oClearRenderTargetView, ClearRenderTargetView);*/
        Logger::Write("DirectX", "Hooked D3D11.", Logger::Debug);
    } else {
        Logger::Write("DirectX", "Failed to hook D3D!", Logger::Warning);
    }
}

void D3D::UnHook() {
    kiero::unbind(140); // Present
    kiero::unbind(145); // ResizeBuffers
    kiero::unbind(8); // Present
    kiero::unbind(13); // ResizeBuffers
    kiero::unbind(73); // DrawIndexed
    kiero::unbind(111); // ClearRenderTargetView
    Logger::Write("DirectX", "Removed bindings.", Logger::Debug);

    D2D::Shutdown();
    ImGuiHelper::Shutdown();


    D2D::Shutdown();
    ImGuiHelper::Shutdown();

    mBackBuffer11Rtv.clear();
    mBackBuffer11Tex.clear();
    context->Flush();
    Logger::Write("DirectX", "Cleaned up D3D.", Logger::Debug);


    Logger::Write("DirectX", "Unhooked D3D.", Logger::Debug);
}
