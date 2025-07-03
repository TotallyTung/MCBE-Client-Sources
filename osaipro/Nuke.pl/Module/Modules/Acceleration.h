#pragma once
#include "Module.h"
class Acceleration : public IModule {
public:
	std::vector<Entity*> targetListKasoku;
	float speed = 6.8f;
	int movetick = 20;
	int pitch = 10;
	bool Crystal = false;
	Acceleration();
	~Acceleration();
	int delay = 0;
	bool damaged = false;
	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() override;
};