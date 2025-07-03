#pragma once

#include <Hook/Hook.hpp>
#include <Hook/HookManager.hpp>
#include <SDK/SigManager.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <Utils/Structs.hpp>
//
// Created by vastrakai on 6/29/2024.
//



class D3DHook : public Hook
{
public:
    D3DHook() : Hook() {
        mName = "D3DHook";
        FrameTransforms = std::make_unique<std::queue<FrameTransform>>();
    }

    static inline std::unique_ptr<std::queue<FrameTransform>> FrameTransforms = nullptr;
    static inline int transformDelay = 3;
    static inline bool forceFallback = false;

    static bool loadTextureFromEmbeddedResource(const char* resourceName, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
    static bool createTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width,
                                      int* out_height);
    static bool createTextureFromData(const uint8_t* data, int width, int height, ID3D11ShaderResourceView** out_srv);
    static HRESULT present(IDXGISwapChain3* swapChain, UINT syncInterval, UINT flags);
    static HRESULT resizeBuffers(IDXGISwapChain3* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
    static void initImGui(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
    static void shutdownImGui();
    static void igNewFrame();
    static void igEndFrame();
    void init() override;
    static void s_init();
    void shutdown() override;
    static void s_shutdown();
};

