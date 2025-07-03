#pragma once

#include "Render/Font.h"
#include "Matrix/GLMatrix.h"
#include "Render/LevelRender.h"
#include "Minecraft/Minecraft.h"
#include "Minecraft/Game/MinecraftGame.h"
#include "Minecraft/Game/Options.h"
#include "GuiData.h"
#include "../Packet/Network/RakNetConnector.h"

// Minecraft Sim: 48 8B ? ? ? ? ? E8 ? ? ? ? 88 87 ? ? ? ? 48 8B ? ? ? ? ? 48 8B

class ClientInstance {
public:
	Player* getLocalPlayer() {
		return Memory::CallVirtualFunc<Player*>(OffsetManager::ClientInstance_getLocalPlayer, this);
	}

	BlockSource* getBlockSource() {
		return getLocalPlayer()->getComponent<BlockSourceComponent>()->mRegion;
	}

	Options* getOptions() {
		return Memory::CallVirtualFunc<Options*>(OffsetManager::ClientInstance_getOptions, this);
	}

	bool isMouseGrabbed() {
		return Memory::CallVirtualFunc<bool>(OffsetManager::ClientInstance_isMouseGrabbed, this);
	}

	void releaseMouse() {
		static void* func = SigManager::ClientInstance_releaseMouse;
		Memory::CallFastcall<void>(func, this);
	}

	void grabMouse() {
		static void* func = SigManager::ClientInstance_grabMouse;
		Memory::CallFastcall<void>(func, this);
	}

	Vector2<float> getFov() {
		float x = hat::member_at<float>(this, OffsetManager::ClientInstance_mFovX);
		float y = hat::member_at<float>(this, OffsetManager::ClientInstance_mFovY);
		return Vector2<float>(x, y);
	}

	LoopbackPacketSender* getLoopbackPacketSender() {
		return hat::member_at<LoopbackPacketSender*>(this, 0xF8);
	}

	LevelRender* getLevelRender() {
		return hat::member_at<LevelRender*>(this, 0xE8);
	}

	GuiData* getGuiData() {
		return hat::member_at<GuiData*>(this, OffsetManager::ClientInstance_mGuiData);
	}

	GLMatrix getGLMatrix() {
		return hat::member_at<GLMatrix>(this, OffsetManager::ClientInstance_mViewMatrix);
	}

	inline bool WorldToScreen(Vector3<float> pos, Vector2<float>& screen, Vector2<float> fov, Vector3<float> origin, GLMatrix glmatrix)
	{
		if (!getGuiData()) {
			return false;
		}
		Vector2<float> displaySize = getGuiData()->mWindowResolution;
		class LevelRender* lr = getLevelRender();

		pos.x -= origin.x;
		pos.y -= origin.y;
		pos.z -= origin.z;

		std::shared_ptr<GLMatrix> matrix = std::shared_ptr<GLMatrix>(glmatrix.correct());

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
	CLASS_FIELD(class MinecraftGame*, MinecraftGame, 0xC8); // 1.21.60
	CLASS_FIELD(class Minecraft*, Minecraft, 0xD8); // 1.21.60
};