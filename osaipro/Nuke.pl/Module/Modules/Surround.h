
#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class Surround : public IModule {
private:
	int delay = 0;
	bool citySwitch = true;

public:
	int origSlot;

	SettingEnum type;
	bool renderSurround = true;
	bool doAirplace = false;
	//SettingEnum rotate;  // Rotations (none->obvious
						 //			  normal->clientside rotate
						 //			  silent->sends packet to go look at the crystal
						 //			  pitchUp->so that shit works on APVP

	Surround();
	~Surround();
	bool blockSwap = true;
	int prevSlot3 = -1;
	Vec2 rotAngle;
	bool pn = false;

	bool jumpDisable = false;
	bool disableOnComplete2 = true;
	bool center = true;
	bool spoof = true;

	std::vector<Vec3> regularBlocks = {

		Vec3(0, -1, 0),

		Vec3(1, -1, 0),
		Vec3(1, 0, 0),

		Vec3(0, -1, 1),
		Vec3(0, 0, 1),

		Vec3(-1, -1, 0),
		Vec3(-1, 0, 0),

		Vec3(0, -1, -1),
		Vec3(0, 0, -1),

	};

	std::vector<Vec3> sidesBlocksOnly = {
		Vec3(1, 0, 0),
		Vec3(0, 0, 1),
		Vec3(-1, 0, 0),
		Vec3(0, 0, -1),
		Vec3(0, -1, 0),
	};


	//void getObby();
	bool isSideEmpty(std::vector<Vec3>);
	void onMove(MoveInputHandler* hand);
	//Vec3 playerPosWithFC();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	//virtual bool isHoldMode() override;
	virtual void onWorldTick(GameMode* gm) override;
	virtual void onLevelRender() override;
	//void onSendPacket(C_Packet* packet);
	//virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable();
	//virtual void onMove(C_MoveInputHandler* hand) override;
};