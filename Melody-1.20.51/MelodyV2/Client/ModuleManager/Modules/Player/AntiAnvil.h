// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class AntiAnvil : public Module {
private:
public:

	AntiAnvil();
	virtual void onNormalTick(Actor* actor) override;
};
