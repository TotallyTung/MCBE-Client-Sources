#include "DirectXHook.h"

#include "../../Manager/Module.h"

#include "../../Manager/Manager.h"
#include "../../Manager/Client.h"
#include "../../Include/imgui/imgui.h"
#include "../../Include/imgui/imgui_impl_dx11.h"
#include "../../Include/imgui/imgui_impl_win32.h"
#include "../../Include/imgui/imgui_impl_dx12.h"
#include "DirectXHook.h"
#include "../../Include/imgui/Font/Fonts.h"
#include "../../Include/imgui/imgui_notify.h"

Manager* scMgr = nullptr;

bool initContext = false;
bool initImGui = false;

enum SwapChain_DeviceType { INVALID_DEVICE_TYPE, D3D11, D3D12 };

typedef HRESULT(__thiscall* PresentD3D12)(IDXGISwapChain3*, UINT, UINT);
PresentD3D12 oPresentD3D;

typedef HRESULT(__thiscall* resize_buffers_t)(IDXGISwapChain3*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
resize_buffers_t original_resize_buffers;

struct FrameContext {

    ID3D12CommandAllocator* commandAllocator = nullptr;
    ID3D12Resource* main_render_target_resource = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;

};

UINT buffersCounts = -1;
FrameContext* frameContext = nullptr;

ID3D12CommandQueue* d3d12CommandQueue = nullptr;

ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers = nullptr;
ID3D12GraphicsCommandList* d3d12CommandList = nullptr;
ID3D12CommandAllocator* allocator = nullptr;
ID3D11DeviceContext* ppContext = nullptr;

ID3D11Texture2D* pBackBuffer = nullptr;

ID3D11RenderTargetView* mainRenderTargetView = nullptr;

//Devices
SwapChain_DeviceType deviceType = SwapChain_DeviceType::INVALID_DEVICE_TYPE;
ID3D11Device* d3d11Device = nullptr;
ID3D12Device* d3d12Device = nullptr;

void tryTickImGui(Module* mod) {
    __try {
        mod->onImGuiRender();
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {

    }
};

auto hookPresentD3D(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags) -> HRESULT {
    auto window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");

    auto callRendr = [&]() {
        if (scMgr != nullptr) {
            for (auto category : scMgr->categories) {
                for (auto mod : category->modules) {
                    if (mod->isEnabled) tryTickImGui(mod);
                }
            }
        }
    };

    if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
        deviceType = SwapChain_DeviceType::D3D11;
    }
    else if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
        deviceType = SwapChain_DeviceType::D3D12;
    };

    if (deviceType == SwapChain_DeviceType::INVALID_DEVICE_TYPE)
        goto out;

    if (deviceType == SwapChain_DeviceType::D3D11) {
        if (!initContext) {

            ImGui::CreateContext();
            auto& io = ImGui::GetIO();
            io.Fonts->AddFontFromMemoryCompressedTTF(ProductSans_compressed_data, ProductSans_compressed_size, 48.f);
            io.Fonts->AddFontFromMemoryCompressedBase85TTF(Mojangles_compressed_data_base85, 48.f);
            io.Fonts->Build();

            initContext = true;
        };

        d3d11Device->GetImmediateContext(&ppContext);
        ppSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
    
        if (!initImGui)
        {
            ImGui_ImplWin32_Init(window);
            ImGui_ImplDX11_Init(d3d11Device, ppContext);
            initImGui = true;
        }

        pBackBuffer->Release();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        callRendr();


        ImGui::EndFrame();
        ImGui::Render();

        ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        mainRenderTargetView->Release();
        ppContext->Release();

    }
    else if (deviceType == SwapChain_DeviceType::D3D12) {

        if (!initContext) {

            ImGui::CreateContext();
            auto& io = ImGui::GetIO();
            io.Fonts->AddFontFromMemoryCompressedTTF(ProductSans_compressed_data, ProductSans_compressed_size, 48.f);
            io.Fonts->AddFontFromMemoryCompressedBase85TTF(Mojangles_compressed_data_base85, 48.f);
            io.Fonts->Build();
        };

        DXGI_SWAP_CHAIN_DESC sdesc;
        ppSwapChain->GetDesc(&sdesc);
        sdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sdesc.OutputWindow = window;
        sdesc.Windowed = ((GetWindowLongPtr(window, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

        buffersCounts = sdesc.BufferCount;
        frameContext = new FrameContext[buffersCounts];

        D3D12_DESCRIPTOR_HEAP_DESC descriptorImGuiRender = {};
        descriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        descriptorImGuiRender.NumDescriptors = buffersCounts;
        descriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        if (d3d12DescriptorHeapImGuiRender == nullptr)
            if (FAILED(d3d12Device->CreateDescriptorHeap(&descriptorImGuiRender, IID_PPV_ARGS(&d3d12DescriptorHeapImGuiRender))))
                goto out;

        if (d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)) != S_OK)
            return false;

        for (size_t i = 0; i < buffersCounts; i++) {
            frameContext[i].commandAllocator = allocator;
        };

        if (d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, NULL, IID_PPV_ARGS(&d3d12CommandList)) != S_OK ||
            d3d12CommandList->Close() != S_OK)
            return false;

        D3D12_DESCRIPTOR_HEAP_DESC descriptorBackBuffers;
        descriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        descriptorBackBuffers.NumDescriptors = buffersCounts;
        descriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        descriptorBackBuffers.NodeMask = 1;

        if (d3d12Device->CreateDescriptorHeap(&descriptorBackBuffers, IID_PPV_ARGS(&d3d12DescriptorHeapBackBuffers)) != S_OK)
            return false;

        const auto rtvDescriptorSize = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = d3d12DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();

        for (size_t i = 0; i < buffersCounts; i++) {
            ID3D12Resource* pBackBuffer = nullptr;

            frameContext[i].main_render_target_descriptor = rtvHandle;
            ppSwapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&pBackBuffer));
            d3d12Device->CreateRenderTargetView(pBackBuffer, nullptr, rtvHandle);
            frameContext[i].main_render_target_resource = pBackBuffer;
            rtvHandle.ptr += rtvDescriptorSize;

            pBackBuffer->Release();
        };

        if (!initContext) {
            ImGui_ImplWin32_Init(window);
            ImGui_ImplDX12_Init(d3d12Device, buffersCounts,
                DXGI_FORMAT_R8G8B8A8_UNORM, d3d12DescriptorHeapImGuiRender,
                d3d12DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
                d3d12DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
            initContext = true;
        };

        if (d3d12CommandQueue == nullptr)
            goto out;

        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        callRendr();

        FrameContext& currentFrameContext = frameContext[ppSwapChain->GetCurrentBackBufferIndex()];
        currentFrameContext.commandAllocator->Reset();

        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = currentFrameContext.main_render_target_resource;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

        d3d12CommandList->Reset(currentFrameContext.commandAllocator, nullptr);
        d3d12CommandList->ResourceBarrier(1, &barrier);
        d3d12CommandList->OMSetRenderTargets(1, &currentFrameContext.main_render_target_descriptor, FALSE, nullptr);
        d3d12CommandList->SetDescriptorHeaps(1, &d3d12DescriptorHeapImGuiRender);

        ImGui::EndFrame();
        ImGui::Render();

        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), d3d12CommandList);

        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

        d3d12CommandList->ResourceBarrier(1, &barrier);
        d3d12CommandList->Close();

        d3d12CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&d3d12CommandList));

        d3d12DescriptorHeapBackBuffers->Release();
        d3d12CommandList->Release();
        allocator->Release();

        currentFrameContext.main_render_target_resource->Release();
        currentFrameContext.commandAllocator->Release();

        d3d12Device->Release();

        delete frameContext;

    };

    goto out;

