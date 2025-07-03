#pragma once
#include "../../ModuleBase/Module.h"

class PopCounter : public Module {
public:
	UIColor headcolor = UIColor(255, 0, 0, 200);
	UIColor limbcolor = UIColor(0, 255, 0, 200);
	UIColor bodycolor = UIColor(255, 255, 0, 200);
	bool sendchat = false;
	bool notifi = false;
	bool cys;
	float Dura = 2.f;
	float Fade = 255;
	float YLock = 0.f;
	int popcount = 0;
	int Health = 20;
	bool totem = true;
	bool render = false;
	std::string names;
	virtual void onNormalTick(LocalPlayer* actor) override;
	PopCounter();
};