#include "MCR.h"
#include "../Client/Client.h"
#include "../SDK/Game.h"
#include "../imgui/imgui.h"


BaseActorRenderContext MCR::baseActorRenderCtx;
MinecraftUIRenderContext* MCR::renderCtx = nullptr;
ScreenContext* MCR::screenCtx = nullptr;
ShaderColor* MCR::currentColor = nullptr;
Tessellator* MCR::tessellator = nullptr;
mce::MaterialPtr* MCR::uiMaterial = nullptr;
mce::MaterialPtr* MCR::blendMaterial = nullptr;
Font* MCR::mcFont = nullptr;
Vec3<float> MCR::origin;
float MCR::deltaTime = 0.016f;
std::vector<MCR::Render3dData> MCR::quad3dRenderList;
std::vector<MCR::Render3dData> MCR::line3dRenderList;
void MCR::onRenderScreen(MinecraftUIRenderContext* ctx) {
	if (!ctx || !ctx->clientInstance || !ctx->screenContext || !ctx->clientInstance->mcGame)
		return;

	/* this is annoying to navigate so ima add comments */
	baseActorRenderCtx = BaseActorRenderContext(ctx->screenContext, ctx->clientInstance, ctx->clientInstance->mcGame);
	renderCtx = ctx;
	screenCtx = ctx->screenContext;
	currentColor = ctx->screenContext->currentShaderColor;
	tessellator = ctx->screenContext->tessellator;

	if (uiMaterial == nullptr) {
		HashedString matName("ui_textured_and_glcolor");
		uiMaterial = mce::MaterialPtr::createMaterial(matName);
	}

	if (blendMaterial == nullptr) {
		HashedString blendMatName("fullscreen_cube_overlay_blend");
		blendMaterial = mce::MaterialPtr::createMaterial(blendMatName);
	}

	mcFont = ctx->clientInstance->mcGame->mcFont;
	if (!mcFont) return;

	ModuleManager::onMCRender(renderCtx);

	static Hud* hudmod = ModuleManager::getModule<Hud>();
	if (!hudmod) return;

	/* arraylist */
	if (hudmod->arraylist) {
		LocalPlayer* localPlayer = Game::ourPlayer();
		if (!localPlayer) return;
		if (!Game::clientInstance || !Game::clientInstance->guiData) return;

		Vec2<float> windowSize = Game::clientInstance->guiData->windowSizeReal;
		Vec2<float> basePos(
			std::clamp(hudmod->arraylistx, 0.f, windowSize.x),
			std::clamp(hudmod->arraylisty, 0.f, windowSize.y)
		);

		float textSize = 1.5f;
		bool useBold = hudmod->bold;
		if (useBold) {
			textSize = 1.2f;
		}
		float textHeight = 10.f;
		float padding = 2.f;
		auto& moduleList = ModuleManager::moduleList;
		if (moduleList.empty()) return;
		std::sort(moduleList.begin(), moduleList.end(), [](Module* a, Module* b) {
			if (!a || !b) return false;
			std::string aFullText = a->getModuleName();
			std::string aMode = a->getModeText();
			if (!aMode.empty() && aMode != "NULL") aFullText += " " + aMode;

			std::string bFullText = b->getModuleName();
			std::string bMode = b->getModeText();
			if (!bMode.empty() && bMode != "NULL") bFullText += " " + bMode;

			float aWidth = MCR::getTextWidth ? MCR::getTextWidth(aFullText, 1.5f) : 0.f;
			float bWidth = MCR::getTextWidth ? MCR::getTextWidth(bFullText, 1.5f) : 0.f;

			return aWidth > bWidth;
			});

		bool flipX = hudmod->arraylistx < 500.f;
		static std::unordered_map<std::string, float> animatedYPositions;
		static std::unordered_map<std::string, float> animatedXPositions;
		static std::unordered_map<std::string, float> opacityValues;
		std::vector<Module*> visibleModules;
		for (auto& mod : moduleList) {
			if (mod && mod->isEnabled() && mod->isVisible()) {
				visibleModules.push_back(mod);
				std::string fullModuleKey = mod->getModuleName() + mod->getModeText();
				if (opacityValues.find(fullModuleKey) == opacityValues.end()) {
					opacityValues[fullModuleKey] = 0.f;
				}
			}
		}
		for (auto it = opacityValues.begin(); it != opacityValues.end(); ) {
			bool found = false;
			std::string currentKey = it->first;
			for (auto& mod : visibleModules) {
				std::string fullModuleKey = mod->getModuleName() + mod->getModeText();
				if (fullModuleKey == currentKey) {
					found = true;
					break;
				}
			}
			if (!found) {
				it->second = std::lerp(it->second, 0.f, 0.15f);
				if (it->second < 0.01f) {
					it = opacityValues.erase(it);
					animatedYPositions.erase(currentKey);
					animatedXPositions.erase(currentKey);
				}
				else {
					++it;
				}
			}
			else {
				++it;
			}
		}

		if (visibleModules.empty()) return;

		float currentTargetY = basePos.y;

		for (auto& mod : visibleModules) {
			std::string moduleName = mod->getModuleName();
			std::string modeText = mod->getModeText();
			std::string fullModuleKey = moduleName + modeText;
			float textWidth = 0.f;

			if (MCR::getTextWidth) {
				textWidth = MCR::getTextWidth(moduleName, 1.5f);
				if (!modeText.empty() && modeText != "NULL") {
					textWidth += MCR::getTextWidth(" " + modeText, 1.5f);
				}
			}
			opacityValues[fullModuleKey] = std::lerp(opacityValues[fullModuleKey], 1.f, 0.15f);
			Vec2<float> targetTextPos = basePos;
			if (!flipX) {
				targetTextPos.x -= textWidth;
			}
			if (animatedYPositions.find(fullModuleKey) == animatedYPositions.end()) {
				animatedYPositions[fullModuleKey] = currentTargetY;
			}
			if (animatedXPositions.find(fullModuleKey) == animatedXPositions.end()) {
				animatedXPositions[fullModuleKey] = flipX ? -textWidth : windowSize.x;
			}
			float& currentY = animatedYPositions[fullModuleKey];
			float& currentX = animatedXPositions[fullModuleKey];

			currentY = std::lerp(currentY, currentTargetY, 0.15f);
			currentX = std::lerp(currentX, targetTextPos.x, 0.15f);

			Vec2<float> textPos(currentX, currentY);
			Vec2<float> bgStart(textPos.x - padding, textPos.y - padding / 2);
			Vec2<float> bgEnd(textPos.x + textWidth + padding, textPos.y + textHeight + padding / 2);
			float opacity = opacityValues[fullModuleKey];
			UIColor bgColor(0, 0, 0, static_cast<int>(100 * opacity));
			UIColor textColor(255, 255, 255, static_cast<int>(255 * opacity));
			UIColor modeTextColor(200, 200, 200, static_cast<int>(200 * opacity));
			MCR::fillRectangle(Vec4<float>(bgStart.x, bgStart.y, bgEnd.x, bgEnd.y), bgColor);
			std::string displayModuleName = useBold ? MCTF::BOLD + moduleName : moduleName;
			MCR::drawText(textPos, displayModuleName, textColor, textSize);
			if (!modeText.empty() && modeText != "NULL") {
				float modeOffset = MCR::getTextWidth(moduleName + " ", 1.5f);
				std::string displayModeText = useBold ? MCTF::BOLD + modeText : modeText;
				MCR::drawText(Vec2<float>(textPos.x + modeOffset, textPos.y), displayModeText, modeTextColor, textSize);
			}

			currentTargetY += textHeight + padding;
			if (currentTargetY > windowSize.y) break;
		}
	}
	if (hudmod->watermark) {
		float textSize = 1.5f;
		bool useBold = hudmod->bold;
		if (useBold) {
			textSize = 1.2f;
		}
		Vec2<float> wmPos(
			std::clamp(hudmod->watermarkx, 0.f, Game::clientInstance->guiData->windowSizeReal.x),
			std::clamp(hudmod->watermarky, 0.f, Game::clientInstance->guiData->windowSizeReal.y)
		);
		float wmTextWidth = MCR::getTextWidth("strataware", 1.5f);
		float wmHeight = 10.f;
		Vec2<float> bgStart(wmPos.x - 2.f, wmPos.y - 1.f);
		Vec2<float> bgEnd(wmPos.x + wmTextWidth + 2.f, wmPos.y + wmHeight + 1.f);
		MCR::fillRectangle(Vec4<float>(bgStart.x, bgStart.y, bgEnd.x, bgEnd.y), UIColor(0, 0, 0, 100));

		std::string watermarkText = hudmod->bold ? MCTF::BOLD + std::string("strataware") : "strataware";
		MCR::drawText(wmPos, watermarkText, UIColor(255, 255, 255), textSize);
		Vec2<float> freePos(wmPos.x + wmTextWidth + 5.f, wmPos.y);
		std::string freeText = "free";
		MCR::drawText(freePos, freeText, UIColor(169, 169, 169), textSize);
	}

	if (hudmod->showPosition) {
		auto* player = Game::ourPlayer();
		if (player) {
			float textSize = hudmod->bold ? 1.2f : 1.5f;
			Vec3<float> pos = player->getPos();
			std::string posText = hudmod->bold
				? MCTF::BOLD + ("XYZ: " + std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y) + ", " + std::to_string((int)pos.z))
				: "XYZ: " + std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y) + ", " + std::to_string((int)pos.z);
			Vec2<float> textPos(hudmod->positionOffsetX, hudmod->positionOffsetY);
			float textWidth = MCR::getTextWidth(posText, textSize);
			float textHeight = 10.f;
			Vec2<float> bgStart(textPos.x - 2.f, textPos.y - 1.f);
			Vec2<float> bgEnd(textPos.x + textWidth + 2.f, textPos.y + textHeight + 1.f);
			MCR::fillRectangle(Vec4<float>(bgStart.x, bgStart.y, bgEnd.x, bgEnd.y), UIColor(0, 0, 0, 100));
			MCR::drawText(textPos, posText, UIColor(255, 255, 255), textSize);
		}
	}
	if (hudmod->showHealth) {
		auto* player = Game::ourPlayer();
		if (player) {
			float health = player->getHealth();
			float absorption = player->getAbsorption();
			float maxHealth = 20.f;
			float totalHealth = health + absorption;
			int hpPercent = static_cast<int>((health / maxHealth) * 100);
			float r = 255.f;
			float g = 0.f;
			float b = 0.f;
			if (totalHealth > maxHealth) {
				r = 255.f;
				g = 255.f;
				b = 0.f;
			}
			else {
				if (hpPercent <= 50) {
					g = (hpPercent / 50.0f) * 255.0f;
				}
				else {
					g = 255.0f;
					r = (1.0f - (hpPercent - 50.0f) / 50.0f) * 255.0f;
				}
			}
			UIColor hpColor = UIColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
			std::string healthText = hudmod->bold
				? MCTF::BOLD + ("HP: " + std::to_string(hpPercent) + "%")
				: "HP: " + std::to_string(hpPercent) + "%";
			Vec2<float> textPos(hudmod->healthOffsetX, hudmod->healthOffsetY);
			float textWidth = MCR::getTextWidth("HP: 100%", 1.5f);
			float textHeight = 10.f;
			Vec2<float> bgStart(textPos.x - 2.f, textPos.y - 1.f);
			Vec2<float> bgEnd(textPos.x + textWidth + 2.f, textPos.y + textHeight + 1.f);
			MCR::fillRectangle(Vec4<float>(bgStart.x, bgStart.y, bgEnd.x, bgEnd.y), UIColor(0, 0, 0, 100));
			MCR::drawText(textPos, healthText, hpColor, 1.5f);
		}
	}
	if (!Game::canUseMoveKeys()) {
		MCR::drawText(Vec2<float>(0, 0), "free", UIColor(255, 255, 255), 1.f);
	}
	renderCtx->flushText(0.f);
}


