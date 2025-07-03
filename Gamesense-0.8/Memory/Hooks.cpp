#include "Hooks.h"

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp> // perspective, translate, rotate
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>		 // mat4
#include <glm/trigonometric.hpp> //radians
#include <unordered_set>
#include <iostream>
#include "../Gamesense/Loader.h"
#include "../SDK/Tag.h"
#include <iomanip>
#include "../Utils/ImGuiUtils.h"
#define destroyrate *(float*)((__int64)gm + 0x24)
Hooks g_Hooks;
using namespace std;
bool isTicked = false;
bool overrideStyledReturn = false;
TextHolder styledReturnText;
bool isHUDHidden = false;

void blockRotate(glm::mat4 &matrix, float upper)
{
	float floatY = -1.30F;
	matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, upper, -0.20F));
	matrix = glm::rotate<float>(matrix, -1.98F, glm::vec3(0.0F, 1.0F, 0.0F));
	matrix = glm::rotate<float>(matrix, -floatY, glm::vec3(4.0F, 0.0F, 0.0F));
	matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
}

void slideBlockRotate(glm::mat4 &matrix, float upper)
{
	matrix = glm::translate<float>(matrix, glm::vec3(-0.5F, upper, 0.0F));
	matrix = glm::rotate<float>(matrix, 30.0F, glm::vec3(0.0F, 1.0F, 0.0F));
	matrix = glm::rotate<float>(matrix, -80.0F, glm::vec3(1.0F, 0.0F, 0.0F));
	matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
}

void Hooks::Init()
{
	logF("Setting up Hooks...");
	// Vtables
	{
		// GameMode::vtable
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 8B D9 48 89 01 8B FA 48 8B 89 ? ? ? ? 48 85 C9 74 ? 48 8B 01 BA ? ? ? ? FF 10 48 8B 8B");
	int offset = *reinterpret_cast<int *>(sigOffset + 3);
	uintptr_t **gameModeVtable = reinterpret_cast<uintptr_t **>(sigOffset + offset + /*length of instruction*/ 7);
	if (gameModeVtable == 0x0 || sigOffset == 0x0)
		logF("C_GameMode signature not working!!!");
	else
	{
		g_Hooks.GameMode_startDestroyBlockHook = make_unique<FuncHook>(gameModeVtable[1], Hooks::GameMode_startDestroyBlock);
		g_Hooks.GameMode__stopDestroyBlockHook = std::make_unique<FuncHook>(gameModeVtable[4], Hooks::GameMode__stopDestroyBlock);
		g_Hooks.GameMode_getPickRangeHook = make_unique<FuncHook>(gameModeVtable[10], Hooks::GameMode_getPickRange);
		g_Hooks.GameMode_attackHook = make_unique<FuncHook>(gameModeVtable[14], Hooks::GameMode_attack);
	}
}

// BlockLegacy::vtable
{
	intptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 06 EB 03 49 8B F4 4D 89 26"); // BlockLegacy constructor
	int offset = *reinterpret_cast<int *>(sigOffset + 3);
	uintptr_t **blockLegacyVtable = reinterpret_cast<uintptr_t **>(sigOffset + offset + 7);
	if (blockLegacyVtable == 0x0 || sigOffset == 0x0)
		logF("C_BlockLegacy signature not working!!!");
	else
	{
	}
}

// LocalPlayer::vtable
{
	uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 48 8B 89 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 8B 10");
	int offset = *reinterpret_cast<int *>(sigOffset + 3);
	uintptr_t **localPlayerVtable = reinterpret_cast<uintptr_t **>(sigOffset + offset + /*length of instruction*/ 7);
	if (localPlayerVtable == 0x0 || sigOffset == 0x0)
		logF("C_LocalPlayer signature not working!!!");
	else
	{
		g_Hooks.Actor_rotationHook = make_unique<FuncHook>(localPlayerVtable[27], Hooks::Actor_rotation);
		g_Hooks.Actor_swingHook = make_unique<FuncHook>(localPlayerVtable[219], Hooks::Actor_swing);
		g_Hooks.setPosHook = std::make_unique<FuncHook>(localPlayerVtable[19], Hooks::setPos);
		g_Hooks.Actor__baseTick = make_unique<FuncHook>(localPlayerVtable[49], Hooks::Actor_baseTick);
		g_Hooks.Mob__isImmobileHook = make_unique<FuncHook>(localPlayerVtable[91], Hooks::Mob__isImmobile);
		g_Hooks.Player_tickWorldHook = make_unique<FuncHook>(localPlayerVtable[364], Hooks::Player_tickWorld);
	}
}

// MoveInputHandler::vtable
{
	uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 49 89 48 ? 49 89 80 ? ? ? ? 49 89 80 ? ? ? ? 48 39 87 ? ? ? ? 74 20 48 8B 8F");
	int offset = *reinterpret_cast<int *>(sigOffset + 3);
	uintptr_t **moveInputVtable = reinterpret_cast<uintptr_t **>(sigOffset + offset + 7);
	if (moveInputVtable == 0x0 || sigOffset == 0x0)
		logF("C_MoveInputHandler signature not working!!!");
	else
	{
		g_Hooks.MoveInputHandler_tickHook = make_unique<FuncHook>(moveInputVtable[1], Hooks::MoveInputHandler_tick);
	}
}

// PackAccessStrategy vtables for isTrusted
{

	uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
	int offset = *reinterpret_cast<int *>(sigOffset + 3);
	uintptr_t **directoryPackVtable = reinterpret_cast<uintptr_t **>(sigOffset + offset + 7);

	{
		g_Hooks.DirectoryPackAccessStrategy__isTrustedHook = make_unique<FuncHook>(directoryPackVtable[6], Hooks::DirectoryPackAccessStrategy__isTrusted);
	}

	uintptr_t sigOffset2 = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 4C 8D B1 ?? ?? ?? ?? 49 8B 46 08"); // Was 48 8D 05 ?? ?? ?? ?? 48 89 03 49 8D 57 in 1.18.2
	int offset2 = *reinterpret_cast<int *>(sigOffset2 + 3);
	uintptr_t **directoryPackVtable2 = reinterpret_cast<uintptr_t **>(sigOffset2 + offset2 + 7);

	{
		g_Hooks.ZipPackAccessStrategy__isTrustedHook = make_unique<FuncHook>(directoryPackVtable2[6], Hooks::ReturnTrue);
	}
	g_Hooks.SkinRepository___checkSignatureFileInPack = make_unique<FuncHook>(FindSignature("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 79"), Hooks::ReturnTrue);
}
}

