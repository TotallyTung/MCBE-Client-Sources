#include "NoHurtcam.h"

NoHurtcam::~NoHurtcam() {
}
NoHurtcam::NoHurtcam() : IModule(0, Category::VISUAL, "Disables rotation of the camera when hit.") {
}
const char* NoHurtcam::getModuleName() {
	return "NoHurtcam";
}
void NoHurtcam::onEnable() {
	Utils::nopBytes((BYTE*)((uintptr_t)cancelHurtcam), 10);
}
void NoHurtcam::onDisable() {
	Utils::patchBytes((BYTE*)((uintptr_t)cancelHurtcam), (BYTE*)("\xC7\x81\x38\x07\x00\x00\x0A\x00\x00\x00"), 10);
}