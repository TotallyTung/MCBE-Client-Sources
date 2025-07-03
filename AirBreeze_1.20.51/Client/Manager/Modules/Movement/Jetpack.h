
#include "../../Module.h"

class Jetpack : public Module {
public:
	Jetpack(Category* c) : Module(c, "Jetpack", "Propels you forward, like a jetpack would!") {
	this->key = 'F';
	Float("Value", 0.1f, 1.0f, &speed);
	Float("Glide", -0.1f, 0.1f, &glide);
	//this->isEnabled = true;
	};
public:
    auto onTick(void) -> void;
    //auto onGameMode(GameMode*) -> void override;
private:
    float speed = 0.350f;
    float glide = -0.01f;

	auto onGameMode(GameMode* GM) -> void;
};