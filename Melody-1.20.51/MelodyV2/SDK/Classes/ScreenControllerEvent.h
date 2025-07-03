// Created by Tony on 2024-10-10 10:01:54

#pragma once

#include "ScreenController.h"

class ScreenControllerEvent
{
	ScreenController* controller;
public:
	explicit ScreenControllerEvent(ScreenController* controller)
	{
		this->controller = controller;
	}

	ScreenController* getController()
	{
		return this->controller;
	}
};