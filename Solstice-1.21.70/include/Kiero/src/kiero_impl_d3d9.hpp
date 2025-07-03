#pragma once

#include "kiero_d3dcommon.hpp"

#include <d3d9.h>
#include <winrt/base.h>

namespace kiero
{
    template<>
    inline Status initRenderType<RenderType::D3D9>()
    {
        using PFN_DIRECT3D_CREATE9 = decltype(&::Direct3DCreate9);

        HMODULE libD3D9;
        if ((libD3D9 = ::GetModuleHandle(_TEXT("d3d9.dll"))) == nullptr)
        {
            return Status::ModuleNotFoundError;
        }

        const auto Direct3DCreate9 = reinterpret_cast<PFN_DIRECT3D_CREATE9>(::GetProcAddress(libD3D9, "Direct3DCreate9"));
        if (Direct3DCreate9 == nullptr)
        {
            return Status::UnknownError;
        }

        winrt::com_ptr<IDirect3D9> direct3D9{Direct3DCreate9(D3D_SDK_VERSION), winrt::take_ownership_from_abi};
        if (direct3D9 == nullptr)
        {
            return Status::UnknownError;
        }

        const auto window = d3d::createTempWindow();

        D3DPRESENT_PARAMETERS params;
        params.BackBufferWidth = 0;
        params.BackBufferHeight = 0;
        params.BackBufferFormat = D3DFMT_UNKNOWN;
        params.BackBufferCount = 0;
        params.MultiSampleType = D3DMULTISAMPLE_NONE;
        params.MultiSampleQuality = NULL;
        params.SwapEffect = D3DSWAPEFFECT_DISCARD;
        params.hDeviceWindow = static_cast<HWND>(window);
        params.Windowed = 1;
        params.EnableAutoDepthStencil = 0;
        params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
        params.Flags = NULL;
        params.FullScreen_RefreshRateInHz = 0;
        params.PresentationInterval = 0;

        winrt::com_ptr<IDirect3DDevice9> device;
        if (direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, params.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &params, device.put()) < 0)
        {
            return Status::UnknownError;
        }

        g_methodsTable = static_cast<uintptr_t*>(::calloc(119, sizeof(uintptr_t)));
        ::memcpy(g_methodsTable, *reinterpret_cast<uintptr_t**>(device.get()), 119 * sizeof(uintptr_t));

        return Status::Success;
    }
}
