#pragma once

class DamageEffects : public Module
{
public:
	DamageEffects(int keybind = Keys::NUM_0, bool enabled = false) :
		Module("DamageEffects", "Visual", "Visual effects for damage", keybind, enabled) {

	};
};