#include "NameTags.h"
bool itemTags = false;
float opacityitem = 0.f;
NameTags::NameTags() : IModule(0, Category::VISUAL, "Shows better nametags above players that can be seen from a lot more far aways") {
	this->registerBoolSetting("Hide Minecraft", &this->HideMinecraftNametags,true);
	this->registerBoolSetting("Display Armor", &this->displayArmor, this->displayArmor);
	this->registerBoolSetting("Underline", &underline, underline);
	this->registerBoolSetting("Item Tags", &itemTags, itemTags);
	this->registerFloatSetting("opacity", &opacity, opacity, 0, 1);
	this->registerFloatSetting("Item Tags->opacity", &opacityitem, opacityitem, 0, 1);
}

NameTags::~NameTags() {
}

const char* NameTags::getModuleName() {
	return ("NameTags");
}
void drawNametags(Entity* ent, bool isRegularEntitie) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();
	static auto fk = moduleMgr->getModule<FakePlayer>();

	if (nameTagsMod == nullptr)
		return;
	if (ent != localPlayer) {
		if (ent->isPlayer() && nameTagsMod != nullptr) {
			if (ent->timeSinceDeath > 0)
				return;

			if (ent->getNameTag()->getTextLength() < 1)
				return;

			nameTagsMod->nameTags.insert(Utils::sanitize(ent->getNameTag()->getText()));
			float dist = ent->getPos()->dist(Game.getClientInstance()->levelRenderer->getOrigin());
			DrawUtils::drawNameTags(ent, fmax(0.6f, 3.f / dist), true, true);
			DrawUtils::flush();
		}

		if (ent->getEntityTypeId() == 64 && itemTags) {
			ItemStack* stack = (ItemStack*)((uintptr_t)ent + 1832);

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
			} else {
				textbuild << "No item";
			}
			std::string text = textbuild.str();

			Vec2 textPos = DrawUtils::worldToScreen(*ent->getPos());
			float size = fmax(0.6f, 3.f / Game.getClientInstance()->levelRenderer->getOrigin().dist(*ent->getPos()));

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

			Vec4 rectPos;
			rectPos.x = textPos.x - 1.f * size;
			rectPos.y = textPos.y - 1.f * size;
			rectPos.z = textPos.x + textWidth + 1.f * size;
			rectPos.w = textPos.y + textHeight + 2.f * size;

			Vec2 itemPos = textPos;
			textPos.x += 15.f * itemSize + 3.f * size;

			DrawUtils::fillRectangle(rectPos, MC_Color(20, 20, 20), opacityitem);
			DrawUtils::drawItem(stack, itemPos, 1.f, itemSize, false);
			DrawUtils::drawText(textPos, &text, MC_Color(255, 255, 255), size);
			DrawUtils::flush();
		}
	}
}
void NameTags::onPreRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr || !GameData::canUseMoveKeys()) return;

	Game.forEachEntity(drawNametags);
}

// rr