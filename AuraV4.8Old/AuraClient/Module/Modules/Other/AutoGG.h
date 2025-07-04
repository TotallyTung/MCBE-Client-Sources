#pragma once
#include <filesystem>
#include <set>

#include "../../Utils/TargetUtil.h"
#include "../Module.h"

class AutoGG : public IModule {
private:
	bool mention = true;

	std::set<C_Entity*> attackList = {};
	std::vector<C_Entity*> eraseList = {};
	void getInsultMessages();
	std::vector<std::string> InsultMessageVector;

public:
	AutoGG();
	~AutoGG();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onAttack(C_Entity* attackedEnt) override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};
