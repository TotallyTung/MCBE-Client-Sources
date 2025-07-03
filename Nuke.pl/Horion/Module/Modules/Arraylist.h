#pragma once

#include "..\ModuleManager.h"
#include "Module.h"

class Arraylist : public IModule {
public:
	bool showKey = true;
	bool showMode = true;

	bool bottom = true;
	bool outline = true;
	int alpha = 90;
	float blurStrength = 5.f;
	Arraylist();
	~Arraylist();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onImGuiRender(ImDrawList* d) override;
};
