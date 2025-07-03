// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../Utils/RenderUtils.h"
#include "../../../../SDK/Classes/ScreenView.h"
#include "../../../../SDK/Classes/UILayer.h"
#include "../../../../SDK/Classes/MinecraftUIRenderContext.h"
#include "../../../../SDK/GameData.h"

#include "../MinecraftUIRenderContext/DrawImageHook.h"
#include "../MinecraftUIRenderContext/DrawTextHook.h"
#include "../Others/SendPacketHook.h"
#include "../Others/NineSlicehook.h"

#include "../../../Client.h"
Vec2<float> titlePos = { 0,0 };
float anim = 0;
Vec2<float> titleSize = { 0,0 };
namespace GuiInfo
{
	static float DeltaTime = 0.016f;
	static float GuiScale = 1;

	static Vec2<float> TrueScreenRes = { 0, 0 };
	static Vec2<float> ScreenRes = { 0, 0 };

	static Vec2<short> TrueMousePos = { 0, 0 };
	static Vec2<short> MousePos = { 0, 0 };
}
class SetUpAndRenderHook {
protected:
	using func_t = __int64(__fastcall*)(ScreenView*, MinecraftUIRenderContext*);
	static inline func_t func;

	static __int64 SetUpAndRender(ScreenView* _this, MinecraftUIRenderContext* renderCtx) {
		mc.setClientInstance(renderCtx->clientInstance);
		RenderUtils::SetUp(_this, renderCtx);
		static bool hooked = false;
		if (UILayer::Is(_this, UILayer::MainMenu_Screen))
		{
			anim = Math::lerp(1, anim, GuiInfo::DeltaTime * 10);
			Vec4<float> screenRes = Vec4<float>(0, 0, GuiInfo::ScreenRes.x, GuiInfo::ScreenRes.y);

			// Dont put below 2 <3
			constexpr float finalZoom = 2;
			static float zoom = 0, opacity = 0;
			zoom = Math::lerp(20, finalZoom, anim), opacity = Math::lerp(0, 1, anim);

			Vec2<float> size = Vec2<float>(screenRes.z * zoom, screenRes.w * zoom);

			// Lerp our mouse pos for smooth effect
			static float x = GuiInfo::MousePos.x, y = GuiInfo::MousePos.y;
			x = Math::Math::lerp(GuiInfo::MousePos.x, x, GuiInfo::DeltaTime * 8);
			y = Math::Math::lerp(GuiInfo::MousePos.y, y, GuiInfo::DeltaTime * 8);
			Vec2<float> mousePos = Vec2<float>(x, y);

			// Calculate the offset from the middle of the screen ye
			Vec2<float> rectCenter = Vec2<float>(screenRes.z / 2, screenRes.w / 2);
			Vec2<float> offset = mousePos - rectCenter;
			Vec2<float> rectPosition = rectCenter - offset - Vec2<float>(size.x / 2, size.y / 2);

			// Final rect area
			Vec4<float> rect = Vec4<float>(rectPosition.x, rectPosition.y, rectPosition.x + size.x, rectPosition.y + size.y);

			RenderUtils::renderImage(Utils::getClientPath() + "Assets\\mainMenuBackground.jpg", rect);
			RenderUtils::flushImage(MC_Color(255, 255, 255), opacity);
		}
		if (!hooked) {
			uintptr_t* CtxVTable = *(uintptr_t**)renderCtx;
			uintptr_t* loopbackPacketSenderVTable = *(uintptr_t**)(renderCtx->clientInstance->loopbackPacketSender);
			DrawTextHook::init(CtxVTable[5]);
			DrawImageHook::init(CtxVTable[7]);
			SendPacketHook::init(loopbackPacketSenderVTable[1]);
			NineSlicehook::init(CtxVTable[8]);
			hooked = true;
		}
		__int64 result = func(_this, renderCtx);
		if (_this->visualTree->root->GetName() == UILayer::Debug_DebugScreen) {
			client->moduleMgr->onRender(renderCtx);
		}
		return result;
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::SetupAndRenderHook);
		MemoryUtils::CreateHook("SetUpAndRenderHook", address, (void*)&SetUpAndRenderHook::SetUpAndRender, (void*)&func);
	}
};