// Signatures
{
	void *_sendChatMessage = reinterpret_cast<void *>(FindSignature("48 89 5C 24 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4C 8B EA 4C 8B F9 48 8B 49"));
	g_Hooks.ClientInstanceScreenModel_sendChatMessageHook = make_unique<FuncHook>(_sendChatMessage, Hooks::ClientInstanceScreenModel_sendChatMessage);

	void *_renderCtx = reinterpret_cast<void *>(FindSignature("48 8B ? 48 89 ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 4C 8B ? 48 89 ? ? ? 4C 8B"));
	g_Hooks.RenderTextHook = make_unique<FuncHook>(_renderCtx, Hooks::RenderText);
	g_Hooks.RenderTextHook->enableHook();

	void *setupRender = reinterpret_cast<void *>(FindSignature("48 89 5C 24 10 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B DA 48 8B F9 33 D2 41 B8"));
	g_Hooks.UIScene_setupAndRenderHook = make_unique<FuncHook>(setupRender, Hooks::UIScene_setupAndRender);

	void *render = reinterpret_cast<void *>(FindSignature("48 89 5C 24 18 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B DA 48 8B F9 B9 10 ? ? ?"));
	g_Hooks.UIScene_renderHook = make_unique<FuncHook>(render, Hooks::UIScene_render);

	void *fogColorFunc = reinterpret_cast<void *>(FindSignature("41 0F 10 08 48 8B C2 0F"));
	g_Hooks.Dimension_getFogColorHook = make_unique<FuncHook>(fogColorFunc, Hooks::Dimension_getFogColor);

	void *timeOfDay = reinterpret_cast<void *>(FindSignature("44 8B C2 B8 ? ? ? ? F7 EA"));
	g_Hooks.Dimension_getTimeOfDayHook = make_unique<FuncHook>(timeOfDay, Hooks::Dimension_getTimeOfDay);

	void *chestTick = reinterpret_cast<void *>(FindSignature("48 89 5C 24 ? 56 57 41 56 48 83 EC ? 48 83 79"));
	g_Hooks.ChestBlockActor_tickHook = make_unique<FuncHook>(chestTick, Hooks::ChestBlockActor_tick);

	void *getRenderLayer = reinterpret_cast<void *>(FindSignature("8B 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC F3 0F 10 81"));
	g_Hooks.BlockLegacy_getRenderLayerHook = make_unique<FuncHook>(getRenderLayer, Hooks::BlockLegacy_getRenderLayer);

	void *getLightEmission = reinterpret_cast<void *>(FindSignature("0F B6 81 ? ? ? ? 88 02 48 8B C2"));
	g_Hooks.BlockLegacy_getLightEmissionHook = make_unique<FuncHook>(getLightEmission, Hooks::BlockLegacy_getLightEmission);

	void *autoComplete = reinterpret_cast<void *>(FindSignature("48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 41 8B D9"));
	g_Hooks.PleaseAutoCompleteHook = make_unique<FuncHook>(autoComplete, Hooks::PleaseAutoComplete);

	uintptr_t **packetSenderVtable = reinterpret_cast<uintptr_t **>(*(uintptr_t *)g_Data.getClientInstance()->loopbackPacketSender);
	g_Hooks.LoopbackPacketSender_sendToServerHook = make_unique<FuncHook>(packetSenderVtable[2], Hooks::LoopbackPacketSender_sendToServer);

	void *getFov = reinterpret_cast<void *>(FindSignature("40 53 48 83 EC ?? 0F 29 7C 24 ?? 44"));
	g_Hooks.LevelRendererPlayer_getFovHook = make_unique<FuncHook>(getFov, Hooks::LevelRendererPlayer_getFov);

	void *tick_entityList = reinterpret_cast<void *>(FindSignature("48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? E8 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B D8 ?? ?? ?? ?? ?? ?? 48 99"));
	g_Hooks.MultiLevelPlayer_tickHook = make_unique<FuncHook>(tick_entityList, Hooks::MultiLevelPlayer_tick);

	void *keyMouseFunc = reinterpret_cast<void *>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC ? ? ? ? 0F 29 74 24 ? 0F 29 7C 24 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F0"));
	g_Hooks.HIDController_keyMouseHook = make_unique<FuncHook>(keyMouseFunc, Hooks::HIDController_keyMouse);

	void *renderLevel = reinterpret_cast<void *>(FindSignature("48 89 5C 24 10 48 89 74 24 20 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 49 8B F8 48 8B DA"));
	g_Hooks.LevelRenderer_renderLevelHook = make_unique<FuncHook>(renderLevel, Hooks::LevelRenderer_renderLevel);

	void *playerCallBackHook = reinterpret_cast<void *>(FindSignature("F3 0F ?? ?? ?? ?? 00 00 ?? 0F ?? 00 F3 0F ?? ?? F3 0F ?? ?? 04"));
	g_Hooks.playerCallBack_Hook = make_unique<FuncHook>(playerCallBackHook, Hooks::playerCallBack);

	void *clickHook = reinterpret_cast<void *>(FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9"));
	g_Hooks.ClickFuncHook = make_unique<FuncHook>(clickHook, Hooks::ClickFunc);

	void *chatLogHook = reinterpret_cast<void *>(FindSignature("40 55 57 41 56 48 83 EC ? 48 8B 81"));
	g_Hooks.ChatLogHook = make_unique<FuncHook>(chatLogHook, Hooks::chatLogHookFunc);

	void *chestScreenControllerTick = reinterpret_cast<void *>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B 17"));
	g_Hooks.ChestScreenController_tickHook = make_unique<FuncHook>(chestScreenControllerTick, Hooks::ChestScreenController_tick);

	void *fullbright = reinterpret_cast<void *>(FindSignature("48 83 EC ? 80 B9 ? ? ? ? ? 48 8D 54 24 ? 48 8B 01 74 ? 41 B8 ? ? ? ? FF 50 ? 48 8B 10 48 85 D2 74 ? 48 8B 42 ? 48 8B 88 ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 48 83 C4 ? C3 F3 0F 10 42 ? 48 83 C4 ? C3 41 B8 ? ? ? ? FF 50 ? 48 8B 10 48 85 D2 75 ? E8 ? ? ? ? CC E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC F3 0F 11 4C 24"));
	g_Hooks.GetGammaHook = make_unique<FuncHook>(fullbright, Hooks::GetGamma);

	void *RakNetInstance__tick = reinterpret_cast<void *>(FindSignature("48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D AC 24 20 FD FF FF 48 81 EC E0 03 00 00"));
	g_Hooks.RakNetInstance_tickHook = make_unique<FuncHook>(RakNetInstance__tick, Hooks::RakNetInstance_tick);

	void *ConnectionRequest__create = reinterpret_cast<void *>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 49 8B D9 48 89 55 E8"));
	g_Hooks.ConnectionRequest_createHook = make_unique<FuncHook>(ConnectionRequest__create, Hooks::ConnectionRequest_create);

	void *_getSkinPack = reinterpret_cast<void *>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B EA 48 8B F1"));
	g_Hooks.SkinRepository___loadSkinPackHook = make_unique<FuncHook>(_getSkinPack, Hooks::SkinRepository___loadSkinPack);

	void *lerpFunc = reinterpret_cast<void *>(FindSignature("8B 02 89 81 ? ? ? ? 8B 42 ? 89 81 ? ? ? ? 8B 42 ? 89 81 ? ? ? ? C3 CC CC CC CC CC 48 89 5C 24"));
	g_Hooks.Actor_lerpMotionHook = make_unique<FuncHook>(lerpFunc, Hooks::Actor_lerpMotion);

	void *ForceThirdPerson = reinterpret_cast<void *>(Utils::getBase() + 0xCA3D40); // 48 83 ec ? 48 8b 01 48 8d 54 24 ? 41 b8 ? ? ? ? ff 50 ? 48 8b 10 48 85 d2 74 ? 48 8b 42 ? 48 8b 88 ? ? ? ? 48 85 c9 74 ? e8 ? ? ? ? 48 83 c4 ? c3 8b 42 ? 48 83 c4 ? c3 e8 ? ? ? ? cc cc cc cc cc cc cc cc cc cc cc cc 48 83 ec ? 48 8b 01 48 8d 54 24 ? 41 b8 ? ? ? ? ff 50 ? 48 8b 10 48 85 d2 74 ? 48 8b 42 ? 48 8b 88 ? ? ? ? 48 85 c9 74 ? e8 ? ? ? ? 48 83 c4 ? c3 8b 42 ? 48 83 c4 ? c3 e8 ? ? ? ? cc cc cc cc cc cc cc cc cc cc cc cc 48 83 ec ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 44 24 ? 48 8b 01 41 b8 ? ? ? ? 88 54 24 ? 48 8d 54 24 ? ff 50 ? 48 8b 08 48 85 c9 74 ? 48 8d 54 24 ? e8 ? ? ? ? 48 8b 4c 24 ? 48 33 cc e8 ? ? ? ? 48 83 c4 ? c3 e8 ? ? ? ? cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc 48 83 ec ? 48 8b 01 48 8d 54 24 ? 41 b8 ? ? ? ? ff 50 ? 48 8b 10 48 85 d2 74 ? 48 8b 42 ? 48 8b 88 ? ? ? ? 48 85 c9 74 ? e8 ? ? ? ? 48 83 c4 ? c3 0f b6 42 ? 48 83 c4 ? c3 e8 ? ? ? ? cc cc cc cc cc cc cc cc cc cc cc 48 83 ec ? 48 8b 01
	g_Hooks.forceThirdPerson = make_unique<FuncHook>(ForceThirdPerson, Hooks::ForceThirdPersonLol);

	void *destroySpeed = reinterpret_cast<void *>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B FA 0F 29 74 24 ? 48 8B 91"));
	g_Hooks.getDestroySpeedHook = make_unique<FuncHook>(destroySpeed, Hooks::getDestroySpeed);

	void* swingSpeed = reinterpret_cast<void*>(Utils::getBase() + 0x20029F0);
	g_Hooks.Mob__GetCurrentSwingDuration = make_unique<FuncHook>(swingSpeed, Hooks::Mob_GetCurrentSwingDuration);

#ifdef TEST_DEBUG
	// tf?
	void *addAction = reinterpret_cast<void *>(FindSignature("55 56 57 41 56 41 57 48 83 EC ?? 45 0F B6 F8 4C 8B F2 48 8B F1 48 8B 01 48 8B 88") - 5);
	g_Hooks.InventoryTransactionManager__addActionHook = make_unique<FuncHook>(addAction, Hooks::InventoryTransactionManager__addAction);
#endif

	void *localPlayerUpdateFromCam = reinterpret_cast<void *>(FindSignature("48 89 5C 24 10 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 80"));
	g_Hooks.LocalPlayer__updateFromCameraHook = make_unique<FuncHook>(localPlayerUpdateFromCam, Hooks::LocalPlayer__updateFromCamera);

	static constexpr auto counterStart = __COUNTER__ + 1;
#define lambda_counter (__COUNTER__ - counterStart)

	void *levelRendererBobView = reinterpret_cast<void *>(FindSignature("48 8B C4 48 89 58 20 57 48 ?? ?? ?? ?? 00 00 0F ?? ?? ?? 0F ?? ?? ?? ?? 0F ?? ?? ?? ?? 0F ?? ?? ??"));

	static auto bobViewHookF = [](__int64 _this, glm::mat4 &matrix, float lerpT)
	{
		static auto origFunc = g_Hooks.lambdaHooks.at(lambda_counter)->GetFastcall<void, __int64, glm::mat4 &, float>();
		static auto animations = moduleMgr->getModule<Animations>();
		static auto viewMod = moduleMgr->getModule<ViewModel>();
		static auto killaura = moduleMgr->getModule<Killaura>();
		auto p = g_Data.getLocalPlayer();
		float degrees = fmodf(p->getPosOld()->lerp(p->getPos(), lerpT).x, 5) - 2.5f;
		auto pos = g_Data.getClientInstance()->levelRenderer->getOrigin();
		degrees *= 180 / 2.5f;

		glm::mat4 View = matrix;
		matrix = View;
		if (viewMod->isEnabled()) {
			if (viewMod->mode.getSelectedValue() == 0) {
				if (viewMod->translate)
					matrix = glm::translate<float>(matrix, glm::vec3(viewMod->xPos, viewMod->yPos, viewMod->zPos));

				if (viewMod->scale)
					matrix = glm::scale<float>(matrix, glm::vec3(viewMod->xScale, viewMod->yScale, viewMod->zScale));

				if (viewMod->rotate)
					matrix = glm::rotate<float>(matrix, degrees, glm::vec3(viewMod->xRotate, viewMod->yRotate, viewMod->zRotate));
			}
			else if (viewMod->mode.getSelectedValue() == 1) {
				if (viewMod->translate && !g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->getItem()->isFood() && g_Data.isRightClickDown())
					matrix = glm::translate<float>(matrix, glm::vec3(viewMod->xPos, viewMod->yPos, viewMod->zPos));

				if (viewMod->scale && !g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->getItem()->isFood() && g_Data.isRightClickDown())
					matrix = glm::scale<float>(matrix, glm::vec3(viewMod->xScale, viewMod->yScale, viewMod->zScale));

				if (viewMod->rotate && !g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->getItem()->isFood() && g_Data.isRightClickDown())
					matrix = glm::rotate<float>(matrix, degrees, glm::vec3(viewMod->xRotate, viewMod->yRotate, viewMod->zRotate));
			}
		}
		// Blocking Animation
		auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
		if (animations->isEnabled())
		{
			if (animations->shouldBlock && g_Data.canUseMoveKeys() && g_Data.isInGame() && !clickGUI->isEnabled())
			{
				// 1.7
				if (animations->mode.getSelectedValue() == 1)
				{
					lerpT = 0.f;
					matrix = glm::translate<float>(matrix, glm::vec3(0.42222223281, 0.0, -0.16666666269302368));
					matrix = glm::translate<float>(matrix, glm::vec3(-0.1f, 0.15f, -0.2f));
					blockRotate(matrix, 0.25f);
				}

				// Slide
				if (animations->mode.getSelectedValue() == 2)
				{
					matrix = glm::translate<float>(matrix, glm::vec3(0.42222223281, 0.0, -0.16666666269302368));
					matrix = glm::translate<float>(matrix, glm::vec3(-0.15f, 0.15f, -0.2f));
					slideBlockRotate(matrix, 0.2f);
				}

				// Old
				if (animations->mode.getSelectedValue() == 3)
				{
					matrix = glm::translate<float>(matrix, glm::vec3(5.54, 0.85, -2.00));
					matrix = glm::scale<float>(matrix, glm::vec3(2, 2, 2));
					float degrees = 13;
					degrees *= 180 / 4;

					auto pos = g_Data.getClientInstance()->levelRenderer->getOrigin();

					glm::mat4 View = matrix;

					matrix = View;
					matrix = glm::rotate<float>(matrix, glm::radians<float>(degrees), glm::vec3(2.58, -4.40, -3.50));
					matrix = glm::translate<float>(matrix, glm::vec3(1.08, -0.02, -0.02));
					matrix = glm::scale<float>(matrix, glm::vec3(4, 4, 4));
					matrix = glm::translate<float>(matrix, glm::vec3(0.5, 0.4, 0.4));
				}

				// Spin
				if (animations->mode.getSelectedValue() == 4)
				{
					auto player = g_Data.getLocalPlayer();
					float degrees = fmodf(player->getPosOld()->lerp(player->getPos(), lerpT).x, 5) - 2.5f;
					degrees *= 180 / 2.5f;

					auto pos = g_Data.getClientInstance()->levelRenderer->getOrigin();

					glm::mat4 View = matrix;

					matrix = View;
					matrix = glm::rotate<float>(matrix, glm::radians<float>(degrees), glm::vec3(0, 0, 1));
				}
			}
		}
		return origFunc(_this, matrix, lerpT);
	};

	auto urfather = NULL;																											  // dis very tru
	shared_ptr<FuncHook> bobViewHook = make_shared<FuncHook>(levelRendererBobView, (decltype(&bobViewHookF.operator()))bobViewHookF); // i wanna fix but dk how, fuck

	g_Hooks.lambdaHooks.push_back(bobViewHook);

#undef lambda_counter
	logF("Hooks initialized");
}

// clang-format on
}

