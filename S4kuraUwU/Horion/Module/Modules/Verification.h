#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class Verification : public IModule {
private:
	bool passedTest = false;			
	const std::string allowedIGNS[1] = {""};
	bool test = false;
	float theX = .69f;
	float theY = -1750.f;
	float theZ = .69f;


public:
	int userIs = 0;
	bool mustLock = true;

	Verification();
	~Verification();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable();
};
