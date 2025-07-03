#pragma once

#include "Font.h"
#include "GLMatrix.h"
#include "MinecraftGame.h"
#include "TimerClass.h"
#include "GuiData.h"
#include "LevelRender.h"
#include "bgfx.h"
#include "../Network/RakNetConnector.h"
#include "../Network/ConnectionRequest.h"

// Define 
#define LocalPlayerIndex 30 // 1.21.73
#define BockSourceIndex 29 // 1.21.73
#define DisableInputIndex 342 // 1.21.73
#define ReleaseMouseIndex 343 // 1.21.73
#define GrabMouseIndex 344 // 1.21.73

class ClientInstance {
public:
	// Use CallVFunc to call the VTables by there index.

	Player* getLocalPlayer() {
		return (Player*)Memory::CallVFunc<LocalPlayerIndex, Player*>(this); // 1.21.73
	}

	BlockSource* getBlockSource() {
		return Memory::CallVFunc<BockSourceIndex, BlockSource*>(this); // 1.21.73
	}

	void setDisableInput(bool disable) {
		return Memory::CallVFunc<DisableInputIndex, void>(this, disable); // 1.21.73
	}

	void releaseMouse() { // grab - 1
		return Memory::CallVFunc<ReleaseMouseIndex, void>(this); // 1.21.73
	}

	void grabMouse() {
		return Memory::CallVFunc<GrabMouseIndex, void>(this); // 1.21.73
	}

	bgfx_context* getBGFX()
	{
		static auto pCtx = Memory::scanSig("48 8B 0D ? ? ? ? 66 44 89 74 24 ? E8 ? ? ? ? 0F B7 08 66 89 0F E9 ? ? ? ? 8B 53 3C 49 2B C8", 3);
		return *reinterpret_cast<bgfx_context**>(pCtx);
	}

	Vector2<float> getFov() { // Check for fov in clientinstance
		Vector2<float> fov;
		fov.x = *(float*)(reinterpret_cast<uintptr_t>(this) + 0x748); // 1.21.73
		fov.y = *(float*)(reinterpret_cast<uintptr_t>(this) + 0x75C); // 1.21.73
		return fov;
	}

	GLMatrix* getGLMatrix() {
		return (GLMatrix*)((uintptr_t)(this) + 0x388); // 1.21.73
	}

	inline bool WorldToScreen(Vector3<float> pos, Vector2<float>& screen)
	{ // pos = pos 2 w2s, screen = output screen coords
		if (!getGuiData()) {
			return false;
		}
		Vector2<float> displaySize = getGuiData()->getmcResolution();
		class LevelRender* lr = getLevelRender();
		Vector3<float> origin = lr->getOrigin();
		Vector2<float> fov = getFov();

		pos.x -= origin.x;
		pos.y -= origin.y;
		pos.z -= origin.z;

		auto glmatrix = getGLMatrix();
		std::shared_ptr<GLMatrix> matrix = std::shared_ptr<GLMatrix>(glmatrix->correct());

		float x = matrix->transformx(pos);
		float y = matrix->transformy(pos);
		float z = matrix->transformz(pos);

		if (z > 0) return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}
public:
	BUILD_ACCESS(class LoopbackPacketSender*, LoopbackPacketSender, 0xF8); // 1.21.73
	BUILD_ACCESS(class MinecraftGame*, MinecraftGame, 0xD0); // 1.21.73
	BUILD_ACCESS(class TimerClass*, TimerClass, 0xD8); // 1.21.73
	BUILD_ACCESS(class GuiData*, GuiData, 0x5B0); // 1.21.73
	BUILD_ACCESS(class LevelRender*, LevelRender, 0xE8); // 1.21.73
};