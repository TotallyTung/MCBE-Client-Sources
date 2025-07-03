#include "GlintColor.h"

GlintColor::GlintColor() : Module("GlintColor", "NULL", Category::RENDER) {
	registerSetting(new ColorSetting("Color", "NULL", &color, color));
	//glintColorPtr = (mce::Color*)(MemoryUtil::getGameAddress() + 0x57BD6A0);

	uintptr_t movssGlintColor = MemoryUtil::findSignature("F3 0F 10 05 ? ? ? ? 48 8B D7 48 8B");
	glintColorPtr = (mce::Color*)(movssGlintColor + *(int*)(movssGlintColor + 4));
}

void GlintColor::onDisable() {
	if (glintColorPtr != nullptr)
		*glintColorPtr = mce::Color(0.38f, 0.19f, 0.60799998f, 1.0f);
}

void GlintColor::onClientTick() {
	if (glintColorPtr != nullptr)
		*glintColorPtr = color.toMCColor();
}