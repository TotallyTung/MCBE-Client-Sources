#include "Hooks.h"

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp>  // perspective, translate, rotate
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>         // mat4
#include <glm/trigonometric.hpp>  // radians

#include "../Fonts/BahnSchrift.h"
#include "../Fonts/Mojangles.h"
#include "../Fonts/ProductSansRegular.h"
#include "../Horion/Loader.h"
#include "../SDK/Tag.h"
#include "../Utils/ClientColors.h"
#include "../Utils/ColorUtil.h"
#include "../Utils/ImGuiUtils.h"
using namespace std;
Hooks g_Hooks;
bool isTicked = false;
bool overrideStyledReturn = false;
TextHolder styledReturnText;
// #define TEST_DEBUG

typedef HRESULT(__thiscall* Present)(IDXGISwapChain3*, UINT, UINT);
Present oPresent;

typedef HRESULT(__thiscall* ResizeBuffers)(IDXGISwapChain*, int, int, int, DXGI_FORMAT, int);
ResizeBuffers oResizeBuffers;

void Hooks::Init() {
	logF("Setting up Hooks...");
	// clang-format off

	// Signatures
	{
		// vtables better than sigs

		void* _renderCtx = reinterpret_cast<void*>(FindSignature("48 8B ? 48 89 ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 4C 8B ? 48 89 ? ? ? 4C 8B"));
		g_Hooks.RenderTextHook = make_unique<FuncHook>(_renderCtx, Hooks::RenderText);

		void* render = reinterpret_cast<void*>(FindSignature("48 89 5c 24 ? 57 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 84 24 ? ? ? ? 48 8b da 48 8b f9 b9"));
		g_Hooks.UIScene_renderHook = make_unique<FuncHook>(render, Hooks::UIScene_render);

		void* key = reinterpret_cast<void*>(FindSignature("48 89 5c 24 ? 57 48 83 ec ? 8b 05 ? ? ? ? 8b da"));
		g_Hooks.KeyMapHook = make_unique<FuncHook>(key, Hooks::KeyMapHookCallback);

		void* _sendChatMessage = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4C 8B EA 4C 8B F9 48 8B 49"));
		g_Hooks.ClientInstanceScreenModel_sendChatMessageHook = make_unique<FuncHook>(_sendChatMessage, Hooks::ClientInstanceScreenModel_sendChatMessage);

		void* chestTick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 56 57 41 56 48 83 EC ? 48 83 79"));
		g_Hooks.ChestBlockActor_tickHook = make_unique<FuncHook>(chestTick, Hooks::ChestBlockActor_tick);

		void* lerpFunc = reinterpret_cast<void*>(FindSignature("8B 02 89 81 ? ? ? ? 8B 42 ? 89 81 ? ? ? ? 8B 42 ? 89 81 ? ? ? ? C3 CC CC CC CC CC 48 89 5C 24"));
		g_Hooks.Actor_lerpMotionHook = make_unique<FuncHook>(lerpFunc, Hooks::Actor_lerpMotion);

		void* getRenderLayer = reinterpret_cast<void*>(FindSignature("8B 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC F3 0F 10 81"));
		g_Hooks.BlockLegacy_getRenderLayerHook = make_unique<FuncHook>(getRenderLayer, Hooks::BlockLegacy_getRenderLayer);

		void* getLightEmission = reinterpret_cast<void*>(FindSignature("0F B6 81 ? ? ? ? 88 02 48 8B C2"));
		g_Hooks.BlockLegacy_getLightEmissionHook = make_unique<FuncHook>(getLightEmission, Hooks::BlockLegacy_getLightEmission);

		void* getGameEdition = reinterpret_cast<void*>(FindSignature("8B 91 ?? ?? ?? ?? 85 D2 74 1C 83 EA 01"));
		g_Hooks.AppPlatform_getGameEditionHook = make_unique<FuncHook>(getGameEdition, Hooks::AppPlatform_getGameEdition);

		void* getFov = reinterpret_cast<void*>(FindSignature("40 53 48 83 EC ?? 0F 29 7C 24 ?? 44"));
		g_Hooks.LevelRendererPlayer_getFovHook = make_unique<FuncHook>(getFov, Hooks::LevelRendererPlayer_getFov);

		void* tick_entityList = reinterpret_cast<void*>(FindSignature("48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? E8 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B D8 ?? ?? ?? ?? ?? ?? 48 99"));
		g_Hooks.MultiLevelPlayer_tickHook = make_unique<FuncHook>(tick_entityList, Hooks::MultiLevelPlayer_tick);

		void* keyMouseFunc = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC ? ? ? ? 0F 29 74 24 ? 0F 29 7C 24 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F0"));
		g_Hooks.HIDController_keyMouseHook = make_unique<FuncHook>(keyMouseFunc, Hooks::HIDController_keyMouse);

		void* renderLevel = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 48 89 74 24 20 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 49 8B F8 48 8B DA"));
		g_Hooks.LevelRenderer_renderLevelHook = make_unique<FuncHook>(renderLevel, Hooks::LevelRenderer_renderLevel);

		void* playerCallBackHook = reinterpret_cast<void*>(FindSignature("F3 0F ?? ?? ?? ?? 00 00 ?? 0F ?? 00 F3 0F ?? ?? F3 0F ?? ?? 04"));
		g_Hooks.playerCallBack_Hook = make_unique<FuncHook>(playerCallBackHook, Hooks::playerCallBack);

		void* clickHook = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9"));
		g_Hooks.ClickFuncHook = make_unique<FuncHook>(clickHook, Hooks::ClickFunc);

		void* chestScreenControllerTick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B 17"));
		g_Hooks.ChestScreenController_tickHook = make_unique<FuncHook>(chestScreenControllerTick, Hooks::ChestScreenController_tick);

		void* autoComplete = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 41 8B D9"));
		g_Hooks.PleaseAutoCompleteHook = make_unique<FuncHook>(autoComplete, Hooks::PleaseAutoComplete);

		void* fullbright = reinterpret_cast<void*>(Utils::getBase() + 0xCA3F70);
		g_Hooks.GetGammaHook = make_unique<FuncHook>(fullbright, Hooks::GetGamma);

		void* RakNetInstance__tick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D AC 24 20 FD FF FF 48 81 EC E0 03 00 00"));
		g_Hooks.RakNetInstance_tickHook = make_unique<FuncHook>(RakNetInstance__tick, Hooks::RakNetInstance_tick);

		void* ConnectionRequest__create = reinterpret_cast<void*>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 49 8B D9 48 89 55 E8"));
		g_Hooks.ConnectionRequest_createHook = make_unique<FuncHook>(ConnectionRequest__create, Hooks::ConnectionRequest_create);

		void* _getSkinPack = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B EA 48 8B F1"));
		g_Hooks.SkinRepository___loadSkinPackHook = make_unique<FuncHook>(_getSkinPack, Hooks::SkinRepository___loadSkinPack);

#ifdef TEST_DEBUG
		void* addAction = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC 30 45 0F B6 F8 4C 8B F2 48 8B F1 48 8B 01 48 8B 88 ? ? ? ? 48 85 C9"));
		g_Hooks.InventoryTransactionManager__addActionHook = make_unique<FuncHook>(addAction, Hooks::InventoryTransactionManager__addAction);
#endif

		void* localPlayerUpdateFromCam = reinterpret_cast<void*>(FindSignature("48 8b c4 53 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 84 24 ? ? ? ? 4d 8b d0"));
		g_Hooks.LocalPlayer__updateFromCameraHook = make_unique<FuncHook>(localPlayerUpdateFromCam, Hooks::LocalPlayer__updateFromCamera);

		void* renderNameTags = reinterpret_cast<void*>(FindSignature("48 8b c4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d a8 ? ? ? ? 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 44 0f 29 40 ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 49 8b f1"));
		g_Hooks.LevelRendererPlayer__renderNameTagsHook = make_unique<FuncHook>(renderNameTags, Hooks::LevelRendererPlayer__renderNameTags);

		void* destroySpeed = reinterpret_cast<void*>(FindSignature("48 89 5c 24 ? 48 89 74 24 ? 57 48 83 ec ? 48 8b fa 0f 29 74 24 ? 48 8b 91"));
		g_Hooks.getDestroySpeedHook = make_unique<FuncHook>(destroySpeed, Hooks::getDestroySpeed);

		void* actorisInWall = reinterpret_cast<void*>(FindSignature("40 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 01 48 8D 54 24 ? 0F 57 DB 41 B8 ? ? ? ? 48 8B D9 FF 90 ? ? ? ? 48 8B 8B"));
		g_Hooks.ActorisInWallHook = make_unique<FuncHook>(actorisInWall, Hooks::Actor__isInWall);

		//void* testFunc = reinterpret_cast<void*>(FindSignature("40 53 48 83 ec ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 44 24 ? 48 8b 01 48 8d 54 24 ? 0f 57 db 41 b8 ? ? ? ? 48 8b d9 ff 90 ? ? ? ? 48 8b 8b"));
		//g_Hooks.testFunctionHook = make_unique<FuncHook>(testFunc, Hooks::testFunction);

		static constexpr auto counterStart = __COUNTER__ + 1;
#define lambda_counter (__COUNTER__ - counterStart)

		void* levelRendererBobView = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 20 57 48 ?? ?? ?? ?? 00 00 0F ?? ?? ?? 0F ?? ?? ?? ?? 0F ?? ?? ?? ?? 0F ?? ?? ??"));

		static auto bobViewHookF = [](__int64 _this, glm::mat4& matrix, float lerpT) {
			static auto origFunc = g_Hooks.lambdaHooks.at(lambda_counter)->GetFastcall<void, __int64, glm::mat4&, float>();

			static auto testMod = moduleMgr->getModule<ViewModel>();
			auto p = Game.getLocalPlayer();
			float degrees = fmodf(p->getPosOld()->lerp(p->getPos(), lerpT).x, 5) - 2.5f;
			degrees *= 180 / 2.5f;

			auto pos = Game.getClientInstance()->levelRenderer->getOrigin();

			glm::mat4 View = matrix;

			matrix = View;
			if (testMod->isEnabled()) {
				if (testMod->doTranslate)
					matrix = glm::translate<float>(matrix, glm::vec3(testMod->xTrans, testMod->yTrans, testMod->zTrans));

				if (testMod->doScale)
					matrix = glm::scale<float>(matrix, glm::vec3(testMod->xMod, testMod->yMod, testMod->zMod));
			}
			return origFunc(_this, matrix, lerpT);
			};

		using BobViewHookFunc = decltype(&bobViewHookF.operator());
		auto bobViewHook = make_shared<FuncHook>(levelRendererBobView, static_cast<BobViewHookFunc>(bobViewHookF));

		g_Hooks.lambdaHooks.push_back(bobViewHook);

#undef lambda_counter

		logF("Hooks initialized");
	}

	// Vtables
	{
		// LoopbackPacketSender::vtable
		if (Game.getClientInstance()->loopbackPacketSender != nullptr) {
			uintptr_t** packetSenderVtable = reinterpret_cast<uintptr_t**>(*(uintptr_t*)Game.getClientInstance()->loopbackPacketSender);
			if (packetSenderVtable == nullptr)
				logF("LoopbackPacketSenderVtable is invalid");
			else {
				g_Hooks.LoopbackPacketSender_sendToServerHook = make_unique<FuncHook>(packetSenderVtable[2], Hooks::LoopbackPacketSender_sendToServer);

				g_Hooks.LoopbackPacketSender_sendToClientHook = make_unique<FuncHook>(packetSenderVtable[4], Hooks::LoopbackPacketSender_sendToClient); //I use the second sendToClient
			}
		}
		else logF("LoopbackPacketSender is null");

		// MoveInputHandler::vtable
		if (Game.getClientInstance()->getMoveTurnInput() != nullptr) {
			uintptr_t** moveInputVtable = reinterpret_cast<uintptr_t**>(*(uintptr_t*)Game.getClientInstance()->getMoveTurnInput());
			if (moveInputVtable == 0x0)
				logF("MoveInputHandler signature not working!!!");
			else {
				g_Hooks.MoveInputHandler_tickHook = make_unique<FuncHook>(moveInputVtable[1], Hooks::MoveInputHandler_tick);
			}
		}
		else logF("MoveTurnInput is null");

		//The reason im using a sig is because injecting on the menu causes LocalPlayer to be null so i cant get the vtable from just doing Game.getLocalPlayer(). Same with Gamemode bc i get that from local player.
		// LocalPlayer::vtable
		{
			uintptr_t** localPlayerVtable = GetVtableFromSig("48 8d 05 ? ? ? ? 48 89 01 48 8b 89 ? ? ? ? 48 8b 01 ff 90 ? ? ? ? 48 8b 10", 3);
			if (localPlayerVtable == 0x0)
				logF("LocalPlayer signature not working!!!");
			else {
				g_Hooks.Actor_startSwimmingHook = make_unique<FuncHook>(localPlayerVtable[201], Hooks::Actor_startSwimming);

				g_Hooks.Actor__setRotHook = make_unique<FuncHook>(localPlayerVtable[27], Hooks::Actor__setRot);

				g_Hooks.Actor_swingHook = make_unique<FuncHook>(localPlayerVtable[219], Hooks::Actor_swing);

				g_Hooks.JumpPowerHook = make_unique<FuncHook>(localPlayerVtable[345], Hooks::JumpPower); //jump from ground with movement proxy

				//g_Hooks.setPosHook = make_unique<FuncHook>(localPlayerVtable[19], Hooks::setPos);

				g_Hooks.Actor_baseTickHook = make_unique<FuncHook>(localPlayerVtable[49], Hooks::Actor_baseTick);

				g_Hooks.Mob__isImmobileHook = make_unique<FuncHook>(localPlayerVtable[91], Hooks::Mob__isImmobile);

				g_Hooks.Player_tickWorldHook = make_unique<FuncHook>(localPlayerVtable[364], Hooks::Player_tickWorld);

				g_Hooks.Actor_aiStepHook = make_unique<FuncHook>(localPlayerVtable[291], Hooks::Actor_aiStep);

				g_Hooks.Actor_isOnFireHook = make_unique<FuncHook>(localPlayerVtable[102], Hooks::Actor_isOnFire);

				//g_Hooks.Actor__isInvisibleHook = make_unique<FuncHook>(localPlayerVtable[59], Hooks::Actor__isInvisible);
			}
		}

		// GameMode::vtable
		{
			uintptr_t** gameModeVtable = reinterpret_cast<uintptr_t**>(Utils::getBase() + 0x423CD30);
			if (gameModeVtable == 0x0)
				logF("GameMode signature not working!!!");
			else {
				g_Hooks.GameMode_startDestroyBlockHook = make_unique<FuncHook>(gameModeVtable[1], Hooks::GameMode_startDestroyBlock);

				g_Hooks.GameMode__stopDestroyBlockHook = make_unique<FuncHook>(gameModeVtable[4], Hooks::GameMode__stopDestroyBlock);

				g_Hooks.GameMode_getPickRangeHook = make_unique<FuncHook>(gameModeVtable[10], Hooks::GameMode_getPickRange);

				g_Hooks.GameMode_attackHook = make_unique<FuncHook>(gameModeVtable[14], Hooks::GameMode_attack);
			}
		}
		logF("Vtables initialized");
	}
}

