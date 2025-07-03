#include "NoClipGlide.h"

NoClipGlide::NoClipGlide() : IModule(0x0, Category::MOVEMENT, "Automatically makes noclip glide!") {
	registerFloatSetting("Glide Value", &glide, glide, -0.15f, 0.00f);
}

NoClipGlide::~NoClipGlide() {
}

const char* NoClipGlide::getRawModuleName() {
	return ("NoClipGlide");
}