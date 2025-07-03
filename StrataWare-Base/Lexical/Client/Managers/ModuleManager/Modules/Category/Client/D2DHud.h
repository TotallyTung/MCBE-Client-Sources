#pragma once
#include "../../ModuleBase/Module.h"

class HUD : public Module {
private:
	inline std::string getEffectTimeLeftStr(MobEffectInstance* mEffectInstance) {
		uint32_t timeLeft = mEffectInstance->mDuration;
		uint32_t timeReal = (uint32_t)(timeLeft / 20);
		std::string m = std::to_string(timeReal / 60);
		std::string s;
		if (timeReal % 60 < 10)
			s += "0";
		s += std::to_string(timeReal % 60);
		return m + ":" + s;
	}
public:
	bool showPosition = true;
	bool showDirection = false;
	bool showArmor = true;
	bool showEffects = true;
	bool arraylist = false;
	int offset = 15;
	int opacity = 125;
	int spacing = -1;
	int size = 0;
	bool bottom = false;
	bool showModes = false;
	HUD();
	~HUD();
	static bool sortByLength(Module* lhs, Module* rhs);
	void onD2DRender() override;
};