void Hooks::Restore() {
	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(10);
}

void Hooks::Enable() {
	//SwapChain Hook
	bool isDx11 = false;
	if (kiero::init(kiero::RenderType::D3D12) != kiero::Status::Success) {
		if (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success) {
			logF("Failed to initialize kiero!");
			return;
		}
		isDx11 = true;
	}

	if (!isDx11) {
		if (kiero::bind(145, (void**)&oResizeBuffers, resizeBuffersHook) != kiero::Status::Success) {
			logF("Failed to hook Resize Buffers Hook!");
			return;
		}
		logF("Successfully hooked Resize Buffers Hook!");
		if (kiero::bind(140, (void**)&oPresent, presentCallback) != kiero::Status::Success) {
			logF("Failed to hook Present D3D12 Hook!");
			return;
		}
		logF("Successfully hooked Present D3D12 Hook!");
	}
	else {
		if (kiero::bind(13, (void**)&oResizeBuffers, resizeBuffersHook) != kiero::Status::Success) {
			logF("Failed to hook Resize Buffers Hook!");
			return;
		}
		logF("Successfully hooked Resize Buffers Hook!");
		if (kiero::bind(8, (void**)&oPresent, Hooks::presentCallback) != kiero::Status::Success) {
			logF("Failed to hook Present D3D11 Hook!");
			return;
		}
		logF("Successfully hooked Present D3D11 Hook!");
	}
	logF("Hooks enabled");
	MH_EnableHook(MH_ALL_HOOKS);
}

