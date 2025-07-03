#pragma once

class ToggleSounds : public Module
{
private:
	int sound = 0;

public:
	ToggleSounds(int keybind = Keyboard::NONE, bool enabled = false) :
		Module("ToggleSounds", "Misc", "play sound for toggled module", keybind, enabled)
	{
		registerEnumSetting("Mode", "The mode of speed niga", { "Click", "Pop", "Sigma" }, &sound);
	}

	void onEvent(UpdateEvent* event) {
		Game::Core::soundtype = sound;
		Game::Core::togglesound = true;
	}

	void onEnabled() {
		Game::Core::togglesound = true;
	}

	void onDisabled() {
		Game::Core::togglesound = false;
	}
};