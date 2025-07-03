#include "CameraTweaks.h"

CameraTweaks::CameraTweaks() : Module("CameraTweaks", "Allows modification of the third person camera", Category::RENDER) {
	registerSetting(new BoolSetting("Clip", "Allows the camera to clip through blocks", &clip, false));
	registerSetting(new SliderSetting<float>("Distance", "The distance the third person camera is from the player", &distance, 4.f, 1.f, 50.f));

	if (Addresses::CameraClipCheck != 0) {
		targetAddress = reinterpret_cast<void*>(Addresses::CameraClipCheck);
		MemoryUtil::copyBytes(targetAddress, (void*)ogBytes, 5);
	}
}

void CameraTweaks::onDisable() {
	if (targetAddress == nullptr)
		return;

	if (nopedBytes) {
		MemoryUtil::patchBytes(targetAddress, (void*)ogBytes, 5);
		nopedBytes = false;
	}
}

void CameraTweaks::onClientTick() {
	if (targetAddress == nullptr)
		return;

	if (clip) {
		if (!nopedBytes) {
			MemoryUtil::nopBytes(targetAddress, 5);
			nopedBytes = true;
		}
	}
	else {
		if (nopedBytes) {
			MemoryUtil::patchBytes(targetAddress, (void*)ogBytes, 5);
			nopedBytes = false;
		}
	}
}