void Hooks::onImGuiRender(ImDrawList* d) {
	ImGuiIO& io = ImGui::GetIO();
	static CustomFont* customFontMod = moduleMgr->getModule<CustomFont>();
	static ImFont* mcFont = io.Fonts->Fonts[0];
	static ImFont* pdsFont = io.Fonts->Fonts[1];
	static ImFont* bhsFont = io.Fonts->Fonts[2];
	if (customFontMod->fontMode.getSelectedValue() == 0) {
		io.FontDefault = mcFont;
		ImGuiUtils::fontSize = 22.f;
	}
	else if (customFontMod->fontMode.getSelectedValue() == 1) {
		io.FontDefault = pdsFont;
		ImGuiUtils::fontSize = 25.f;
	}
	else if (customFontMod->fontMode.getSelectedValue() == 2) {
		io.FontDefault = bhsFont;
		ImGuiUtils::fontSize = 22.f;
	}
	ImGuiUtils::setDrawList(d);
	moduleMgr->onImGuiRender(d);
	/*
	{
		glmatrixf* badrefdef =  Game.getClientInstance()->getRefDef();
		shared_ptr<glmatrixf> refdef = shared_ptr<glmatrixf>(badrefdef->correct());

		static unsigned int balls = 60; //15
		static unsigned int speed = 0;
		static unsigned int anim = 0;
		static float smoothing = 1.f; // lerpMotion
		anim++;
		speed += 6; // 2
		const float coolAnim = 0.9f + 0.9f * sin(((float)anim / 60) * PI * 0.75f);
		Vec3 playerPos = Vec3(323.f, -61.f, -89.f);//Game.getLocalPlayer()->eyePos0.sub(0.f, 1.6f, 0.f); // TargetPos
		//float duration = 1.f - abs(sin((anim / 60) * PI * 0.5f)) / 1.f;
		vector<ImVec2> pointsList;
		static Vec3 ballPos = Vec3(0.f, 0.f, 0.f);
		ballPos = ballPos.add(playerPos.sub(ballPos).mul(smoothing));
		for (int i = speed; i < 360 + speed; i += (int)(360 / balls)) {
			float calcYaw = (i + 90) * (PI / 180);
			float x = cos(calcYaw) * 0.75f;
			float z = sin(calcYaw) * 0.75f;
			Vec2 pointsVec2;
			if (refdef->OWorldToScreen(Game.getClientInstance()->levelRenderer->getOrigin(), ballPos.add(x, coolAnim, z), pointsVec2, Game.getClientInstance()->getFov(), Game.getGuiData()->windowSizeReal)) {
				pointsList.push_back(ImVec2(pointsVec2.x, pointsVec2.y));
			}
		}
		for (int i = 0; i < pointsList.size(); i++) {
			//UIColor color = ColorUtil::getRainbowColor(3.f, 1.f, 1.f, i * 50);
			UIColor color = UIColor(255, 255, 255, 255);
			int next = i + 1;
			if (next >= pointsList.size()) next = 0;
			//d->AddCircleFilled(pointsList[i], 5.f, ImColor(255, 255, 255, 255));
			d->AddShadowCircle(pointsList[i], 1.f, color.toImColor(), 50.f, ImVec2(0.f, 0.f));
			d->AddLine(pointsList[i], pointsList[next], color.toImColor(), 2.f);
		}
	}
	*/
	Notifications::Render(d);
}

HRESULT Hooks::presentCallback(IDXGISwapChain3* pSwapChain, UINT syncInterval, UINT flags) {
	if (Game.getClientInstance() == nullptr || Game.getGuiData() == nullptr) return oPresent(pSwapChain, syncInterval, flags);
	HWND window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");
	ID3D11Device* d3d11Device = nullptr;
	ID3D12Device* d3d12Device = nullptr;

	if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
		ID3D11DeviceContext* ppContext = nullptr;
		ID3D11Texture2D* pBackBuffer = nullptr;
		IDXGISurface* dxgiBackBuffer = nullptr;
		ID3D11RenderTargetView* mainRenderTargetView = nullptr;

		d3d11Device->GetImmediateContext(&ppContext);
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
		d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);

		if (!g_Hooks.initImGui) {
			if (ImGui::GetCurrentContext() == nullptr) {
				ImGui::CreateContext();
			}

			ImGuiIO& io = ImGui::GetIO();
			ImGui::StyleColorsDark();
			//io.Fonts->AddFontFromMemoryTTF((void*)Arial, sizeof(Arial), 50.f);
			io.Fonts->AddFontFromMemoryTTF((void*)Mojangles, sizeof(Mojangles), 50.f); // 200.f
			io.Fonts->AddFontFromMemoryTTF((void*)ProductSansRegular, sizeof(ProductSansRegular), 50.f);
			io.Fonts->AddFontFromMemoryTTF((void*)BahnSchrift, sizeof(BahnSchrift), 50.f);
			io.IniFilename = NULL;

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(d3d11Device, ppContext);
			g_Hooks.initImGui = true;
		}

		ImFX::NewFrame(d3d11Device, dxgiBackBuffer, GetDpiForWindow(window));
		pBackBuffer->Release();
		//ImGuiContext* g = ImGui::GetCurrentContext();
		//g->HoveredIdAllowOverlap = true;
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame(ImVec2(Game.getGuiData()->windowSizeReal.x, Game.getGuiData()->windowSizeReal.y));

		ImGui::NewFrame();
		onImGuiRender(ImGui::GetBackgroundDrawList());
		ImGui::EndFrame();
		ImGui::Render();

		//Finish ImGui draw
		ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImDrawData* drawData = ImGui::GetDrawData();
		ImGui_ImplDX11_RenderDrawData(drawData);
		ImFX::EndFrame();

		ppContext->Flush();
		//dxgiBackBuffer->Release();
		ppContext->Release();
		if (mainRenderTargetView) mainRenderTargetView->Release();
		d3d11Device->Release();
	}
	else if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
		static_cast<ID3D12Device5*>(d3d12Device)->RemoveDevice();
	}

	return oPresent(pSwapChain, syncInterval, flags);
}

HRESULT Hooks::resizeBuffersHook(IDXGISwapChain* ppSwapChain, int bufferCount, int width, int height, DXGI_FORMAT newFormat, int swapChainFlags) {

	ImFX::CleanupFX();
	return oResizeBuffers(ppSwapChain, bufferCount, width, height, newFormat, swapChainFlags);

};

bool Hooks::playerCallBack(Player* lp, __int64 a2, __int64 a3) {
	static auto oTick = g_Hooks.playerCallBack_Hook->GetFastcall<bool, Player*, __int64, __int64>();
	if (Game.getLocalPlayer() == nullptr) return oTick(lp, a2, a3);
	if (moduleMgr != nullptr && lp != nullptr && Game.getLocalPlayer() != nullptr && lp == Game.getLocalPlayer())
		moduleMgr->onPlayerTick(lp);

	if (!Game.getLocalPlayer() || !Game.getLocalPlayer()->level || !*(&Game.getLocalPlayer()->region + 1))
		g_Hooks.entityList.clear();

	if (Game.getLocalPlayer() != nullptr && lp == Game.getLocalPlayer()) {
		vector<EntityListPointerHolder> validEntities;
		for (const auto& ent : g_Hooks.entityList) {
			auto entity = ent.ent;
			MEMORY_BASIC_INFORMATION info;
			VirtualQuery(ent.ent, &info, sizeof(MEMORY_BASIC_INFORMATION));
			if (info.State & MEM_FREE) continue;
			if (info.State & MEM_RESERVE) continue;

			if (entity == nullptr) continue;

			if (entity != nullptr && (__int64)entity != 0xFFFFFFFFFFFFFCD7 && ent.ent != nullptr && *(__int64*)ent.ent != 0xFFFFFFFFFFFFFCD7 && *(__int64*)ent.ent > 0x6FF000000000 && *(__int64*)ent.ent < 0x800000000000 && *((int64_t*)ent.ent + 1) < 0x6FF000000000 && *(__int64*)ent.ent <= (__int64)(Utils::getBase() + 0x10000000) && entity->isAlive())
				validEntities.push_back(ent);
		}
		g_Hooks.entityList.clear();
		g_Hooks.entityList = validEntities;
	}
	return oTick(lp, a2, a3);
}

void* Hooks::Player_tickWorld(Player* _this, __int64 unk) {
	static auto oTick = g_Hooks.Player_tickWorldHook->GetFastcall<void*, Player*, __int64>();
	auto o = oTick(_this, unk);

	if (_this != nullptr && Game.getLocalPlayer() != nullptr && _this == Game.getLocalPlayer()) {
		GameMode* gm = Game.getLocalPlayer()->getGameMode();
		if (_this && gm) {
			GameData::updateGameData(gm);
			moduleMgr->onWorldTick(gm);
		}
	}
	return o;
}

