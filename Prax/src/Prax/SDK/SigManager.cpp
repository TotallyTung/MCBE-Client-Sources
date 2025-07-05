//
// Created by vastrakai on 9/12/2023.
//

#include <Prax/Utils/System/Memory.h>
#include <Prax/Utils/Misc.h>
#include <Prax/Utils/String.h>
#include <xorstr.hpp>
#include "SigManager.h"
#include <future>
#include <magic_enum_all.hpp>

#define DEFINE(name, str) const hat::signature_view name = ([]() {  \
    static constexpr auto sig = hat::compile_signature<str>();      \
    return hat::signature_view{sig};                                \
})();

DEFINE(Signatures::Reach, "F3 0F ? ? ? ? ? ? 44 0F ? ? 76 ? C6 44 24 64");
DEFINE(Signatures::MobEquipmentPacket_MobEquipmentPacket, "E8 ? ? ? ? 90 48 8B ? E8 ? ? ? ? 45 33 ? 4C 8D ? ? ? ? ? 48 8B ? 48 8B ? E8 ? ? ? ? 45 8D"); // MobEquipmentPacket::MobEquipmentPacket
DEFINE(Signatures::SplashTextRenderer_render, "48 8B ? 55 53 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 44 0F ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 4D 8B ? 4D 8B ? 48 89"); // SplashTextRenderer::render
DEFINE(Signatures::MinecraftPackets_createPacket, "E8 ? ? ? ? 90 48 83 BD ? ? 00 00 ? 0F 84 ? ? ? ? ff"); // MinecraftPackets::createPacket. alternative sig:
DEFINE(Signatures::inventory_dropSlot, "85 D2 0F 88 ? ? ? ? 48 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? ? 48 81 EC"); // inventory::dropSlot
DEFINE(Signatures::RakNet_RakPeer_runUpdateCycle, "48 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B ? 0F 29 ? ? ? ? ? ? 0F 29 ? ? ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 4C 8B ? 48 89 ? ? 4C 8B ? 48 89"); // RakNet::RakPeer::runUpdateCycle
DEFINE(Signatures::Gamemode_getDestroyRate, "48 89 ? ? ? 57 48 83 EC ? 48 8B ? 0F 29 ? ? ? 48 8B ? ? E8"); // Gamemode::getDestroyRate
DEFINE(Signatures::ActorRenderDispatcher_render, "48 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? 4D 8B ? 49 8B ? 48 8B ? 4C 8B ? 41 8B"); // ActorRenderDispatcher::render
DEFINE(Signatures::ContainerScreenController_tick, "E8 ? ? ? ? 48 8B 8B ? ? ? ? 48 8D 93 ? ? ? ? 41 B8 ? ? ? ? 8B F8"); // ContainerScreenControllerHook
DEFINE(Signatures::Actor_shouldRenderNametagBp, "0F 84 ? ? ? ? 4C 8B ? ? 4D 3B ? ? ? ? ? ? ? 49 8B"); // Actor::shouldRenderNametagBp
DEFINE(Signatures::Actor_shouldRenderNametag, "0F 84 ? ? ? ? 49 8B ? 48 8B ? E8 ? ? ? ? 84 C0 ? ? ? ? ? ? 49 8D"); // Actor::shouldRenderNametag
DEFINE(Signatures::renderCtxAddr, "E8 ? ? ? ? 48 8B 44 24 ? 48 8D 4C 24 ? 48 8B 80"); // Setupandrender
DEFINE(Signatures::keymap, "E8 ? ? ? ? 33 D2 0F B6 CB"); // keymap
DEFINE(Signatures::CameraClipFunc, "40 ? 56 57 48 81 EC ? ? ? ? 44 0F"); // CameraClipFunc
DEFINE(Signatures::Mouse, "E8 ? ? ? ? 40 88 6C 1F"); // Mouse
DEFINE(Signatures::Gamemode_baseUseItem, "E8 ? ? ? ? 84 C0 74 ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 85"); // Gamemode::baseUseItem
DEFINE(Signatures::Mob_getCurrentSwingDuration, "E8 ? ? ? ? 80 BB ? ? ? ? ? 74 ? 8b ? ? ? ? ? ff"); // Mob::getCurrentSwingDuration
DEFINE(Signatures::bobHurt, "40 53 56 48 83 EC 78 ? ? 7C"); // bobHurt thing
DEFINE(Signatures::fluxSwing, "E8 ? ? ? ? F3 0F ? ? ? ? ? ? F3 0F ? ? ? ? ? ? 48 8B ? F3 0F ? ? 48 8B"); // flux swing
DEFINE(Signatures::Mob_getJumpComponent, "E8 ? ? ? ? 48 85 C0 74 ? C6 40 ? ? 48 83 C4 ? 5B"); // Mob::getJumpComponent (alternative sig: "40 53 48 83 EC ? 48 8B DA BA 15 CF 00 41")
DEFINE(Signatures::ItemStack_getCustomName, "E8 ? ? ? ? 48 8D ? ? ? ? ? 48 8B ? E8 ? ? ? ? 48 8B ? ? ? 48 83 FA ? 0F 82"); // ItemStack::getCustomName
DEFINE(Signatures::ContainerScreenController_handleAutoPlace, "E8 ? ? ? ? 66 ? ? ? ? ? ? ? 0F 8C"); // ContainerScreenController::handleAutoPlace
DEFINE(Signatures::ItemUseSlowdownSystem_isSlowedByItemUse, "40 ? 48 83 EC ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? 48 8B ? 4D 85 ? 74 ? 8B 01 48 8D ? ? ? 49 8D ? ? 48 89")
DEFINE(Signatures::AppPlatform_getDeviceId, "48 89 ? ? ? 48 89 ? ? ? 56 57 41 ? 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? 48 8B ? 48 89 ? ? ? 33 ED")
DEFINE(Signatures::ConnectionRequest_create, "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B D9 48 89 55")
DEFINE(Signatures::TapSwingAnim, "F3 44 ? ? ? ? ? ? ? 41 0F ? ? F3 0F ? ? ? ? C6 40 38 ? 48 8B ? ? 41 0F ? ? E8 ? ? ? ? 48 8B");
DEFINE(Signatures::Actor_canSee, "E8 ? ? ? ? 84 C0 74 ? 48 8B ? ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? E9");
DEFINE(Signatures::Actor_getAttribute, "E8 ? ? ? ? F3 0F 10 B0 ? ? ? ? 48 8D 4D");
DEFINE(Signatures::Actor_getNameTag, "E8 ? ? ? ? 48 8B ? 48 8B ? 48 83 78 18 ? 48 8B");
DEFINE(Signatures::Actor_setNameTag, "48 89 ? ? ? 57 48 83 EC ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? 48 85 ? 0F 84 ? ? ? ? 48 8B ? 4C 8B");
DEFINE(Signatures::Actor_setPosition, "E8 ? ? ? ? 48 8D ? ? 66 0F ? ? ? ? 48 8B ? 49 8B");
DEFINE(Signatures::EnchantUtils_getEnchantLevel, "48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 48 8B ? 0F B6 ? 33 FF");
DEFINE(Signatures::RenderItemInHandDescription_ctor, "48 89 ? ? ? 48 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 83 EC ? 4D 8B ? 4D 8B ? 4C 8B ? 48 8B ? 45 33");
DEFINE(Signatures::SimulatedPlayer_simulateJump, "48 89 ? ? ? 57 48 83 EC ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 47");
DEFINE(Signatures::SneakMovementSystem_tickSneakMovementSystem, "32 C0 88 43 ? 84 C0");
DEFINE(Signatures::Player_applyTurnDelta, "48 8B ? 48 89 ? ? 48 89 ? ? 55 57 41 ? 41 ? 41 ? 48 8D ? ? 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 44 0F ? ? ? 44 0F ? ? ? 44 0F ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? 4C 8B");
DEFINE(Signatures::NoRenderParticles, "E8 ? ? ? ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 85 ? 74 ? 48 8B ? 48 8B ? E8");
DEFINE(Signatures::LevelRendererPlayer_renderOutlineSelection, "48 8B ? 55 53 56 57 41 ? 41 ? 48 8D ? ? 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 44 0F");
DEFINE(Signatures::LevelRendererPlayer_renderOutlineSelectionOpcode, "0F 10 ? ? ? ? ? 0F 11 ? C6 40 10 ? 0F 57 ? 0F 11 ? ? 0F 11 ? ? 48 8D");
DEFINE(Signatures::PlayerMovement_clearInputState, "E8 ? ? ? ? 48 89 ? ? 88 9F ? ? ? ? 48 8D");
DEFINE(Signatures::ItemInHandRenderer_renderBp, "F3 0F ? ? EB ? 0F 57 ? F3 41");
DEFINE(Signatures::OverworldDimension_getBrightnessDependentFogColor, "41 0F 10 08 48 8B C2 0F");
DEFINE(Signatures::InventoryTransactionManager_addAction, "40 ? 55 41 ? 48 83 EC ? 48 8B ? 48 8B");
DEFINE(Signatures::NetworkStackItemDescriptor_ctor, "E8 ? ? ? ? 48 8D ? ? 4C 3B ? 74 ? 48 8B ? ? 48 C7 45 8F");
DEFINE(Signatures::InventoryTransaction_addAction, "E8 ? ? ? ? 48 8B 17 48 8B 42 ? 48 8B ? ? 48 ? ? 74 ?");
DEFINE(Signatures::ComplexInventoryTransaction_vtable, "48 8D 05 ? ? ? ? 48 89 01 8B 42 ? 89 41 ? 48 83 C1 ? 48 83 C2 ? E8 ? ? ? ? 90 48 8D 05");
DEFINE(Signatures::ItemUseInventoryTransaction_vtable, "48 8D 05 ? ? ? ? 48 89 07 8B 43 ? 89 47 ? 8B 53");
DEFINE(Signatures::ItemUseOnActorInventoryTransaction_vtable, "48 8D 05 ? ? ? ? 48 89 4B ? 0F 57 C0 48 89 4B ? 48 89 4B ? 48 8B 5C 24 ? 48 89 07 48 8D 05 ? ? ? ? 48 89 4F ? 89 4F");
DEFINE(Signatures::ItemReleaseInventoryTransaction_vtable, "48 8D ? ? ? ? ? 48 8B ? 48 89 ? 8B FA 48 83 C1 ? E8 ? ? ? ? 48 8D ? ? E8 ? ? ? ? 48 8D ? ? E8");
DEFINE(Signatures::ItemStack_EMPTY_ITEM, "48 8D 1D ? ? ? ? 48 8B D3 48 8D 4D ? E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 45 ? 89 B5");
DEFINE(Signatures::ItemStack_vtable, "48 8D 05 ? ? ? ? 48 89 85 ? ? ? ? 89 BD ? ? ? ? C6 85 ? ? ? ? ? 48 8D 95");
DEFINE(Signatures::ItemStack_fromDescriptor, "E8 ? ? ? ? 90 F6 86 ? ? ? ? ?");
DEFINE(Signatures::MainView_instance, "48 8B 05 ? ? ? ? C6 40 ? ? 0F 95 C0");
DEFINE(Signatures::RakPeer_sendImmediate, "40 ? 56 57 41 ? 41 ? 41 ? 41 ? 48 81 EC ? ? ? ? 48 8D ? ? ? 48 89 ? ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 48 8B");
DEFINE(Signatures::ActorCollision_setOnGround, "E8 ? ? ? ? EB ? 0F B6 ? ? 48 8D ? ? E8"); // ActorCollision::setOnGround
DEFINE(Signatures::ActorCollision_isOnGround, "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? F3 0F ? ? F3 0F ? ? ? F3 0F"); // ActorCollision::isOnGround
std::map<SignatureEnum, uintptr_t> SigManager::Signatures = {};