void MCR::onRenderWorld(LevelRenderer* levelRenderer, ScreenContext* screenContext) {

	quad3dRenderList.clear();
	line3dRenderList.clear();

	origin = levelRenderer->levelRendererPlayer->realOrigin;
	ModuleManager::onLevelRender();

	setColor(mce::Color(1.f, 1.f, 1.f, 1.f));
	if (!quad3dRenderList.empty()) {
		tessellator->begin(VertextFormat::QUAD);
		for (auto& quad3dRenderData : quad3dRenderList) {
			mce::Color mcColor = quad3dRenderData.color.toMCColor();
			tessellator->color(mcColor.r, mcColor.g, mcColor.b, mcColor.a);
			tessellator->vertex(quad3dRenderData.pos.x, quad3dRenderData.pos.y, quad3dRenderData.pos.z);
		}
		MeshHelpers::renderMeshImmediately(screenCtx, tessellator, blendMaterial);
		quad3dRenderList.clear();
	}

	if (!line3dRenderList.empty()) {
		tessellator->begin(VertextFormat::LINE_LIST);
		for (auto& line3dRenderData : line3dRenderList) {
			mce::Color mcColor = line3dRenderData.color.toMCColor();
			tessellator->color(mcColor.r, mcColor.g, mcColor.b, mcColor.a);
			tessellator->vertex(line3dRenderData.pos.x, line3dRenderData.pos.y, line3dRenderData.pos.z);
		}
		MeshHelpers::renderMeshImmediately(screenCtx, tessellator, blendMaterial);
		line3dRenderList.clear();
	}
}

