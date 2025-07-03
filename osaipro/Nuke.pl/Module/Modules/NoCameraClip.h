#pragma once
#include "Module.h"
class CameraNoClip : public IModule {
public:
	CameraNoClip();
	~CameraNoClip();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};