void Hooks::Restore()
{
	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(10);
}

void Hooks::Enable()
{
	logF("Hooks enabled");
	MH_EnableHook(MH_ALL_HOOKS);
}
void Hooks::Verify()
{
}
int Hooks::ForceThirdPersonLol(__int64 a1)
{
	static auto func = g_Hooks.forceThirdPerson->GetFastcall<int, __int64>();
	if (a1 == 1)
		g_Hooks.isThirdPerson = true;
	if (moduleMgr->getModule<Freecam>()->isEnabled())
		return 1;
	else
		return func(a1);
}

bool Hooks::playerCallBack(C_Player *lp, __int64 a2, __int64 a3)
{
	static auto oTick = g_Hooks.playerCallBack_Hook->GetFastcall<bool, C_Player *, __int64, __int64>();
	if (moduleMgr != nullptr && lp != nullptr && g_Data.getLocalPlayer() != nullptr && lp == g_Data.getLocalPlayer())
		moduleMgr->onPlayerTick(lp);

	if (!g_Data.getLocalPlayer() || !g_Data.getLocalPlayer()->pointingStruct || !*(&g_Data.getLocalPlayer()->region + 1) || !g_Data.isInGame())
		g_Hooks.entityList.clear();

	if (g_Data.getLocalPlayer() != nullptr && lp == g_Data.getLocalPlayer())
	{
		std::vector<EntityListPointerHolder> validEntities;
		for (const auto &ent : g_Hooks.entityList)
		{
			auto entity = ent.ent;
			MEMORY_BASIC_INFORMATION info;
			VirtualQuery(ent.ent, &info, sizeof(MEMORY_BASIC_INFORMATION));
			if (info.State & MEM_FREE)
				continue;
			if (info.State & MEM_RESERVE)
				continue;

			if (entity == nullptr)
				continue;

			if (entity != nullptr && (__int64)entity != 0xFFFFFFFFFFFFFCD7 && ent.ent != nullptr && *(__int64 *)ent.ent != 0xFFFFFFFFFFFFFCD7 && *(__int64 *)ent.ent > 0x6FF000000000 && *(__int64 *)ent.ent < 0x800000000000 && *((int64_t *)ent.ent + 1) < 0x6FF000000000 && *(__int64 *)ent.ent <= (__int64)(Utils::getBase() + 0x10000000) && entity->isAlive())
				validEntities.push_back(ent);
		}
		g_Hooks.entityList.clear();
		g_Hooks.entityList = validEntities;
	}
	return oTick(lp, a2, a3);
}

void *Hooks::Player_tickWorld(C_Player *_this, __int64 unk)
{
	static auto oTick = g_Hooks.Player_tickWorldHook->GetFastcall<void *, C_Player *, __int64>();
	auto o = oTick(_this, unk);

	if (_this == g_Data.getLocalPlayer())
	{
		// TODO: refactor all modules to not use GameMode
		C_GameMode *gm = *reinterpret_cast<C_GameMode **>(reinterpret_cast<__int64>(_this) + 0x1238);
		GameData::updateGameData(gm);
		moduleMgr->onWorldTick(gm);
	}
	return o;
}

float Hooks::getDestroySpeed(C_Player *_this, C_Block &block)
{
	static auto oFunc = g_Hooks.getDestroySpeedHook->GetFastcall<float, C_Player *, C_Block &>();
	return oFunc(_this, block);
}

