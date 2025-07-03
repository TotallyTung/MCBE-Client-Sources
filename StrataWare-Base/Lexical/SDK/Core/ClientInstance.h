#pragma once
#include "../../Utils/MemoryUtil.h"
#include "../World/Level/BlockSource.h"
#include "../World/LocalPlayer.h"
#include "../Render/LevelRenderer.h"
#include "MinecraftGame.h"
#include "Minecraft.h"
#include "GuiData.h"
#include "ClientHMDState.h"
#include "mce.h"
struct GLMatrix {
public:
	float matrix[16];
	float matrix_nest[4][4];

	GLMatrix* correct() {
		GLMatrix* newMatPtr = new GLMatrix;

		for (int i = 0; i < 4; i++) {
			newMatPtr->matrix[i * 4 + 0] = matrix[0 + i];
			newMatPtr->matrix[i * 4 + 1] = matrix[4 + i];
			newMatPtr->matrix[i * 4 + 2] = matrix[8 + i];
			newMatPtr->matrix[i * 4 + 3] = matrix[12 + i];
		}
		return newMatPtr;
	};

	bool OWorldToScreen(Vec3<float> origin, Vec3<float> pos, Vec2<float>& screen, Vec2<float>& fov, Vec2<float>& displaySize) const
	{
		pos = pos.sub(origin);

		/*float x = transformDot(0, pos);
		float y = transformDot(1, pos);
		float z = transformDot(2, pos);*/

		float x = transformx(pos);
		float y = transformy(pos);
		float z = transformz(pos);

		if (z > 0)
			return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}

	float transformDot(int index, const Vec3<float>& p) const {
		return p.x * matrix[index * 4 + 0] +
			p.y * matrix[index * 4 + 1] +
			p.z * matrix[index * 4 + 2] +
			matrix[index * 4 + 3];
	}

	__forceinline float transformx(const Vec3<float>& p) const {
		return p.x * matrix[0] + p.y * matrix[4] + p.z * matrix[8] + matrix[12];
	}

	__forceinline float transformy(const Vec3<float>& p) const {
		return p.x * matrix[1] + p.y * matrix[5] + p.z * matrix[9] + matrix[13];
	}

	__forceinline float transformz(const Vec3<float>& p) const {
		return p.x * matrix[2] + p.y * matrix[6] + p.z * matrix[10] + matrix[14];
	}
};


class ClientInstance {
public:
	CLASS_MEMBER(MinecraftGame*, mcGame, 0xC8); // "textures/ui/pointer" -> xrefs first function -> xref second line -> what acces to that vtable -> what acces to a2
	CLASS_MEMBER(Minecraft*, minecraft, 0xD0);
	CLASS_MEMBER(GuiData*, guiData, 0x558);	// "ui_invert_overlay" -> xrefs first function -> continue xrefs -> continue xrefs -> offset below!
public:
	ClientHMDState* getHMDState() {
		// Offset: 48 8d 8e ? ? ? ? 48 8b d6 e8 ? ? ? ? 90 48 8d 8e ? ? ? ? e8 + 3
		return reinterpret_cast<ClientHMDState*>((uintptr_t)this + 0x5A8);
	}
	BlockSource* getRegion() {
		// ourPlayer index - 1
		return MemoryUtil::CallVFunc<28, BlockSource*>(this);
	}

	LocalPlayer* ourPlayer() {
		// IDA: search for string "variable.player_x_rotation" -> xrefs first function -> find what access to a3
		return MemoryUtil::CallVFunc<29, LocalPlayer*>(this);
	}

	void playUI(std::string str, float a1, float a2)
	{
		if (this == nullptr)
			return;

		MemoryUtil::CallVFunc<338, void>(this, str, a1, a2);
	}

	GLMatrix* getbadrefdef() {
		return (GLMatrix*)((uintptr_t)(this) + 0x330);
	}

	inline bool WorldToScreen(Vec3<float> pos, Vec2<float>& screen) //good for any version 
	{ // pos = pos 2 w2s, screen = output screen coords
		if (!getguiData()) {
			return false;
		}
		Vec2<float> displaySize = getguiData()->windowSizeReal;
		//LevelRenderer* lr = levelRenderer;
		if (getLevelRenderer() == nullptr) return false;

		Vec3<float> origin = getLevelRenderer()->levelRendererPlayer->Origin;
		Vec2<float> fov = getFov();

		pos.x -= origin.x;
		pos.y -= origin.y;
		pos.z -= origin.z;

		auto badrefdef = getbadrefdef();
		std::shared_ptr<GLMatrix> refdef = std::shared_ptr<GLMatrix>(badrefdef->correct());

		float x = refdef->transformx(pos);
		float y = refdef->transformy(pos);
		float z = refdef->transformz(pos);

		if (z > 0) return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}
	Vec2<float> getFov() { // Check for fov in clientinstance
		Vec2<float> fov;
		fov.x = *(float*)(reinterpret_cast<uintptr_t>(this) + 0x6F0); // 1.21.2
		fov.y = *(float*)(reinterpret_cast<uintptr_t>(this) + 0x704); // 1.21.2
		return fov;
	}

	LevelRenderer* getLevelRenderer() {
		// IDA: search for string "variable.player_x_rotation" -> xrefs first function -> find what function have 4 param and access with a3 -> what acces a3
		return MemoryUtil::CallVFunc<204, LevelRenderer*>(this);
	}

	void grabMouse() {
		// IDA: search for string "#hotbar_size_y" -> xrefs first function -> vtable of a1 -> the function that below the second "_guard_check_icall_nop" -> idk lol
		MemoryUtil::CallVFunc<332, void>(this);
	}

	void releasebMouse() {
		// grabMouse index + 1
		MemoryUtil::CallVFunc<333, void>(this);
	}
};
