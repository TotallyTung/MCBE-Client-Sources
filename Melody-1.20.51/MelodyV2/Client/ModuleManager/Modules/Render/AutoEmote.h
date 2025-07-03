// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class AutoEmote : public Module {
private:
	
	int EmoteDelay = 2;
public:
	AutoEmote();

	virtual void onNormalTick(Actor* actor) override;
};