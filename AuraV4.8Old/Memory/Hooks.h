#pragma once

#ifndef PERFORMANCE_TEST
//#define PERFORMANCE_TEST
#endif

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp>  // perspective, translate, rotate
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>         // mat4
#include <glm/trigonometric.hpp>  //radians

#include "../AuraClient/Command/CommandMgr.h"
#include "../AuraClient/Config/ConfigManager.h"
#include "../Utils/DrawUtils.h"
#include "../Utils/ImmediateGui.h"
#include "../AuraClient/Menu/ConfigManagerMenu.h"
#include "../AuraClient/Menu/HudEditor.h"
#include "../AuraClient/Menu/ClickGui.h"
#include "../AuraClient/Menu/TabGui.h"
#include "../AuraClient/Module/ModuleManager.h"
#include "../SDK/CBlockLegacy.h"
#include "../SDK/CCamera.h"
#include "../SDK/CChestBlockActor.h"
#include "../SDK/CGameMode.h"
#include "../SDK/CMinecraftUIRenderContext.h"
#include "../SDK/CMoveInputHandler.h"
#include "../SDK/CRakNetInstance.h"
#include "../SDK/CUIScene.h"
#include "../SDK/TextHolder.h"
#include "../Utils/SkinUtil.h"
#include "../Utils/TextFormat.h"
#include "GameData.h"
#include "MinHook.h"
//#include "../Packet/Game/Game.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <intrin.h>

#include <thread>

#include "../Utils/Logger.h"
#include "../include/d3dx11async.h"

class VMTHook;
class FuncHook;

struct CoolSkinData {
	TextHolder unknown;
	TextHolder unknown2;
	TextHolder skinResourcePatch;  // 0x040
	TextHolder geometryName;       // 0x060 "geometry.humanoid.custom"
	unsigned char gap2[0x40];      // 0x080
	void* startAnimatedFrames;     // 0x0C0
	void* endAnimatedFrames;       // 0x0C8
	unsigned char gap3[0x8];       // 0x0D0
	TextHolder geometryData;       // 0x0D8
	TextHolder skinAnimationData;  // 0x0F8
	unsigned char gap4[0x20];      // 0x118
	bool isPremiumSkin;            // 0x138
	bool isPersonaSkin;
	bool isCapeOnClassicSkin;
	void* startPersonaPieces;
	void* endPersonaPieces;
	unsigned char gap5[0x8];  // 0x150
	TextHolder armSize;       // 0x158
	unsigned char gap6[0x8];  // 0x178
	void* startPieces;
};

class Hooks {
private:
	bool shouldRender = true;
	// bool shouldRenderAura = true;
public:
	char currentScreenName[100];
	std::vector<std::shared_ptr<FuncHook>> lambdaHooks;
	struct EntityListPointerHolder {
		C_Entity* ent;
		int addedTick;
	};
	bool isThirdPerson = false;
	std::vector<EntityListPointerHolder> entityList;
	bool shouldLocalPlayerBeImmobile = false;

