// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../SDK/Classes/LocalPlayer.h"

class TargetUtils {
public:
	inline static std::vector<std::string> Friend;
	static bool isTargetValid(Actor* target, bool isMob = false);
	static bool sortByDist(Actor* a1, Actor* a2);
};