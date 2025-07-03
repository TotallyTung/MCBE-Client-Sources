#pragma once

#include <cstdint>
#include <future>

enum class SigType {
    Sig,
    ReferenceSig
};

#define DEFINE_SIG(name, str, sig_type, offset) \
public: \
static inline void* name; \
private: \
static void name##_initializer() { \
    auto result = scanSig(hat::compile_signature<str>(), #name, offset); \
    if (!result.has_result()) { \
        name = 0; \
        return; \
    } \
    if (sig_type == SigType::Sig) name = reinterpret_cast<void*>((void*)result.get()); \
    else name = reinterpret_cast<void*>((void*)result.rel(offset)); \
} \
static inline std::function<void()> name##_function = (mSigInitializers.emplace_back(name##_initializer), std::function<void()>()); \
public:

class SigManager {
    static hat::scan_result scanSig(hat::signature_view sig, const std::string& name, int offset = 0) {
        mSigScanCount++;

        auto minecraft = hat::process::get_process_module();
        auto result = hat::find_pattern(sig, ".text", minecraft);

        if (!result.has_result()) {
            Console::critical("[signatures] failed to find " + name);
            mSigs[name] = 0;
            return {};
        }
        else {
            Console::debug("[signatures] found " + name);
        }

        if (offset == 0) mSigs[name] = reinterpret_cast<uintptr_t>(result.get());
        else mSigs[name] = reinterpret_cast<uintptr_t>(result.rel(offset));

        return result;
    }

    static inline std::vector<std::function<void()>> mSigInitializers;
    static inline int mSigScanCount;
public:
    static inline bool mIsInitialized = false;
    static inline std::unordered_map<std::string, uintptr_t> mSigs;

