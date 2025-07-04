#pragma once
#include "../Module.h"
class ViewModel : public IModule {
private:
	int delay = 0;

public:
	bool Reset = false;
	bool Translate = true;
	bool Scale = true;
	bool Rotate = false;
	vec3_t scale = vec3_t(1.f, 1.f, 1.f);
	vec3_t trans = vec3_t(0.f, 0.f, 0.f);
	vec3_t rotat = vec3_t(1.f, 1.f, 1.f);
	float RotatePos = 0.f;
	ViewModel();
	~ViewModel();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