void SigManager::SearchSig(hat::signature_view sig, SignatureEnum name) {

    hat::scan_result result = hat::find_pattern(sig);

    if (!result.has_result())
        Logger::Write(xorstr_("Signatures"), xorstr_("Scan failed for: ") + (std::string) magic_enum::enum_name(name),
                      Logger::LogType::Debug);

    Signatures[name] = reinterpret_cast<uintptr_t>(result.get());;
}

/*
"48 8B ? ? ? ? ? 48 89 ? ? ? 48 89 ? ? ? F0 FF ? ? 66 0F ? ? ? ? EB ? 0F 57 ? 66 0F 73 D8 ? 66 48 ? ? ? 0F 57 ? 66 0F ? ? ? ? ? ? 48 85 ? 74 ? 8B 42 ? 85 C0 74 ? 0F 1F" // containerManagerModel (+3 offset)
"49 8B ? ? ? ? ? 74 ? 48 85 ? 74 ? E8" // dimension (+3 offset)
"48 8B ? ? ? ? ? 48 8B ? 48 8B ? ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? 48 8B" // gameMode (+3 offset)
"48 8B ? ? ? ? ? 48 85 ? 74 ? 66 0F ? ? ? ? ? ? 0F 5B ? 0F 2F" // hurtTime (+3 offset)
"0F B6 ? ? ? ? ? 80 7B 10" // isDestroying (+3 offset)
"80 BB ? ? ? ? ? 74 ? 8B 93 ? ? ? ? ff" // isSwinging (+3 offset)
"49 8B ? ? ? ? ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 84 C0 75 ? 49 8B ? ? ? ? ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 84 C0 74" // level (+3 offset)
"48 8B ? ? ? ? ? 80 BA B8 00 00 00 ? 75 ? 48 8B ? ? ? ? ? 48 8B ? 8B 52 ? 48 8B ? ? FF 15 ? ? ? ? 4C 8B ? EB ? 4C 8D ? ? ? ? ? 48 C7 C7" // supplies (+3 offset)
"89 93 ? ? ? ? 85 C0 74" // swingProgress (+2 offset)
"8B 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC 48 89 ? ? ? 56 48 83 EC" // ticksUsingItem (+2 offset)
"F3 0F ? ? ? ? ? ? F3 0F ? ? 0F 2F ? 73 ? F3 0F ? ? ? ? ? ? F3 0F ? ? F3 0F ? ? C3 CC CC CC 48 89" // timeAlongSwing (+4 offset)
*/
// name, sig, offset to add to sig

