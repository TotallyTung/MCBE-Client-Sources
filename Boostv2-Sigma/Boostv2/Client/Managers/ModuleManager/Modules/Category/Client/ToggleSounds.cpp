#include "ToggleSounds.h"
#include "../../Client/Client.h"

ToggleSounds::ToggleSounds() : Module("ToggleSounds", "Plays sound when ur toggling", Category::CLIENT) {

}

ToggleSounds::~ToggleSounds() {
}
bool ToggleSounds::isVisible() {
	return false;
}