#pragma once
class StopDestroyBlockHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(GameMode*, BlockPos*);
	static inline func_t oFunc;

	static void StopDestroyBlockCallback(GameMode* _this, BlockPos* pos) {
		oFunc(_this, pos);
	}
public:
	StopDestroyBlockHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&StopDestroyBlockCallback;
	}
};