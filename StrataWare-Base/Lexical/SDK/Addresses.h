#pragma once
#include <memory>

namespace Addresses {
	extern uintptr_t ClientInstance_update;
	extern uintptr_t ClientInstanceScreenModel_sendChatMessage;
	extern uintptr_t Options_getGamma;
	extern uintptr_t Options_getViewPerspective;
	extern uintptr_t ScreenView_setUpAndRender;
	extern uintptr_t BaseActorRenderContext_Constructor;
	extern uintptr_t MobEquipmentPacket_MobEquipmentPacket;
	extern uintptr_t ItemInHandRenderer_render;
	extern uintptr_t ItemInHandRenderer_renderOffhandItem;
	extern uintptr_t ItemRenderer_renderGuiItemNew;
	extern uintptr_t Tessellator_begin;
	extern uintptr_t Tessellator_vertex;
	extern uintptr_t Tessellator_color;
	extern uintptr_t MeshHelpers_renderMeshImmediately;
	extern uintptr_t LevelRenderer_renderLevel;
	extern uintptr_t LevelRendererPlayer_getFov;
	extern uintptr_t LevelRendererPlayer_renderHitSelect;
	extern uintptr_t try_getActorEquipmentComponent;
	extern uintptr_t try_getActorHeadRotationComponent;
	extern uintptr_t try_getActorTypeComponent;
	extern uintptr_t try_getDimensionTypeComponent;
	extern uintptr_t try_getMobBodyRotationComponent;
	extern uintptr_t try_getMoveInputComponent;
	extern uintptr_t try_getRenderPositionComponent;
	extern uintptr_t try_getRuntimeIDComponent;
	extern uintptr_t Actor_calculateAttackDamage;
	extern uintptr_t Actor_getNameTag;
	extern uintptr_t Actor_getEffect;
	extern uintptr_t Actor_hasEffect;
	extern uintptr_t Actor_setPos;
	extern uintptr_t ActorCollision_hasHorizontalCollision;
	extern uintptr_t ActorCollision_setOnGround;
	extern uintptr_t GameMode_baseUseItem;
	extern uintptr_t GameMode_getDestroyRate;
	extern uintptr_t BlockSource_getSeenPercent;
	extern uintptr_t MobEffectInstance_getDisplayName;
	extern uintptr_t InventoryAction_Constructor;
	extern uintptr_t InventoryTransaction_addAction;
	extern uintptr_t InventoryTransactionManager_addAction;
	extern uintptr_t LevelRendererPlayer_bobHurt;
	extern uintptr_t ItemStackBase_CopyConstructor;
	extern uintptr_t ItemStack_EMPTY_ITEM;
	extern uintptr_t ItemStackBase_isBlock;
	extern uintptr_t ItemStack_getDestroySpeed;
	extern uintptr_t Item_getDamageValue;
	extern uintptr_t NetworkItemStackDescriptor_Constructor;
	extern uintptr_t MinecraftPackets_createPacket;
	extern uintptr_t InventoryTransactionPacket_Constructor;
	extern uintptr_t KeyPressFunc;
	extern uintptr_t KeyMouseFunc;
	extern uintptr_t GetCameraComponent;
	extern uintptr_t CauseHurtCamFunc;
	extern uintptr_t MobEffectListPtr;
	extern uintptr_t CameraClipCheck;
	extern uintptr_t PacketSenderVTable;
	extern uintptr_t BlockSourceVTable;
	extern uintptr_t PlayerVTable;
	extern uintptr_t GameModeVTable;
	extern uintptr_t LevelVTable;
	extern uintptr_t ItemStackVTable;
	extern uintptr_t ComplexInventoryTransactionVTable;

	void init();
}