void Hooks::Actor_aiStep(Entity* _this) {
	static auto Func = g_Hooks.Actor_aiStepHook->GetFastcall<void, Entity*>();
	static PacketMine* packetMineMod = moduleMgr->getModule<PacketMine>();
	LocalPlayer* localPlayer = Game.getLocalPlayer();

	if (packetMineMod->isEnabled() && _this == localPlayer) {
		//*((char*)_this + 7500) = 0;
		packetMineMod->updateBreak(localPlayer, Game.getGameMode());
	}
	Func(_this);
}

void Hooks::ClientInstanceScreenModel_sendChatMessage(void* _this, TextHolder* text) {
	static auto oSendMessage = g_Hooks.ClientInstanceScreenModel_sendChatMessageHook->GetFastcall<void, void*, TextHolder*>();

	if (text->getTextLength() <= 0)
		return oSendMessage(_this, text);

	auto message = text->getText();
	if (*message == cmdMgr->prefix) {
		cmdMgr->execute(message);
		return;
	}
	else if (*message == '.') {
		static once_flag flag;
		call_once(flag, [] {
			Game.getClientInstance()->getGuiData()->displayClientMessageF("%sYour Horion prefix is: \"%s%c%s\"", RED, YELLOW, cmdMgr->prefix, RED);
			Game.getClientInstance()->getGuiData()->displayClientMessageF("%sEnter \"%s%cprefix .%s\" to reset your prefix", RED, YELLOW, cmdMgr->prefix, RED);
			});
	}
	return oSendMessage(_this, text);
}

void Hooks::Actor_baseTick(Entity* ent) {
	static auto oFunc = g_Hooks.Actor_baseTickHook->GetFastcall<void, Entity*>();
	LocalPlayer* player = Game.getLocalPlayer();
	if (!player || !player->getlevel()) return oFunc(ent);

	static int tickCountThen = 0;
	int tickCountNow = *(int*)((__int64)player->getlevel() + 0x690);

	if (tickCountNow != tickCountThen) {
		g_Hooks.entityList.clear();
		tickCountThen = tickCountNow;
	}

	moduleMgr->onBaseTick(ent);

	if (ent->isClientSide()) {
		EntityListPointerHolder e;
		e.addedTick = tickCountNow;
		e.ent = ent;

		bool found = false;
		for (const auto& entity : g_Hooks.entityList)
			if (entity.ent == ent && entity.addedTick == tickCountNow) {
				found = true;
				break;
			}

		if (!found)
			g_Hooks.entityList.push_back(e);
	}
	return oFunc(ent);
}

void Hooks::KeyMapHookCallback(unsigned char key, bool isDown) {
	static auto oFunc = g_Hooks.KeyMapHook->GetFastcall<void, unsigned char, bool>();
	bool shouldCancel = false;
	GameData::keys[key] = isDown;

	moduleMgr->onKey((int)key, isDown, shouldCancel);
	moduleMgr->onKeyUpdate((int)key, (isDown && GameData::canUseMoveKeys()));
	ClickGui::onKeyUpdate((int)key, isDown);

	if (shouldCancel) return;
	return oFunc(key, isDown);
}

__int64 Hooks::UIScene_render(UIScene* uiscene, __int64 screencontext) {
	static auto oRender = g_Hooks.UIScene_renderHook->GetFastcall<__int64, UIScene*, __int64>();
	static auto hudModule = moduleMgr->getModule<HudModule>();
	static auto blockScan = moduleMgr->getModule<StorageESP>();

	g_Hooks.shouldRender = false;

	TextHolder alloc{};
	uiscene->getScreenName(&alloc);

	if (!g_Hooks.shouldRender) {
		g_Hooks.shouldRender = (strcmp(alloc.getText(), "debug_screen") == 0);
		if (!g_Hooks.shouldRender)
			if (alloc.getTextLength() < 100) strcpy_s(g_Hooks.currentScreenName, alloc.getText());
	}

	alloc.alignedTextLength = 0;

	return oRender(uiscene, screencontext);
}

__int64 Hooks::RenderText(ScreenView* screenView, MinecraftUIRenderContext* renderCtx) {
	static auto oText = g_Hooks.RenderTextHook->GetFastcall<__int64, ScreenView*, MinecraftUIRenderContext*>();

	GuiData* dat = Game.getClientInstance()->getGuiData();
	DrawUtils::setCtx(renderCtx, dat);
	static bool dontcallme = false;
	if (!dontcallme) {
		if (!Game.initisfinished()) {
			auto wid = Game.getClientInstance()->getGuiData()->windowSize;
			std::string loadingText = "1/6 (Initialize Client...)";
			Game.loading();
			// Fill Background and Texts
			{
				DrawUtils::fillRectangle(Vec4(
					0,
					0,
					Game.getClientInstance()->getGuiData()->widthGame,
					Game.getClientInstance()->getGuiData()->heightGame),
					MC_Color(0, 0, 0), 0.2f);
				if (Game.time >= 30) loadingText = "2/6 (Setup GameData...)";
				if (Game.time >= 60) loadingText = "3/6 (Setup Modules...)";
				if (Game.time >= 80) loadingText = "4/6 (Setup Commands...)";
				if (Game.time >= 70) loadingText = "5/6 (Initialize Hooks...)";
				if (Game.time >= 100) loadingText = "6/6 (Loading Client...)";
				if (Game.time >= 150) loadingText = "O/6 (Done!)";
				Vec2 textPos = Vec2((Game.getClientInstance()->getGuiData()->widthGame - DrawUtils::getTextWidth(&loadingText, 3.f)) / 2, (Game.getClientInstance()->getGuiData()->heightGame / 2) - 30);

				DrawUtils::drawText(textPos, &loadingText, MC_Color(1.f, 1.f, 1.f), 3.f, 1.f);
				float radius = 150;
				DrawUtils::fillRectangle(Vec4(
					Game.getClientInstance()->getGuiData()->widthGame / 2 - radius,
					Game.getClientInstance()->getGuiData()->heightGame / 2 + 5,
					Game.getClientInstance()->getGuiData()->widthGame / 2 + radius,
					Game.getClientInstance()->getGuiData()->heightGame / 2 + 15),
					MC_Color(0, 0, 0), 0.3f);
				DrawUtils::fillRectangle(Vec4(
					Game.getClientInstance()->getGuiData()->widthGame / 2 - radius,
					Game.getClientInstance()->getGuiData()->heightGame / 2 + 5,
					(Game.getClientInstance()->getGuiData()->widthGame / 2 - radius) + ((radius * 2) * ((float)Game.time / 200)),
					Game.getClientInstance()->getGuiData()->heightGame / 2 + 15),
					MC_Color(1.f, 1.f, 1.f), 0.5f);
				DrawUtils::flush();
			}

			return oText(screenView, renderCtx);
		}
		else
			dontcallme = true;
	}
	//DrawUtils::updateGameDeltaTime(screenView);

	static bool hookedRenderCtx = false;
	if (!hookedRenderCtx) {
		auto VTable = *(uintptr_t**)renderCtx;
		g_Hooks.drawImagekHook = make_unique<FuncHook>(VTable[7], Hooks::drawImageCallback);
		g_Hooks.drawImagekHook->enableHook();
		MH_EnableHook((void*)VTable[7]);
		hookedRenderCtx = true;
	}

	if (GameData::shouldHide() || !moduleMgr->isInitialized())
		return oText(screenView, renderCtx);
	string screenName(g_Hooks.currentScreenName);
	if (strcmp(screenName.c_str(), "nether_loading_progress_screen - nether") == 0) {

	}
	if (g_Hooks.shouldRender) {
		DrawUtils::updateGameDeltaTime(screenView);
		static auto hudModule = moduleMgr->getModule<HudModule>();
		static auto blockScan = moduleMgr->getModule<StorageESP>();
		static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>();
		static auto clientThemeModule = moduleMgr->getModule<ClientTheme>();

		Game.frameCount++;

		auto wid = Game.getClientInstance()->getGuiData()->windowSize;

		moduleMgr->onPreRender(renderCtx);
		clientThemeModule->onRender(renderCtx);
		DrawUtils::flush();

		__int64 retval = oText(screenView, renderCtx);

		bool shouldPostRender = true;

		{
			if (clickGuiModule->isEnabled()) {
				ClickGui::render();
				shouldPostRender = false;
			}
		}
		DrawUtils::flush();
		if (shouldPostRender) moduleMgr->onPostRender(renderCtx);
		DrawUtils::shouldToggleLeftClick = false;
		DrawUtils::shouldToggleRightClick = false;
		DrawUtils::flush();
		return retval;
	}
	return oText(screenView, renderCtx);
}
__int64 Hooks::drawImageCallback(MinecraftUIRenderContext* renderCtx, TexturePtr* texturePtr, Vec2* imagePos, Vec2* imageDimension, Vec2* UvPos, Vec2* UvSize) {
	static auto func = g_Hooks.drawImagekHook->GetFastcall<__int64, MinecraftUIRenderContext*, TexturePtr*, Vec2*, Vec2*, Vec2*, Vec2*>();
	if (!moduleMgr->isInitialized())
		return func(renderCtx, texturePtr, imagePos, imageDimension, UvPos, UvSize);

	static Colors* colorsMod = moduleMgr->getModule<Colors>();
	UIColor mainColor(colorsMod->getColor());
	static const bool coolHotbar = true;
	const string menu = getenv("AppData") + string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\" + Utils::getClientName() + "\\Assets\\MenuBackground.png");
	if (!Game.isInGame()) {
		if (strcmp(texturePtr->getFilePath().getText(), "textures/ui/title") == 0 && (~strcmp(texturePtr->getFilePath().getText(), "assets/minecraft/textures/gui/title/minecraft") == 0 || !(strcmp(texturePtr->getFilePath().getText(), "assets/minecraft/textures/gui/title/edition") == 0))) {
			Vec2 mousePos = *Game.getClientInstance()->getMousePos();

			// Convert mousePos to visual Pos
			{
				Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
				Vec2 windowSizeReal = Game.getClientInstance()->getGuiData()->windowSizeReal;

				mousePos = mousePos.div(windowSizeReal);
				mousePos = mousePos.mul(windowSize);
			}

			Vec4 rect = Vec4(-100.f, -100.f, Game.getGuiData()->widthGame + 100, Game.getGuiData()->heightGame + 100);
			Vec2 center(Game.getGuiData()->widthGame / 2, Game.getGuiData()->heightGame / 2);
			float diffX = 0;  // center.sub(mousePos).x;
			float diffY = 0;  // center.sub(mousePos).y;
			//DrawUtils::drawImage(menu, Vec2(rect.x - diffX, rect.y - diffY), Vec2(rect.z + diffX, rect.w + diffY), Vec2(0.f, 0.f), Vec2(1.f, 1.f), true);
		}
	}
	if (strcmp(texturePtr->getFilePath().getText(), "textures/ui/title") == 0) {
		Vec2 wid = Game.getGuiData()->windowSize;
		Vec2 textPos = Vec2((wid.x - DrawUtils::getTextWidth(&Utils::getClientName(), 7.5f)) / 2.f, imagePos->y);
		DrawUtils::drawText(textPos, &Utils::getClientName(), MC_Color(mainColor.r, mainColor.g, mainColor.b, 255), 7.5f, 1.f, true);
		return 0;
	}
	else if (strcmp(texturePtr->getFilePath().getText(), "textures/ui/selected_hotbar_slot") == 0) {
		static float PosX = imagePos->x;
		PosX = smoothLerp(imagePos->x, PosX, DrawUtils::deltaTime * 15.f);
		imagePos->x = PosX;
		//return 0;
	}
	/* // Cool Hotbar stuff
	const char* prefix = "textures/ui/hotbar_";
	int prefixLen = strlen(prefix);
	if (strncmp(texturePtr->filePath->filePathStr.getText(), prefix, prefixLen) == 0)
	{
		return 0;
	}
	*/
	return func(renderCtx, texturePtr, imagePos, imageDimension, UvPos, UvSize);
}

