#pragma once

#define getLocalPlayerIndex 28
class ClientInstance
{
private:
	char pad_0000[200]; //0x0000
public:
	class MinecraftGame* minecraftGame; //0x00C8
	class Minecraft* minecraft; //0x00D0
private:
	char pad_00D8[24]; //0x00D8
public:
	class LoopbackPacketSender* loopbackPacketSender; //0x00F0
private:
	char pad_00F8[1128]; //0x00F8
public:
	class GuiData* guiData; //0x0560

	LoopbackPacketSender* getPacketSender()
	{
		return *reinterpret_cast<LoopbackPacketSender**>((uintptr_t)this + 0xF0);
	}

	Player* getLocalPlayer() {
		return (class Player*)CallVFunc<getLocalPlayerIndex, Player*>(this);
	}

	void grabCursor()
	{
		return CallVFunc<326, void>(this);
	}

	void releaseCursor()
	{
		return CallVFunc<327, void>(this);
	}

	glmatrixf Matrix1; //0x0300

	Vector2<float> getFov() {
		Vector2<float> fov;
		fov.x = *(float*)(reinterpret_cast<uintptr_t>(this) + 0x6F8); //good for 1.20.51
		fov.y = *(float*)(reinterpret_cast<uintptr_t>(this) + 0x70C); //good for 1.20.51
		return fov;
	}

	glmatrixf* getMatrixCorrection() 
	{
		glmatrixf toReturn = glmatrixf();

		for (int i = 0; i < 4; i++) {
			toReturn.matrix[i * 4 + 0] = Matrix1.matrix[0 + i];
			toReturn.matrix[i * 4 + 1] = Matrix1.matrix[4 + i];
			toReturn.matrix[i * 4 + 2] = Matrix1.matrix[8 + i];
			toReturn.matrix[i * 4 + 3] = Matrix1.matrix[12 + i];
		}

		return &toReturn;
	};

	__forceinline float transformx(const Vector3<float>& p)
	{
		auto matrix = getMatrixCorrection()->matrix;
		return p.x * matrix[0] + p.y * matrix[4] + p.z * matrix[8] + matrix[12];
	}

	__forceinline float transformy(const Vector3<float>& p)
	{
		auto matrix = getMatrixCorrection()->matrix;
		return p.x * matrix[1] + p.y * matrix[5] + p.z * matrix[9] + matrix[13];
	}

	__forceinline float transformz(const Vector3<float>& p)
	{
		auto matrix = getMatrixCorrection()->matrix;
		return p.x * matrix[2] + p.y * matrix[6] + p.z * matrix[10] + matrix[14];
	}

	inline bool WorldToScreen(Vector3<float> pos, Vector2<float>& screen, Vector2<float> displaySize) //good for any version 
	{ // pos = pos 2 w2s, screen = output screen coords
		if (!getGuiData()) {
			return false;
		}
		//Vector2<float> displaySize = getGuiData()->windowSize;
		//LevelRenderer* lr = levelRenderer;
		if (levelRenderer == nullptr) return false;

		Vector3<float> origin = levelRenderer->levelRendererPlayer->origin;
		Vector2<float> fov = getFov();

		pos.x -= origin.x;
		pos.y -= origin.y;
		pos.z -= origin.z;

		float x = transformx(pos);
		float y = transformy(pos);
		float z = transformz(pos);

		if (z > 0) return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}

	glmatrixf* getbadrefdef() {
		return (glmatrixf*)((uintptr_t)(this) + 0x330); //good for 1.20.51
	}

	class Actor* player;
public:
	BUILD_ACCESS(this, class GuiData*, GuiData, 0x560);//good for 1.20.51
	BUILD_ACCESS(this, class LevelRenderer*, levelRenderer, 0xE0);
public:
	AS_FIELD(class Player*, LocalPlayer, getLocalPlayer);
}; //Size: 0x0568

class LoopbackPacketSender
{
public:
	char pad_0000[32]; //0x0000
	class NetworkSystem* NetworkSystem; //0x0020
}; //Size: 0x0028

class NetworkSystem
{
public:
	char pad_0000[128]; //0x0000
	class RakNet* rakNet; //0x0080
}; //Size: 0x0088

class RakNet
{
public:
	char pad_0000[96]; //0x0000
	class RaknetConnector* raknetConnector; //0x0060
}; //Size: 0x0068

class RaknetConnector
{
public:
	char pad_0000[632]; //0x0000
	class RakPeer* rakpeer; //0x0278
	char pad_0280[408]; //0x0280
	char ipv4[16]; //0x0418
	char pad_0428[16]; //0x0428
	char* host; //0x0438
	char pad_0440[24]; //0x0440
	int16_t port; //0x0458
	char pad_045A[56]; //0x045A
}; //Size: 0x0492

class RakPeer
{
public:
	char pad_0000[264]; //0x0000
}; //Size: 0x0108

class MinecraftGame
{
public:
	char pad_0000[400]; //0x0000
	bool canMoveKey; //0x0190
}; //Size: 0x0191

class Minecraft
{
private:
	char pad_0x0000[0xD8];  //0x0000
public:
	float* tickTimer;   // 0x00D8
	float* renderTimer;  // 0x00E0

	void setTimerSpeed(float tps) {
		*this->tickTimer = tps;
	}
	void setRenderTimerSpeed(float tps) {
		*this->renderTimer = tps;
	}
}; //Size: 0x00E8

class TickTimer
{				 
public:
	float tps; //0x0000
	char pad_0004[4]; //0x0004
	float tickDelta; //0x0008
	float tpsMultiplier; //0x000C
	float lastTickDelta; //0x0010
	char pad_0014[4]; //0x0014
	float seconds; //0x0018
	float frameDelta; //0x001C
}; //Size: 0x0020

class GuiData
{
public:
	char pad_0000[48]; //0x0000
	Vector2<float> windowSize; //0x0030
	Vector2<float> lastWindowSize; //0x0038
	Vector2<float> scaledWindowSize; //0x0040
	char pad_0048[4]; //0x0048
	float scale; //0x004C
	float scaleInverse; //0x0050
}; //Size: 0x0054