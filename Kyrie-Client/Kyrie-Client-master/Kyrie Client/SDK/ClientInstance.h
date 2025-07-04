#pragma once
#include "GuiData.h"
#include "../Utils/Utils.h"
#include "Player.h"
#include "LevelRenderer.h"

class ClientInstance {
private:
	uintptr_t** VTable;
public:
	BUILD_ACCESS(this, LevelRenderer*, levelRenderer, 0x00E0);
	//BUILD_ACCESS(this, glmatrixf*, badrefdef, 0x330);
	BUILD_ACCESS(this, GuiData*, guiData, 0x560);
public:

	glmatrixf* getBadRefDef() {
		return reinterpret_cast<glmatrixf*>((uintptr_t)(this) + 0x330);
	};

	Vec2<float>* getMousePos() {
		return reinterpret_cast<Vec2<float>*>((uintptr_t)this + 0x498);
	}

	Vec2<float> getFov() {
		Vec2<float> fov;
		fov.x = *reinterpret_cast<float*>((uintptr_t)(this) + 0x6F8);
		fov.y = *reinterpret_cast<float*>((uintptr_t)(this) + 0x70C);
		return fov;
	}

	Player* getLocalPlayer() {
		return Utils::CallVFunc<27, Player*>(this);
	}

	void grabMouse() {
		Utils::CallVFunc<322, void>(this);
	}

	void releasebMouse() {
		Utils::CallVFunc<323, void>(this);
	}
};