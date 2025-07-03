// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../Utils/ColorUtils.h"
#include "../../../../SDK/Classes/ScreenContext.h"
#include "../../../../SDK/Classes/TextHolder.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

class FontDrawTransformedHook {
protected:
	using func_t = __int64(__thiscall*)(void*, ScreenContext*, TextHolder*, float, float, MC_Color, float, float, bool, float);
	static inline func_t func;

	static void FontDrawTransformedCallback(void* _this, ScreenContext* screenContext, TextHolder* textStr, float posX, float posY, MC_Color color, float angle, float scale, bool centered, float maxWidth) {
		static std::vector<std::string> texts = {
			"MelodyV2 is riel?",
			"Use keys insert to open melody clickgui",
			"NIGGGGGGGGERRRRSSS",
			"Tony have the ebiggest rizz ever",
			"Fuk u wants monkey",
			"EHHHHHHHHAHHEHHEHE",
			"I dirve a cars ini minecraft fr fr!",
			"Remember! to study before playing games",
			"dont get addicted into mc too much",
			"Fuck yo bitchs"
			// Add more strings here...
		};

		static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
		static std::uniform_int_distribution<int> dist(0, texts.size() - 1);

		static auto start = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

		if (elapsed >= 10) {
			start = std::chrono::steady_clock::now();
			textStr->setText(texts[dist(rng)].c_str());
		}

		func(_this, screenContext, textStr, posX, posY, color, angle, scale, centered, maxWidth);
	}

public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::FontDrawTransformedHook);
		MemoryUtils::CreateHook("FontDrawTransformedHook", address, (void*)&FontDrawTransformedHook::FontDrawTransformedCallback, (void*)&func);
	}
};