	static void Init();
	static void Restore();
	static void Enable();

private:
	static void setPos(C_Entity* ent, vec3_t& pos);
	static void Actor_baseTick(C_Entity* _this);
	static void Actor_aiStep(C_Entity* _this);
	static __int64 UIScene_setupAndRender(C_UIScene* uiscene, __int64 screencontext);
	static __int64 UIScene_render(C_UIScene* uiscene, __int64 screencontext);
	static void* Player_tickWorld(C_Player* _this, __int64);
	static float getDestroySpeed(C_Player* _this, C_Block& block);
	static void chatLogHookFunc(__int64 a1, TextHolder* msg, uint32_t a2);
	static bool playerCallBack(C_Player* lp, __int64 a2, __int64 a3);
	static __int64 RenderText(__int64 a1, C_MinecraftUIRenderContext* renderCtx);
	static float* Dimension_getFogColor(__int64, float* color, __int64 brightness, float a4);
	static float Dimension_getTimeOfDay(__int64, int a2, float a3);
	static float Dimension_getSunIntensity(__int64, float a2, vec3_t* a3, float a4);
	static void ChestBlockActor_tick(C_ChestBlockActor*, void* a);
	static void Actor_lerpMotion(C_Entity* _this, vec3_t);
	static void PleaseAutoComplete(__int64 _this, __int64 a2, TextHolder* text, int a4);
	static void LoopbackPacketSender_sendToServer(C_LoopbackPacketSender* a, C_Packet* packet);
	static float LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3);
	static void MultiLevelPlayer_tick(C_EntityList* entityList);
	static void GameMode_startDestroyBlock(C_GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5);
	static void HIDController_keyMouse(C_HIDController* _this, void* a2, void* a3);
	static int BlockLegacy_getRenderLayer(C_BlockLegacy* a1);
	static __int64 LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3);
	static void ClickFunc(__int64 a1, char a2, char a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, char a8);
	static __int64 MoveInputHandler_tick(C_MoveInputHandler* _this, C_Entity* a2);
	static __int64 ChestScreenController_tick(C_ChestScreenController* _this);
	static float GetGamma(uintptr_t* a1);
	static __int64 MinecraftGame_onAppSuspended(__int64 _this);
	static void Actor_swing(C_Entity* _this);
	static void RakNetInstance_tick(C_RakNetInstance* _this, __int64 a2, __int64 a3);
	static float GameMode_getPickRange(C_GameMode* _this, __int64 a2, char a3);
	static __int64 GameMode_attack(C_GameMode* _this, C_Entity*);
	static __int64 ConnectionRequest_create(__int64 _this, __int64 privateKeyManager, void* a3, TextHolder* selfSignedId, TextHolder* serverAddress, __int64 clientRandomId, TextHolder* skinId, SkinData* skinData, __int64 capeData, CoolSkinData* coolSkinStuff, TextHolder* deviceId, int inputMode, int uiProfile, int guiScale, TextHolder* languageCode, bool sendEduModeParams, TextHolder* tenantId, __int64 unused, TextHolder* platformUserId, TextHolder* thirdPartyName, bool thirdPartyNameOnly, TextHolder* platformOnlineId, TextHolder* platformOfflineId, TextHolder* capeId);
	static void InventoryTransactionManager_addAction(C_InventoryTransactionManager* a1, C_InventoryAction* a2);
	static bool DirectoryPackAccessStrategy__isTrusted(__int64 _this);
	static bool ReturnTrue(__int64 _this);
	static __int64 SkinRepository___loadSkinPack(__int64 _this, __int64 pack, __int64 a3);
	static GamerTextHolder* toStyledString(__int64 strIn, GamerTextHolder* strOut);
	static __int64 prepFeaturedServers(__int64 a1);
	static __int64 prepFeaturedServersFirstTime(__int64 a1, __int64 a2);
	static void ClientInstanceScreenModel_sendChatMessage(void* _this, TextHolder* text);
	static void LocalPlayer__updateFromCamera(__int64 a1, C_Camera* a2);
	static bool Mob__isImmobile(C_Entity*);
	static __int8* BlockLegacy_getLightEmission(C_BlockLegacy* _this, __int8* a2);
	static void InventoryTransactionManager__addAction(C_InventoryTransactionManager*, C_InventoryAction&);
	static void LevelRendererPlayer__renderNameTags(__int64 a1, __int64 a2, TextHolder* name, __int64 a4);
	static void Actor_rotation(C_Entity* _this, vec2_t& angle);
	static int ForceThirdPersonLol(__int64 a1);
	static void GameMode__stopDestroyBlock(C_GameMode* _this, vec3_ti* BlockPos);
	static void LevelRendererPlayer__bobHurt(__int64 _this, glm::mat4& matrix, float lerpT);

	std::unique_ptr<FuncHook> GameMode__stopDestroyBlockHook;
	std::unique_ptr<FuncHook> setPosHook;
	std::unique_ptr<FuncHook> Actor__baseTick; 
	std::unique_ptr<FuncHook> Actor_rotationHook;
	std::unique_ptr<FuncHook> UIScene_setupAndRenderHook;
	std::unique_ptr<FuncHook> UIScene_renderHook;
	std::unique_ptr<FuncHook> RenderTextHook; 
	std::unique_ptr<FuncHook> forceThirdPerson;
	std::unique_ptr<FuncHook> Dimension_getFogColorHook;
	std::unique_ptr<FuncHook> Dimension_getTimeOfDayHook;
	std::unique_ptr<FuncHook> playerCallBack_Hook;
	std::unique_ptr<FuncHook> BlockLegacy_getLightEmissionHook;
	std::unique_ptr<FuncHook> Dimension_getSunIntensityHook;
	std::unique_ptr<FuncHook> ChestBlockActor_tickHook;
	std::unique_ptr<FuncHook> Actor_lerpMotionHook;
	std::unique_ptr<FuncHook> PleaseAutoCompleteHook;
	std::unique_ptr<FuncHook> LoopbackPacketSender_sendToServerHook;
	std::unique_ptr<FuncHook> LevelRendererPlayer_getFovHook;
	std::unique_ptr<FuncHook> MultiLevelPlayer_tickHook;
	std::unique_ptr<FuncHook> GameMode_startDestroyBlockHook;
	std::unique_ptr<FuncHook> Player_tickWorldHook;
	std::unique_ptr<FuncHook> Actor_aiStepHook;
	std::unique_ptr<FuncHook> getDestroySpeedHook;
	std::unique_ptr<FuncHook> ClientInstanceScreenModel_sendChatMessageHook;
	std::unique_ptr<FuncHook> HIDController_keyMouseHook;
	std::unique_ptr<FuncHook> BlockLegacy_getRenderLayerHook;
	std::unique_ptr<FuncHook> LevelRenderer_renderLevelHook;
	std::unique_ptr<FuncHook> ClickFuncHook;
	std::unique_ptr<FuncHook> ChatLogHook;
	std::unique_ptr<FuncHook> MoveInputHandler_tickHook;
	std::unique_ptr<FuncHook> ChestScreenController_tickHook;
	std::unique_ptr<FuncHook> GetGammaHook;
	std::unique_ptr<FuncHook> MinecraftGame_onAppSuspendedHook;
	std::unique_ptr<FuncHook> Actor_swingHook;
	std::unique_ptr<FuncHook> RakNetInstance_tickHook;
	std::unique_ptr<FuncHook> GameMode_getPickRangeHook;
	std::unique_ptr<FuncHook> GameMode_attackHook;
	std::unique_ptr<FuncHook> ConnectionRequest_createHook;
	std::unique_ptr<FuncHook> InventoryTransactionManager_addActionHook;
	std::unique_ptr<FuncHook> DirectoryPackAccessStrategy__isTrustedHook;
	std::unique_ptr<FuncHook> ZipPackAccessStrategy__isTrustedHook;
	std::unique_ptr<FuncHook> SkinRepository___checkSignatureFileInPack;
	std::unique_ptr<FuncHook> SkinRepository___loadSkinPackHook;
	std::unique_ptr<FuncHook> toStyledStringHook;
	std::unique_ptr<FuncHook> prepFeaturedServersHook;
	std::unique_ptr<FuncHook> prepFeaturedServersFirstTimeHook;
	std::unique_ptr<FuncHook> LocalPlayer__updateFromCameraHook;
	std::unique_ptr<FuncHook> LevelRendererPlayer__bobHurtHook;
	std::unique_ptr<FuncHook> Mob__isImmobileHook;
	std::unique_ptr<FuncHook> InventoryTransactionManager__addActionHook;
	std::unique_ptr<FuncHook> LevelRendererPlayer__renderNameTagsHook;
};