void MCR::setColor(const mce::Color& color) {
	currentColor->color = color;
	currentColor->dirty = true;
}

void MCR::setColor(const UIColor& color) {
	currentColor->color = color.toMCColor();
	currentColor->dirty = true;
}

void MCR::drawText(const Vec2<float>& textPos, const std::string& textStr, const UIColor& color, float textSize) {

	std::string text = textStr;
	mce::Color mcColor = color.toMCColor();

	RectangleArea rect = {};
	rect._x0 = textPos.x;
	rect._x1 = textPos.x;
	rect._y0 = textPos.y;
	rect._y1 = textPos.y;

	TextMeasureData textMeasureData = {};
	textMeasureData.fontSize = textSize;
	textMeasureData.linePadding = 0.f;
	textMeasureData.renderShadow = true;
	textMeasureData.showColorSymbol = false;
	textMeasureData.hideHyphen = false;

	static CaretMeasureData caretMeasureData = {};
	caretMeasureData.position = -1.f;
	caretMeasureData.shouldRender = false;

	renderCtx->drawText(mcFont, &rect, &text, &mcColor, mcColor.a, 0, &textMeasureData, &caretMeasureData);
}

float MCR::getTextWidth(const std::string& textStr, float textSize) {
	std::string text = textStr;
	return renderCtx->getLineLength(mcFont, &text, textSize, false);
}

