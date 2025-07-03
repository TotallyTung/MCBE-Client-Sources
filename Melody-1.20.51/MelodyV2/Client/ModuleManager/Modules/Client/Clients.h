// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class Clients : public Module {
public:
	~Clients();
	Clients();
	virtual bool isEnabled() override;
	virtual bool isVisible()override;
};
