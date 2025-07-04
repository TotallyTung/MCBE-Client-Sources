#pragma once
#include "../Module.h"

class Clients : public Module {
public:
	~Clients();
	Clients();
	virtual bool isEnabled() override;
	virtual bool isVisible()override;
};