float MCR::getTextHeight(float textSize) {
	return (9.f * textSize);
}

void MCR::drawQuad(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Vec2<float>& p4, const UIColor& color) {
	setColor(color);
	tessellator->begin(VertextFormat::QUAD, 4);

	tessellator->vertex(p1.x, p1.y, 0.f);
	tessellator->vertex(p2.x, p2.y, 0.f);
	tessellator->vertex(p3.x, p3.y, 0.f);
	tessellator->vertex(p4.x, p4.y, 0.f);

	MeshHelpers::renderMeshImmediately(screenCtx, tessellator, uiMaterial);
}
void MCR::drawLine(const Vec2<float>& p1, const Vec2<float>& p2, const UIColor& color) {
	setColor(color);
	tessellator->begin(VertextFormat::LINE_LIST, 2);
	tessellator->vertex(p1.x, p1.y, 0.f);
	tessellator->vertex(p2.x, p2.y, 0.f);
	MeshHelpers::renderMeshImmediately(screenCtx, tessellator, uiMaterial);
}



void MCR::fillRectangle(const Vec4<float>& rect, const UIColor& color) {
	drawQuad(Vec2<float>(rect.x, rect.w), Vec2<float>(rect.z, rect.w), Vec2<float>(rect.z, rect.y), Vec2<float>(rect.x, rect.y), color);
}



/*void MCR::drawItem(const Vec2<float>& pos, ItemStack* itemStack, float scale, bool showDurabilityBar) {
	baseActorRenderCtx.itemRenderer->renderGuiItemNew(&baseActorRenderCtx, itemStack, 0, pos.x, pos.y, 1.f, scale, false);

	if (itemStack->item.get()->isGlint(itemStack)) {
		setColor(mce::Color(1.f, 1.f, 1.f, 1.f));
		baseActorRenderCtx.itemRenderer->renderGuiItemNew(&baseActorRenderCtx, itemStack, 0, pos.x, pos.y, 1.f, scale, true);
	}

	if (showDurabilityBar) {
		Item* item = itemStack->item.get();
		short maxDamage = item->getMaxDamage();
		short damageValue = item->getDamageValue(itemStack->mUserData);
		short durability = maxDamage - damageValue;

		if (maxDamage != 0 && durability != maxDamage) {
			float percent = (float)durability / (float)maxDamage;
			UIColor color = ColorUtil::lerp(UIColor(255, 0, 0), UIColor(0, 255, 0), percent);
			Vec4<float> barRect = Vec4<float>(pos.x + (2.f * scale), pos.y + (13.5f * scale), pos.x + (14.f * scale), pos.y + (15.f * scale));
			Vec4<float> durabilityRect = Vec4<float>(barRect.x, barRect.y, barRect.x + ((barRect.z - barRect.x) * percent), barRect.y + (1.f * scale));

			fillRectangle(barRect, UIColor(0, 0, 0, 255));
			fillRectangle(durabilityRect, color);
		}
	}
}*/
void MCR::drawItem(ItemStack* item, Vec2<float> pos, float opacity, float scale) {
	baseActorRenderCtx.itemRenderer->renderGuiItemNew(&baseActorRenderCtx, item, 0, pos.x, pos.y, opacity, scale, false);
	ItemStackBase* itemBase = (ItemStackBase*)item;
	if (item->item->isGlint(itemBase)) {
		MCR::setColor(mce::Color(1.f, 1.f, 1.f, opacity));
		baseActorRenderCtx.itemRenderer->renderGuiItemNew(&baseActorRenderCtx, item, 0, pos.x, pos.y, opacity, scale, true);
	}
}

