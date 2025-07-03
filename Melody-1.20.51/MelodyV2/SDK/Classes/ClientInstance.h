// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "MinecraftGame.h"
#include "Moveinputhand.h"
#include "LoopbackPacketSender.h"
#include "GuiData.h"
#include "LocalPlayer.h"
#include "LevelRenderer.h"
#include "UIProfanityContext.h"
#include "../../Utils/MemoryUtils.h"
#include "Minecraft.h"

class ClientInstance {
	uintptr_t** VTable;
public:
	BUILD_ACCESS(this, MinecraftGame*, minecraftGame, 0xC8);
	BUILD_ACCESS(this, LoopbackPacketSender*, loopbackPacketSender, 0xF0);
	BUILD_ACCESS(this, GuiData*, guiData, 0x560);
	BUILD_ACCESS(this, Minecraft*, minecraft, 0xD0);
	BUILD_ACCESS(this, MoveInputHand*, moveinput, 0x1F0);
	//BUILD_ACCESS(this, UIProfanityContext*, getProfanityContext, 0xBA0);
	//BA0
public:
	glmatrixf* getbadrefdef() {
		return (glmatrixf*)((uintptr_t)(this) + 0x330);
	}
	Vec2<float> getMousePos() {
		return *reinterpret_cast<Vec2<float>*>((uintptr_t)this + 0x498);
	}
	Vec2<float> getFov()
	{
		float fovX = *reinterpret_cast<float*>((uintptr_t)(this) + 0x6F8);
		float fovY = *reinterpret_cast<float*>((uintptr_t)(this) + 0x70C);
		return Vec2<float>(fovX, fovY);
	}
	LocalPlayer* getLocalPlayer() {
		return MemoryUtils::CallVFunc<28, LocalPlayer*>(this);
	}
	/*[[nodiscard]] UIProfanityContext* getProfanityContext()
	{
		return MemoryUtils::CallVFunc<2976, UIProfanityContext*>(this);
	}*/
	LevelRenderer* getLevelRenderer() {
		return MemoryUtils::CallVFunc<198, LevelRenderer*>(this);
	}

	void grabMouse() {
		MemoryUtils::CallVFunc<326, void>(this);
	}

	void releasebMouse() {
		MemoryUtils::CallVFunc<327, void>(this);
	}
};