void Hooks::ChestBlockActor_tick(ChestBlockActor* _this, BlockSource* source) {
	static auto oTick = g_Hooks.ChestBlockActor_tickHook->GetFastcall<void, ChestBlockActor*, BlockSource*>();
	oTick(_this, source);
	static auto* storageEspMod = moduleMgr->getModule<StorageESP>();
	if (_this != nullptr && storageEspMod->isEnabled())
		GameData::addChestToList(_this);
}

void Hooks::Actor_lerpMotion(Entity* _this, Vec3 motVec) {
	static auto oLerp = g_Hooks.Actor_lerpMotionHook->GetFastcall<void, Entity*, Vec3>();

	if (Game.getLocalPlayer() != _this)
		return oLerp(_this, motVec);

	static auto noKnockbackmod = moduleMgr->getModule<Velocity>();
	if (noKnockbackmod->isEnabled()) {
		static void* networkSender = nullptr;
		if (!networkSender)
			networkSender = reinterpret_cast<void*>(9 + FindSignature("48 8B CB FF ?? ?? ?? ?? 00 C6 47 ?? 01 48 8B 5C 24"));

		if (networkSender == _ReturnAddress()) {
			motVec = _this->velocity.lerp(motVec, noKnockbackmod->xModifier, noKnockbackmod->yModifier, noKnockbackmod->xModifier);
		}
	}

	oLerp(_this, motVec);
}

int Hooks::AppPlatform_getGameEdition(__int64 _this) {
	static auto oGetEditon = g_Hooks.AppPlatform_getGameEditionHook->GetFastcall<signed int, __int64>();

	static auto mod = moduleMgr->getModule<EditionFaker>();
	if (mod->isEnabled()) {
		return mod->getFakedEditon();
	}

	return oGetEditon(_this);
}

void Hooks::PleaseAutoComplete(__int64 a1, __int64 a2, TextHolder* text, int a4) {
	static auto oAutoComplete = g_Hooks.PleaseAutoCompleteHook->GetFastcall<void, __int64, __int64, TextHolder*, int>();
	char* tx = text->getText();
	if (tx != nullptr && text->getTextLength() >= 1 && tx[0] == '.') {
		string search = tx + 1;                                              // Dont include the '.'
		transform(search.begin(), search.end(), search.begin(), ::tolower);  // make the search text lowercase

		struct LilPlump {
			string cmdAlias;
			IMCCommand* command = 0;
			bool shouldReplace = true;  // Should replace the current text in the box (autocomplete)

			bool operator<(const LilPlump& o) const {
				return cmdAlias < o.cmdAlias;
			}
		};  // This is needed so the set sorts it alphabetically

		set<LilPlump> searchResults;

		vector<IMCCommand*>* commandList = cmdMgr->getCommandList();
		for (auto it = commandList->begin(); it != commandList->end(); ++it) {  // Loop through commands
			IMCCommand* c = *it;
			auto* aliasList = c->getAliasList();
			for (auto it = aliasList->begin(); it != aliasList->end(); ++it) {  // Loop through aliases
				string cmd = *it;
				LilPlump plump;

				for (size_t i = 0; i < search.size(); i++) {  // Loop through search string
					char car = search.at(i);
					if (car == ' ' && i == cmd.size()) {
						plump.shouldReplace = false;
						break;
					}
					else if (i >= cmd.size())
						goto outerContinue;

					if (car != cmd.at(i))  // and compare
						goto outerContinue;
				}
				// Not at outerContinue? Then we got a good result!
				{
					cmd.insert(0, 1, '.');  // Prepend the '.'

					plump.cmdAlias = cmd;
					plump.command = c;
					searchResults.emplace(plump);
				}

			outerContinue:
				continue;
			}
		}

		if (!searchResults.empty()) {
			LilPlump firstResult = *searchResults.begin();

			size_t maxReplaceLength = firstResult.cmdAlias.size();
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin()++; it != searchResults.end(); it++) {
					auto alias = it->cmdAlias;
					maxReplaceLength = min(maxReplaceLength, alias.size());

					for (int i = 0; i < maxReplaceLength; i++) {
						if (alias[i] != firstResult.cmdAlias[i]) {
							maxReplaceLength = i;
							break;
						}
					}
				}
			}
			else
				maxReplaceLength = firstResult.cmdAlias.size();

			Game.getGuiData()->displayClientMessageF("==========");
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin(); it != searchResults.end(); ++it) {
					LilPlump plump = *it;
					Game.getGuiData()->displayClientMessageF("%s%s - %s%s", plump.cmdAlias.c_str(), GRAY, ITALIC, plump.command->getDescription());
				}
			}
			if (firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1)[0] == 0)
				Game.getGuiData()->displayClientMessageF("%s%s %s- %s", WHITE, firstResult.cmdAlias.c_str(), GRAY, firstResult.command->getDescription());
			else
				Game.getGuiData()->displayClientMessageF("%s%s %s %s- %s", WHITE, firstResult.cmdAlias.c_str(), firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1 /*exclude prefix*/), GRAY, firstResult.command->getDescription());

			if (firstResult.shouldReplace) {
				if (search.size() == firstResult.cmdAlias.size() - 1 && searchResults.size() == 1) {
					maxReplaceLength++;
					firstResult.cmdAlias.append(" ");
				}
				text->setText(firstResult.cmdAlias.substr(0, maxReplaceLength));  // Set text
				using syncShit = void(__fastcall*)(TextHolder*, TextHolder*);
				static syncShit sync = reinterpret_cast<syncShit>(Utils::FindSignature("40 53 48 83 EC ? 48 8B DA 48 8D 4C 24 ? E8 ? ? ? ? 90 48 8B 40 ? 48 8B 08 48 8B 01 48 8B D3 FF 90 ? ? ? ? 90 F0 48 FF 0D ? ? ? ? 48 8B 44 24 ? 48 85 C0 74 ? 48 83 38 ? 74 ? 80 7C 24 ? ? 74 ? F0 48 FF 0D ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? 80 7C 24 ? ? 74 ? E8 ? ? ? ? C6 44 24 ? ? 48 8D 4C 24 ? E8 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 80 7C 24 ? ? 74 ? 48 8B 4C 24 ? E8 ? ? ? ? 90 48 83 C4 ? 5B C3 B9 ? ? ? ? E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 40 53"));
				sync(text, text);
			}
		}
		return;
	}
	oAutoComplete(a1, a2, text, a4);
}

