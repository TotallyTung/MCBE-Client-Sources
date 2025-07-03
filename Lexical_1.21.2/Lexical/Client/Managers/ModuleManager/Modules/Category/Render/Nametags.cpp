#include "NameTags.h"

float tagSize = 0.70f;
UIColor friendColor = (0, 255, 255, 255);
NameTags::NameTags() : Module("NameTags", "Better nametags.", Category::RENDER) {
	registerSetting(new SliderSetting<float>("Size", "NULL", &tagSize, 0.5f, 0.f, 1.f));
	registerSetting(new SliderSetting<float>("Opacity", "NULL", &opacity, 1.f, 0.f, 1.f));
	registerSetting(new BoolSetting("Self", "Render urself", &showSelf, true));
	registerSetting(new BoolSetting("Underline", "Show a line under", &underline, false));
	registerSetting(new BoolSetting("Dynamic Size", "NULL", &dynamicSize, true));
}

static bool envaledchar(char c) {
	return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
}

std::string sanitizex(const std::string& text) {
	std::string out;
	bool wasValid = true;
	for (char c : text) {
		bool isValid = !envaledchar(c);
		if (wasValid) {
			if (!isValid) {
				wasValid = false;
			}
			else {
				out += c;
			}
		}
		else {
			wasValid = isValid;
		}
	}
	return out;
}


void NameTags::onD2DRender() {
	LocalPlayer* lp = Game::getLocalPlayer();
	if (lp == nullptr) return;
	for (Actor* actor : lp->getlevel()->getRuntimeActorList()) {
		if (TargetUtil::isTargetValid(actor) || (actor == lp && showSelf)) {
			Vec2<float> pos;
			if (Game::clientInstance->WorldToScreen(actor->getEyePos().add2(0.f, 0.75f, 0.f), pos)) {
				std::string name = *actor->getNameTag();

				// Define o tamanho base a partir da configuração tagSize
				float textSize = 1.f * tagSize;
				if (dynamicSize) {
					// Calcula a distância entre o localPlayer e o ator
					float distance = lp->getPos().dist(actor->getPos());
					float dynamicFactor;
					if (distance <= 1.f) {
						dynamicFactor = 3.f;
					}
					else if (distance >= 4.f) {
						dynamicFactor = 1.0f;
					}
					else {
						// Linearmente interpolando de 5 (em 1 bloco) até 0.5 (em 100 blocos)
						dynamicFactor = 3.f - ((distance - 1.f) * (2.0f / 3.f));
					}
					textSize = tagSize * dynamicFactor;

				}

				std::string sanitizedMessage = sanitizex(name);
				float textWidth = D2D::getTextWidth(sanitizedMessage, textSize);
				float textHeight = D2D::getTextHeight(sanitizedMessage, textSize);
				float textPadding = 1.f * textSize;
				Vec2<float> textPos = Vec2<float>(pos.x - textWidth / 2.f, pos.y - textHeight / 2.f);
				Vec4<float> rectPos = Vec4<float>(
					textPos.x - textPadding * 3.0f,
					textPos.y - textPadding,
					textPos.x + textWidth + textPadding * 3.0f,
					textPos.y + textHeight + textPadding
				);

				Vec4<float> underlineRect = Vec4<float>(
					rectPos.x,
					rectPos.w - 1.f * textSize,
					rectPos.z,
					rectPos.w
				);

				D2D::fillRectangle(rectPos, UIColor(12, 27, 46, (int)(255 * opacity)));
				if (underline)
					D2D::fillRectangle(underlineRect, UIColor(255, 255, 255, 255));
				D2D::drawText(textPos, sanitizedMessage, UIColor(255, 255, 255, 255), textSize, true);
			}
		}
	}
}

int waitForInit = 0;
void NameTags::onMCRender(MinecraftUIRenderContext* renderCtx) {
	if (Game::getLocalPlayer() == nullptr) return;
	if (waitForInit >= 20) {
		for (Actor* actor : Game::getLocalPlayer()->getlevel()->getRuntimeActorList()) {
			if (TargetUtil::isTargetValid(actor) || (actor == Game::getLocalPlayer() && showSelf)) {
				Vec2<float> pos;
				if (MCR::worldToScreen(actor->getEyePos().add2(0.f, 0.85f, 0.f), pos)) {
					std::string name = *actor->getNameTag();
					float textSize = 1.f * tagSize;
					float textWidth = MCR::getTextWidth(name, textSize);
					float textHeight = MCR::getTextHeight(textSize);
					float textPadding = 1.f * textSize;
					Vec2<float> textPos = Vec2<float>(pos.x - textWidth / 2.f, pos.y - textHeight / 2.f);
					Vec4<float> rectPos = Vec4<float>(textPos.x - textPadding * 2.f,
						textPos.y - textPadding,
						textPos.x + textWidth + textPadding * 2.f,
						textPos.y + textHeight + textPadding);
					if (showItems) {
						float scale = tagSize * 0.75f;
						float spacing = scale + 15.f;
						int x = 0;
						int y = rectPos.y - 5.f;
						if (actor->getOffhandSlot() != nullptr && actor->getOffhandSlot()->isValid()) x += scale * spacing;
						for (int i = 0; i < 4; i++) {
							ItemStack* armor = actor->getArmor(i);
							if (armor->isValid() && armor != nullptr) {
								x += scale * spacing;
							}
						}
						ItemStack* selected = actor->getCarriedItem();
						if (selected->isValid() && selected != nullptr) x += scale * spacing;
						float xReal = (rectPos.x + rectPos.z - x) / 2;
						if (actor->getOffhandSlot()->isValid() && actor->getOffhandSlot() != nullptr) {
							ItemStack* offhand = actor->getOffhandSlot();
							MCR::drawItem(offhand, Vec2<float>(xReal, y), 1.f, scale);
							//MCR::drawItemDurability(offhand, Vec2<float>(xReal, y), scale, 1.f, 2);
							if (offhand->mCount > 1) {
								MCR::drawText(Vec2<float>(xReal + 10.f, y + 1.5f), std::to_string((int)offhand->mCount), UIColor(1.f, 1.f, 1.f), scale);
							}
							xReal += scale * spacing;
						}
						for (int i = 0; i < 4; i++) {
							ItemStack* armor = actor->getArmor(i);
							if (armor->isValid() && armor != nullptr) {
								MCR::drawItem(armor, Vec2<float>(xReal, y), 1.f, scale);
								//MCR::drawItemDurability(armor, Vec2<float>(xReal, y), scale, 1.f, 2);
								if (armor->mCount > 1) {
									MCR::drawText(Vec2<float>(xReal + 10.f, y + 1.5f), std::to_string((int)armor->mCount), UIColor(1.f, 1.f, 1.f), scale);
								}
								xReal += scale * spacing;
							}
						}
						if (selected->isValid() && selected != nullptr) {
							MCR::drawItem(selected, Vec2<float>(xReal, y), 1.f, scale);
							//MCR::drawItemDurability(selected, Vec2<float>(xReal, y), scale, 1.f, 2);
							if (selected->mCount > 1) {
								MCR::drawText(Vec2<float>(xReal + 10.f, y + 1.5f), std::to_string((int)selected->mCount), UIColor(1.f, 1.f, 1.f), scale);
							}
						}
					}
				}
			}
		}
	}
	else waitForInit++;
}