    // Sigs:
    DEFINE_SIG(ScreenView_setupAndRender, "48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 98 FD", SigType::Sig, 0);
    DEFINE_SIG(HoverTextRenderer_render, "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 48 81 EC ? ? ? ? 0F 29 70 E8 0F 29 78 D8 44 0F 29 40 ? 49 8B D9 49 8B F8 48 8B F1 48 8B 6A 10", SigType::Sig, 0);
    DEFINE_SIG(ClientInstance_grabMouse, "40 ? 48 83 EC ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 84 C0 74 ? 48 8B ? ? ? ? ? 48 8B ? 48 8B ? ? ? ? ? 48 83 C4 ? 5B 48 FF ? ? ? ? ? 48 83 C4 ? 5B C3 40", SigType::Sig, 0);
    DEFINE_SIG(ClientInstance_releaseMouse, "40 ? 48 83 EC ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 84 C0 74 ? 48 8B ? ? ? ? ? 48 8B ? 48 8B ? ? ? ? ? 48 83 C4 ? 5B 48 FF ? ? ? ? ? 48 83 C4 ? 5B C3 48 89", SigType::Sig, 0);
    DEFINE_SIG(Actor_VTable, "48 8D ? ? ? ? ? 48 89 ? BA ? ? ? ? 44 8D ? ? 44 8D ? ? 66 C7 44 24 20 ? ? E8 ? ? ? ? 48 8B", SigType::Sig, 0);
    DEFINE_SIG(Actor_getNameTag, "48 83 EC 28 48 8B 81 28 01 00 00 48 85 C0 74 4F", SigType::Sig, 0);
    DEFINE_SIG(RakNet_RakPeer_runUpdateCycle, "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 0F 29 B4 24 ? ? ? ? 0F 29 BC 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B EA 48 89 54 24 ? 48 8B D9", SigType::Sig, 0);
    DEFINE_SIG(RakNet_RakPeer_sendImmediate, "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 4C 8B 95 ? ? ? ? 40 32 FF", SigType::Sig, 0);
    DEFINE_SIG(Level_getRuntimeActorList, "48 89 ? ? ? 55 56 57 48 83 EC ? 48 8B ? 48 89 ? ? ? 33 D2", SigType::Sig, 0);
    DEFINE_SIG(Keyboard_feed, "48 83 EC ? ? ? C1 4C 8D 05", SigType::Sig, 0);
    DEFINE_SIG(MouseDevice_feed, "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24", SigType::Sig, 0);
    DEFINE_SIG(RenderController_getHurtColor, "E8 ? ? ? ? 0F 28 05 ? ? ? ? 0F 11 85 68 02 00 00", SigType::Sig, 0);
    DEFINE_SIG(Font_drawTransformed, "48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 55 41 54 41 55 41 56 41 57 48 8D A8 48 FF", SigType::Sig, 0);
    DEFINE_SIG(Mob_getCurrentSwingDuration, "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC 30 8B 41 18 48 8D", SigType::Sig, 0);
    DEFINE_SIG(BlockPalette_getBlock, "40 53 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B DA C7 44 24", SigType::Sig, 0);
    DEFINE_SIG(FillingContainer_dropSlot, "85 D2 0F 88 ? ? ? ? 48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D AC 24", SigType::Sig, 0);
    DEFINE_SIG(ItemStack_getEnchantValue, "48 89 ? ? ? 48 89 ? ? ? 57 48 81 EC ? ? ? ? 48 8B ? 0F B6 ? 33 FF", SigType::Sig, 0);
    DEFINE_SIG(GuiData_displayClientMessage, "40 55 53 56 57 41 56 48 8D AC 24 A0 FE FF FF 48 81 EC 60 02 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 50 01 00 00 41", SigType::Sig, 0);
    DEFINE_SIG(ChatScreenController_addStaticScreenVars, "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 4C 89 74 24 ? 55 48 8B EC 48 83 EC 40 48 8B F2 48 8B F9", SigType::Sig, 0);
    DEFINE_SIG(ActorRenderDispatcher_render, "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F7 4C 89 4C 24 ?", SigType::Sig, 0);
    DEFINE_SIG(ActorAnimationControllerPlayer_applyToPose, "40 53 55 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 48 63 AC 24", SigType::Sig, 0);
    DEFINE_SIG(ChatScreenController_displayClientMessage, "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 2F 48 8B F9 48 83 B9 ? ? ? ? ?", SigType::Sig, 0);
    DEFINE_SIG(MinecraftPackets_createPacket, "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 50 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B D9 48 89 4C 24 ?", SigType::Sig, 0);
    DEFINE_SIG(TextComponent_setText, "48 89 5C 24 ? 48 89 6C 24 ? 57 48 83 EC 30 48 8D 99 ? ? ? ? 48 8B E9 48 8B 43 18 48 8B FA", SigType::Sig, 0);
    DEFINE_SIG(Tessellator_resetTransform, "40 53 48 81 EC ? ? ? ? C6 81 ? ? ? ? ? 48 8B D9 84 D2 74 41 0F 28 05", SigType::Sig, 0); // Tessellator::resetTransform(Tessellator *this, bool setToIdentity)
    DEFINE_SIG(Tessellator_setRotation, "48 89 5C 24 ? 57 48 81 EC ? ? ? ? F3 41 0F 10 00 48 8D 99 ? ? ? ? F3 41 0F 10 50 ? 4C", SigType::Sig, 0); // Tessellator::setRotation(Tessellator *this, float angle, const Vec3 *axis)
    DEFINE_SIG(Tessellator_color, "80 B9 ? ? ? ? ? 4C 8B C1 75", SigType::Sig, 0); // 80 B9 ? ? ? ? ? 4C 8B C1 75 77 F3 0F 10 25 ? ? ? ? F3 0F // Tessellator::color(Tessellator *this, const mce::mColor *c)
    DEFINE_SIG(Tessellator_vertex, "40 57 48 81 EC ? ? ? ? 0F 29 7C ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 8B 81", SigType::Sig, 0); // Tessellator::vertex(Tessellator *this, double x, double y, float z)
    DEFINE_SIG(Tessellator_vertexUV, "40 57 48 81 EC ? ? ? ? 0F 29 7C 24 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 8B 81 ? ? ? ?", SigType::Sig, 0); // Tessellator::vertexUV(Tessellator *this, double x, double y, float z, float u, float v)
    DEFINE_SIG(MeshHelpers_renderMeshImmediately, "E8 ? ? ? ? 41 C6 44 24 38 01 F3 0F 10 1D", SigType::ReferenceSig, 1); // MeshHelpers::renderMeshImmediately(ScreenContext* screenContext, Tessellator* tessellator, const mce::MaterialPtr* material)
    DEFINE_SIG(Tessellator_begin, "40 53 55 48 83 EC 28 80 B9", SigType::Sig, 0); // idk
    DEFINE_SIG(mce_MaterialPtrConst, "48 89 5C 24 ? 48 89 74 24 ? 48 89 4C 24 ? 57 48 83 EC ? 4C 8B CA", SigType::Sig, 0);