void Hooks::LoopbackPacketSender_sendToServer(LoopbackPacketSender* a, Packet* packet) {
	static auto oFunc = g_Hooks.LoopbackPacketSender_sendToServerHook->GetFastcall<void, LoopbackPacketSender*, Packet*>();

	static auto autoSneakMod = moduleMgr->getModule<AutoSneak>();
	static auto blinkMod = moduleMgr->getModule<Blink>();
	static auto noPacketMod = moduleMgr->getModule<NoPacket>();
	static auto noSwingMod = moduleMgr->getModule<NoSwing>();
	static AutoCrystal* autoCrystalMod = moduleMgr->getModule<AutoCrystal>();
	if (autoCrystalMod->isEnabled() && autoCrystalMod->shouldChangeID) {
		if (packet->isInstanceOf<InventoryTransactionPacket>()) {
			InventoryTransactionPacket* itp = reinterpret_cast<InventoryTransactionPacket*>(packet);
			if (itp->complexTransaction->actionType == 3) {
				ComplexInventoryTransaction* invComplex = itp->complexTransaction;
				*(int*)((__int64)(invComplex)+104) = autoCrystalMod->highestID;
			}
		}
		else if (packet->isInstanceOf<LevelSoundEventPacket>()) return;
	}
	/*
	if (packet->isInstanceOf<InventoryTransactionPacket>()) {
		InventoryTransactionPacket* pk = (InventoryTransactionPacket*)(packet);
		Game.getGuiData()->displayClientMessageF("%i", pk->complexTransaction->actionType);
		if (pk->complexTransaction->actionType == 2) {
			ItemUseInventoryTransaction* test = (ItemUseInventoryTransaction*)pk->complexTransaction;
			NetworkItemStackDescriptor a(Game.getLocalPlayer()->getSupplies()->inventory->getItemStack(0));
			//test->networkItemStack = a;
			test->slot = 0;
		}
	}
	*/

	if (noSwingMod->isEnabled() && noSwingMod->server && packet->isInstanceOf<AnimatePacket>())
		return;

	//if (noPacketMod->isEnabled())
	//	return;
	if (noPacketMod->isEnabled()) {
		if (noPacketMod->SoundPacket && packet->isInstanceOf<LevelSoundEventPacket>()) return;
	}

	if (blinkMod->isEnabled()) {
		if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
			if (blinkMod->isEnabled()) {
				if (packet->isInstanceOf<C_MovePlayerPacket>()) {
					C_MovePlayerPacket* meme = reinterpret_cast<C_MovePlayerPacket*>(packet);
					meme->onGround = true;                                                            // Don't take Fall Damages when turned off
					blinkMod->getMovePlayerPacketHolder()->push_back(new C_MovePlayerPacket(*meme));  // Saving the packets
				}
				else {
					blinkMod->getPlayerAuthInputPacketHolder()->push_back(new PlayerAuthInputPacket(*reinterpret_cast<PlayerAuthInputPacket*>(packet)));
				}
			}
			return;  // Dont call LoopbackPacketSender_sendToServer
		}
	}
	else if (!blinkMod->isEnabled()) {
		if (blinkMod->getMovePlayerPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getMovePlayerPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getMovePlayerPacketHolder()->clear();
			return;
		}
		if (blinkMod->getPlayerAuthInputPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getPlayerAuthInputPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getPlayerAuthInputPacketHolder()->clear();
			return;
		}
	}

	if (autoSneakMod->isEnabled() && Game.getLocalPlayer() != nullptr && autoSneakMod->silentlol && packet->isInstanceOf<PlayerActionPacket>()) {
		auto* pp = reinterpret_cast<PlayerActionPacket*>(packet);

		if (pp->action == 12 && pp->entityRuntimeId == Game.getLocalPlayer()->entityRuntimeId)
			return;  // dont send uncrouch
	}

	moduleMgr->onSendPacket(packet);

	/*if (strcmp(packet->getName()->getText(), "EmotePacket") == 0) {
		auto varInt = reinterpret_cast<__int64*>(reinterpret_cast<__int64>(packet) + 0x28);
		auto text = reinterpret_cast<TextHolder*>(reinterpret_cast<__int64>(packet) + 0x30);
		auto bet = reinterpret_cast<unsigned char*>(reinterpret_cast<__int64>(packet) + 0x50);
		logF("emote %llX %s %i", *varInt, text->getText(), *bet);
	} fix emote crashing*/
	oFunc(a, packet);
}

void Hooks::LoopbackPacketSender_sendToClient(networkhandler* _this, const void* networkIdentifier, Packet* packet, int a4) {
	auto func = g_Hooks.LoopbackPacketSender_sendToClientHook->GetFastcall<void, networkhandler*, const void*, Packet*, int>();

	moduleMgr->onSendClientPacket(packet);
	func(_this, networkIdentifier, packet, a4);
}

float Hooks::LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3) {
	static auto oGetFov = g_Hooks.LevelRendererPlayer_getFovHook->GetFastcall<float, __int64, float, bool>();
	return oGetFov(_this, a2, a3);
}

void Hooks::MultiLevelPlayer_tick(EntityList* _this) {
	static auto oTick = g_Hooks.MultiLevelPlayer_tickHook->GetFastcall<void, EntityList*>();
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer != nullptr && localPlayer->getGameMode() != nullptr) moduleMgr->onTick(localPlayer->getGameMode());
	oTick(_this);
	GameData::EntityList_tick(_this);
}

bool Hooks::GameMode_startDestroyBlock(GameMode* _this, Vec3i* a2, uint8_t face, bool* a4) {
	static auto oFunc = g_Hooks.GameMode_startDestroyBlockHook->GetFastcall<bool, GameMode*, Vec3i*, uint8_t, bool*>();
	static PacketMine* packetMineMod = moduleMgr->getModule<PacketMine>();

	if (packetMineMod->isEnabled()) {
		packetMineMod->setBreakPos(_this, a2, face);
		oFunc(_this, a2, face, a4);
		return false;
	}

	return oFunc(_this, a2, face, a4);
}

void Hooks::GameMode__stopDestroyBlock(GameMode* _this, Vec3i* blockPos) {
	static auto func = g_Hooks.GameMode__stopDestroyBlockHook->GetFastcall<void, GameMode*, Vec3i*>();

	static PacketMine* packetMineMod = moduleMgr->getModule<PacketMine>();
	if (packetMineMod->isEnabled()) return;

	func(_this, blockPos);
}

void Hooks::HIDController_keyMouse(HIDController* _this, void* a2, void* a3) {
	static auto oFunc = g_Hooks.HIDController_keyMouseHook->GetFastcall<void, HIDController*, void*, void*>();
	GameData::setHIDController(_this);
	isTicked = true;
	oFunc(_this, a2, a3);
	return;
}

int Hooks::BlockLegacy_getRenderLayer(BlockLegacy* a1) {
	static auto oFunc = g_Hooks.BlockLegacy_getRenderLayerHook->GetFastcall<int, BlockLegacy*>();

	static auto xrayMod = moduleMgr->getModule<Xray>();
	if (xrayMod->isEnabled()) {
		char* text = a1->name.getText();
		if (strstr(text, "ore") == NULL)
			if (strcmp(text, "lava") != NULL)
				if (strcmp(text, "water") != NULL)
					if (strcmp(text, "portal") != NULL)
						if (strcmp(text, "amethyst_block") != NULL)
							if (strcmp(text, "ancient_debris") != NULL)
								if (strcmp(text, "command_block") != NULL)
									if (strcmp(text, "repeating_command_block") != NULL)
										if (strcmp(text, "chain_command_block") != NULL)
											if (strcmp(text, "structure_block") != NULL)
												if (strcmp(text, "deny") != NULL)
													if (strcmp(text, "allow") != NULL)
														if (strcmp(text, "bedrock") != NULL)
															if (strcmp(text, "border_block") != NULL)
																return 10;
	}
	return oFunc(a1);
}