/*struct SigOffsetPair {
    std::string sig;
    int offset;

    SigOffsetPair(std::string sig, int offset) : sig(sig), offset(offset) {}
};

std::map<std::string, SigOffsetPair> offsetSigs = {
        {"containerManagerModel", SigOffsetPair("48 8B ? ? ? ? ? 48 89 ? ? ? 48 89 ? ? ? F0 FF ? ? 66 0F ? ? ? ? EB ? 0F 57 ? 66 0F 73 D8 ? 66 48 ? ? ? 0F 57 ? 66 0F ? ? ? ? ? ? 48 85 ? 74 ? 8B 42 ? 85 C0 74 ? 0F 1F", 3)},
        {"dimension", SigOffsetPair("49 8B ? ? ? ? ? 74 ? 48 85 ? 74 ? E8", 3)},
        {"gameMode", SigOffsetPair("48 8B ? ? ? ? ? 48 8B ? 48 8B ? ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? 48 8B", 3)},
        {"hurtTime", SigOffsetPair("48 8B ? ? ? ? ? 48 85 ? 74 ? 66 0F ? ? ? ? ? ? 0F 5B ? 0F 2F", 3)},
        {"isDestroying", SigOffsetPair("0F B6 ? ? ? ? ? 80 7B 10", 3)},
        {"isSwinging", SigOffsetPair("80 BB ? ? ? ? ? 74 ? 8B 93 ? ? ? ? ff", 2)},
        {"level", SigOffsetPair("49 8B ? ? ? ? ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 84 C0 75 ? 49 8B ? ? ? ? ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 84 C0 74", 3)},
        {"supplies", SigOffsetPair("48 8B ? ? ? ? ? 80 BA B8 00 00 00 ? 75 ? 48 8B ? ? ? ? ? 48 8B ? 8B 52 ? 48 8B ? ? FF 15 ? ? ? ? 4C 8B ? EB ? 4C 8D ? ? ? ? ? 48 C7 C7", 3)},
        {"swingProgress", SigOffsetPair("89 93 ? ? ? ? 85 C0 74", 2)},
        {"ticksUsingItem", SigOffsetPair("8B 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC 48 89 ? ? ? 56 48 83 EC", 2)},
        {"timeAlongSwing", SigOffsetPair("F3 0F ? ? ? ? ? ? F3 0F ? ? 0F 2F ? 73 ? F3 0F ? ? ? ? ? ? F3 0F ? ? F3 0F ? ? C3 CC CC CC 48 89", 4)}
};*/

