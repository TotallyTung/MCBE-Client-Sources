#include "AutoPhase.h"
float amountX2 = 0.85;
float amountZ2 = 0.85;
AutoPhases::AutoPhases() : IModule(0x0, Category::EXPLOIT, "Clip through walls") {
	registerFloatSetting("Offset X", &amountX2, amountX2, -0.1f, 1.f, 0.01f, "The amount to corner clip in the X value");
	registerFloatSetting("Offset Z", &amountZ2, amountZ2, -0.1f, 1.f, 0.01f, "The amount to corner clip in the Z value");
}
const char* AutoPhases::getRawModuleName() {
	return ("Clip");
}
void AutoPhases::onTick(C_GameMode* gm) {
	bool pressed = MoveUtil::keyPressed();
	if (pressed && mode.getSelectedValue() == 1)
		setEnabled(false);
	vec3_t Pos = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	Pos.x += amountX2;
	Pos.z += amountZ2;
	g_Data.getLocalPlayer()->setPos(Pos);
	gm->player->aabb.upper.y = gm->player->aabb.lower.y;
}

void AutoPhases::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
}