void Hooks::ClientInstanceScreenModel_sendChatMessage(void *_this, TextHolder *text)
{
	static auto oSendMessage = g_Hooks.ClientInstanceScreenModel_sendChatMessageHook->GetFastcall<void, void *, TextHolder *>();

	if (text->getTextLength() > 0)
	{
		char *message = text->getText();

		if (*message == cmdMgr->prefix)
		{
			cmdMgr->execute(message);

			return;
		}
		else if (*message == '.')
		{
			static bool helpedUser = false;
			if (!helpedUser)
			{
				helpedUser = true;
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sYour prefix is: \"%s%c%s\"", GRAY, YELLOW, cmdMgr->prefix, RED);
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sEnter \"%s%cprefix .%s\" to reset your prefix", GRAY, YELLOW, cmdMgr->prefix, RED);
			}
		}
	}
	oSendMessage(_this, text);
}

void Hooks::Actor_baseTick(C_Entity *ent)
{
	static auto oFunc = g_Hooks.Actor__baseTick->GetFastcall<void, C_Entity *>();
	C_LocalPlayer *player = g_Data.getLocalPlayer();
	if (!player || !player->getPointingStruct())
		return oFunc(ent);

	static int tickCountThen = 0;
	int tickCountNow = *(int *)((__int64)player->getPointingStruct() + 0x690);

	if (tickCountNow != tickCountThen)
	{
		g_Hooks.entityList.clear();
		tickCountThen = tickCountNow;
	}
	if (ent->isClientSide())
	{
		EntityListPointerHolder e;
		e.addedTick = tickCountNow;
		e.ent = ent;

		bool found = false;
		for (const auto &entity : g_Hooks.entityList)
			if (entity.ent == ent && entity.addedTick == tickCountNow)
			{
				found = true;
				break;
			}

		if (!found)
			g_Hooks.entityList.push_back(e);
	}
	return oFunc(ent);
}

void Hooks::Actor_rotation(C_Entity *_this, vec2_t &angle)
{
	static auto oFunc = g_Hooks.Actor_rotationHook->GetFastcall<void, C_Entity *, vec2_t &>();
	oFunc(_this, angle);
}

__int64 Hooks::UIScene_setupAndRender(C_UIScene *uiscene, __int64 screencontext)
{
	static auto oSetup = g_Hooks.UIScene_setupAndRenderHook->GetFastcall<__int64, C_UIScene *, __int64>();

	g_Hooks.shouldRender = false;
	// g_Hooks.shouldRender = uiscene->isPlayScreen();

	return oSetup(uiscene, screencontext);
}

__int64 Hooks::UIScene_render(C_UIScene *uiscene, __int64 screencontext)
{
	static auto oRender = g_Hooks.UIScene_renderHook->GetFastcall<__int64, C_UIScene *, __int64>();
	static auto chestStealer = moduleMgr->getModule<ChestStealer>();
	static auto clickGUIMod = moduleMgr->getModule<ClickGUIMod>();
	// static auto invManager = moduleMgr->getModule<InvManager>();
	static auto scaffold = moduleMgr->getModule<Scaffold>();

	g_Hooks.shouldRender = false;

	TextHolder alloc{};
	uiscene->getScreenName(&alloc);

	if (alloc.getTextLength() < 100)
	{
		strcpy_s(g_Hooks.currentScreenName, alloc.getText());
	}

	if (!g_Hooks.shouldRender)
	{
		g_Hooks.shouldRender = (strcmp(alloc.getText(), "start_screen") == 0 || strcmp(alloc.getText(), "hud_screen") == 0);
	}

	if (clickGUIMod->isEnabled() && strcmp(alloc.getText(), "pause_screen") == 0 || strcmp(alloc.getText(), "respawn_screen") == 0 || strcmp(alloc.getText(), "disconnect_screen") == 0)
	{
		clickGUIMod->setEnabled(false);
		g_Data.getClientInstance()->grabMouse();
	}
	alloc.alignedTextLength = 0;

	return oRender(uiscene, screencontext);
}

