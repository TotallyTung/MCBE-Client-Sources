#pragma once
#include <Utils/SysUtils/xorstr.hpp>
//
// Created by vastrakai on 6/25/2024.
//

enum class OffsetType {
    Index,
    FieldOffset,
};

// kinda aids ik stfu
#define DEFINE_INDEX_FIELD_TYPED(type, name, str, index_offset, offset_type) \
public: \
static inline type (name) = 0; \
private: \
static void name##_initializer() { \
    static bool initialized = false; \
    if (initialized) return; \
    initialized = true; \
    auto result = scanSig(hat::compile_signature<str>(), xorstr_(#name), index_offset); \
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
    auto result = scanSig(hat::compile_signature<str>(), xorstr_(#name), index_offset); \
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
#include <libhat/Scanner.hpp>

class OffsetProvider {
    static hat::scan_result scanSig(hat::signature_view sig, const std::string& name, int offset = 0);

    static inline std::vector<std::function<void()>> mSigInitializers;
    static inline int mSigScanCount;
public:
    static inline bool mIsInitialized = false;
    static inline std::unordered_map<std::string, uintptr_t> mSigs;

    DEFINE_INDEX_FIELD(ClientInstance_getLocalPlayer, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 8B 88 ? ? ? ? C1 E1", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(ClientInstance_getBlockSource, "48 8b 80 ? ? ? ? ff 15 ? ? ? ? 48 8b c8 48 85 c0 75 ? 48 83 c4", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(ClientInstance_getOptions, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? 48 8B ? FF 15 ? ? ? ? 3C ? 75 ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 3C", 3, OffsetType::Index);
    //DEFINE_INDEX_FIELD(ClientInstance_getScreenName, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 90 48 85 ? 74 ? 48 8B ? E8 ? ? ? ? 90 48 89 ? ? ? 48 8B ? ? ? 48 89 ? ? ? 48 85 ? 74 ? E8 ? ? ? ? 48 8B ? ? ? 48 85 ? 74 ? 48 8B ? E8 ? ? ? ? 48 8B ? 48 8B ? ? ? 48 8B ? ? ? 48 83 C4 ? 5F C3 E8 ? ? ? ? 90 CC CC CC CC CC 48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 48 8B ? ? 48 83 C1", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(ClientInstance_getMouseGrabbed, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 84 C0 75 ? 48 8B ? ? ? ? ? 48 85 ? 0F 84 ? ? ? ? 80 38 ? 0F 84 ? ? ? ? 48 8B ? ? ? ? ? 48 85 ? 74 ? F0 FF ? ? 0F 10 ? ? ? ? ? 0F 11 ? ? ? ? ? F2 0F", 3, OffsetType::Index); // PLEASE CHECK AFTER IDK IF THIS IS CORRECT!!!
    DEFINE_INDEX_FIELD(ClientInstance_setDisableInput, "48 8B ? ? ? ? ? 48 8B ? FF 15 ? ? ? ? 48 85 ? 74 ? 48 8B ? 48 8B ? 48 8B", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(ClientInstance_grabMouse, "48 8B ? ? ? ? ? 48 8B ? FF 15 ? ? ? ? 48 85 ? 74 ? 48 8B ? 48 8B ? 48 8B", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(ClientInstance_releaseMouse, "48 8B ? ? ? ? ? 48 8B ? FF 15 ? ? ? ? 48 85 ? 74 ? 48 8B ? 48 8B ? 48 8B", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(ClientInstance_mMinecraftSim, "48 8B ? ? ? ? ? E8 ? ? ? ? 88 87 ? ? ? ? 48 8B ? ? ? ? ? 48 8B", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(ClientInstance_mLevelRenderer, "48 8B ? ? ? ? ? 48 85 ? 74 ? 48 8B ? ? ? ? ? 48 05 ? ? ? ? C3", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(ClientInstance_mPacketSender, "48 8B ? ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8B ? ? ? ? ? 48 8B ? 48 8B", 3, OffsetType::FieldOffset);
    //DEFINE_INDEX_FIELD(ClientInstance_mGuiData, "48 8B ? ? ? ? ? 48 8B ? 48 85 ? 74 ? 33 C9 48 89 ? 48 89 ? ? 48 8B ? ? 48 85 ? 74 ? F0 FF ? ? 48 8B ? ? 48 8B ? ? 48 8B ? ? 48 89 ? 48 89 ? ? 48 85 ? 74 ? E8 ? ? ? ? 48 8B ? 48 85 ? 74 ? 48 83 38 ? 74 ? 48 8B ? 48 83 C4 ? 5B C3 E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC 48 8B ? ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 89 ? ? ? 57", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(MinecraftGame_playUi, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 83 C4 ? C3 48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 48 8B", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(MinecraftGame_mClientInstances, "48 8B ? ? ? ? ? 48 8B ? 48 8B ? ? 80 78 19 ? 75 ? 48 8B ? 48 8B ? 80 78 19 ? 74 ? 80 79 19 ? 75 ? 80 79 20 ? 76 ? 48 8B ? 48 3B", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(MinecraftGame_mProfanityContext, "49 8B ? ? ? ? ? ? 49 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8D ? ? E8", 4, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(MinecraftGame_mMouseGrabbed, "80 B9 ? ? ? ? ? 0F 84 ? ? ? ? 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 85 C0", 2, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(MainView_bedrockPlatform, "48 8B ? ? ? ? ? 48 8B ? B2 ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 84 C0", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(Actor_baseTick, "48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8B 8B ? ? ? ? 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 45 84 ED", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(MinecraftSim_mGameSim, "49 8b 85 ? ? ? ? 8b 58", 3, OffsetType::FieldOffset);
    //DEFINE_INDEX_FIELD(MinecraftSim_mRenderSim, "48 8b 4d ? f3 0f 10 51 ? 48 8b 80", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(MinecraftSim_mGameSession, "48 8B ? ? ? ? ? 48 85 ? 0F 84 ? ? ? ? 44 38 ? ? 0F 84 ? ? ? ? 48 8B", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD_TYPED(uint8_t, GameSession_mEventCallback, "48 8B ? ? 48 85 ? 74 ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 48 8B ? ? 48 85 ? 74 ? 48 8B ? 48 8B ? ? 48 83 C4 ? 5B 48 FF ? ? ? ? ? 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC CC 48 89", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD_TYPED(uint8_t, BedrockPlatformUWP_mcGame, "48 8B ? ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 84 C0 74 ? 48 8B ? ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 84 C0", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(bgfx_d3d12_RendererContextD3D12_m_commandQueue, "48 8B ? ? ? ? ? 4C 8B ? ? 48 8B ? 48 8B ? ? 48 8B ? ? FF 15 ? ? ? ? 48 FF ? ? 48 8B ? ? 48 8B ? 48 3B ? 74 ? 48 8B", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(bgfx_context_m_renderCtx, "48 8B ? ? ? ? ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 48 89 ? ? ? ? ? 48 63", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(BlockLegacy_mBlockId, "44 0F ? ? ? ? ? ? B8 ? ? ? ? 48 8B ? 48 8B", 4, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(BlockLegacy_mayPlaceOn, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 49 8D ? ? 0F B6 ? E8 ? ? ? ? 48 85 ? 75 ? 49 8B ? ? 48 85 ? 0F 84 ? ? ? ? 48 83 C1 ? E8 ? ? ? ? 48 85 ? 74 ? 45 0F", 3, OffsetType::Index); // 1.21.51
    DEFINE_INDEX_FIELD(Actor_mContainerManagerModel, "49 8B ? ? ? ? ? 48 89 ? ? ? 48 89 ? ? ? F0 FF ? ? 66 0F ? ? ? ? EB ? 0F 57 ? 66 0F 73 D8 ? 66 48 ? ? ? 0F 57 ? 66 0F ? ? ? ? ? ? 48 85", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(Actor_mGameMode, "49 8B 8E ? ? ? ? 45 84 FF 74 ? 84 DB", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(Actor_mSupplies, "48 8B ? ? ? ? ? 80 BA B0 00 00 00 ? 75 ? 48 8B ? ? ? ? ? 8B 52 ? 48 8B ? 48 8B ? ? 48 FF ? ? ? ? ? 48 8D ? ? ? ? ? C3 48 89", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(PlayerInventory_mContainer, "48 8B ? ? ? ? ? 8B 52 ? 48 8B ? 48 8B ? ? 48 FF ? ? ? ? ? 48 8D", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD_TYPED(uint8_t, Container_getItem, "48 8B ? ? FF 15 ? ? ? ? 4C 8B ? 41 80 7E 23", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(LevelRenderer_mRendererPlayer, "48 8B ? ? ? ? ? F3 0F ? ? ? ? ? ? 0F AF", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(LevelRendererPlayer_mCameraPos, "f3 0f 10 8b ? ? ? ? f3 0f 5c 83 ? ? ? ? f3 0f 10 93", 4, OffsetType::FieldOffset); // 1.21.51
    //DEFINE_INDEX_FIELD(LevelRendererPlayer_mFovX, "0F 10 ? ? ? ? ? 0F 11 ? ? 0F 10 ? ? ? ? ? 0F 11 ? ? 0F 10 ? ? ? ? ? 0F 11 ? ? 0F 10 ? ? ? ? ? 0F 11 ? ? 80 B9 58 0F 00 00", 3, OffsetType::FieldOffset); // 1.21.51
    //DEFINE_INDEX_FIELD(LevelRendererPlayer_mFovY, "89 87 ? ? ? ? 41 8B ? ? ? 89 87 ? ? ? ? 41 8B ? ? ? 89 87 ? ? ? ? 41 8B ? ? ? 89 87 ? ? ? ? 41 8B ? ? ? 89 87 ? ? ? ? 41 8B ? ? ? 89 87 ? ? ? ? 41 8B ? ? ? F3 0F", 2, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(Actor_mSwinging, "88 ? ? ? ? ? EB ? 33 ? 89", 2, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(Actor_swing, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? ? ? ? ? 48 8B ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 48 8B", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(Actor_mLevel, "48 8B ? ? ? ? ? 48 89 ? ? ? F3 0F ? ? ? 4C 8B", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(Level_getHitResult, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? 49 8B ? ? 48 8B ? ? ? ? ? 49 8B ? FF 15 ? ? ? ? F3 0F", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(Level_getPlayerList, "48 8B 80 ? ? ? ? FF 15 ? ? ? ? 8B 48 ? 39 8E ? ? ? ? 0F 84", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(Level_getPlayerMovementSettings, "48 8b 82 ? ? ? ? ff 15 ? ? ? ? 80 78 ? ? 0f 84", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(Level_getLevelData, "48 8b 80 ? ? ? ? ff 15 ? ? ? ? 4c 8d 90", 3, OffsetType::Index); // Also works on .40
    DEFINE_INDEX_FIELD(LevelData_mTick, "48 8b 98 ? ? ? ? 48 8b b5", 3, OffsetType::FieldOffset);
    //DEFINE_INDEX_FIELD(Actor_mDestroying, "38 9f ? ? ? ? 74 ? 48 8b 8f ? ? ? ? 48 8b 01", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD_TYPED(uint8_t, UIProfanityContext_mEnabled, "80 79 ? ? 74 ? 80 79 ? ? 74 ? 45 ? ? 75 ? 4C 8D", 2, OffsetType::FieldOffset);
    //DEFINE_INDEX_FIELD(Bone_mPartModel, "8B 81 ? ? ? ? 89 82 ? ? ? ? F3 0F ? ? ? ? ? ? 0F 57", 2, OffsetType::FieldOffset); // this isn't even correct lmao
    DEFINE_INDEX_FIELD(Actor_mHurtTimeComponent, "48 8b 86 ? ? ? ? 48 85 c0 74 ? 66 0f 6e 00", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD(BlockSource_getChunk, "48 8B ? ? ? ? ? 44 89 ? ? ? FF 15 ? ? ? ? 48 83 C4", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(BlockSource_setBlock, "48 8B ? ? ? ? ? FF 15 ? ? ? ? EB ? 41 83 FF", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(LevelChunk_mSubChunks, "48 2B ? ? ? ? ? 48 8B ? 48 C1 FF ? 48 0F", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD_TYPED(uint8_t, BlockSource_mBuildHeight, "0F BF ? ? 3B C8 0F 8D ? ? ? ? 8B 0F", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD_TYPED(uint8_t, ContainerManagerModel_getContainerType, "48 8B ? ? FF 15 ? ? ? ? 84 C0 75 ? 48 8B ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 84 C0 74 ? B0", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD_TYPED(uint8_t, ContainerManagerModel_getSlot, "48 8B ? ? FF 15 ? ? ? ? 0F B6 ? ? 44 89", 3, OffsetType::Index); // what the actual fcuk is wrong with me.
    DEFINE_INDEX_FIELD(Actor_mSerializedSkin, "4C 8B ? ? ? ? ? 48 8B ? 48 8B ? ? 89 44", 3, OffsetType::FieldOffset);
    DEFINE_INDEX_FIELD_TYPED(uint8_t, BlockLegacy_getCollisionShape, "48 8B ? ? FF 15 ? ? ? ? 0F 10 ? 0F 11 ? ? ? ? ? ? F2 0F ? ? ? F2 0F", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(BlockSource_clip, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 90 48 8D ? ? ? ? ? 48 8D ? ? ? ? ? E8 ? ? ? ? 48 8B", 3, OffsetType::Index);

    DEFINE_INDEX_FIELD(ClientInstance_getInputHandler, "48 8B ? ? ? ? ? FF 15 ? ? ? ? 8D 7E", 3, OffsetType::Index);
    DEFINE_INDEX_FIELD(ClientInputMappingFactory_mKeyboardMouseSettings, "49 8B 9D ? ? ? ? 48 89 5D ? 4D 8B AD ? ? ? ? 4C 89 6D ? E9", 3, OffsetType::FieldOffset); // TODO: replace with betta siggy

    //DEFINE_INDEX_FIELD_TYPED(uint8_t, ClientInputHandler_mMappingFactory, "48 8B ? ? EB ? 48 8D ? ? 48 85 ? 74 ? 48 8B ? E8 ? ? ? ? 48 8B ? 48 8B ? ? ? 48 33 ? E8 ? ? ? ? 48 8B ? ? ? 48 8B ? ? ? 48 83 C4 ? 5F C3 E8 ? ? ? ? CC CC CC CC CC CC 48 89", 3, OffsetType::FieldOffset);
    DEFINE_FIELD(ClientInputHandler_mMappingFactory, 0x28);

    // TODO: Move all CLASS_FIELD declarations and hat::member_at offsets to here
    DEFINE_FIELD(ContainerScreenController_tryExit, 12);
    DEFINE_FIELD(ItemStack_reInit, 3);
    DEFINE_FIELD(MinecraftUIRenderContext_drawImage, 7);
    DEFINE_FIELD(Actor_mEntityIdentifier, 0x240);
    DEFINE_FIELD(Actor_mLocalName, 0xCB0);
    DEFINE_FIELD(ClientInstance_mViewMatrix, 0x388);
    DEFINE_FIELD(ClientInstance_mFovX, 0x748); // 1.21.70
    DEFINE_FIELD(ClientInstance_mFovY, 0x75C); // 1.21.70
    DEFINE_FIELD(Block_mRuntimeId, 0xC0);
    DEFINE_FIELD(Block_mLegacy, 0x78); // 1.21.51
    DEFINE_FIELD(ClientInstance_getScreenName, 271); // i will make sig for it later (maybe)
    DEFINE_FIELD(ClientInstance_mGuiData, 0x5B0); // i will make sig for it later (maybe)
    DEFINE_FIELD(BlockPalette_mLevel, 0x80);
    DEFINE_FIELD(Actor_mDestroying, 2869);
    DEFINE_FIELD(MinecraftSim_mRenderSim, 0xE8);



    static void initialize();
    static void deinitialize();
};
