#include "EffectUtils.h"

std::vector<std::string> effectName = {
	"None",
	"Speed",
	"Slowness",
	"Haste",
	"Mining Fatigue",
	"Strength",
	"Instant Health",
	"Instant Damage",
	"Jump Boost",
	"Nausea",
	"Regeneration",
	"Resistance",
	"Fire Resistance",
	"Water Breathing",
	"Invisibility",
	"Blindess",
	"Night Vision",
	"Hunger",
	"Weakness",
	"Posion",
	"Wither",
	"Health Boost",
	"Absorption",
	"Saturation",
	"Levitation",
	"Fatal Poison",
	"Conduit Power",
	"Slow Falling",
	"Bad Omen",
	"Village Hero",
	"Darkness"
};

std::vector<UIColor> effectColor = {
	UIColor(0, 0, 0),
	UIColor(124, 175, 198),
	UIColor(90, 108, 129),
	UIColor(217, 192, 67),
	UIColor(74, 66, 23),
	UIColor(147, 36, 35),
	UIColor(0, 0, 0),
	UIColor(0, 0, 0),
	UIColor(34, 255, 76),
	UIColor(85, 29, 74),
	UIColor(205, 92, 171),
	UIColor(153, 69, 58),
	UIColor(228, 154, 58),
	UIColor(46, 82, 153),
	UIColor(127, 131, 146),
	UIColor(31, 31, 35),
	UIColor(31, 31, 161),
	UIColor(88, 118, 83),
	UIColor(72, 77, 72),
	UIColor(78, 147, 49),
	UIColor(53, 42, 39),
	UIColor(248, 125, 35),
	UIColor(37, 82, 165),
	UIColor(248, 36, 35),
	UIColor(206, 255, 255),
	UIColor(78, 147, 49),
	UIColor(0, 191, 255),
	UIColor(248, 248, 255),
	UIColor(255, 250, 240),
	UIColor(143, 188, 143),
	UIColor(30, 30, 30)
};

MobEffectInstance* EffectUtils::getEffect(EFFECTID id) {
	return mc.getLocalPlayer()->getEffect((int)id);
}

unsigned int EffectUtils::getTimeLeft(MobEffectInstance* effect) {
	return *(unsigned int*)((__int64)effect + 0x4);
}

unsigned int EffectUtils::getEffectLevel(MobEffectInstance* effect) {
	return *(unsigned int*)((__int64)effect + 0x14);
}

bool EffectUtils::hasEffect(EFFECTID id) {
	if (getEffect(id) == nullptr) return false;
	if (getTimeLeft(getEffect(id)) == 0) return false;
	return true;
}

std::string EffectUtils::getEffectName(EFFECTID id) {
	return effectName[(int)id];
}

UIColor EffectUtils::getEffectColor(EFFECTID id) {
	return effectColor[(int)id];
}