extern Hooks g_Hooks;

class FuncHook {
public:
	void* funcPtr;
	void* funcReal;

	FuncHook(void* func, void* hooked) {
		funcPtr = func;

		MH_STATUS ret = MH_CreateHook(func, hooked, &funcReal);
		if (ret == MH_OK && (__int64)func > 10) {
		} else
			logF("MH_CreateHook = %i", ret);
	};

	FuncHook(uintptr_t func, void* hooked) {
		funcPtr = reinterpret_cast<void*>(func);

		MH_STATUS ret = MH_CreateHook(funcPtr, hooked, &funcReal);
		if (ret == MH_OK && (__int64)funcPtr > 10) {
		} else
			logF("MH_CreateHook = %i", ret);
	};

	void enableHook(bool enable = true) {
		if (funcPtr != nullptr) {
			int ret = enable ? MH_EnableHook(funcPtr) : MH_DisableHook(funcPtr);
			if (ret != MH_OK)
				logF("MH_EnableHook = %i", ret);
		} else
			logF("enableHook() called with nullptr func!");
	}

	~FuncHook() {
		Restore();
	}

	void Restore() {
		if (funcPtr != nullptr)
			MH_DisableHook(funcPtr);
	}

	template <typename TRet, typename... TArgs>
	inline auto* GetFastcall() {
		using Fn = TRet(__fastcall*)(TArgs...);
		return reinterpret_cast<Fn>(funcReal);
	};
};