__int64 Hooks::RenderText(__int64 a1, C_MinecraftUIRenderContext* renderCtx)
{
	static auto oText = g_Hooks.RenderTextHook->GetFastcall<__int64, __int64, C_MinecraftUIRenderContext*>();
	C_GuiData* dat = g_Data.getClientInstance()->getGuiData();

	DrawUtils::setCtx(renderCtx, dat);
	ImGuiUtils::ctx(renderCtx, dat);

	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();

	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;

		mousePos = mousePos.div(windowSizeReal);
		mousePos = mousePos.mul(windowSize);
	}

	static bool leftClickDown;
	leftClickDown = g_Data.isLeftClickDown();
	string screenName(g_Hooks.currentScreenName);

	// DrawUtils::drawText(vec2_t(18, 18), &screenName, MC_Color(255, 255, 255), 1, 1);

	if (strcmp(screenName.c_str(), "nether_loading_progress_screen - nether") == 0)
	{
		static auto chestStealer = moduleMgr->getModule<ChestStealer>();
		// static auto invManager = moduleMgr->getModule<InvManager>();
		static auto killaura = moduleMgr->getModule<Killaura>();
		static auto phase = moduleMgr->getModule<Phase>();

		if (chestStealer->isEnabled() && chestStealer->autoDisable)
			chestStealer->setEnabled(false);
		if (phase->isEnabled())
			;
	}
	if (strcmp(screenName.c_str(), "inventory_screen") == 0 || strcmp(screenName.c_str(), "small_chest_screen") == 0 || strcmp(screenName.c_str(), "large_chest_screen") == 0 || strcmp(screenName.c_str(), "ender_chest_screen") == 0 || strcmp(screenName.c_str(), "shulker_box_screen") == 0)
	{
	}

	if (isHUDHidden || GameData::shouldHide() || !g_Hooks.shouldRender || !moduleMgr->isInitialized())
		return oText(a1, renderCtx);

	static auto configManager = moduleMgr->getModule<ConfigManagerMod>();
	static auto notificationsMod = moduleMgr->getModule<Notifications>();
	static auto clickGUIMod = moduleMgr->getModule<ClickGUIMod>();
	static auto interfaceMod = moduleMgr->getModule<Interface>();
	// static auto playerList = moduleMgr->getModule<PlayerList>();
	static auto arraylist = moduleMgr->getModule<ArrayList>();
	static auto watermark = moduleMgr->getModule<Watermark>();
	static PacketMine* mod = moduleMgr->getModule<PacketMine>();
	HImGui.startFrame();

	g_Data.frameCount++;

		auto wid = g_Data.getClientInstance()->getGuiData()->windowSize;

		// Call PreRender() functions
		moduleMgr->onPreRender(renderCtx);
		DrawUtils::flush();

		__int64 retval = oText(a1, renderCtx);

		bool shouldPostRender = true;
		bool shouldRenderTabGui = true;

		{
			// Main Menu
			string screenName(g_Hooks.currentScreenName);
			if (strcmp(screenName.c_str(), "start_screen") == 0)
			{

			}
			else
			{
				shouldRenderTabGui = interfaceMod->tabGUI && interfaceMod->isEnabled();

				// ClickGUI
				if (clickGUIMod->isEnabled())
				{
					if (clickGUIMod->hasOpenedGUI)
					{
						ClickGui::render();
						shouldPostRender = true;
						shouldRenderTabGui = false;
					}
				}

				// ConfigManager
				if (configManager->isEnabled())
					ConfigManagerMenu::render();

				if (shouldRenderTabGui)
					TabGui::render();
			}
		}

		if (shouldPostRender)
			moduleMgr->onPostRender(renderCtx);
		// PACKETMINE!
		C_GameMode* gm = g_Data.getCGameMode();
		if (g_Data.getLocalPlayer() != nullptr && g_Data.getCGameMode() != nullptr) {
			if (mod->isEnabled()) {
				auto distance = mod->breakPos.toVector3().dist(g_Data.getLocalPlayer()->getHumanPos());

				if (distance >= moduleMgr->getModule<PacketMine>()->swapDel)
					mod->breakPos == vec3_ti(0, 0, 0);

				// Continuous
				float increasingMine = (mod->breakPos == vec3_ti(0, 0, 0)) ? 0.f : increasingMine;
				vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(mod->breakPos.toVector3());

				if (mod->breakPos != vec3_ti(0, 0, 0) && mod->mineType.getSelectedValue() == 0) {
					C_Block* block = g_Data.getLocalPlayer()->region->getBlock(mod->breakPos);

					if (block != nullptr && block->blockLegacy->blockId != 0 && block->blockLegacy->blockId != 7) {
						C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
						C_Inventory* inv = supplies->inventory;
						int slotP = supplies->selectedHotbarSlot;
						int prev22 = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;

						if (destroyrate > 1.2f)
							destroyrate = mod->increase;

						if (destroyrate >= 0.95f) {
							if (mod->swap.getSelectedValue() == 1) {
								mod->origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
								mod->findAPickaxe();
							}

							if (mod->swap.getSelectedValue() == 2) {
								for (int n = 0; n < 9; n++) {
									C_ItemStack* stack = inv->getItemStack(n);
									if (stack->item != nullptr && (*stack->item)->itemId == 49) {
										slotP = n;
									}
									mod->findAPickaxe();
									supplies->selectedHotbarSlot = slotP;
									
								}
							}
						}

						// Rotate only when breaking is almost done
						//if (destroyrate >= 0.967f) {
						//	g_Data.getLocalPlayer()->setRot(angle);
						//	g_Data.getLocalPlayer()->yawUnused1 = angle.y;
						//}

						if (destroyrate >= 1.f) {
							gm->destroyBlock(&mod->breakPos, mod->face);

							if (mod->swap.getSelectedValue() == 2)
								supplies->selectedHotbarSlot = prev22;

							if (mod->swap.getSelectedValue() == 1)
								mod->returnSlot();
						}
						else {
							destroyrate += 1.3f / 130.f * (60.f / g_Data.fps);
						}
					}
					else {
						destroyrate = mod->increase;
					}
				}




				// Instant
				if (mod->breakPos != vec3_ti(0, 0, 0) && mod->mineType.getSelectedValue() == 1) {
					C_Block* block = g_Data.getLocalPlayer()->region->getBlock(mod->breakPos);
					if (block != nullptr && block->blockLegacy->blockId != 0 && block->blockLegacy->blockId != 7) {
						destroyrate = 0.99;
						if (destroyrate >= 1.f) {
							gm->destroyBlock(&mod->breakPos, mod->face);
							destroyrate = 0.99f;
						}
						else {
							destroyrate += 1.f / 135.f * (60.f / g_Data.fps);
						}
						vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(mod->breakPos.toVector3());


						//g_Data.getLocalPlayer()->setRot(angle);
					}
					else {
						angle = vec2_t(0, 0);
						destroyrate = 0.99f;
					}

				}
				// Instant Strict
				if (mod->breakPos != vec3_ti(0, 0, 0) && mod->mineType.getSelectedValue() == 2) {
					C_Block* block = g_Data.getLocalPlayer()->region->getBlock(mod->breakPos);
					if (block != nullptr && block->blockLegacy->blockId != 0 && block->blockLegacy->blockId != 7) {
						if (destroyrate >= 0.90f && mod->swap.getSelectedValue() != 0) {
							mod->hasSwapped = true;
						}
						if (destroyrate >= 1.f) {
							gm->destroyBlock(&mod->breakPos, mod->face);
							destroyrate = 0.99f;
						}
						else {
							destroyrate += 1.f / 135.f * (60.f / g_Data.fps);
						}
					}
					else {
						angle = vec2_t(0, 0);
						destroyrate = 0.99f;
					}
					


					//g_Data.getLocalPlayer()->setRot(angle);
				}
				// Once
				if (mod->breakPos != vec3_ti(0, 0, 0) && mod->mineType.getSelectedValue() == 3) {
					C_Block* block = g_Data.getLocalPlayer()->region->getBlock(mod->breakPos);
					if (block != nullptr && block->blockLegacy->blockId != 0 && block->blockLegacy->blockId != 7) {
						C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
						C_Inventory* inv = supplies->inventory;
						int slotP = supplies->selectedHotbarSlot;
						int prev222 = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
						if (destroyrate >= 0.95f) {
							if (mod->swap.getSelectedValue() == 1) {
								mod->origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
								mod->findAPickaxe();
							}
							if (mod->swap.getSelectedValue() == 2) {
								for (int n = 0; n < 9; n++) {
									C_ItemStack* stack = inv->getItemStack(n);
									if (stack->item != NULL && (*stack->item)->itemId == 49) {
										slotP = n;
									}
									supplies->selectedHotbarSlot = slotP;
								}
							}
						}
						if (destroyrate >= 1.f) {
							gm->destroyBlock(&mod->breakPos, mod->face);
							mod->breakPos = vec3_ti(0, 0, 0);
							if (mod->swap.getSelectedValue() == 1)
								mod->returnSlot();
						}
						else {
							destroyrate += 1.f / 135.f * (60.f / g_Data.fps);
						}
						


						//g_Data.getLocalPlayer()->setRot(angle);

						if (mod->swap.getSelectedValue() == 2)
							supplies->selectedHotbarSlot = prev222;
					}
					else {
						angle = vec2_t(0, 0);
						destroyrate = mod->increase;
					}
				}
				if (destroyrate > 0.f) {
					// Renders
					auto blockPos = mod->breakPos.toVector3().add(0.5, 0.5, 0.5);
					float test = destroyrate;
					float opac = mod->opacity * 2;
					float progress2(destroyrate * 100);
					std::ostringstream oss;
					oss << std::fixed << std::setprecision(2) << progress2;
					std::string currentProgress = oss.str() + "%";
					if (mod->renderProgress && mod->breakPos != vec3_ti(0, 0, 0))
					DrawUtils::drawTextInWorld(&currentProgress, mod->breakPos.toVector3().add(0, 0, 0), 0.7f, vec3_ti(180, 180, 180), vec3_ti(12, 12, 12), 0.f);
					// None
					if (mod->anim.getSelectedValue() == 0) {
						// Outline
						if (mod->render.getSelectedValue() == 3 || mod->render.getSelectedValue() == 2) {
							DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, 1.f);
							DrawUtils::drawBox(mod->breakPos.toVector3().sub(0, 0, 0), mod->breakPos.toVector3().add(1, 1, 1), .5f, false);
						}
						// Fill Render
						if (mod->render.getSelectedValue() == 1 || mod->render.getSelectedValue() == 3) {
							DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, mod->opacity);
							DrawUtils::drawFilledBox(blockPos);
						}
					}
					// Grow
					if (mod->anim.getSelectedValue() == 1) {
						// Outline
						if (mod->render.getSelectedValue() == 3 || mod->render.getSelectedValue() == 2) {
							DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, 1.f);
							DrawUtils::drawBox(mod->breakPos.toVector3().add(0.5f).sub(0.5f * test), mod->breakPos.toVector3().add(0.5f).add(0.5f * test), 0.5f, false, true);
						}
						// Fill Render
						if (mod->render.getSelectedValue() == 1 || mod->render.getSelectedValue() == 3) {
							DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, mod->opacity);
							DrawUtils::drawFilledBox(blockPos, true, test * 28);
						}
					}
					// Fade
					if (mod->anim.getSelectedValue() == 2) {
						// Outline
						if (mod->render.getSelectedValue() == 3 || mod->render.getSelectedValue() == 2) {
							DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, 0.1f * test * 2 * opac);
							DrawUtils::drawBox(mod->breakPos.toVector3().sub(0, 0, 0), mod->breakPos.toVector3().add(1, 1, 1), .5f, false);
						}
						// Fill Render
						if (mod->render.getSelectedValue() == 1 || mod->render.getSelectedValue() == 3) {
							DrawUtils::setColor(1.f - 1.f * test, 1.f * test, 0.f, 0.1f * test * 2 * opac);
							DrawUtils::drawFilledBox(blockPos);
						}
					}
				}
				
			}
		}
		HImGui.endFrame();
		DrawUtils::flush();


		DrawUtils::flush();

		return retval;
}

float *Hooks::Dimension_getFogColor(__int64 _this, float *color, __int64 a3, float a4)
{
	static auto oGetFogColor = g_Hooks.Dimension_getFogColorHook->GetFastcall<float *, __int64, float *, __int64, float>();
	return oGetFogColor(_this, color, a3, a4);
}

float Hooks::Dimension_getTimeOfDay(__int64 _this, int a2, float a3)
{
	static auto oGetTimeOfDay = g_Hooks.Dimension_getTimeOfDayHook->GetFastcall<float, __int64, int, float>();
	return oGetTimeOfDay(_this, a2, a3);
}

float Hooks::Dimension_getSunIntensity(__int64 a1, float a2, vec3_t *a3, float a4)
{
	static auto oGetSunIntensity = g_Hooks.Dimension_getSunIntensityHook->GetFastcall<float, __int64, float, vec3_t *, float>();

	return oGetSunIntensity(a1, a2, a3, a4);
}

void Hooks::ChestBlockActor_tick(C_ChestBlockActor *_this, void *a)
{
	static auto oTick = g_Hooks.ChestBlockActor_tickHook->GetFastcall<void, C_ChestBlockActor *, void *>();
	oTick(_this, a);
	static auto *phase = moduleMgr->getModule<Phase>();
	static auto *stashFinder = moduleMgr->getModule<StashFinder>();

	if (stashFinder->isEnabled() && _this != nullptr)
		StashFinder::chestNotify(_this);

	if (_this != nullptr)
		GameData::addChestToList(_this);
}