    // Xref Sigs: 
    DEFINE_SIG(mce_RenderMaterialGroup_ui, "48 8B 05 ? ? ? ? 48 8D 55 90 48 8D 0D ? ? ? ? 48 8B 40 08 FF 15 ? ? ? ? 48 8B D8", SigType::ReferenceSig, 3);
    DEFINE_SIG(ClientInstance_mBgfx, "48 8B ? ? ? ? ? 48 8D ? ? ? ? ? FF 15 ? ? ? ? 0F B7", SigType::ReferenceSig, 3);
    DEFINE_SIG(NetworkStackItemDescriptor_ctor, "E8 ? ? ? ? 90 48 8B ? 48 8D ? ? ? ? ? E8 ? ? ? ? 4C 8D ? ? ? ? ? 4C 89 ? ? ? ? ? 48 8D", SigType::ReferenceSig, 1);
    DEFINE_SIG(InventoryTransaction_addAction, "E8 ? ? ? ? 49 8B 46 08 48 8B 48 40", SigType::ReferenceSig, 1);
    DEFINE_SIG(ContainerScreenController_tick, "E8 ? ? ? ? 48 8B ? ? ? ? ? 48 8D ? ? ? ? ? 41 B8 ? ? ? ? 8B F8", SigType::ReferenceSig, 1);
    DEFINE_SIG(ContainerScreenController_handleAutoPlace, "E8 ? ? ? ? 66 ? ? ? ? ? ? ? 0F 8C", SigType::ReferenceSig, 1);
    DEFINE_SIG(ItemStack_getCustomName, "48 89 ? ? ? 57 48 83 EC ? 48 8B ? 48 8B ? E8 ? ? ? ? 84 C0 74 ? 48 8B ? 48 8B ? E8 ? ? ? ? 48 8B", SigType::ReferenceSig, 1);
    DEFINE_SIG(GameMode_baseUseItem, "E8 ? ? ? ? 84 C0 74 ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 85", SigType::ReferenceSig, 1);
    DEFINE_SIG(GameMode_getDestroyRate, "E8 ? ? ? ? 0F 28 ? 49 8B ? ? E8", SigType::ReferenceSig, 1);

    // TODO: Identify proper function names for these and refactor them accordingly
    DEFINE_SIG(BobHurt, "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 81 C1", SigType::Sig, 0);
    DEFINE_SIG(TapSwingAnim, "F3 44 ? ? ? ? ? ? ? 4C 8D ? ? ? 48 8B ? 48 C7 44 24 30 ? ? ? ? c7 44 24 ? ? ? ? ? 8b 08", SigType::ReferenceSig, 5); // F3 0F ? ? ? ? ? ? F3 0F ? ? ? ? ? ? 41 0F ? ? F3 0F ? ? ? ? ? ? 0F 28 ? F3 0F ? ? 84 DB
    DEFINE_SIG(FluxSwing, "E8 ? ? ? ? 48 8B ? F3 0F ? ? ? ? ? ? F3 0F ? ? ? ? ? ? F3 0F ? ? ? ? ? ? C6 40 38 ? 48 8B ? EB", SigType::Sig, 0);

    static void initialize() 
    {
        for (int i = 0; i < mSigInitializers.size(); i++) {
            mSigInitializers[i]();
        }

        mIsInitialized = true;
    }

    static void deinitialize() 
    {
        mSigInitializers.clear();
        mSigs.clear();

        mIsInitialized = false;
    }
};