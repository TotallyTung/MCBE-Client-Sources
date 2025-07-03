#pragma once

namespace bgfx {
    class Context 
    {
    public:
        CLASS_FIELD(bgfx::Frame*, m_render, 0x0C4F6680);
        CLASS_FIELD(uintptr_t, m_clearQuad, 0x0C4F6680);
        CLASS_FIELD(uintptr_t, m_textVideoMemBlitter, 0x0C6C07E0);
        CLASS_FIELD(bgfx::RendererContextI*, m_renderCtx, 0x0C6D29D0);
        CLASS_FIELD(int, m_debug, 0x05D9B64C);
        CLASS_FIELD(bool, m_rendererInitialized, 0x5D9B758);
        CLASS_FIELD(bool, m_exit, 0x0C6C086F);
        CLASS_FIELD(bool, m_flipAfterRender, 0x0C6C086C);
        CLASS_FIELD(bool, m_singleThreaded, 0x0C6C0871);
        CLASS_FIELD(bool, m_flipped, 0x0C6C0871 + 0x1);

    public:
        bgfx::RendererContextI* getRenderContext() {
            return hat::member_at<bgfx::RendererContextI*>(this, OffsetManager::bgfx_context_m_renderCtx);
        }

        static bgfx::Context* get() {
            return *reinterpret_cast<bgfx::Context**>(SigManager::ClientInstance_mBgfx);
        }
    };
}