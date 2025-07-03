#pragma once
#include "../SDK/GameData.h"
enum class EFFECTID : int {
	NONE = 0,
	SPEED = 1,
	SLOWNESS = 2,
	HASTE = 3,
	MININGFATIGUE = 4,
	STRENGTH = 5,
	INSTANT_HEALTH = 6,
	INSTANT_DAMAGE = 7,
	JUMP_BOOST = 8,
	NAUSEA = 9,
	REGENERATION = 10,
	RESISTANCE = 11,
	FIRE_RESISTANCE = 12,
	WATER_BREATHING = 13,
	INVISIBILITY = 14,
	BLINDNESS = 15,
	NIGHT_VISION = 16,
	HUNGER = 17,
	WEAKNESS = 18,
	POISON = 19,
	WITHER = 20,
	HEALTH_BOOST = 21,
	ABSORPTION = 22
};

class EffectUtils {
public:
	static bool hasEffect(EFFECTID id);
	static unsigned int getTimeLeft(MobEffectInstance* effect);
	static unsigned int getEffectLevel(MobEffectInstance* effect);
	static MobEffectInstance* getEffect(EFFECTID id);
	static std::string getEffectName(EFFECTID id);
	static UIColor getEffectColor(EFFECTID id);
	static std::string getTimeLeftStr(MobEffectInstance* effect) {
		unsigned int timeLeft = getTimeLeft(effect);
		unsigned int timeReal = timeLeft / 20;
		std::string m = std::to_string(timeReal / 60);
		std::string s;// = std::to_string(timeReal % 60);
		if (timeReal % 60 < 10) s += "0";
		s += std::to_string(timeReal % 60);
		return m + ":" + s;
	}
};