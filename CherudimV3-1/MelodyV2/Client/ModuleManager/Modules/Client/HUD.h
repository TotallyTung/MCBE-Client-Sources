#pragma once
#include "../Module.h"

class EffectHud {
private:
	std::string name;
	UIColor effectColor;
	unsigned int effectId;
public:
	EffectHud(const std::string effectName, const UIColor& color, unsigned int id) {
		this->name = effectName;
		this->effectColor = color;
		this->effectId = id;
	}
	MobEffectInstance* getEffect() {
		return mc.getLocalPlayer()->getEffect(this->effectId);
	}
	std::string getEffectName() {
		return this->name;
	}
	UIColor getColor() {
		return this->effectColor;
	}
	unsigned int getEffectId() {
		return this->effectId;
	}
	unsigned int getEffectTimeLeft() {
		return *(unsigned int*)((__int64)getEffect() + 0x4);
	}
	std::string getEffectTimeLeftStr() {
		unsigned int timeLeft = getEffectTimeLeft();
		unsigned int timeReal = timeLeft / 20;
		std::string m = std::to_string(timeReal / 60);
		std::string s;// = std::to_string(timeReal % 60);
		if (timeReal % 60 < 10) s += "0";
		s += std::to_string(timeReal % 60);
		return m + ":" + s;
	}
	unsigned int getEffectLevel() {
		return *(unsigned int*)((__int64)getEffect() + 0x14);
	}
};

class HUD : public Module {
private:
	bool showFps = true;
	bool showCoords = true;
	bool showSpeed = true;
	bool SAYNIGGGA = true;
	bool showEffects = true;
	std::vector<EffectHud*> effectList;
	std::string intToRoman(int num);
	static bool sortByAlphabet(EffectHud*& a1, EffectHud*& a2);

	float size = 1.f;
public:
	bool Hotbar = false;
	HUD();
	~HUD();

	virtual void onNormalTick(Actor* actor) override;
	virtual void onImGuiRender(ImDrawList* drawlist) override;
};