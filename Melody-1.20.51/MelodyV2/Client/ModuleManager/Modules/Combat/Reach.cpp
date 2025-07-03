// Created by Tony on 2024-10-10 10:01:54

#include "Reach.h"

Reach::Reach() : Module("Reach", "Extend reach.", Category::COMBAT) {
	addSlider<float>("Reach", "NULL", ValueType::FLOAT_T, &reachValue, 3.f, 7.f);
}

void Reach::onEnable() {
	if (SurvivalreachPtr != nullptr) {
		*SurvivalreachPtr = 3.0f;
		DWORD oldProt = 0;
		VirtualProtect(SurvivalreachPtr, sizeof(float), oldProt, &oldProt);
	}
}

void Reach::onNormalTick(Actor* actor) {
	if (SurvivalreachPtr == nullptr) {
		uintptr_t survivalReachSig = findSig(Sigs::global::survivalReach);
		uintptr_t survivalReachOffsetRef = survivalReachSig + 4;
		uint32_t survivalReachOffset = *reinterpret_cast<uint32_t*>(survivalReachOffsetRef);
		SurvivalreachPtr = reinterpret_cast<float*>(survivalReachOffsetRef + survivalReachOffset + 4);

		DWORD oldProt = 0;
		VirtualProtect(SurvivalreachPtr, 4, PAGE_EXECUTE_READWRITE, &oldProt);
	}

	if (SurvivalreachPtr != nullptr) {
		*SurvivalreachPtr = reachValue;
	}
}
