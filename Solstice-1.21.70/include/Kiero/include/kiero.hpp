#pragma once

#ifndef KIERO_HPP
#define KIERO_HPP

#include <cstdint>

namespace kiero
{
    enum class Status
    {
        UnknownError = -1,
        NotSupportedError = -2,
        ModuleNotFoundError = -3,

        AlreadyInitializedError = -4,
        NotInitializedError = -5,

        Success = 0,
    };

    enum class RenderType
    {
        None,

        D3D9,
        D3D10,
        D3D11,
        D3D12,

        OpenGL,
        Vulkan,

        Auto
    };

    Status init(RenderType renderType);
    void shutdown();

    Status bind(uint16_t index, void** original, void* function);
    void unbind(uint16_t index);

    RenderType getRenderType();
    uintptr_t* getMethodsTable();
}

#endif // KIERO_HPP
