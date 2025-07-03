// Created by Tony on 2024-10-10 10:01:54

#pragma once

#include "MinecraftTimer.h"
#include "Level.h"

class Minecraft {
private:
	char pad_0x0[216]; //0x0
public:
	MinecraftTimer* mctimer; //0xD8
	MinecraftTimer* mcrenderTimer; //0xE0
	Level* getlevel() {
		__int64 gamesess = *reinterpret_cast<__int64*>(reinterpret_cast<__int64>(this) + 0x18);
		if (gamesess) {
			return **reinterpret_cast<Level***>(gamesess + 48);
		}
		else
			return nullptr;
	}
};