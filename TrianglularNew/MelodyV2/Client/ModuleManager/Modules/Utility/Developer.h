#pragma once
#include "../Module.h"
# define M_PI 3.14159265358979323846 /* pi */

class Developer : public Module {
private:
public:
	Developer();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel);
	virtual void onNormalTick(Actor* actor) override;
	virtual void onRender(MinecraftUIRenderContext* ctx) override;
	virtual void onImGuiRender(ImDrawList* drawlist) override;
};