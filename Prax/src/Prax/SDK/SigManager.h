#pragma once
//
// Created by vastrakai on 9/12/2023.
//


#include <map>
#include <include/libhat.hpp>
#include <future>




namespace Signatures {
    extern const hat::signature_view Reach;
    extern const hat::signature_view MobEquipmentPacket_MobEquipmentPacket;
    extern const hat::signature_view Actor_shouldRenderNametag;
    extern const hat::signature_view Actor_shouldRenderNametagBp;
    extern const hat::signature_view SplashTextRenderer_render;
    extern const hat::signature_view MinecraftPackets_createPacket;
    extern const hat::signature_view inventory_dropSlot;
    extern const hat::signature_view RakNet_RakPeer_runUpdateCycle;
    extern const hat::signature_view Gamemode_getDestroyRate;
    extern const hat::signature_view ActorRenderDispatcher_render;
    extern const hat::signature_view ContainerScreenController_tick;
    extern const hat::signature_view renderCtxAddr;
    extern const hat::signature_view keymap;
    extern const hat::signature_view CameraClipFunc;
    extern const hat::signature_view Mouse;
    extern const hat::signature_view Gamemode_baseUseItem;
    extern const hat::signature_view Mob_getCurrentSwingDuration;
    extern const hat::signature_view bobHurt;
    extern const hat::signature_view fluxSwing;
    extern const hat::signature_view Mob_getJumpComponent;
    extern const hat::signature_view ItemStack_getCustomName;
    extern const hat::signature_view ContainerScreenController_handleAutoPlace;
    extern const hat::signature_view BinaryStream_writeUnsignedVarInt64;
    extern const hat::signature_view BinaryStream_writeUnsignedVarInt;
    extern const hat::signature_view BinaryStream_writeFloat;
    extern const hat::signature_view BinaryStream_writeUnsignedChar;
    extern const hat::signature_view BinaryStream_writeUnsignedInt;
    extern const hat::signature_view BinaryStream_writeNetworkStackItemDescriptor;
    extern const hat::signature_view ItemStackRequestAction_swap;
    extern const hat::signature_view ItemStackRequestAction_drop;
    extern const hat::signature_view ItemStackRequestData;
    extern const hat::signature_view ItemUseSlowdownSystem_isSlowedByItemUse;
    extern const hat::signature_view AppPlatform_getDeviceId;
    extern const hat::signature_view ConnectionRequest_create;
    extern const hat::signature_view TapSwingAnim;
    extern const hat::signature_view Actor_canSee;
    extern const hat::signature_view Actor_getAttribute;
    extern const hat::signature_view Actor_getNameTag;
    extern const hat::signature_view Actor_setNameTag;
    extern const hat::signature_view Actor_setPosition;
    extern const hat::signature_view EnchantUtils_getEnchantLevel;
    extern const hat::signature_view RenderItemInHandDescription_ctor;
    extern const hat::signature_view SimulatedPlayer_simulateJump;
    extern const hat::signature_view SneakMovementSystem_tickSneakMovementSystem;
    extern const hat::signature_view Player_applyTurnDelta;
    extern const hat::signature_view NoRenderParticles;
    extern const hat::signature_view LevelRendererPlayer_renderOutlineSelection;
    extern const hat::signature_view LevelRendererPlayer_renderOutlineSelectionOpcode;
    extern const hat::signature_view PlayerMovement_clearInputState;
    extern const hat::signature_view ItemInHandRenderer_renderBp;
    extern const hat::signature_view OverworldDimension_getBrightnessDependentFogColor;
    extern const hat::signature_view InventoryTransactionManager_addAction;
    extern const hat::signature_view NetworkStackItemDescriptor_ctor;
    extern const hat::signature_view InventoryTransaction_addAction;
    extern const hat::signature_view ComplexInventoryTransaction_vtable;
    extern const hat::signature_view ItemUseInventoryTransaction_vtable;
    extern const hat::signature_view ItemUseOnActorInventoryTransaction_vtable;
    extern const hat::signature_view ItemReleaseInventoryTransaction_vtable;
    extern const hat::signature_view ItemStack_EMPTY_ITEM;
    extern const hat::signature_view ItemStack_vtable;
    extern const hat::signature_view ItemStack_fromDescriptor;
    extern const hat::signature_view MainView_instance;
    extern const hat::signature_view RakPeer_sendImmediate;
    extern const hat::signature_view ActorCollision_setOnGround;
    extern const hat::signature_view ActorCollision_isOnGround;
}

enum class SignatureEnum {
    Reach = 0,
    MobEquipmentPacket_MobEquipmentPacket,
    Actor_shouldRenderNametagBp,
    Actor_shouldRenderNametag,
    SplashTextRenderer_render,
    MinecraftPackets_createPacket,
    inventory_dropSlot,
    RakNet_RakPeer_runUpdateCycle,
    Gamemode_getDestroyRate,
    ActorRenderDispatcher_render,
    ContainerScreenController_tick,
    renderCtxAddr,
    Keyboard,
    CameraClipFunc,
    Mouse,
    Gamemode_baseUseItem,
    Mob_getCurrentSwingDuration,
    bobHurt,
    fluxSwing,
    Mob_getJumpComponent,
    ItemStack_getCustomName,
    ContainerScreenController_handleAutoPlace,
    ItemUseSlowdownSystem_isSlowedByItemUse,
    AppPlatform_getDeviceId,
    ConnectionRequest_create,
    TapSwingAnim,
    Actor_canSee,
    Actor_getAttribute,
    Actor_getNameTag,
    Actor_setNameTag,
    Actor_setPosition,
    EnchantUtils_getEnchantLevel,
    RenderItemInHandDescription_ctor,
    SimulatedPlayer_simulateJump,
    SneakMovementSystem_tickSneakMovementSystem,
    Player_applyTurnDelta,
    NoRenderParticles,
    LevelRendererPlayer_renderOutlineSelection,
    LevelRendererPlayer_renderOutlineSelectionOpcode,
    PlayerMovement_clearInputState,
    ItemInHandRenderer_renderBp,
    OverworldDimension_getBrightnessDependentFogColor,
    InventoryTransactionManager_addAction,
    NetworkStackItemDescriptor_ctor,
    InventoryTransaction_addAction,
    ComplexInventoryTransaction_vtable,
    ItemUseInventoryTransaction_vtable,
    ItemUseOnActorInventoryTransaction_vtable,
    ItemReleaseInventoryTransaction_vtable,
    ItemStack_EMPTY_ITEM,
    ItemStack_vtable,
    ItemStack_fromDescriptor,
    MainView_instance,
    RakPeer_sendImmediate,
    ActorCollision_setOnGround,
    ActorCollision_isOnGround
};

class SigManager {
public:

    static std::map<SignatureEnum, uintptr_t> Signatures;

    static void SearchSig(hat::signature_view sig, SignatureEnum name);
    static void Init();
    static uintptr_t GetSignatureAddress(SignatureEnum name);
};