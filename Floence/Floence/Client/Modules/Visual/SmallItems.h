#pragma once

class SmallItems : public Module
{
public:
	SmallItems(int keybind = 7, bool enabled = true) :
		Module("SmallItems", "Visual", "Display matrix", keybind, enabled)
	{
		addBool("SmallItems", "SmallItem", &SmallItem);

	};

	bool SmallItem = false;

	void onEvent(ViewBobbingTickEvent* event) override
	{
		glm::mat4& matrix = *event->Matrix;
		matrix = glm::translate(matrix, glm::vec3(50.f / 100, -20.f / 100, -60.f / 100));
	}

};