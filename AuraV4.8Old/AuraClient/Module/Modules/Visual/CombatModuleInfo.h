#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class CombatInfo : public IModule {
private:
	float CombatInfoX = 100.f;
	float CombatInfoY = 100.f;
	float speed = 0.f;
	float scale = 1.f;

public:
	CombatInfo();
	~CombatInfo();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};
#pragma once