void Hooks::Actor_lerpMotion(C_Entity *_this, vec3_t motVec)
{
	static auto oLerp = g_Hooks.Actor_lerpMotionHook->GetFastcall<void, C_Entity *, vec3_t>();

	if (g_Data.getLocalPlayer() != nullptr && g_Data.getLocalPlayer() != _this)
		return oLerp(_this, motVec);

	static auto velocity = moduleMgr->getModule<Velocity>();
	if (velocity->isEnabled())
	{
		static void *networkSender = nullptr;
		if (!networkSender)
			networkSender = reinterpret_cast<void *>(9 + FindSignature("48 8B CB FF ?? ?? ?? ?? 00 C6 47 ?? 01 48 8B 5C 24"));

		if (networkSender == _ReturnAddress())
			motVec = _this->velocity.lerp(motVec, velocity->x, velocity->y, velocity->x);
	}

	oLerp(_this, motVec);
}

void Hooks::PleaseAutoComplete(__int64 a1, __int64 a2, TextHolder *text, int a4)
{
	static auto oAutoComplete = g_Hooks.PleaseAutoCompleteHook->GetFastcall<void, __int64, __int64, TextHolder *, int>();
	char *tx = text->getText();
	if (tx != nullptr && text->getTextLength() >= 1 && tx[0] == '.')
	{
		string search = tx + 1;												// Dont include the '.'
		transform(search.begin(), search.end(), search.begin(), ::tolower); // make the search text lowercase

		struct LilPlump
		{
			string cmdAlias;
			IMCCommand *command = 0;
			bool shouldReplace = true; // Should replace the current text in the box (autocomplete)

			bool operator<(const LilPlump &o) const
			{
				return cmdAlias < o.cmdAlias;
			}
		}; // This is needed so the set sorts it alphabetically

		set<LilPlump> searchResults;

		vector<IMCCommand *> *commandList = cmdMgr->getCommandList();
		for (auto it = commandList->begin(); it != commandList->end(); ++it)
		{ // Loop through commands
			IMCCommand *c = *it;
			auto *aliasList = c->getAliasList();
			for (auto it = aliasList->begin(); it != aliasList->end(); ++it)
			{ // Loop through aliases
				string cmd = *it;
				LilPlump plump;

				for (size_t i = 0; i < search.size(); i++)
				{ // Loop through search string
					char car = search.at(i);
					if (car == ' ' && i == cmd.size())
					{
						plump.shouldReplace = false;
						break;
					}
					else if (i >= cmd.size())
						goto outerContinue;

					if (car != cmd.at(i)) // and compare
						goto outerContinue;
				}
				// Not at outerContinue? Then we got a good result!
				{
					cmd.insert(0, 1, '.'); // Prepend the '.'

					plump.cmdAlias = cmd;
					plump.command = c;
					searchResults.emplace(plump);
				}

			outerContinue:
				continue;
			}
		}

		if (!searchResults.empty())
		{
			LilPlump firstResult = *searchResults.begin();

			size_t maxReplaceLength = firstResult.cmdAlias.size();
			if (searchResults.size() > 1)
			{
				for (auto it = searchResults.begin()++; it != searchResults.end(); it++)
				{
					auto alias = it->cmdAlias;
					maxReplaceLength = min(maxReplaceLength, alias.size());

					for (int i = 0; i < maxReplaceLength; i++)
					{
						if (alias[i] != firstResult.cmdAlias[i])
						{
							maxReplaceLength = i;
							break;
						}
					}
				}
			}
			else
				maxReplaceLength = firstResult.cmdAlias.size();

			g_Data.getGuiData()->displayClientMessageF("==========");
			if (searchResults.size() > 1)
			{
				for (auto it = searchResults.begin(); it != searchResults.end(); ++it)
				{
					LilPlump plump = *it;
					g_Data.getGuiData()->displayClientMessageF("%s%s - %s%s", plump.cmdAlias.c_str(), GRAY, ITALIC, plump.command->getDescription());
				}
			}
			if (firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1)[0] == 0)
				g_Data.getGuiData()->displayClientMessageF("%s%s %s- %s", WHITE, firstResult.cmdAlias.c_str(), GRAY, firstResult.command->getDescription());
			else
				g_Data.getGuiData()->displayClientMessageF("%s%s %s %s- %s", WHITE, firstResult.cmdAlias.c_str(), firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1 /*exclude prefix*/), GRAY, firstResult.command->getDescription());

			if (firstResult.shouldReplace)
			{
				if (search.size() == firstResult.cmdAlias.size() - 1 && searchResults.size() == 1)
				{
					maxReplaceLength++;
					firstResult.cmdAlias.append(" ");
				}
				text->setText(firstResult.cmdAlias.substr(0, maxReplaceLength)); // Set text
				using syncShit = void(__fastcall *)(TextHolder *, TextHolder *);
				static syncShit sync = reinterpret_cast<syncShit>(Utils::FindSignature("40 53 48 83 EC ? 48 8B DA 48 8D 4C 24 ? E8 ? ? ? ? 90 48 8B 40 ? 48 8B 08 48 8B 01 48 8B D3 FF 90 ? ? ? ? 90 F0 48 FF 0D ? ? ? ? 48 8B 44 24 ? 48 85 C0 74 ? 48 83 38 ? 74 ? 80 7C 24 ? ? 74 ? F0 48 FF 0D ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? 80 7C 24 ? ? 74 ? E8 ? ? ? ? C6 44 24 ? ? 48 8D 4C 24 ? E8 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 80 7C 24 ? ? 74 ? 48 8B 4C 24 ? E8 ? ? ? ? 90 48 83 C4 ? 5B C3 B9 ? ? ? ? E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 40 53"));
				sync(text, text);
			}
		}
		return;
	}
	oAutoComplete(a1, a2, text, a4);
}

void Hooks::LoopbackPacketSender_sendToServer(C_LoopbackPacketSender *a, C_Packet *packet)
{
	static auto oFunc = g_Hooks.LoopbackPacketSender_sendToServerHook->GetFastcall<void, C_LoopbackPacketSender *, C_Packet *>();

	static auto freecam = moduleMgr->getModule<Freecam>();
	static auto sneakMod = moduleMgr->getModule<Sneak>();
	static auto blinkMod = moduleMgr->getModule<Blink>();
	static auto speed = moduleMgr->getModule<Speed>();
	bool noPacket = false;

	if (noPacket && g_Data.isInGame())
		return;

	if (blinkMod->isEnabled() || (freecam->isEnabled() && freecam->cancelPackets))
	{
		if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>())
		{
			if (blinkMod->isEnabled())
			{
				if (packet->isInstanceOf<C_MovePlayerPacket>())
				{
					C_MovePlayerPacket *movePacket = reinterpret_cast<C_MovePlayerPacket *>(packet);
					movePacket->onGround = true;
					blinkMod->getMovePlayerPacketHolder()->push_back(new C_MovePlayerPacket(*movePacket));
				}
				else
					blinkMod->getPlayerAuthInputPacketHolder()->push_back(new PlayerAuthInputPacket(*reinterpret_cast<PlayerAuthInputPacket *>(packet)));
				return;
			}
		}
	}
	else if (!blinkMod->isEnabled())
	{
		if (blinkMod->getMovePlayerPacketHolder()->size() > 0)
		{
			for (auto it : *blinkMod->getMovePlayerPacketHolder())
			{
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getMovePlayerPacketHolder()->clear();
			return;
		}
		if (blinkMod->getPlayerAuthInputPacketHolder()->size() > 0)
		{
			for (auto it : *blinkMod->getPlayerAuthInputPacketHolder())
			{
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getPlayerAuthInputPacketHolder()->clear();
			return;
		}
	}

	moduleMgr->onSendPacket(packet);

	oFunc(a, packet);
}

float Hooks::LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3)
{
	static auto oGetFov = g_Hooks.LevelRendererPlayer_getFovHook->GetFastcall<float, __int64, float, bool>();
	return oGetFov(_this, a2, a3);
}

void Hooks::MultiLevelPlayer_tick(C_EntityList *_this)
{
	static auto oTick = g_Hooks.MultiLevelPlayer_tickHook->GetFastcall<void, C_EntityList *>();
	C_GameMode *gm = g_Data.getCGameMode();
	if (gm != nullptr)
		moduleMgr->onTick(gm);
	oTick(_this);
	GameData::EntityList_tick(_this);
}

void Hooks::GameMode_startDestroyBlock(C_GameMode *_this, vec3_ti *a2, uint8_t face, void *a4, void *a5)
{
	static auto oFunc = g_Hooks.GameMode_startDestroyBlockHook->GetFastcall<void, C_GameMode *, vec3_ti *, uint8_t, void *, void *>();
	static PacketMine* mod = moduleMgr->getModule<PacketMine>();
	if (mod->isEnabled()) {
		mod->breakPos = *a2;
		*(float*)((__int64)_this + 0x24) = 0.f;
		mod->face = face;
	}
	else {
		mod->breakPos = vec3_ti(0, 0, 0);
		mod->face = 0;
	}
	oFunc(_this, a2, face, a4, a5);
}

