#include "NameTags.h"
float tagOpacity = 0.8;
NameTags::NameTags() : IModule(0, Category::VISUAL, "Displays a more detailed name")
{
	registerFloatSetting("Scale", &scalen, scalen, 0.3f, 1.5f);
	//registerEnumSetting("Render", &renderMode, 1);
	//renderMode.addEntry("None", 0);
	//renderMode.addEntry("Underline", 1);
	//renderMode.addEntry("Rectangle", 2);
	registerBoolSetting("Items", &displayArmor, displayArmor, "Displays players items");
	registerBoolSetting("ItemTags", &itemTags, itemTags, "Display dropped items nametags, From Trong with love<3");
	registerBoolSetting("Show Percentage", &showPercentage, showPercentage, "Shows item nametags");
	registerBoolSetting("Show Bar", &showBar, showBar, "Shows bar nametags");
	registerBoolSetting("Show Enchantment", &showEnchantment, showEnchantment, "Shows item enchantment");
	registerBoolSetting("Health", &drawhealth, drawhealth);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	registerFloatSetting("ItemTagOpacity", &tagOpacity, tagOpacity, 0.f, 1.f);
}

const char *NameTags::getRawModuleName()
{
	return ("NameTags");
}

void drawNameTags(C_Entity *ent, bool)
{
	static auto nametags = moduleMgr->getModule<NameTags>();

	if (TargetUtil::isValidTarget(ent) && nametags->isEnabled() || (FriendList::findPlayer(ent->getNameTag()->getText()) && ent->getNameTag()->getTextLength() >= 1 && ent->getEntityTypeId() == 319))
	{
		nametags->nameTags.insert(Utils::sanitize(ent->getNameTag()->getText()));
		float dist = ent->getPos()->dist(*g_Data.getLocalPlayer()->getPos());
		// DrawUtils::drawNameTags(ent, fmax(0.6f, 3.f / dist));
		DrawUtils::drawNameTags(ent, fmax(nametags->scalen, 3.f / dist));
		DrawUtils::flush();
	}
	if (nametags->isEnabled() && ent->getEntityTypeId() == 64 && nametags->itemTags) { // LOL i just copy it from DaDrip
		C_ItemStack* stack = (C_ItemStack*)((uintptr_t)ent + 0x728);

		std::stringstream textbuild;
		if (stack->item != nullptr) {
			if (stack->count > 1)
				textbuild << std::to_string(stack->count) << "x ";

			bool wasSpace = true;
			std::string name = stack->getItem()->name.getText();
			for (auto i = 0; i < name.size(); i++) {
				if (wasSpace) {
					name[i] = toupper(name[i]);
					wasSpace = false;
				}

				if (name[i] == '_') {
					wasSpace = true;
					name[i] = ' ';
				}
			}

			textbuild << name;
		}
		else {
			textbuild << "No item";
		}
		std::string text = textbuild.str();

		vec2_t textPos = DrawUtils::worldToScreen(ent->eyePos0);
		float size = fmax(0.6f, 3.f / g_Data.getClientInstance()->levelRenderer->getOrigin().dist(ent->eyePos0));

		float itemHeight = 15.f * size;
		float textHeight = DrawUtils::getFontHeight(size);
		float itemSize = size;

		if (itemHeight > textHeight) {
			itemSize *= textHeight / itemHeight;
			itemHeight = textHeight;
		}

		float textWidth = DrawUtils::getTextWidth(&text, size) + (15.f * itemSize + 3.f * size);

		textPos.y -= textHeight * 1.5f;
		textPos.x -= textWidth / 2.f;

		vec4_t rectPos;
		rectPos.x = textPos.x - 1.f * size;
		rectPos.y = textPos.y - 1.f * size;
		rectPos.z = textPos.x + textWidth + 1.f * size;
		rectPos.w = textPos.y + textHeight + 2.f * size;

		vec2_t itemPos = textPos;
		textPos.x += 15.f * itemSize + 3.f * size;

		DrawUtils::fillRectangle(rectPos, MC_Color(20, 20, 20), tagOpacity);
		DrawUtils::drawItem(stack, itemPos, 1.f, itemSize, false);
		DrawUtils::drawText(textPos, &text, MC_Color(255, 255, 255), size);
		DrawUtils::flush();
	}
}

void NameTags::onPreRender(C_MinecraftUIRenderContext *renderCtx)
{
	if (!g_Data.canUseMoveKeys())
		return;

	// retarded
	if (ingameNametagSetting && hideTags)
		if (!gotPrevSetting)
		{
			lastSetting = *ingameNametagSetting;
			gotPrevSetting = true;
			*ingameNametagSetting = false;
		}
		else
			*ingameNametagSetting = false; // disable other ppl's nametags

	g_Data.forEachEntity(drawNameTags);
}

void NameTags::onDisable()
{
	if (ingameNametagSetting && gotPrevSetting && hideTags)
	{
		*ingameNametagSetting = lastSetting;
		gotPrevSetting = false;
	}
}
