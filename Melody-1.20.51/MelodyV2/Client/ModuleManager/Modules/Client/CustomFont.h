// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class CustomFont : public Module {
private:
public:
	int fontType = 1;
	bool fontShadow = true;
	CustomFont();

	virtual bool isEnabled() override;
	virtual void setEnabled(bool enabled) override;
	virtual bool isVisible() override;
};