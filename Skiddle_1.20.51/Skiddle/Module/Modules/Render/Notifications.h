#pragma once

class Notifications : public Module
{
public:
	Notifications(int keybind = Keyboard::NONE, bool enabled = false) :
		Module("Notifications", "Render", "Display the notifications", keybind, enabled)
	{}

	void onEnabled() {
		Game::Skiddle::showNotification = true;
	}

	void onEvent(UpdateEvent* event) {
		Game::Skiddle::showNotification = true;
	}
	
	void onDisabled() {
		Game::Skiddle::showNotification = false;
	}
};