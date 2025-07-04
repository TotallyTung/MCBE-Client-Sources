#include "Arraylist.h"
#include "../../../../Client.h"
#include <algorithm>
#include <set>

Arraylist::Arraylist() : Module(0x0, Category::CLIENT, "Arraylist") {
}

const char* Arraylist::getModuleName() {
	return "Arraylist";
}

void Arraylist::onModifySettingEnum(SettingEnum* se) {
}

bool sortByLength(const std::shared_ptr<Module>& lhs, const std::shared_ptr<Module>& rhs) {
	std::string name = lhs->getModuleName();
	std::string name2 = rhs->getModuleName();
	/*
	if (lhs->getModName() != nullptr) {
		std::string modName = " " + std::string(lhs->getModName());
		name += modName;
	}
	if (rhs->getModName() != nullptr) {
		std::string modName = " " + std::string(rhs->getModName());
		name2 += modName;
	}
	*/
	return (RenderUtils::getTextWidth(name.c_str(), 1.f) > RenderUtils::getTextWidth(name2.c_str(), 1.f));
}

void Arraylist::onImGuiRender(ImDrawList* d) {
	static Colors* colorsMod = client->moduleMgr->getModule<Colors>();
	float offset = 0.f;
	Vec2<float> windowSize = Vec2<float>(ImGui::GetIO().DisplaySize.x - offset, ImGui::GetIO().DisplaySize.y - offset);
	float textPadding = 1.f * 2.f;
	float textHeight = RenderUtils::getTextHeight("A", 1.f);

	float y = offset;

	std::vector<std::shared_ptr<Module>> modList = client->moduleMgr->getModuleList();
	//std::sort(modList.begin(), modList.end(), sortByLength);
	for (std::shared_ptr<Module> mod : modList) {
		float textWidth = RenderUtils::getTextWidth(mod->getModuleName(), 1.f);
		mod->ArrListDuration = lerpSync(mod->ArrListDuration, (mod->isEnabled() && mod->isVisible()) ? 1.f : 0.f, 0.1f);
		RenderUtils::drawText(d, Vec2<float>(windowSize.x - textPadding * 2.f - textWidth * mod->ArrListDuration, y), mod->getModuleName(), Color(colorsMod->getColor().r, colorsMod->getColor().g, colorsMod->getColor().b, (int)(255 * mod->ArrListDuration)), 1.f, true);
		y += textHeight * mod->ArrListDuration;
	}
	
}