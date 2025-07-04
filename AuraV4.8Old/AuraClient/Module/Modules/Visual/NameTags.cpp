#include "NameTags.h"
#include "../pch.h"

NameTags::NameTags() : IModule(0, Category::VISUAL, "Displays peoples names through walls") {
	registerFloatSetting("Scale", &scalen, scalen, 0.3f, 1.f);
	registerBoolSetting("Underline", &underline, underline);
	registerBoolSetting("Armor", &displayArmor, displayArmor);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
}

const char* NameTags::getModuleName() {
	return ("NameTags");
}

void drawNameTags(C_Entity* ent, bool) {
	static auto nametags = moduleMgr->getModule<NameTags>();

	if (ent->timeSinceDeath > 0)
		return;
	if (ent->getNameTag()->getTextLength() < 1)
		return;
	if (TargetUtil::isValidTarget(ent) && nametags->isEnabled() || (FriendList::findPlayer(ent->getNameTag()->getText()) && ent->getNameTag()->getTextLength() >= 1 && ent->getEntityTypeId() == 319)) {
		nametags->nameTags.insert(Utils::sanitize(ent->getNameTag()->getText()));
		float dist = ent->getPos()->dist(*g_Data.getLocalPlayer()->getPos());
		//DrawUtils::drawNameTags(ent, fmax(0.6f, 3.f / dist));
		DrawUtils::drawNameTags(ent, fmax(nametags->scalen, 3.f / dist));
		DrawUtils::flush();
	}
}

void NameTags::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!g_Data.canUseMoveKeys()) return;

	// retarded
	/*if (ingameNametagSetting)
		if (!gotPrevSetting) {
			lastSetting = *ingameNametagSetting;
			gotPrevSetting = true;
			*ingameNametagSetting = false;
		} else
			*ingameNametagSetting = false;  //disable other ppl's nametags*/

	g_Data.forEachEntity(drawNameTags);
}

void NameTags::onDisable() {
	/*if (ingameNametagSetting && gotPrevSetting) {
		*ingameNametagSetting = lastSetting;
		gotPrevSetting = false;
	}*/
}