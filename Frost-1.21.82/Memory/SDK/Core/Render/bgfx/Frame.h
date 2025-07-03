#pragma once

namespace bgfx {
    struct Frame 
    {
        CLASS_FIELD(int, m_debug, 0x061CD7D8 - 0x4);
        CLASS_FIELD(bgfx::CommandBuffer, m_cmdPre, 0x061CD7D8);
        CLASS_FIELD(bgfx::CommandBuffer, m_cmdPost, 0x620D7E0);
    };
    // Size: 0x2DEE8C0
}