void SigManager::Init() {
    uint64_t startTime = Misc::GetCurrentMs();
    Logger::Write(xorstr_("Signatures"), xorstr_("Caching offsets..."));
    /*for (auto& [name, pair] : offsetSigs) {
        auto sig = hat::parse_signature(pair.sig).value();
        hat::scan_result result = hat::find_pattern(sig);
        if (!result.has_result())
        {
            Logger::Write(xorstr_("Signatures"), xorstr_("Scan failed for: ") + name, Logger::LogType::Debug);
            continue;
        }

        auto realResult = reinterpret_cast<uintptr_t>(result.get());;
        auto rel = realResult + pair.offset;
        // Read the int at the address
        int offset = *reinterpret_cast<int*>(rel);

        Logger::Write(xorstr_("Signatures"), xorstr_("Offset for ") + name + xorstr_(" is ") + String::ToHex(offset), Logger::LogType::Debug);
    }*/

    Logger::Write(xorstr_("Signatures"), xorstr_("Caching signatures..."));

    // Search for all signatures
    SearchSig(Signatures::Reach, SignatureEnum::Reach);
    SearchSig(Signatures::MobEquipmentPacket_MobEquipmentPacket, SignatureEnum::MobEquipmentPacket_MobEquipmentPacket);
    SearchSig(Signatures::SplashTextRenderer_render, SignatureEnum::SplashTextRenderer_render);
    SearchSig(Signatures::MinecraftPackets_createPacket, SignatureEnum::MinecraftPackets_createPacket);
    SearchSig(Signatures::inventory_dropSlot, SignatureEnum::inventory_dropSlot);
    SearchSig(Signatures::RakNet_RakPeer_runUpdateCycle, SignatureEnum::RakNet_RakPeer_runUpdateCycle);
    SearchSig(Signatures::Gamemode_getDestroyRate, SignatureEnum::Gamemode_getDestroyRate);
    SearchSig(Signatures::ActorRenderDispatcher_render, SignatureEnum::ActorRenderDispatcher_render);
    SearchSig(Signatures::ContainerScreenController_tick, SignatureEnum::ContainerScreenController_tick);
    SearchSig(Signatures::Actor_shouldRenderNametagBp, SignatureEnum::Actor_shouldRenderNametagBp);
    SearchSig(Signatures::Actor_shouldRenderNametag, SignatureEnum::Actor_shouldRenderNametag);
    SearchSig(Signatures::renderCtxAddr, SignatureEnum::renderCtxAddr);
    SearchSig(Signatures::keymap, SignatureEnum::Keyboard);
    SearchSig(Signatures::CameraClipFunc, SignatureEnum::CameraClipFunc);
    SearchSig(Signatures::Mouse, SignatureEnum::Mouse);
    SearchSig(Signatures::Gamemode_baseUseItem, SignatureEnum::Gamemode_baseUseItem);
    SearchSig(Signatures::Mob_getCurrentSwingDuration, SignatureEnum::Mob_getCurrentSwingDuration);
    SearchSig(Signatures::bobHurt, SignatureEnum::bobHurt);
    SearchSig(Signatures::fluxSwing, SignatureEnum::fluxSwing);
    SearchSig(Signatures::Mob_getJumpComponent, SignatureEnum::Mob_getJumpComponent);
    SearchSig(Signatures::ItemStack_getCustomName, SignatureEnum::ItemStack_getCustomName);
    SearchSig(Signatures::ContainerScreenController_handleAutoPlace, SignatureEnum::ContainerScreenController_handleAutoPlace);
    SearchSig(Signatures::ItemUseSlowdownSystem_isSlowedByItemUse, SignatureEnum::ItemUseSlowdownSystem_isSlowedByItemUse);
    SearchSig(Signatures::AppPlatform_getDeviceId, SignatureEnum::AppPlatform_getDeviceId);
    SearchSig(Signatures::ConnectionRequest_create, SignatureEnum::ConnectionRequest_create);
    SearchSig(Signatures::TapSwingAnim, SignatureEnum::TapSwingAnim);
    SearchSig(Signatures::Actor_canSee, SignatureEnum::Actor_canSee);
    SearchSig(Signatures::Actor_getAttribute, SignatureEnum::Actor_getAttribute);
    SearchSig(Signatures::Actor_getNameTag, SignatureEnum::Actor_getNameTag);
    SearchSig(Signatures::Actor_setNameTag, SignatureEnum::Actor_setNameTag);
    SearchSig(Signatures::Actor_setPosition, SignatureEnum::Actor_setPosition);
    SearchSig(Signatures::EnchantUtils_getEnchantLevel, SignatureEnum::EnchantUtils_getEnchantLevel);
    SearchSig(Signatures::RenderItemInHandDescription_ctor, SignatureEnum::RenderItemInHandDescription_ctor);
    SearchSig(Signatures::SimulatedPlayer_simulateJump, SignatureEnum::SimulatedPlayer_simulateJump);
    SearchSig(Signatures::SneakMovementSystem_tickSneakMovementSystem, SignatureEnum::SneakMovementSystem_tickSneakMovementSystem);
    SearchSig(Signatures::Player_applyTurnDelta, SignatureEnum::Player_applyTurnDelta);
    SearchSig(Signatures::NoRenderParticles, SignatureEnum::NoRenderParticles);
    SearchSig(Signatures::LevelRendererPlayer_renderOutlineSelection, SignatureEnum::LevelRendererPlayer_renderOutlineSelection);
    SearchSig(Signatures::LevelRendererPlayer_renderOutlineSelectionOpcode, SignatureEnum::LevelRendererPlayer_renderOutlineSelectionOpcode);
    SearchSig(Signatures::PlayerMovement_clearInputState, SignatureEnum::PlayerMovement_clearInputState);
    SearchSig(Signatures::ItemInHandRenderer_renderBp, SignatureEnum::ItemInHandRenderer_renderBp);
    SearchSig(Signatures::OverworldDimension_getBrightnessDependentFogColor, SignatureEnum::OverworldDimension_getBrightnessDependentFogColor);
    SearchSig(Signatures::InventoryTransactionManager_addAction, SignatureEnum::InventoryTransactionManager_addAction);
    SearchSig(Signatures::NetworkStackItemDescriptor_ctor, SignatureEnum::NetworkStackItemDescriptor_ctor);
    SearchSig(Signatures::InventoryTransaction_addAction, SignatureEnum::InventoryTransaction_addAction);
    SearchSig(Signatures::ComplexInventoryTransaction_vtable, SignatureEnum::ComplexInventoryTransaction_vtable);
    SearchSig(Signatures::ItemUseInventoryTransaction_vtable, SignatureEnum::ItemUseInventoryTransaction_vtable);
    SearchSig(Signatures::ItemUseOnActorInventoryTransaction_vtable, SignatureEnum::ItemUseOnActorInventoryTransaction_vtable);
    SearchSig(Signatures::ItemReleaseInventoryTransaction_vtable, SignatureEnum::ItemReleaseInventoryTransaction_vtable);
    SearchSig(Signatures::ItemStack_EMPTY_ITEM, SignatureEnum::ItemStack_EMPTY_ITEM);
    SearchSig(Signatures::ItemStack_vtable, SignatureEnum::ItemStack_vtable);
    SearchSig(Signatures::ItemStack_fromDescriptor, SignatureEnum::ItemStack_fromDescriptor);
    SearchSig(Signatures::MainView_instance, SignatureEnum::MainView_instance);
    SearchSig(Signatures::RakPeer_sendImmediate, SignatureEnum::RakPeer_sendImmediate);
    SearchSig(Signatures::ActorCollision_setOnGround, SignatureEnum::ActorCollision_setOnGround);
    SearchSig(Signatures::ActorCollision_isOnGround, SignatureEnum::ActorCollision_isOnGround);

    uint64_t endTime = Misc::GetCurrentMs();
    double timeTaken = (endTime - startTime) / 1000.0;
    //ToastManager::ShowToast("Prax", "Found " + std::to_string(Signatures.size()) + " signatures in " + std::to_string(timeTaken) + " seconds");
    Logger::Write(xorstr_("Signatures"), xorstr_("Found ") + std::to_string(Signatures.size()) + xorstr_(" signatures in ") + std::to_string(timeTaken) + xorstr_(" seconds"));

    // Print out all the signatures that were found
    for (auto& sig : Signatures) {
        if (sig.second != 0)
            Logger::Write(xorstr_("Signatures"), (std::string)magic_enum::enum_name(sig.first) + xorstr_(" at ") + Misc::GetModuleOfAddress(sig.second), Logger::LogType::Debug);
    }
    // Print out all the signatures that were not found
    for (auto& sig : Signatures) {
        if (sig.second == 0)
            Logger::Write(xorstr_("Signatures"), xorstr_("Failed to find ") + (std::string)magic_enum::enum_name(sig.first), Logger::LogType::Error);
    }
}

uintptr_t SigManager::GetSignatureAddress(const SignatureEnum name) {
    if (Signatures.find(name) == Signatures.end()) {
        Logger::Write(
            xorstr_("Signatures"),
            xorstr_("Failed to find address in map for signature of ") + (std::string)magic_enum::enum_name(name),
            Logger::LogType::Error
        );
        return 0;
    }

    return Signatures[name];
}