void MCR::drawItemDurability(ItemStack* itemStack, Vec2<float> itemPos, float scale, float opacity, int mode) {
	Item* item = itemStack->item.get();
	float maxDamage = item->getMaxDamage();
	float damageValue = item->getDamageValue(itemStack->mUserData);
	float currentDamage = maxDamage - damageValue;
	if (currentDamage == maxDamage) return;
	float percentage = currentDamage / maxDamage * 100;
	float fraction = percentage / 100;
	UIColor durabilityColor = ColorUtil::lerp(UIColor(255, 0, 0), UIColor(0, 255, 0), fraction);
	if (mode == 0 || mode == 2) {
		Vec4<float> barPos = Vec4<float>(itemPos.x + 1.5f * scale, itemPos.y + 14.f * scale, itemPos.x + 15.f * scale, itemPos.y + 16.f * scale);
		Vec4<float> rect = Vec4<float>(barPos.x, barPos.y, barPos.x + ((barPos.z - barPos.x) / 100 * (int)percentage), barPos.w - 1.f * scale);
		MCR::fillRectangle(barPos, UIColor(0.f, 0.f, 0.f));
		MCR::fillRectangle(rect, durabilityColor);
	}
	if (mode == 1 || mode == 2) {
		std::string duraText = std::to_string((int)percentage);
		MCR::drawText(Vec2<float>(itemPos.x + 7.5f, itemPos.y - 6.5f), duraText, durabilityColor, scale);
	}
}

