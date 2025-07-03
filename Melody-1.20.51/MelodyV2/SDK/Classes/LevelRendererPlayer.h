// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../Utils/Math.h"

class LevelRendererPlayer {
private:
	char pad_0x0[0x5E4];
public:
	Vec3<float> cameraPos1; // 0x5E4 origin1
	Vec3<float> cameraPos2; // 0xF0 origin2 this can be changed
};