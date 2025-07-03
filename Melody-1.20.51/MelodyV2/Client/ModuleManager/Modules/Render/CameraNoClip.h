// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class CameraNoClip : public Module {
private:
	void* targetAddress = (void*)findSig(Sigs::hook::CameraNoClip);
public:
	CameraNoClip();

	virtual void onEnable() override;
	virtual void onDisable() override;
};
