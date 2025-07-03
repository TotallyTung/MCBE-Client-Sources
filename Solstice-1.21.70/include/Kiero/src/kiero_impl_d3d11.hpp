#pragma once

#include "kiero_d3dcommon.hpp"

#include <d3d11.h>
#include <winrt/base.h>

namespace kiero
{
    template<>
    inline Status initRenderType<RenderType::D3D11>()
    {
        HMODULE libD3D11;
        if ((libD3D11 = ::GetModuleHandle(_T("d3d11.dll"))) == nullptr)
        {
            return Status::ModuleNotFoundError;
        }

        const auto D3D11CreateDeviceAndSwapChain = reinterpret_cast<PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN>(::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain"));
        if (D3D11CreateDeviceAndSwapChain == nullptr)
        {
            return Status::UnknownError;
        }

        D3D_FEATURE_LEVEL featureLevel;
        constexpr D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

        const auto window = d3d::createTempWindow();
        const DXGI_SWAP_CHAIN_DESC swapChainDesc = d3d::createSwapChainDesc(static_cast<HWND>(window));

        winrt::com_ptr<IDXGISwapChain> swapChain;
        winrt::com_ptr<ID3D11Device> device;
        winrt::com_ptr<ID3D11DeviceContext> context;

        if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, swapChain.put(), device.put(), &featureLevel, context.put()) < 0)
        {
            return Status::UnknownError;
        }

        g_methodsTable = static_cast<uintptr_t*>(::calloc(205, sizeof(uintptr_t)));
        ::memcpy(g_methodsTable, *reinterpret_cast<uintptr_t**>(swapChain.get()), 18 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 18, *reinterpret_cast<uintptr_t**>(device.get()), 43 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 18 + 43, *reinterpret_cast<uintptr_t**>(context.get()), 144 * sizeof(uintptr_t));

        return Status::Success;
    }
}
