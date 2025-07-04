#pragma once
#include "../FuncHook.h"
#include <libhat/Access.hpp>

class GetCameraComponentHook : public FuncHook {
private:
	using func_t = void* (__thiscall*)(__int64, __int64);
	static inline func_t oFunc;

	static void* GetCameraComponentCallback(__int64 a1, __int64 a2) {
		void* result = oFunc(a1, a2);
		static void* cameraDistanceCall = reinterpret_cast<void*>(MemoryUtil::findSignature("48 85 C0 74 06 F3 44 0F"));
		if (_ReturnAddress() == cameraDistanceCall && result != nullptr) {
			static CameraTweaks* cameraTweaksMod = ModuleManager::getModule<CameraTweaks>();
			float& cameraDistance = hat::member_at<float>(result, 0x14);

			if (cameraTweaksMod->isEnabled())
				cameraDistance = cameraTweaksMod->distance;
			else
				cameraDistance = 4.f;
		}
		/*
		void* callFrom = _ReturnAddress();
		if ((uintptr_t)callFrom > MemoryUtil::getGameAddress() + 0x43CCDF0 && (uintptr_t)callFrom < MemoryUtil::getGameAddress() + 0x43CD146) {
			Game::DisplayClientMessage("%llx", callFrom);
			if (result != nullptr) {
				*(float*)((uintptr_t)result + 0x14) = 10.f;
			}
		}
		*/
		return result;
	}
public:
	GetCameraComponentHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&GetCameraComponentCallback;
	}
};