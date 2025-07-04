#pragma once
#include "../Module.h"

class CustomFont : public Module {
private:
public:
	int fontType = 0;
	bool fontShadow = true;
	CustomFont();

	virtual bool isEnabled() override;
	virtual void setEnabled(bool enabled) override;
	virtual bool isVisible() override;
};