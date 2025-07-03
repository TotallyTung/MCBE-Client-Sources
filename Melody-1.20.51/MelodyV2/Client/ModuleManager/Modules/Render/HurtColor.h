// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class HurtColor : public Module {
public:
	bool fade = false;
	UIColor hurtColor = (255, 0, 0, 175);
	HurtColor();
};