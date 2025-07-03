//
// Created by vastrakai on 6/29/2024.
//

#include "bgfx_context.hpp"

#include <d3d12.h>
#include <libhat/Access.hpp>
#include <SDK/OffsetProvider.hpp>
#include <SDK/SigManager.hpp>

ID3D12CommandQueue* bgfx_d3d12_RendererContextD3D12::getCommandQueue()
{
    return hat::member_at<ID3D12CommandQueue*>(this, OffsetProvider::bgfx_d3d12_RendererContextD3D12_m_commandQueue);
}

bgfx_rendercontexti* bgfx_context::getRenderContext()
{
    return hat::member_at<bgfx_rendercontexti*>(this, OffsetProvider::bgfx_context_m_renderCtx);
}

bgfx_context* bgfx_context::get()
{
    return *reinterpret_cast<bgfx_context**>(SigManager::ClientInstance_mBgfx);
}
