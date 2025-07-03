// Created by Tony on 2024-10-10 10:01:54

#include "CameraNoClip.h"

CameraNoClip::CameraNoClip() : Module("CameraNoClip", "Clip the camera through walls.", Category::RENDER) {
}

void CameraNoClip::onEnable() {
	MemoryUtils::nopBytes(targetAddress, 5);
}

void CameraNoClip::onDisable() {
	MemoryUtils::patchBytes(targetAddress, (void*)("\xF3\x44\x0F\x51\xEA"), 5);
}