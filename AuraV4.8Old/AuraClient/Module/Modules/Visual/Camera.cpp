#include "Camera.h"
#include "../pch.h"

Camera::Camera() : IModule(0, Category::VISUAL, "Mofie ur cam e") {
	registerBoolSetting("NoHurtcam", &nohurtcam, nohurtcam);
	registerBoolSetting("Fullbright", &fullbright, fullbright);
	registerIntSetting("Intensity", &intensity, intensity, -25, 25);
}

const char* Camera::getModuleName() {
	return "Camera";
}

float originalGamma = -1;

void Camera::onEnable() {
	if (fullbright) {
		if (gammaPtr != nullptr) {
			originalGamma = *gammaPtr;
			*gammaPtr = 10;
		}
	}
}

void Camera::onTick(C_GameMode* gm) {
	if (fullbright) {
		if (gammaPtr != nullptr && *gammaPtr != 10)
			*gammaPtr = 10;
	}
}

void Camera::onWorldTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (nohurtcam) g_Data.getLocalPlayer()->cancelHurtAnimation();
}

void Camera::onDisable() {
	if (fullbright) {
		if (gammaPtr != nullptr) {
			if (originalGamma >= 0 && originalGamma <= 1)
				*gammaPtr = originalGamma;
			else
				*gammaPtr = 0.5f;
		}
	}
}