#pragma once
#pragma once
#include "../../../../Utils/TargetUtils.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class AnchorAura : public Module {
private:
	int anchordelay = 1;
	int gsdelay = 3;
	int breakdelay = 2;
	bool airplace = true;
	std::string names;
	bool hasDetonated = false;
	bool hasPlacedAnchor = false;
	bool hasCharged = false;
	bool DhasPlacedAnchor = false;
	bool DhasCharged = false;
	int tickTimer = 0;
	bool takenAnchor = false;
	bool takenGS = false;
	int Option = 99;

	//ground level  ~~~ funni initializations
	Vec3<float> bottom1;
	Vec3<float> bottom2;
	Vec3<float> bottom3;
	Vec3<float> bottom4;
	//top level
	Vec3<float> neckBreaker;
	//middle level  ~ last priority because it is the least effective
	Vec3<float> mid1;
	Vec3<float> mid2;
	Vec3<float> mid3;
	Vec3<float> mid4;
	Vec2<float> rotAnglePlace{};
public:
	AnchorAura();
	~AnchorAura();



	int range = 10;
	// Inherited via IModule
	//void charge2(C_GameMode* gm, Vec3<float>* pos);
	virtual void onNormalTick(Actor* actor) override;
	virtual std::string getModName() override;
	virtual void onRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};