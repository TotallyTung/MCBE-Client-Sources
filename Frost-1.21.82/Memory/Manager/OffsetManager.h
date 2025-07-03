#pragma once

#include "Memory.h"

enum class OffsetType {
    Index,
    FieldOffset,
};

#define DEFINE_INDEX_FIELD_TYPED(type, name, str, index_offset, offset_type) \
public: \
static inline type (name) = 0; \
private: \
static void name##_initializer() { \
    static bool initialized = false; \
    if (initialized) return; \
    initialized = true; \
    auto result = scanSig(hat::compile_signature<str>(), #name, index_offset); \
    if (!result.has_result()) return; \
    if ((offset_type) == OffsetType::Index) name = *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(result.get()) + index_offset) / 8; \
    else (name) = *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(result.get()) + index_offset); \
} \
private: \
static inline std::function<void()> name##_function = (mSigInitializers.emplace_back(name##_initializer), std::function<void()>()); \
public:

#define DEFINE_INDEX_FIELD(name, str, index_offset, offset_type) \
public: \
static inline int (name) = 0; \
private: \
static void name##_initializer() { \
    static bool initialized = false; \
    if (initialized) return; \
    initialized = true; \
    auto result = scanSig(hat::compile_signature<str>(), #name, index_offset); \
    if (!result.has_result()) return; \
    if ((offset_type) == OffsetType::Index) (name) = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(result.get()) + (index_offset)) / 8; \
    else (name) = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(result.get()) + (index_offset)); \
} \
private: \
static inline std::function<void()> name##_function = (mSigInitializers.emplace_back(name##_initializer), std::function<void()>()); \
public:

// Defines a direct offset for a field (for offsets/indexes that can't be sigged)
#define DEFINE_FIELD(name, offset) \
public: \
static constexpr inline int (name) = offset;

#include <future>

class OffsetManager {
    static hat::scan_result scanSig(hat::signature_view sig, const std::string& name, int offset = 0) {
        mSigScanCount++;

        auto minecraft = hat::process::get_process_module();
        auto result = hat::find_pattern(sig, ".text", minecraft);

        if (!result.has_result()) {
            Console::critical("[offsets] Failed to find " + name);
            mSigs[name] = 0;
            return {};
        }
        else {
            Console::debug("[offsets] found " + name);
        }

        mSigs[name] = reinterpret_cast<uintptr_t>(result.get()) + offset;
        return result;
    }

    static inline std::vector<std::function<void()>> mSigInitializers;
    static inline int mSigScanCount;
public:
    static inline bool mIsInitialized = false;
    static inline std::unordered_map<std::string, uintptr_t> mSigs;

    DEFINE_INDEX_FIELD(ClientInstance_getLocalPlayer, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 8B 88 ? ? ? ? C1 E1", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(ClientInstance_getOptions, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? 48 8B ? FF 15 ? ? ? ? 3C ? 75 ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 3C", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(ClientInstance_isMouseGrabbed, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 84 C0 75 ? 48 8B ? ? ? ? ? 48 85 ? 0F 84 ? ? ? ? 80 38 ? 0F 84 ? ? ? ? 48 8B ? ? ? ? ? 48 85 ? 74 ? F0 FF ? ? 0F 10 ? ? ? ? ? 0F 11 ? ? ? ? ? F2 0F", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(Actor_swing, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? ? ? ? ? 48 8B ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 48 8B", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(Level_getHitResult, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? 49 8B ? ? 48 8B ? ? ? ? ? 49 8B ? FF 15 ? ? ? ? F3 0F", 3, OffsetType::Index);

    DEFINE_INDEX_FIELD(Actor_mSwinging, "88 ? ? ? ? ? EB ? 33 ? 89", 2, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(bgfx_d3d12_RendererContextD3D12_m_commandQueue, "48 8b 89 ? ? ? ? 4c 8b 46", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(bgfx_context_m_renderCtx, "48 8B ? ? ? ? ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 48 89 ? ? ? ? ? 48 63", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(BlockLegacy_mBlockId, "44 0F ? ? ? ? ? ? B8 ? ? ? ? 48 8B ? 48 8B", 4, OffsetType::FieldOffset);

    DEFINE_FIELD(MinecraftUIRenderContext_drawText, 5);
    DEFINE_FIELD(MinecraftUIRenderContext_drawImage, 7);
    DEFINE_FIELD(MinecraftUIRenderContext_drawNineSlice, 8);

    DEFINE_FIELD(Actor_mLevel, 0x1D8);
    DEFINE_FIELD(Actor_mEntityIdentifier, 0x240);
    DEFINE_FIELD(Player_getGameMode, 0xA88);
    DEFINE_FIELD(Player_getSkin, 0xAE8);
    DEFINE_FIELD(Player_getSupplies, 0x5C8);
    DEFINE_FIELD(Player_getContainerManager, 0x5B0);
    DEFINE_FIELD(ClientInstance_mViewMatrix, 0x388);
    DEFINE_FIELD(ClientInstance_mFovX, 0x748);
    DEFINE_FIELD(ClientInstance_mFovY, 0x75C);
    DEFINE_FIELD(ClientInstance_mGuiData, 0x5B8);

    DEFINE_FIELD(Block_mLegacy, 0x78);
    DEFINE_FIELD(LevelRenderer_mRendererPlayer, 0x328);
    DEFINE_FIELD(LevelRendererPlayer_mCameraPos, 0x65C);

    static void initialize() {
        for (int i = 0; i < mSigInitializers.size(); i++)
        {
            mSigInitializers[i]();
        }

        mIsInitialized = true;
    }

    static void deinitialize() {
        mSigInitializers.clear();
        mSigs.clear();

        mIsInitialized = false;
    }
};
