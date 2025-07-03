#pragma once
#include <d3d12.h>
//
// Created by vastrakai on 6/29/2024.
//

struct bgfx_rendercontexti
{

};

struct bgfx_d3d12_RendererContextD3D12 : bgfx_rendercontexti {
    ID3D12CommandQueue* getCommandQueue();
};

class bgfx_context {
public:
    bgfx_rendercontexti* getRenderContext();
    static bgfx_context* get();
};