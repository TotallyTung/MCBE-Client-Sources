#pragma once

#include "kiero_d3dcommon.hpp"

#include <d3d12.h>
#include <winrt/base.h>

namespace kiero
{
    template<>
    inline Status initRenderType<RenderType::D3D12>()
    {
        using PFN_CREATE_DXGI_FACTORY = decltype(&::CreateDXGIFactory);

        HMODULE libDXGI;
        HMODULE libD3D12;
        if ((libDXGI = ::GetModuleHandle(_T("dxgi.dll"))) == nullptr || (libD3D12 = ::GetModuleHandle(_T("d3d12.dll"))) == nullptr)
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

        const auto D3D12CreateDevice = reinterpret_cast<PFN_D3D12_CREATE_DEVICE>(::GetProcAddress(libD3D12, "D3D12CreateDevice"));
        if (D3D12CreateDevice == nullptr)
        {
            return Status::UnknownError;
        }

        winrt::com_ptr<ID3D12Device> device;
        if (D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(device.put())) < 0)
        {
            return Status::UnknownError;
        }

        D3D12_COMMAND_QUEUE_DESC queueDesc;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.Priority = 0;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 0;

        winrt::com_ptr<ID3D12CommandQueue> commandQueue;
        if (device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(commandQueue.put())) < 0)
        {
            return Status::UnknownError;
        }

        winrt::com_ptr<ID3D12CommandAllocator> commandAllocator;
        if (device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.put())) < 0)
        {
            return Status::UnknownError;
        }

        winrt::com_ptr<ID3D12GraphicsCommandList> commandList;
        if (device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.get(), nullptr, IID_PPV_ARGS(commandList.put())) < 0)
        {
            return Status::UnknownError;
        }

        const auto window = d3d::createTempWindow();
        DXGI_SWAP_CHAIN_DESC swapChainDesc = d3d::createSwapChainDesc(static_cast<HWND>(window));

        winrt::com_ptr<IDXGISwapChain> swapChain;
        if (factory->CreateSwapChain(commandQueue.get(), &swapChainDesc, swapChain.put()) < 0)
        {
            return Status::UnknownError;
        }

        g_methodsTable = static_cast<uintptr_t*>(::calloc(150, sizeof(uintptr_t)));
        ::memcpy(g_methodsTable, *reinterpret_cast<uintptr_t**>(device.get()), 44 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44, *reinterpret_cast<uintptr_t**>(commandQueue.get()), 19 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44 + 19, *reinterpret_cast<uintptr_t**>(commandAllocator.get()), 9 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44 + 19 + 9, *reinterpret_cast<uintptr_t**>(commandList.get()), 60 * sizeof(uintptr_t));
        ::memcpy(g_methodsTable + 44 + 19 + 9 + 60, *reinterpret_cast<uintptr_t**>(swapChain.get()), 18 * sizeof(uintptr_t));

        return Status::Success;
    }
}