__int8* Hooks::BlockLegacy_getLightEmission(BlockLegacy* a1, __int8* a2) {
	static auto oFunc = g_Hooks.BlockLegacy_getLightEmissionHook->GetFastcall<__int8*, BlockLegacy*, __int8*>();
	static auto xrayMod = moduleMgr->getModule<Xray>();

	if (!xrayMod || !xrayMod->isEnabled())
		return oFunc(a1, a2);

	*a2 = 15;
	return a2;
}

__int64 Hooks::LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3) {
	static auto oFunc = g_Hooks.LevelRenderer_renderLevelHook->GetFastcall<__int64, __int64, __int64, __int64>();

	using reloadShit_t = void(__fastcall*)(__int64);
	static reloadShit_t reloadChunk = nullptr;

	if (!reloadChunk) {
		// RenderChunkCoordinator::rebuildAllRenderChunkGeometry
		reloadChunk = reinterpret_cast<reloadShit_t>(FindSignature("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B F9 48 8D A9"));
	}

	static auto xrayMod = moduleMgr->getModule<Xray>();

	static bool lastState = false;
	if (lastState != xrayMod->isEnabled()) {
		// LevelRenderer::rebuildAllRenderChunkGeometry
		lastState = xrayMod->isEnabled();
		unsigned long long* v5;  // rdi
		unsigned long long* i;   // rbx

		v5 = *(unsigned long long**)(_this + 32);
		for (i = (unsigned long long*) * v5; i != v5; i = (unsigned long long*) * i)
			reloadChunk(i[3]);
	}

	__int64 ret = oFunc(_this, a2, a3);

	if (DrawUtils::readyToRender) {
		DrawUtils::setGameRenderContext(a2);
		moduleMgr->onLevelRender();
		DrawUtils::setGameRenderContext(0);
	}

	return ret;
}

