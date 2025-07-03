#include "ESP.h"
float opacity3 = 0.2f;
SettingEnum color1;
bool renderself = true;
ESP::ESP() : IModule(0, Category::VISUAL, "Outlines players")
{
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Filled", 0);
	mode.addEntry("3D", 1);
	mode.addEntry("2D", 2);
	registerEnumSetting("Color", &color1, 0);
	color1.addEntry("Dynamic", 0);
	color1.addEntry("White", 1);
	color1.addEntry("Interface", 2);
	registerBoolSetting("Tracers", &tracers, tracers);
	registerBoolSetting("Item ESP", &itemESP, itemESP);
	registerBoolSetting("Mobs", &mobs, mobs);
	registerFloatSetting("Opacity", &opacity3, opacity3, 0.f, 1.f);
}

const char *ESP::getRawModuleName()
{
	return "ESP";
}

void doRenderStuff(C_Entity *ent, bool isRegularEntitie)
{
	static auto esp = moduleMgr->getModule<ESP>();
	auto i = ColorUtil::interfaceColor(1);
	auto player = g_Data.getLocalPlayer();
	// if (ent->timeSinceDeath > 0) return;
	if (TargetUtil::isValidTarget(ent)) {
		if (color1.selected == 0) {
			if (ent->damageTime > 1)
				DrawUtils::setColor(1.f, 0.f, 0.f, opacity3);
			else
				DrawUtils::setColor(1.f, 1.f, 1.f, opacity3);
		}
		if (color1.selected == 1)
			DrawUtils::setColor(1.f, 1.f, 1.f, opacity3);
		if (color1.selected == 2)
		DrawUtils::setColor(i.r, i.b, i.g, opacity3);
	}
	else if (esp->mobs)
	{
		if (ent->getNameTag()->getTextLength() <= 1 && ent->getEntityTypeId() == 63)
			return;

		if (ent->isInvisible())
			return;

		if (!g_Data.getLocalPlayer()->canAttack(ent, false))
			return;
		if (color1.selected == 0) {
			if (ent->damageTime > 1)
				DrawUtils::setColor(1.f, 0.f, 0.f, opacity3);
			else
				DrawUtils::setColor(1.f, 1.f, 1.f, opacity3);
		}
		if (color1.selected == 1)
			DrawUtils::setColor(1.f, 1.f, 1.f, opacity3);
		if (color1.selected == 2)
			DrawUtils::setColor(i.r, i.b, i.g, opacity3);
	}
	else if (esp->itemESP)
	{
		if (ent->getEntityTypeId() == 64)
		{
			DrawUtils::setColor(1.f, 1.f, 1.f, opacity3);
		}
		else
		{
			return;
		}
	}
	else
		return;

	if (esp->mode.getSelectedValue() == 2)
		DrawUtils::drawBetter2D(ent, (float)fmax(0.3f, 1 / (float)fmax(1, (*player->getPos()).dist(*ent->getPos()) * 3.f)));
	else
		DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, (*player->getPos()).dist(*ent->getPos()))));
}

void ESP::onLevelRender()
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;
	if (tracers)
	{
		auto i = ColorUtil::interfaceColor(1);
		float calcYaw = (player->yaw + 90) * (PI / 180);
		float calcPitch = (player->pitch) * -(PI / 180);

		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * 0.5f;
		moveVec.y = sin(calcPitch) * 0.5f;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * 0.5f;

		const vec3_t origin = g_Data.getClientInstance()->levelRenderer->getOrigin().add(moveVec);
		g_Data.forEachEntity([&](C_Entity *ent, bool valid)
							 {
			if (ent != g_Data.getLocalPlayer() && TargetUtil::isValidTarget(ent)) {
				if (color1.selected == 0) {
					if (ent->damageTime > 1)
						DrawUtils::setColor(1.f, 0.f, 0.f, 1.f);
					else
						DrawUtils::setColor(1.f, 1.f, 1.f, 1.f);
				}
				if (color1.selected == 1)
					DrawUtils::setColor(1.f, 1.f, 1.f, 1.f);
				if (color1.selected == 2)
					DrawUtils::setColor(i.r, i.b, i.g, 1.f);
				DrawUtils::drawLine3d(origin, *ent->getPos());
			} });
	}
}

void ESP::onPreRender(C_MinecraftUIRenderContext *renderCtx)
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;
	auto killaura = moduleMgr->getModule<Killaura>();
	if (killaura->isEnabled() && killaura->renderType.getSelectedValue() == 4)
		return;

	if (g_Data.canUseMoveKeys())
		g_Data.forEachEntity(doRenderStuff);
}