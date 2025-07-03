#pragma once
#include "../Module.h"
class ViewModel : public IModule {
private:
	int delay = 0;

public:
	bool Reset = false;
	bool translate = true;
	bool scale = true;
	bool rotate = true;

	float xPos = 1.f;
	float yPos = 1.f;
	float zPos = 1.f;

	float xScale = 0.f;
	float yScale = 0.f;
	float zScale = 0.f;

	float xRotate = 0.f;
	float yRotate = 0.f;
	float zRotate = 0.f;

	SettingEnum mode = this;

	ViewModel();
	~ViewModel();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
