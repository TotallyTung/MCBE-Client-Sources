#pragma once
#include "../Module.h"

class AutoTntSpammer : public Module {
private:
	enum class State {
		BUILD_OBSTACLE,
		BUILD_TNT,
		TOUCH_WITH_MAGIC_SWORD
	};
	float targetRange = 7.f;
	float wallRange = 5.f;
	bool attackMob = false;

	int getBestWeaponSlot();
	int getObsidian();
	int getTNT();
	void Buildblocksfortnt(std::vector<Vec3<int>> buildvector);
public:
	std::vector<Actor*> targetList;
	AutoTntSpammer();

	virtual void onNormalTick(Actor* actor) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	virtual void onImGuiRender(ImDrawList* drawlist) override;
};