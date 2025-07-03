#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class NoClip : public IModule {
		private: 
			float speed = 0.325f;
	float upanddown = 0.6f;
		float glideMod = -0.00f;
	float glideModEffective = 0;
		Vec3 oldPos;
	bool NoPacket = false;

	public:
	NoClip();
	~NoClip();

	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	virtual void onMove(MoveInputHandler* input);
	virtual void onEnable();
	virtual void onDisable();
};
