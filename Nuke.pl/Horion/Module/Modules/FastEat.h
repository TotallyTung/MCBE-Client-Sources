#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class FastEat : public IModule {
private:
	std::string name;
	bool TimerWasEnabled = false;
	bool _2espeedWasEnabled = false;
	bool cbspeedwe = false;
	bool bhopwe = false;
	bool spwe = false;
	bool Timer2we = false;
	bool Timer3we = false;

public:
	FastEat();
	~FastEat();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};