void Hooks::HIDController_keyMouse(C_HIDController *_this, void *a2, void *a3)
{
	static auto oFunc = g_Hooks.HIDController_keyMouseHook->GetFastcall<void, C_HIDController *, void *, void *>();
	GameData::setHIDController(_this);
	isTicked = true;
	oFunc(_this, a2, a3);
	return;
}

int Hooks::BlockLegacy_getRenderLayer(C_BlockLegacy *a1)
{
	static auto oFunc = g_Hooks.BlockLegacy_getRenderLayerHook->GetFastcall<int, C_BlockLegacy *>();
	return oFunc(a1);
}

__int64 Hooks::LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3)
{
	static auto oFunc = g_Hooks.LevelRenderer_renderLevelHook->GetFastcall<__int64, __int64, __int64, __int64>();

	using reloadShit_t = void(__fastcall *)(__int64);
	static reloadShit_t reloadChunk = nullptr;

	if (!reloadChunk)
	{
		// RenderChunkCoordinator::rebuildAllRenderChunkGeometry
		reloadChunk = reinterpret_cast<reloadShit_t>(FindSignature("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B F9 48 8D A9"));
	}
	bool xrayMod = false;
	static bool lastState = false;
	if (lastState != xrayMod)
	{
		// LevelRenderer::rebuildAllRenderChunkGeometry
		lastState = xrayMod;
		unsigned long long *v5; // rdi
		unsigned long long *i;	// rbx

		v5 = *(unsigned long long **)(_this + 32);
		for (i = (unsigned long long *)*v5; i != v5; i = (unsigned long long *)*i)
			reloadChunk(i[3]);
	}

	auto ret = oFunc(_this, a2, a3);

	DrawUtils::setGameRenderContext(a2);
	moduleMgr->onLevelRender();
	DrawUtils::setGameRenderContext(0);

	return ret;
}

