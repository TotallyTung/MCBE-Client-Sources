#pragma once

#include <d3d11.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <queue>
#include <Prax/SDK/Game/Misc/Structs.h>

#include "../Hook.h"

class D3D : public Hook {
public:
    D3D() : Hook("D3D") {}

    static inline std::queue<FrameTransform> FrameTransforms;
    static inline int transformDelay = 3;

    static HWND GetHWND();


    static bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
    static HRESULT PresentDetour(IDXGISwapChain3* pSwapchain, UINT sync_interval, UINT flags);
    static HRESULT ResizeBuffers(IDXGISwapChain3* pSwapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
    static HRESULT DrawIndexed(struct ID3D11DeviceContext* Context, unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation);

    static HRESULT DrawIndexedInstanced(ID3D12GraphicsCommandList *dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex,
                                        INT BaseVertex);
    static HRESULT ClearRenderTargetView(ID3D11DeviceContext* Context, ID3D11RenderTargetView* RenderTargetView, const FLOAT ColorRGBA[4]);
    void Init() override;
    void UnHook() override;

};