#pragma once

namespace bgfx {
    struct CommandBuffer
    {
        unsigned int m_pos;
        unsigned int m_size;
        unsigned __int8 m_buffer[262144];
    };
}