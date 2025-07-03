#pragma once
#include "../../FuncHook.h"

class BobHurtHook : public FuncHook {
private:
	using func_t = uint32_t(__thiscall*)(void*, glm::mat4*);
	static inline func_t oFunc;

	static uint32_t BobHurtCallBack(void* _this, glm::mat4* matrix) {
		ModuleManager::onMatrixRender(matrix);

		return oFunc(_this, matrix);
	}
public:
	BobHurtHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&BobHurtCallBack;
	}
};
