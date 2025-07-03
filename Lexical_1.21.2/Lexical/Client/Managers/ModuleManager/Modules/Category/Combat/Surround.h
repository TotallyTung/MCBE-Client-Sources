#pragma once
#include "../../ModuleBase/Module.h"
class Surround : public Module {
private:
	bool disableComplete = false;
	bool center = true;
	int switchMode = 0;
	bool Placeabove = false;
	float delaytickjk = 0;
	float delay = 0;
	bool antianchor = false;
	bool anchor = false;
	Vec3<float> sideBlocks[9] = {
	Vec3<float>(1, 0, 0),
	Vec3<float>(0, 0, 1),
	Vec3<float>(-1, 0, 0),
	Vec3<float>(0, 0, -1),
	Vec3<float>(1, -1, 0),
	Vec3<float>(0, -1, 1),
	Vec3<float>(-1, -1, 0),
	Vec3<float>(0, -1, -1),
	Vec3<float>(0, -1, 0),
	};

	int getObsidian();
	void tryBuildBlock(Vec3<int> tryBuildPos);
	void predictBlock(Vec3<float> pos);
public:
	Surround();
	virtual void onNormalTick(LocalPlayer* actor) override;
	void onMoveTick();
	virtual void onSendPacket(Packet* packet) override;
	//virtual void onRender(MinecraftUIRenderContext* ctx) override;
	virtual void onEnable() override;
};