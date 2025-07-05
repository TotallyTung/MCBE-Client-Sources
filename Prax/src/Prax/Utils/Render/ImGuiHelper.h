#pragma once

#include <dxgi.h>
#include <d3d11.h>

class ImGuiHelper {
public:
    static void Init(IDXGISwapChain* pSwapChain, ID3D11Device *pDevice, ID3D11DeviceContext *pContext);

    static void NewFrame();
    static void OnRender();
    static void EndFrame();

    static void Shutdown();

    bool LoadTextureFromFile(const char *filename, ID3D11ShaderResourceView **out_srv, int *out_width, int *out_height);
};