void Hooks::ClickFunc(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
	static auto oFunc = g_Hooks.ClickFuncHook->GetFastcall<void, __int64, char, char, __int16, __int16, __int16, __int16, char>();
	static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>();

	if (g_Hooks.initImGui && ImGui::GetCurrentContext() != nullptr) {
		ImGuiIO& io = ImGui::GetIO();
		if (mouseButton > 0 && mouseButton < 3) {
			io.AddMouseButtonEvent(mouseButton - 1, isDown);
		}
		else if (mouseButton == 4) {
			io.AddMouseWheelEvent(0, (isDown < 0) ? -0.5f : 0.5f);
		}
		else {
			io.AddMousePosEvent(mouseX, mouseY);
		}
		if (io.WantCaptureMouse && isDown) {
			isDown = false;
		}
	}
	// MouseButtons
	// 0 = mouse move
	// 1 = left click
	// 2 = right click
	// 3 = middle click
	// 4 = scroll   (isDown: 120 (SCROLL UP) and -120 (SCROLL DOWN))

	DrawUtils::onMouseClickUpdate((int)mouseButton, isDown);

	if (clickGuiModule->isEnabled()) {
		if (mouseButton == 4) {
			ClickGui::onWheelScroll(isDown > 0);
		}
		if (mouseButton != 0)  // Mouse click event
			return;
	}
	return oFunc(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

__int64 Hooks::MoveInputHandler_tick(MoveInputHandler* a1, Entity* a2) {
	static auto oTick = g_Hooks.MoveInputHandler_tickHook->GetFastcall<__int64, MoveInputHandler*, Entity*>();

	auto ret = oTick(a1, a2);
	moduleMgr->onMove(a1);

	return 0;
}

__int64 Hooks::ChestScreenController_tick(ChestScreenController* a1) {
	static auto oFunc = g_Hooks.ChestScreenController_tickHook->GetFastcall<__int64, ChestScreenController*>();

	static auto chestStealerMod = moduleMgr->getModule<ChestStealer>();
	if (chestStealerMod->isEnabled()) chestStealerMod->chestScreenController_tick(a1);

	return oFunc(a1);
}

float Hooks::GetGamma(uintptr_t* a1) {
	static auto fullbright = moduleMgr->getModule<FullBright>();
	static auto xrayMod = moduleMgr->getModule<Xray>();
	static auto nametagmod = moduleMgr->getModule<NameTags>();
	static auto zoomMod = moduleMgr->getModule<Zoom>();
	static auto viewMod = moduleMgr->getModule<ViewModel>();
	static NoRender* noRenderMod = moduleMgr->getModule<NoRender>();

	uintptr_t** list = (uintptr_t**)a1;

	char obtainedSettings = 0;
	bool hadIt = false;
	for (uint16_t i = 3; i < 450; i++) {
		if (list[i] == nullptr) continue;
		uintptr_t* info = *(uintptr_t**)((uintptr_t)list[i] + 8);
		if (info == nullptr) continue;

		TextHolder* translateName = (TextHolder*)((uintptr_t)info + 0x1D8);
		TextHolder* settingname = (TextHolder*)((uintptr_t)info + 0x188);
		bool* settingGame = (bool*)((uintptr_t)list[i] + 16);
		if (!strcmp(translateName->getText(), "options.smoothlighting")) {
			if (hadIt) continue;
			xrayMod->smoothLightningSetting = settingGame;
			obtainedSettings++;
			hadIt = true;
		}
		else if (!strcmp(settingname->getText(), "gfx_viewbobbing")) {
			if (viewMod->isEnabled()) *settingGame = true;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderBlockEntities")) {
			if (noRenderMod->isEnabled() && noRenderMod->blockEntities) *settingGame = true;
			else *settingGame = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderParticles")) {
			if (noRenderMod->isEnabled() && noRenderMod->particles) *settingGame = true;
			else *settingGame = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderEntities")) {
			if (noRenderMod->isEnabled() && noRenderMod->entities) *settingGame = true;
			else *settingGame = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderWeather")) {
			if (noRenderMod->isEnabled() && noRenderMod->weather) *settingGame = true;
			else *settingGame = false;
			obtainedSettings++;
		}
		if (obtainedSettings == 6) break;
	}

	if (xrayMod->isEnabled())
		return 25.f;

	if (fullbright->isEnabled())
		return 12.f;

	static auto ofunc = g_Hooks.GetGammaHook->GetFastcall<float, uintptr_t*>();
	return ofunc(a1);
}

void Hooks::JumpPower(Entity* a1, float a2) {
	static auto oFunc = g_Hooks.JumpPowerHook->GetFastcall<void, Entity*, float>();
	static auto highJumpMod = moduleMgr->getModule<HighJump>();
	if (highJumpMod->isEnabled() && Game.getLocalPlayer() == a1) {
		a1->velocity.y = highJumpMod->jumpPower;
		return;
	}
	oFunc(a1, a2);
}

void Hooks::Actor_swing(Entity* _this) {
	static auto oFunc = g_Hooks.Actor_swingHook->GetFastcall<void, Entity*>();
	static auto noSwingMod = moduleMgr->getModule<NoSwing>();

	if (noSwingMod->isEnabled() && !noSwingMod->server)
		return;

	oFunc(_this);
}

void Hooks::Actor_startSwimming(Entity* _this) {
	static auto oFunc = g_Hooks.Actor_startSwimmingHook->GetFastcall<void, Entity*>();

	static auto jesusModule = moduleMgr->getModule<Jesus>();
	if (jesusModule->isEnabled() && Game.getLocalPlayer() == _this)
		return;

	oFunc(_this);
}

void Hooks::RakNetInstance_tick(RakNetInstance* _this, __int64 a2, __int64 a3) {
	static auto oTick = g_Hooks.RakNetInstance_tickHook->GetFastcall<void, RakNetInstance*, __int64, __int64>();
	GameData::setRakNetInstance(_this);
	oTick(_this, a2, a3);
}

float Hooks::GameMode_getPickRange(GameMode* _this, __int64 a2, char a3) {
	static auto oFunc = g_Hooks.GameMode_getPickRangeHook->GetFastcall<float, GameMode*, __int64, char>();

	if (Game.getLocalPlayer() != nullptr) {
		static auto BlockReachModule = moduleMgr->getModule<BlockReach>();
		if (BlockReachModule->isEnabled())
			return BlockReachModule->getBlockReach();

		static auto teleportModule = moduleMgr->getModule<Teleport>();
		if (teleportModule->isEnabled())
			return 255;
	}

	return oFunc(_this, a2, a3);
}

__int64 Hooks::ConnectionRequest_create(__int64 _this, __int64 privateKeyManager, void* a3, TextHolder* selfSignedId, TextHolder* serverAddress, __int64 clientRandomId, TextHolder* skinId, SkinData* skinData, __int64 capeData, CoolSkinData* coolSkinStuff, TextHolder* deviceId, int inputMode, int uiProfile, int guiScale, TextHolder* languageCode, bool sendEduModeParams, TextHolder* tenantId, __int64 unused, TextHolder* platformUserId, TextHolder* thirdPartyName, bool thirdPartyNameOnly, TextHolder* platformOnlineId, TextHolder* platformOfflineId, TextHolder* capeId) {
	static auto oFunc = g_Hooks.ConnectionRequest_createHook->GetFastcall<__int64, __int64, __int64, void*, TextHolder*, TextHolder*, __int64, TextHolder*, SkinData*, __int64, CoolSkinData*, TextHolder*, int, int, int, TextHolder*, bool, TextHolder*, __int64, TextHolder*, TextHolder*, bool, TextHolder*, TextHolder*, TextHolder*>();
	Notifications::addNotifBox("Connecting to server...", 5.f);
	moduleMgr->onJoiningServer();
	__int64 res = oFunc(_this, privateKeyManager, a3, selfSignedId, serverAddress, clientRandomId, skinId, skinData, capeData, coolSkinStuff, deviceId, inputMode, uiProfile, guiScale, languageCode, sendEduModeParams, tenantId, unused, platformUserId, thirdPartyName, thirdPartyNameOnly, platformOnlineId, platformOfflineId, capeId);
	return res;
}

void Hooks::InventoryTransactionManager_addAction(InventoryTransactionManager* a1, InventoryAction* a2) {
	static auto Func = g_Hooks.InventoryTransactionManager_addActionHook->GetFastcall<void, InventoryTransactionManager*, InventoryAction*>();
	Func(a1, a2);
}

__int64 Hooks::SkinRepository___loadSkinPack(__int64 _this, __int64 pack, __int64 a3) {
	static auto func = g_Hooks.SkinRepository___loadSkinPackHook->GetFastcall<__int64, __int64, __int64, __int64>();

	// auto res = (*(unsigned __int8 (**)(void))(**(__int64**)(pack + 8) + 48i64))();
	// logF("SkinRepository___loadSkinPack: origin %i, is Trusted: %i", *(int*)((*(__int64*)pack) + 888i64), res);
	*(int*)((*(__int64*)pack) + 888i64) = 2;  // Set pack origin to "2"

	return func(_this, pack, a3);
}

GamerTextHolder* Hooks::toStyledString(__int64 strIn, GamerTextHolder* strOut) {
	static auto func = g_Hooks.toStyledStringHook->GetFastcall<GamerTextHolder*, __int64, GamerTextHolder*>();

	static auto conRequest = reinterpret_cast<__int64>(g_Hooks.ConnectionRequest_createHook->funcPtr);
	if (reinterpret_cast<__int64>(_ReturnAddress()) > conRequest && reinterpret_cast<__int64>(_ReturnAddress()) < conRequest + 10000 && overrideStyledReturn) {
		// Return address is within boundaries of connection request function

		strOut->copyFrom(&styledReturnText);

		return strOut;
	}

	return func(strIn, strOut);
}

__int64 Hooks::InGamePlayScreen___renderLevel(__int64 playScreen, __int64 a2, __int64 a3) {
	static auto func = g_Hooks.InGamePlayScreen___renderLevelHook->GetFastcall<__int64, __int64, __int64, __int64>();
	return func(playScreen, a2, a3);
}

__int64 Hooks::GameMode_attack(GameMode* _this, Entity* ent) {
	static auto func = g_Hooks.GameMode_attackHook->GetFastcall<__int64, GameMode*, Entity*>();
	moduleMgr->onAttack(ent);
	return func(_this, ent);
}

void Hooks::LocalPlayer__updateFromCamera(__int64 a1, Camera* camera, __int64* a3, Entity* a4) {
	static auto func = g_Hooks.LocalPlayer__updateFromCameraHook->GetFastcall<__int64, __int64, Camera*, __int64*, Entity*>();
	auto freecamMod = moduleMgr->getModule<Freecam>();
	freecamMod->camera = camera;
	//camera->nearClippingPlane = 0.000001;
	//camera->farClippingPlane = -5;
	func(a1, camera, a3, a4);
}

bool Hooks::Mob__isImmobile(Entity* ent) {
	static auto func = g_Hooks.Mob__isImmobileHook->GetFastcall<bool, Entity*>();
	static auto freecamMod = moduleMgr->getModule<Freecam>();
	static auto antiImmobileMod = moduleMgr->getModule<AntiImmobile>();
	if (freecamMod->isEnabled())
		return true;
	if (antiImmobileMod->isEnabled() && ent == Game.getLocalPlayer())
		return false;

	return func(ent);
}

void Hooks::Actor__setRot(Entity* _this, Vec2& angle) {
	static auto func = g_Hooks.Actor__setRotHook->GetFastcall<void, Entity*, Vec2&>();
	static auto killauraMod = moduleMgr->getModule<Killaura>();
	static auto freelookMod = moduleMgr->getModule<Freelook>();
	static auto freecamMod = moduleMgr->getModule<Freecam>();
	if (_this == Game.getLocalPlayer()) {
		if (freecamMod->isEnabled()) {
			freecamMod->yaw = angle.y;
			angle = { freecamMod->initialViewAngles.x, freecamMod->initialViewAngles.y };
		}
		if (killauraMod->isEnabled() && !killauraMod->targetListEmpty && killauraMod->rotations) angle = killauraMod->angle;
		if (freelookMod->isEnabled()) angle = freelookMod->oldPos;
	}
	func(_this, angle);
}

void Hooks::InventoryTransactionManager__addAction(InventoryTransactionManager* _this, InventoryAction& action) {
	static auto func = g_Hooks.InventoryTransactionManager__addActionHook->GetFastcall<void, InventoryTransactionManager*, InventoryAction&>();

#ifdef TEST_DEBUG
	char* srcName = "none";
	if (action.sourceItem.item && *action.sourceItem.item)
		srcName = (*action.sourceItem.item)->name.getText();
	char* targetName = "none";
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

void Hooks::LevelRendererPlayer__renderNameTags(__int64 a1, __int64 a2, TextHolder* a3, __int64 a4) {
	static auto func = g_Hooks.LevelRendererPlayer__renderNameTagsHook->GetFastcall<void, __int64, __int64, TextHolder*, __int64>();
	//Game.getGuiData()->displayClientMessageF("%s", a3->getText());
	return func(a1, a2, a3, a4);
}

float Hooks::getDestroySpeed(Player* _this, Block& block) {
	static auto oFunc = g_Hooks.getDestroySpeedHook->GetFastcall<float, Player*, Block&>();
	static auto instaBreakMod = moduleMgr->getModule<InstaBreak>();
	return instaBreakMod->isEnabled() ? INFINITY : oFunc(_this, block);
}

bool Hooks::Actor__isInWall(Entity* ent) {
	static auto func = g_Hooks.ActorisInWallHook->GetFastcall<bool, Entity*>();
	static auto nofallMod = moduleMgr->getModule<NoFall>();

	if (nofallMod->isEnabled() && nofallMod->mode.selected == 4 /*&& Game.getLocalPlayer() == ent*/) {
		return false;
	}

	return func(ent);
}

bool Hooks::Actor_isOnFire(Entity* _this) {
	static auto Func = g_Hooks.Actor_isOnFireHook->GetFastcall<bool, Entity*>();
	static NoRender* noRenderMod = moduleMgr->getModule<NoRender>();
	if (noRenderMod->isEnabled() && noRenderMod->fire && _this == Game.getLocalPlayer()) return false;
	return Func(_this);
}
/*
void Hooks::testFunction(class networkhandler* _this, const void* networkIdentifier, Packet* packet, int a4) {
	auto func = g_Hooks.testHook->GetFastcall<void, networkhandler*, const void*, Packet*, int>();
	static auto test = moduleMgr->getModule<TestModule>();

	if (test->isEnabled()) {
		//if (strcmp(packet->getName()->getText(), "SetTitlePacket") != 0) {
			Game.getClientInstance()->getGuiData()->displayClientMessageF("%s", packet->getName()->getText());
		//}
	}
	if (test->isEnabled() && test->bool1) {
		return;
	}
	moduleMgr->onSendClientPacket(packet);
	func(_this, networkIdentifier, packet, a4);
}*/