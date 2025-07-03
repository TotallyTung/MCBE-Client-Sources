#pragma once
#include "../../Utils/MemoryUtil.h"
#include "../../Utils/Maths.h"

class ClientHMDState {
public:
	CLASS_MEMBER(glm::mat4, mLastLevelViewMatrix, 0xC8);	// 4c 8d 88 ? ? ? ? 4d 8b c1
	CLASS_MEMBER(glm::mat4, mLastLevelProjMatrix, 0x148);	// 4c 8d 80 ? ? ? ? 49 8b d0 48 8d 85
};