out:
    return oPresentD3D(ppSwapChain, syncInterval, flags);

};

typedef void(__thiscall* ExecuteCommandListsD3D12)(ID3D12CommandQueue*, UINT, ID3D12CommandList*);
ExecuteCommandListsD3D12 oExecuteCommandListsD3D12;

auto hookExecuteCommandListsD3D12(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists) -> void {
    if (!d3d12CommandQueue)
        d3d12CommandQueue = queue;

    oExecuteCommandListsD3D12(queue, NumCommandLists, ppCommandLists);
};

HRESULT resize_buffers_callback(IDXGISwapChain3* swap_chain, UINT buffer_count, UINT width, 
                                UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) {
    //reinit ImGui dx11
    initImGui = false;
    return original_resize_buffers(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
}

auto Hook_DirectX::init(void) -> StatusData {
    scMgr = this->mgr;

    if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success) {
        kiero::bind(140, (void**)&oPresentD3D, hookPresentD3D);
        kiero::bind(54, (void**)&oExecuteCommandListsD3D12, hookExecuteCommandListsD3D12);
    } else if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		kiero::bind(8, (void**)&oPresentD3D, hookPresentD3D);
        kiero::bind(13, (void**)&original_resize_buffers, resize_buffers_callback);
    } else {
		return StatusData(MethodStatus::Error, "[Kiero] Failed to initialize!");
	}

    return StatusData(MethodStatus::Error, "[Kiero] Successfully hooked methods!");

};