void MCR::drawBox3dFilled(const AABB& aabb, const UIColor& color, const UIColor& lineColor, float scale) {
	Vec3<float> diff = aabb.upper.sub(aabb.lower);
	Vec3<float> newLower = aabb.lower.sub(MCR::origin);

	Vec3<float> vertices[8] = {
		{newLower.x, newLower.y, newLower.z},
		{newLower.x + diff.x, newLower.y, newLower.z},
		{newLower.x, newLower.y, newLower.z + diff.z},
		{newLower.x + diff.x, newLower.y, newLower.z + diff.z},

		{newLower.x, newLower.y + diff.y, newLower.z},
		{newLower.x + diff.x, newLower.y + diff.y, newLower.z},
		{newLower.x, newLower.y + diff.y, newLower.z + diff.z},
		{newLower.x + diff.x, newLower.y + diff.y, newLower.z + diff.z} 
	};

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(scale), 0.f, glm::vec3(1.0f, 1.0f, 1.0f));
	Vec3<float> newLowerReal = newLower.add(Vec3<float>(0.5f, 0.5f, 0.5f));

	for (int i = 0; i < 8; i++) {
		glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertices[i].x - newLowerReal.x, vertices[i].y - newLowerReal.y, vertices[i].z - newLowerReal.z, 0.0f);
		vertices[i] = Vec3<float>(rotatedVertex.x + newLowerReal.x, rotatedVertex.y + newLowerReal.y, rotatedVertex.z + newLowerReal.z);
	}

	if (color.a > 0) {
		static uint8_t v[48] = { 5, 7, 6, 4, 4, 6, 7, 5, 1, 3, 2, 0, 0, 2, 3, 1, 4, 5, 1, 0, 0, 1, 5, 4, 6, 7, 3, 2, 2, 3, 7, 6, 4, 6, 2, 0, 0, 2, 6, 4, 5, 7, 3, 1, 1, 3, 7, 5 };
		for (int i = 0; i < 48; i++)
			quad3dRenderList.push_back(Render3dData(vertices[v[i]], color));
	}

	if (lineColor.a > 0) {
		static auto drawLine = [](const Vec3<float>& start, const Vec3<float>& end, const UIColor& _color) {
			line3dRenderList.push_back(Render3dData(start, _color));
			line3dRenderList.push_back(Render3dData(end, _color));
		};

		// Top square
		drawLine(vertices[4], vertices[5], lineColor);
		drawLine(vertices[5], vertices[7], lineColor);
		drawLine(vertices[7], vertices[6], lineColor);
		drawLine(vertices[6], vertices[4], lineColor);

		// Bottom Square
		drawLine(vertices[0], vertices[1], lineColor);
		drawLine(vertices[1], vertices[3], lineColor);
		drawLine(vertices[3], vertices[2], lineColor);
		drawLine(vertices[2], vertices[0], lineColor);

		// Sides
		drawLine(vertices[0], vertices[4], lineColor);
		drawLine(vertices[1], vertices[5], lineColor);
		drawLine(vertices[2], vertices[6], lineColor);
		drawLine(vertices[3], vertices[7], lineColor);
	}
}
void MCR::drawRing3d(const Vec3<float>& worldPos, float innerRadius, float outerRadius,
	const UIColor& color, const UIColor& lineColor,
	float scale, const Vec3<float>& rotationAngles,
	int segments) {

	Vec3<float> center = worldPos.sub(MCR::origin);
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, rotationAngles.x, glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, rotationAngles.y, glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, rotationAngles.z, glm::vec3(0, 0, 1));
	transform = glm::scale(transform, glm::vec3(scale));

	std::vector<Vec3<float>> outerRing, innerRing;
	outerRing.reserve(segments + 1);
	innerRing.reserve(segments + 1);

	for (int i = 0; i <= segments; ++i) {
		float angle = 2.0f * PI * i / segments;
		float cosA = cos(angle);
		float sinA = sin(angle);

		glm::vec4 outer = transform * glm::vec4(outerRadius * cosA, 0, outerRadius * sinA, 1.0f);
		glm::vec4 inner = transform * glm::vec4(innerRadius * cosA, 0, innerRadius * sinA, 1.0f);

		outerRing.emplace_back(center.x + outer.x, center.y + outer.y, center.z + outer.z);
		innerRing.emplace_back(center.x + inner.x, center.y + inner.y, center.z + inner.z);
	}

	if (color.a > 0) {
		for (int i = 0; i < segments; ++i) {
			quad3dRenderList.push_back(Render3dData(innerRing[i], color));
			quad3dRenderList.push_back(Render3dData(outerRing[i], color));
			quad3dRenderList.push_back(Render3dData(outerRing[i + 1], color));

			quad3dRenderList.push_back(Render3dData(innerRing[i], color));
			quad3dRenderList.push_back(Render3dData(outerRing[i + 1], color));
			quad3dRenderList.push_back(Render3dData(innerRing[i + 1], color));
		}
	}

	if (lineColor.a > 0) {
		for (int i = 0; i < segments; ++i) {
			line3dRenderList.push_back(Render3dData(innerRing[i], lineColor));
			line3dRenderList.push_back(Render3dData(innerRing[i + 1], lineColor));

			line3dRenderList.push_back(Render3dData(outerRing[i], lineColor));
			line3dRenderList.push_back(Render3dData(outerRing[i + 1], lineColor));
		}
	}
}

bool MCR::worldToScreen(const Vec3<float>& pos, Vec2<float>& out) {

	ClientHMDState* hmdState = Game::clientInstance->getHMDState();

	Vec2<float> screenSize = Game::clientInstance->guiData->windowSizeScaled;
	Vec3<float> relativePos = pos.sub(origin);

	glm::mat4 mvpMatrix = hmdState->mLastLevelProjMatrix * hmdState->mLastLevelViewMatrix;
	glm::vec4 clipSpacePosition = mvpMatrix * glm::vec4(relativePos.x, relativePos.y, relativePos.z, 1.0f);

	if (clipSpacePosition.w <= 0.0f)
		return false;

	clipSpacePosition /= clipSpacePosition.w;

	out = Vec2<float>(
		(clipSpacePosition.x + 1.0f) * (0.5f * screenSize.x),
		(1.0f - clipSpacePosition.y) * (0.5f * screenSize.y)
	);

	return true;
}
