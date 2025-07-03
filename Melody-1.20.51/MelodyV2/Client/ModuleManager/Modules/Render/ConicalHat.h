// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class ConicalHat : public Module {
private:
	UIColor chColor = UIColor(255, 255, 255, 255);
	bool rainbow = false;
public:
	ConicalHat();
	virtual void onLevelRender() override;
};