#include "HookManager.h"
#include "Hooks/World/Entity/BobHurtHook.h"
#include "Hooks/Game/ClientInstanceUpdateHook.h"
#include "Hooks/Game/SendChatMessageHook.h"
#include "Hooks/Input/KeyMapHook.h"
#include "Hooks/Input/KeyMouseHook.h"
#include "Hooks/Render/GetCameraComponentHook.h"
#include "Hooks/Render/GetFovHook.h"
#include "Hooks/Render/GetGammaHook.h"
#include "Hooks/Render/GetViewPerspectiveHook.h"
#include "Hooks/Render/HurtCamHook.h"
#include "Hooks/Render/RenderHitSelectHook.h"
#include "Hooks/Render/RenderLevelHook.h"
#include "Hooks/Render/SetUpAndRenderHook.h"
#include "Hooks/Network/SendDelayedPacketHook.h"
#include "Hooks/Network/SendPacketHook.h"
#include "Hooks/World/Block/GetBlockHook.h"
#include "Hooks/World/Entity/ActorLerpMotionHook.h"
#include "Hooks/World/Entity/ActorNormalTickHook.h"
#include "Hooks/World/Entity/ActorIsOnFireHook.h"
#include "Hooks/World/Entity/ActorSwingHook.h"
#include "Hooks/World/Entity/ActorSetRotHook.h"
#include "Hooks/World/Entity/MobSwingDurationHook.h"
#include "Hooks/World/Entity/PlayerSlowDownHook.h"
#include "Hooks/World/GameMode/StartDestroyBlockHook.h"
#include "Hooks/World/GameMode/StopDestroyBlockHook.h"
#include "Hooks/World/GameMode/GetPickRangeHook.h"
#include "Hooks/World/Level/LevelTickHook.h"
#include "Hooks/DirectX/PresentHook.h"
#include "Hooks/DirectX/ResizeBuffersHook.h"

#include <kiero.h>

void HookManager::init() {
	MH_Initialize();

	RequestHook<ClientInstanceUpdateHook>(Addresses::ClientInstance_update);
	RequestHook<SendChatMessageHook>(Addresses::ClientInstanceScreenModel_sendChatMessage);
	RequestHook<KeyMapHook>(Addresses::KeyPressFunc);
	RequestHook<KeyMouseHook>(Addresses::KeyMouseFunc);
	RequestHook<GetCameraComponentHook>(Addresses::GetCameraComponent);
	RequestHook<GetFovHook>(Addresses::LevelRendererPlayer_getFov);
	RequestHook<GetGammaHook>(Addresses::Options_getGamma);
	RequestHook<GetViewPerspectiveHook>(Addresses::Options_getViewPerspective);
	RequestHook<HurtCamHook>(Addresses::CauseHurtCamFunc);
	RequestHook<RenderHitSelectHook>(Addresses::LevelRendererPlayer_renderHitSelect);
	RequestHook<RenderLevelHook>(Addresses::LevelRenderer_renderLevel);
	RequestHook<SetUpAndRenderHook>(Addresses::ScreenView_setUpAndRender);
	RequestHook<BobHurtHook>("40 53 56 48 83 EC 78 ? ? 7C");
	{
		// Network Hook
		uintptr_t** PacketSenderVTable = (uintptr_t**)Addresses::PacketSenderVTable;
		RequestHook<SendPacketHook>(PacketSenderVTable, 1);
		RequestHook<SendDelayedPacketHook>(PacketSenderVTable, 1);
	}

	{
		// Block Hook
		uintptr_t** BlockSourceVTable = (uintptr_t**)Addresses::BlockSourceVTable;
		RequestHook<GetBlockHook>(BlockSourceVTable, 2);
	}

	{
		// Entity Hook
		uintptr_t** PlayerVTable = (uintptr_t**)Addresses::PlayerVTable;
		RequestHook<ActorLerpMotionHook>(PlayerVTable, 24);
		RequestHook<ActorNormalTickHook>(PlayerVTable, 26);
		RequestHook<ActorIsOnFireHook>(PlayerVTable, 51);
		RequestHook<ActorSwingHook>(PlayerVTable, 117);
		RequestHook<ActorSetRotationHook>("4C 8B DC 49 89 73 10 49 89 7B 20 55 49 8D 6B A1 48 81 EC ? ? ? ? 48 8B");
		RequestHook<SwingDurationHook>("48 89 5C 24 ? 57 48 83 EC 20 48 8B 15 ? ? ? ? 48 8B F9 33 DB");
		RequestHook<SlowDownHook>("40 53 48 83 EC 30 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B DA 4D 85 C0 74 5D");
	}

	{
		// GameMode Hook
		uintptr_t** GameModeVTable = (uintptr_t**)Addresses::GameModeVTable;
		RequestHook<StartDestroyBlockHook>(GameModeVTable, 1);
		RequestHook<StopDestroyBlockHook>(GameModeVTable, 4);
		RequestHook<PickRangeHook>(GameModeVTable, 10);
	}

	{
		// Level Hook
		// Search for "Client%d camera ticking systems" -> xref first function -> What access to a1
		uintptr_t** LevelVTable = (uintptr_t**)Addresses::LevelVTable;
		RequestHook<LevelTickHook>(LevelVTable, 99);
	}

	if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success) {
		uintptr_t** methodsTable = (uintptr_t**)kiero::getMethodsTable();
		RequestHook<PresentHook>(methodsTable, 140);
		RequestHook<ResizeBuffersHook>(methodsTable, 145);
	}
	else if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		uintptr_t** methodsTable = (uintptr_t**)kiero::getMethodsTable();
		RequestHook<PresentHook>(methodsTable, 8);
		RequestHook<ResizeBuffersHook>(methodsTable, 13);
	}
	else {
		return;
	}

	if (!getHook<ClientInstanceUpdateHook>()->enableHook()) {
		return;
	}

	while (Game::clientInstance == nullptr) {
		Sleep(250);
	}

	for (auto& funcHook : hooksCache) {
		funcHook->enableHook();
	}

	// We don't this anymore so we can delete it
	for (auto& funcHook : hooksCache) {
		delete funcHook;
	}
	hooksCache.clear();
}

void HookManager::shutdown() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
	kiero::shutdown();

	for (auto& funcHook : hooksCache) {
		delete funcHook;
	}
	hooksCache.clear();
}