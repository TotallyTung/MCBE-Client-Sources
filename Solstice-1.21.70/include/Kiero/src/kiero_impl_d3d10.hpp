#pragma once

#include "kiero_d3dcommon.hpp"

#include <d3d10_1.h>
#include <d3d10.h>
#include <winrt/base.h>

namespace kiero
{
    template<>
    inline Status initRenderType<RenderType::D3D10>()
    {
        using PFN_CREATE_DXGI_FACTORY = decltype(&::CreateDXGIFactory);
        using PFN_D3D10_CREATE_DEVICE_AND_SWAP_CHAIN = decltype(&::D3D10CreateDeviceAndSwapChain);

        HMODULE libDXGI;
        HMODULE libD3D10;
        if ((libDXGI = ::GetModuleHandle(_T("dxgi.dll"))) == nullptr || (libD3D10 = ::GetModuleHandle(_T("d3d10.dll"))) == nullptr)
        {
            return Status::ModuleNotFoundError;
        }

        const auto CreateDXGIFactory = reinterpret_cast<PFN_CREATE_DXGI_FACTORY>(::GetProcAddress(libDXGI, "CreateDXGIFactory"));
        if (CreateDXGIFactory == nullptr)
        {
            return Status::UnknownError;
        }

        winrt::com_ptr<IDXGIFactory> factory;
        if (CreateDXGIFactory(IID_PPV_ARGS(factory.put())) < 0)
        {
            return Status::UnknownError;
        }

        IDXGIAdapter* adapter;
        if (factory->EnumAdapters(0, &adapter) == DXGI_ERROR_NOT_FOUND)
        {
            return Status::UnknownError;
        }

        const auto D3D10CreateDeviceAndSwapChain = reinterpret_cast<PFN_D3D10_CREATE_DEVICE_AND_SWAP_CHAIN>(::GetProcAddress(libD3D10, "D3D10CreateDeviceAndSwapChain"));
        if (D3D10CreateDeviceAndSwapChain == nullptr)
        {
            return Status::UnknownError;
        }

        const auto window = d3d::createTempWindow();
        DXGI_SWAP_CHAIN_DESC swapChainDesc = d3d::createSwapChainDesc(static_cast<HWND>(window));

        winrt::com_ptr<IDXGISwapChain> swapChain;
        winrt::com_ptr<ID3D10Device> device;

        if (D3D10CreateDeviceAndSwapChain(adapter, D3D10_DRIVER_TYPE_HARDWARE, nullptr, 0, D3D10_SDK_VERSION, &swapChainDesc, swapChain.put(), device.put()) < 0)
        {
            return Status::UnknownError;
        }

        g_methodsTable = static_cast<uintptr_t*>(::calloc(116, sizeof(uintptr_t)));
        ::memcpy(g_methodsTable, *reinterpret_cast<uintptr_t**>(swapChain.get()), 18 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 18, *reinterpret_cast<uintptr_t**>(device.get()), 98 * sizeof(uintptr_t));

        return Status::Success;
    }
}