void Hooks::ClickFunc(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8)
{
	static auto oFunc = g_Hooks.ClickFuncHook->GetFastcall<void, __int64, char, char, __int16, __int16, __int16, __int16, char>();
	static auto configMenu = moduleMgr->getModule<ConfigManagerMod>();
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();

	// MouseButtons
	// 0 = mouse move
	// 1 = left click
	// 2 = right click
	// 3 = middle click
	// 4 = scroll   (isDown: 120 (SCROLL UP) and -120 (SCROLL DOWN))

	ClickGui::onMouseClickUpdate((int)mouseButton, isDown);
	HImGui.onMouseClickUpdate((int)mouseButton, isDown);

	if (isDown)
		if (mouseButton == 1)
			g_Data.leftclickCount++;
		else if (mouseButton == 2)
			g_Data.rightclickCount++;

	if (clickGUI->isEnabled() || configMenu->isEnabled())
	{
		if (mouseButton == 4)
			ClickGui::onWheelScroll(isDown > 0);
		if (mouseButton != 0)
			return; // Mouse click event
	}
	return oFunc(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

void Hooks::chatLogHookFunc(__int64 a1, TextHolder *msg, uint32_t a2)
{
	static auto oFunc = g_Hooks.ChatLogHook->GetFastcall<void, __int64, TextHolder *, uint32_t>();
	auto message = *reinterpret_cast<TextHolder *>(reinterpret_cast<__int64>(msg) + 0x8);
	static auto player = g_Data.getLocalPlayer();

	if (message.getText() != nullptr && player != nullptr)
	{
		string textStr = message.getText();

#ifdef _DEBUG
		if (moduleMgr->getModule<TestModule>()->isEnabled() && moduleMgr->getModule<TestModule>()->copyChatMessages)
			Utils::setClipboardText(string(message.getText()));
#endif

		string name = player->getNameTag()->getText();
		name = Utils::sanitize(name);
	}
	return oFunc(a1, msg, a2);
}

__int64 Hooks::MoveInputHandler_tick(C_MoveInputHandler *a1, C_Entity *a2)
{
	static auto oTick = g_Hooks.MoveInputHandler_tickHook->GetFastcall<__int64, C_MoveInputHandler *, C_Entity *>();

	auto ret = oTick(a1, a2);
	moduleMgr->onMove(a1);

	return 0;
}

__int64 Hooks::ChestScreenController_tick(C_ChestScreenController *a1)
{
	static auto oFunc = g_Hooks.ChestScreenController_tickHook->GetFastcall<__int64, C_ChestScreenController *>();

	static auto chestStealerMod = moduleMgr->getModule<ChestStealer>();
	if (chestStealerMod->isEnabled())
		chestStealerMod->chestScreenController_tick(a1);

	return oFunc(a1);
}

__int8 *Hooks::BlockLegacy_getLightEmission(C_BlockLegacy *a1, __int8 *a2)
{
	static auto oFunc = g_Hooks.BlockLegacy_getLightEmissionHook->GetFastcall<__int8 *, C_BlockLegacy *, __int8 *>();
	return oFunc(a1, a2);
}

float Hooks::GetGamma(uintptr_t *a1)
{
	static auto nametsags = moduleMgr->getModule<NameTags>();
	static auto noRender = moduleMgr->getModule<NoRender>();
	static auto camera = moduleMgr->getModule<Camera>();
	uintptr_t **list = (uintptr_t **)a1;

	char obtainedSettings = 0;
	bool hadIt = false;
	for (uint16_t i = 3; i < 450; i++)
	{
		if (list[i] == nullptr)
			continue;
		uintptr_t *info = *(uintptr_t **)((uintptr_t)list[i] + 8);
		if (info == nullptr)
			continue;

		TextHolder *translateName = (TextHolder *)((uintptr_t)info + 0x1D8);
		TextHolder *settingname = (TextHolder *)((uintptr_t)info + 0x188);

		if (!strcmp(translateName->getText(), "options.smoothlighting"))
		{
			if (hadIt)
				continue;
			bool *smoothlightning = (bool *)((uintptr_t)list[i] + 16);

			obtainedSettings++;
			hadIt = true;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderWeather"))
		{
			bool *disableWeatherParticles = (bool *)((uintptr_t)list[i] + 16);
			if (noRender->isEnabled() && noRender->noweather)
				*disableWeatherParticles = true;
			else
				*disableWeatherParticles = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderEntities"))
		{
			bool *disableRenderEntities = (bool *)((uintptr_t)list[i] + 16);
			if (noRender->isEnabled() && noRender->entities)
				*disableRenderEntities = true;
			else
				*disableRenderEntities = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderBlockEntities"))
		{
			bool *disableRenderBlockEntities = (bool *)((uintptr_t)list[i] + 16);
			if (noRender->isEnabled() && noRender->blockEntities)
				*disableRenderBlockEntities = true;
			else
				*disableRenderBlockEntities = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderParticles"))
		{
			bool *disableRenderParticles = (bool *)((uintptr_t)list[i] + 16);
			if (noRender->isEnabled() && noRender->particles)
				*disableRenderParticles = true;
			else
				*disableRenderParticles = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.hidehud"))
		{
			bool *hideHud = (bool *)((uintptr_t)list[i] + 16);

			isHUDHidden = *hideHud;

			obtainedSettings++;
		}

		if (obtainedSettings == 6)
			break;
	}

	if (camera->isEnabled() && camera->fullbright)
		return camera->intensity;

	static auto ofunc = g_Hooks.GetGammaHook->GetFastcall<float, uintptr_t *>();
	return ofunc(a1);
}

__int64 Hooks::MinecraftGame_onAppSuspended(__int64 _this)
{
	static auto oFunc = g_Hooks.MinecraftGame_onAppSuspendedHook->GetFastcall<__int64, __int64>();
	configMgr->saveConfig();
	return oFunc(_this);
}

void Hooks::Actor_swing(C_Entity *_this)
{
	static auto oFunc = g_Hooks.Actor_swingHook->GetFastcall<void, C_Entity *>();
	static auto killaura = moduleMgr->getModule<Killaura>();
	static auto scaffold = moduleMgr->getModule<Scaffold>();

	bool targetListEmpty = killaura->targetListEmpty;
	bool leftClicking = g_Data.isLeftClickDown();
	bool noSwingMod = false;

	if (!(noSwingMod) || scaffold->isEnabled() && !scaffold->swing && !leftClicking && targetListEmpty)
		return oFunc(_this);
}

void Hooks::RakNetInstance_tick(C_RakNetInstance *_this, __int64 a2, __int64 a3)
{
	static auto oTick = g_Hooks.RakNetInstance_tickHook->GetFastcall<void, C_RakNetInstance *, __int64, __int64>();
	GameData::setRakNetInstance(_this);
	oTick(_this, a2, a3);
}

float Hooks::GameMode_getPickRange(C_GameMode *_this, __int64 a2, char a3)
{
	static auto oFunc = g_Hooks.GameMode_getPickRangeHook->GetFastcall<float, C_GameMode *, __int64, char>();

	if (g_Data.getLocalPlayer() != nullptr)
	{
	}

	return oFunc(_this, a2, a3);
}

__int64 Hooks::ConnectionRequest_create(__int64 _this, __int64 privateKeyManager, void *a3, TextHolder *selfSignedId, TextHolder *serverAddress, __int64 clientRandomId, TextHolder *skinId, SkinData *skinData, __int64 capeData, CoolSkinData *coolSkinStuff, TextHolder *deviceId, int inputMode, int uiProfile, int guiScale, TextHolder *languageCode, bool sendEduModeParams, TextHolder *tenantId, __int64 unused, TextHolder *platformUserId, TextHolder *thirdPartyName, bool thirdPartyNameOnly, TextHolder *platformOnlineId, TextHolder *platformOfflineId, TextHolder *capeId)
{
	static auto oFunc = g_Hooks.ConnectionRequest_createHook->GetFastcall<__int64, __int64, __int64, void *, TextHolder *, TextHolder *, __int64, TextHolder *, SkinData *, __int64, CoolSkinData *, TextHolder *, int, int, int, TextHolder *, bool, TextHolder *, __int64, TextHolder *, TextHolder *, bool, TextHolder *, TextHolder *, TextHolder *>();

	auto geoOverride = g_Data.getCustomGeoOverride();

	logF("Connection Request: InputMode: %i UiProfile: %i GuiScale: %i", inputMode, uiProfile, guiScale);
	TextHolder *fakeName = g_Data.getFakeName();
	__int64 res = oFunc(_this, privateKeyManager, a3, selfSignedId, serverAddress, clientRandomId, skinId, skinData, capeData, coolSkinStuff, deviceId, inputMode, uiProfile, guiScale, languageCode, sendEduModeParams, tenantId, unused, platformUserId, fakeName != nullptr ? fakeName : thirdPartyName, fakeName != nullptr ? true : thirdPartyNameOnly, platformOnlineId, platformOfflineId, capeId);
	return res;
}

void Hooks::InventoryTransactionManager_addAction(C_InventoryTransactionManager *a1, C_InventoryAction *a2)
{
	static auto Func = g_Hooks.InventoryTransactionManager_addActionHook->GetFastcall<void, C_InventoryTransactionManager *, C_InventoryAction *>();
	Func(a1, a2);
}

bool Hooks::DirectoryPackAccessStrategy__isTrusted(__int64 _this)
{
	static auto func = g_Hooks.DirectoryPackAccessStrategy__isTrustedHook->GetFastcall<bool, __int64>();

	static uintptr_t **directoryPackAccessStrategyVtable = 0;

	if (!directoryPackAccessStrategyVtable)
	{
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
		int offset = *reinterpret_cast<int *>(sigOffset + 3);
		directoryPackAccessStrategyVtable = reinterpret_cast<uintptr_t **>(sigOffset + offset + 7);
	}

	if (*reinterpret_cast<uintptr_t *>(_this) == (uintptr_t)directoryPackAccessStrategyVtable)
		return true;

	return func(_this);
}

bool Hooks::ReturnTrue(__int64 _this)
{
	return true;
}

__int64 Hooks::SkinRepository___loadSkinPack(__int64 _this, __int64 pack, __int64 a3)
{
	static auto func = g_Hooks.SkinRepository___loadSkinPackHook->GetFastcall<__int64, __int64, __int64, __int64>();

	// auto res = (*(unsigned __int8 (**)(void))(**(__int64**)(pack + 8) + 48i64))();
	// logF("SkinRepository___loadSkinPack: origin %i, is Trusted: %i", *(int*)((*(__int64*)pack) + 888i64), res);
	*(int *)((*(__int64 *)pack) + 888i64) = 2; // Set pack origin to "2"

	return func(_this, pack, a3);
}

GamerTextHolder *Hooks::toStyledString(__int64 strIn, GamerTextHolder *strOut)
{
	static auto func = g_Hooks.toStyledStringHook->GetFastcall<GamerTextHolder *, __int64, GamerTextHolder *>();

	static auto conRequest = reinterpret_cast<__int64>(g_Hooks.ConnectionRequest_createHook->funcPtr);
	if (reinterpret_cast<__int64>(_ReturnAddress()) > conRequest && reinterpret_cast<__int64>(_ReturnAddress()) < conRequest + 10000 && overrideStyledReturn)
	{
		// Return address is within boundaries of connection request function

		strOut->copyFrom(&styledReturnText);

		return strOut;
	}

	return func(strIn, strOut);
}

__int64 Hooks::GameMode_attack(C_GameMode *_this, C_Entity *ent)
{
	if (!ent)
		return 0;
	auto func = g_Hooks.GameMode_attackHook->GetFastcall<__int64, C_GameMode *, C_Entity *>();
	moduleMgr->onAttack(ent);
	return func(_this, ent);
}

void Hooks::setPos(C_Entity *ent, vec3_t &pos)
{
	auto func = g_Hooks.setPosHook->GetFastcall<void, C_Entity *, vec3_t &>();
	func(ent, pos);
}

void Hooks::LocalPlayer__updateFromCamera(__int64 a1, C_Camera *camera)
{
	auto func = g_Hooks.LocalPlayer__updateFromCameraHook->GetFastcall<void, __int64, C_Camera *>();
	func(a1, camera);
}

bool Hooks::Mob__isImmobile(C_Entity *ent)
{
	auto func = g_Hooks.Mob__isImmobileHook->GetFastcall<bool, C_Entity *>();

	if (ent == g_Data.getLocalPlayer())
	{
		g_Hooks.shouldLocalPlayerBeImmobile = func(ent);
	}

	return func(ent);
}

void Hooks::InventoryTransactionManager__addAction(C_InventoryTransactionManager *_this, C_InventoryAction &action)
{
	auto func = g_Hooks.InventoryTransactionManager__addActionHook->GetFastcall<void, C_InventoryTransactionManager *, C_InventoryAction &>();

#ifdef TEST_DEBUG
	char *srcName = "none";
	if (action.sourceItem.item && *action.sourceItem.item)
		srcName = (*action.sourceItem.item)->name.getText();
	char *targetName = "none";
	if (action.targetItem.item && *action.targetItem.item)
		targetName = (*action.targetItem.item)->name.getText();
	logF("%i %i %i %s %s", action.type, action.slot, action.sourceType, srcName, targetName, action.sourceType);

	/*if(/*action.slot == 14 && action.sourceType == 124 && strcmp(targetName, "none") == 0 && *strcmp(srcName, "stone_shovel") == 0){
		string tag = "{ench:[{id:9s,lvl:1s}]}";
		action.sourceItem.setUserData(move(Mojangson::parseTag(tag)));
	}
	if(/*action.slot == 2 && action.sourceType == 256 && strcmp(srcName, "none") == 0 &&* strcmp(targetName, "stone_shovel") == 0){
		string tag = "{ench:[{id:9s,lvl:1s}]}";
		action.targetItem.setUserData(move(Mojangson::parseTag(tag)));
	}*/
#endif

	func(_this, action);
}

void Hooks::LevelRendererPlayer__renderNameTags(__int64 a1, __int64 a2, TextHolder *a3, __int64 a4)
{
	static auto func = g_Hooks.LevelRendererPlayer__renderNameTagsHook->GetFastcall<void, __int64, __int64, TextHolder *, __int64>();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();

	if (nameTagsMod->isEnabled() && nameTagsMod->nameTags.size() > 0)
	{
		string text = Utils::sanitize(a3->getText());
		size_t found = text.find('\n');

		if (found != string::npos)
			text = text.substr(0, found);

		if (nameTagsMod->nameTags.find(text) != nameTagsMod->nameTags.end())
			return;
	}

	return func(a1, a2, a3, a4);
}

int Hooks::Mob_GetCurrentSwingDuration(C_Entity* _this) {
	static auto func = g_Hooks.Mob__GetCurrentSwingDuration->GetFastcall<int, C_Entity*>();
	SlowSwing* slowSwingMod = moduleMgr->getModule<SlowSwing>();
	if (slowSwingMod->isEnabled() && _this == g_Data.getLocalPlayer()) {
		return slowSwingMod->swingspeed;
	}
	return func(_this);
}

void Hooks::GameMode__stopDestroyBlock(C_GameMode* _this, vec3_ti* BlockPos) {
	static auto func = g_Hooks.GameMode__stopDestroyBlockHook->GetFastcall<void, C_GameMode*, vec3_ti*>();
	static PacketMine* mod = moduleMgr->getModule<PacketMine>();
	if (mod->isEnabled()) return;
	func(_this, BlockPos);
}
