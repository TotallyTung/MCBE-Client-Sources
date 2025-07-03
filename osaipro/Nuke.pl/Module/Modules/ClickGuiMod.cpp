 #include "ClickGuiMod.h"
#include "../../Menu/ClickGui.h"
#include "../../Library/ImGUI/imgui_internal.h"
int particlecount = 0;
float particlespeed = 0;
bool showBG1 = false;
bool triangle = false;
bool sphere = false;
bool sqaure = false;
ClickGuiMod::ClickGuiMod() : IModule(VK_INSERT, Category::CLIENT, "The clickgui - toggle everything just by clicking on it!") {
	registerBoolSetting("Show Tooltips", &showTooltips, showTooltips);
	registerBoolSetting("Particles", &showParticles, showParticles);
	registerBoolSetting("Gradient", &showBG1, showBG1);
	registerBoolSetting("triangle", &triangle, triangle);
	registerBoolSetting("sphere", &sphere, sphere);
	registerBoolSetting("sqaure", &sqaure, sqaure);
/*	guiMode
		.addEntry("Horion", 0)
		.addEntry("t", 1);
	registerEnumSetting("Style", &guiMode, 0);
*/	this->registerFloatSetting("Scale", &this->scale, 0.8f, 0.f, 1.5f);
this->registerFloatSetting("Opacity", &this->opacity, 0.6f, 0.f, 1.5f);
registerIntSetting("Particle Count", &particlecount, particlecount, 0, 500);
registerFloatSetting("Particle Speed", &particlespeed, particlespeed, 0.5f, 2.f);
}
ClickGuiMod::~ClickGuiMod() {
}
const char* ClickGuiMod::getModuleName() {
	return ("Gui");
}
void ClickGuiMod::onEnable() {
	Game.getClientInstance()->releaseMouse();
}
void ClickGuiMod::onDisable() {
	Game.getClientInstance()->grabMouse();
}
bool ClickGuiMod::allowAutoStart() {
	return false;
}
void ClickGuiMod::onLoadConfig(void* conf) {
	IModule::onLoadConfig(conf);
	ClickGui::onLoadConfig(conf);
}
void ClickGuiMod::onSaveConfig(void* conf) {
	IModule::onSaveConfig(conf);
	ClickGui::onSaveConfig(conf);
}
void ClickGuiMod::onImGuiRender(ImDrawList* d) {
	ClickGuiMod* clickGuiMod1 = moduleMgr->getModule<ClickGuiMod>();
	float opacity = 1.0f;
	UIColor mainColor2 = moduleMgr->getModule<Colors>()->getColor(1);
	if (clickGuiMod1 && showBG1) {
		d->AddRectFilledMultiColor(ImVec2(0.f, 0.f), ImGui::GetIO().DisplaySize, ImColor(0, 0, 0, (int)(75 * opacity)), ImColor(0, 0, 0, (int)(75)), ImColor(mainColor2.r, mainColor2.g, mainColor2.b, (int)(75 * opacity)), ImColor(mainColor2.r, mainColor2.g, mainColor2.b, (int)(75 * opacity)));
	}
	if (clickGuiMod1 && clickGuiMod1->showParticles) {
		float start = RandomFloat(0.f, ImGui::GetIO().DisplaySize.x);
		if (particles.size() < particlecount) {
			int count = 1 * (int)(ceil(60.f / ImGui::GetIO().Framerate));
			for (int i = 0; i < count; i++) {
				float randomAngle = RandomFloat(0.f, 6.28319f);
				int shapeType = static_cast<int>(RandomFloat(0.f, 3.f)); 

				particles.emplace_back(std::make_shared<Particle21>(
					Vec2(start, ImGui::GetIO().DisplaySize.y + 10.f),
					Vec2(start, ImGui::GetIO().DisplaySize.y / RandomFloat(1.5f, 2.5f)),
					RandomFloat(3.f, 10.f),
					RandomFloat(1.f, 7.f),
					2.0f,
					randomAngle,
					shapeType
				));
			}
		}

		UIColor mainColor = moduleMgr->getModule<Colors>()->getColor(1);
		for (auto it = particles.begin(); it != particles.end();) {
			std::shared_ptr<Particle21> p = *it;
			ImVec2 p1(p->startPos.x, p->startPos.y - p->size);
			ImVec2 p2(p->startPos.x - p->size, p->startPos.y + p->size);
			ImVec2 p3(p->startPos.x + p->size, p->startPos.y + p->size);
			float angle = p->rotationAngle;
			auto rotate = [&](ImVec2& point, float angle) {
				float cosA = cos(angle);
				float sinA = sin(angle);
				float x = point.x - p->startPos.x;
				float y = point.y - p->startPos.y;
				point.x = p->startPos.x + (cosA * x - sinA * y);
				point.y = p->startPos.y + (sinA * x + cosA * y);
				};
			rotate(p1, angle);
			rotate(p2, angle);
			rotate(p3, angle);
			if (p->shapeType == 0) {
				// Draw square
				if (sqaure) {
					d->AddRectFilled(p1, ImVec2(p1.x + p->size, p1.y + p->size), ImColor(mainColor.r, mainColor.g, mainColor.b, (int)(150 * opacity * p->duration)));
				}
			}
			else if (p->shapeType == 1) {
				// Draw circle 
				if (sphere) {
					d->AddCircleFilled(p1, p->size, ImColor(mainColor.r, mainColor.g, mainColor.b, (int)(150 * opacity * p->duration)));
				}
			}
			else {
				// Draw triangle
				if (triangle) {
					d->AddTriangleFilled(p1, p2, p3, ImColor(mainColor.r, mainColor.g, mainColor.b, (int)(150 * opacity * p->duration)));
				}
			}
			p->startPos.y -= p->speed * particlespeed * (60.f / ImGui::GetIO().Framerate);

			if (p->startPos.y <= p->endPos.y || !clickGuiMod1->showParticles) {
				p->duration = lerp(p->duration, 0.f, 0.1f);
			}

			if (p->duration <= 0.1f) {
				it = particles.erase(it);
			}
			else {
				++it;
			}
		}
	}
}
/*
static float blur = 0.f;
std::shared_ptr<IModule> selectedModule;
Category selectedCategory = Category::COMBAT;
bool isDragging2 = false;
Vec2 dragStart2 = Vec2();
bool shouldReset = false;
void getModuleListByCategory(Category category, std::vector<std::shared_ptr<IModule>>* modList) {
	std::vector<std::shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();

	for (auto& it : *moduleList) {
		if (it->getCategory() == category)
			modList->push_back(it);
	}
}
void ClickGuiMod::onMouseClickUpdate(int key, bool isDown) {
	static auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
	if (clickGUI->isEnabled()) {
		switch (key) {
		case 1:  // Left Click
			isLeftClickDown = isDown;
			if (isDown)
				shouldToggleLeftClick = true;
			else
				isLeftClickDown = false;
			break;
		case 2:  // Right Click
			isRightClickDown = isDown;
			if (!isDown)
				shouldToggleRightClick = true;
			else
				isRightClickDown = false;
		}
	}
}
void ClickGuiMod::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (GameData::canUseMoveKeys())
		Game.getClientInstance()->releaseMouse();
}
void ClickGuiMod::onWheelScroll(bool direction) {
	if (!issettingOpen && canScroll) {
		if (!direction) {
			scrollDirection += 25.f;
		}
		else {
			if (scrollDirection > 0) {
				scrollDirection -= 25.f;
			}
		}
	}
	if (issettingOpen && settingcanScroll) {
		if (!direction) {
			settingscrollDirection += 25.f;
		}
		else {
			if (settingscrollDirection > 0) {
				settingscrollDirection -= 25.f;
			}
		}
	}
}
void ClickGuiMod::onImGuiRender(ImDrawList* d) {
	static bool extended = false;
	static float auau = 0;
	static auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
	if (clickGUI->isEnabled()) {
		if (guiMode.getSelectedValue() == 1) {
			Vec2 guipos = Vec2(ImGuiUtils::getScreenResolution().x / 2, ImGuiUtils::getScreenResolution().y / 2);
			ImGui::StyleColorsDark();

			auto& io = ImGui::GetIO();
			static float lastMouseY = io.MousePos.y;

			ImVec2 windowSize = ImGuiUtils::getScreenResolution();
			ImVec4 guiRect = ImVec4(guipos.x - 450, guipos.y - 275, guipos.x + 450, guipos.y + 275);

			//modRECT
			ImVec4 modRect(guiRect.x + 250, guiRect.y + 20.f, guiRect.z, guiRect.w);

			std::vector<Category> categories = {
				Category::COMBAT,
				Category::MOVEMENT,
				Category::VISUAL,
				Category::PLAYER,
				Category::WORLD,
				Category::MISC,
				Category::CLIENT
			};

			float percent = ClickGuiMod::percent;

			d->AddShadowRect(ImVec2(guiRect.x, guiRect.y), ImVec2(guiRect.z, guiRect.w), UIColor(0, 100, 100, static_cast<int>(255 * percent)).toImColor(), 50.f, ImVec2(0, 0), ImDrawFlags_ShadowCutOutShapeBackground, 20.f);
			d->PushClipRect(ImVec2(guiRect.x, guiRect.y), ImVec2(guiRect.z, guiRect.w), true);
			d->AddRectFilled(ImVec2(modRect.x, guiRect.y + 64.f), ImVec2(guiRect.z, guiRect.w), UIColor(24, 24, 26, static_cast<int>(255 * percent)).toImColor(), 20.f, ImDrawFlags_RoundCornersBottomRight);
			d->AddRectFilled(ImVec2(guiRect.x, guiRect.y), ImVec2(guiRect.x + 250, guiRect.w), UIColor(28, 28, 31, static_cast<int>(255 * percent)).toImColor(), 20.f, ImDrawFlags_RoundCornersLeft);

			UIColor backButtonColor(issettingOpen ? UIColor(255, 255, 255, static_cast<int>(255 * percent)) : UIColor(128, 132, 142, static_cast<int>(255 * percent)));

			ImRect backRect(modRect.x + 35, guiRect.y + 13.f, modRect.x + 35 + ImGuiUtils::getTextWidth("<", 1.5f), guiRect.y + 13.f + ImGuiUtils::getTextHeight(1.5f));

			Vec4 back(modRect.x + 35, guiRect.y + 13.f, modRect.x + 35 + ImGuiUtils::getTextWidth("<", 1.5f), guiRect.y + 13.f + ImGuiUtils::getTextHeight(1.5f));

			Vec2 mousePos = Vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
			ImGuiUtils::drawText(Vec2(guiRect.x + (((modRect.x - guiRect.x) / 2) - ImGuiUtils::getTextWidth(Utils::getClientName().c_str(), 2.2f) / 2), guiRect.y + 8.f), Utils::getClientName().c_str(), UIColor(255, 255, 255, static_cast<int>(255 * percent)), 2.2f);//instance?w
			ImRect dragpos = ImRect(guiRect.x + (((modRect.x - guiRect.x) / 2) - ImGuiUtils::getTextWidth(Utils::getClientName().c_str(), 2.2f) / 2), guiRect.y + 8.f, guiRect.x + (((modRect.x - guiRect.x) / 2) - ImGuiUtils::getTextWidth(Utils::getClientName().c_str(), 2.2f) / 2) + ImGuiUtils::getTextWidth(Utils::getClientName().c_str(), 2.2f), guiRect.y + 8.f + ImGuiUtils::getTextHeight(2.2f));

			Vec4 dragvec4(guiRect.x + (((modRect.x - guiRect.x) / 2) - ImGuiUtils::getTextWidth(Utils::getClientName().c_str(), 2.2f) / 2), guiRect.y + 8.f, guiRect.x + (((modRect.x - guiRect.x) / 2) - ImGuiUtils::getTextWidth(Utils::getClientName().c_str(), 2.2f) / 2) + ImGuiUtils::getTextWidth(Utils::getClientName().c_str(), 2.2f), guiRect.y + 8.f + ImGuiUtils::getTextHeight(2.2f));

			ImGuiUtils::drawRectFilled(Vec4(guiRect.x + 5.f, guiRect.y + 8.f + ImGuiUtils::getTextHeight(2.2f) + 6, modRect.x - 5.f, guiRect.y + 8.f + ImGuiUtils::getTextHeight(2.2f) + 5), UIColor(255, 255, 255, static_cast<int>(255 * percent)));
			const bool containsPos = dragvec4.contains(&mousePos);
			if (isDragging2) {
				if (isLeftClickDown) {
					Vec2 diff = Vec2(mousePos).sub(dragStart2);
					guipos.y = guipos.y + diff.y;
					guipos.x = guipos.x + diff.x;
					{
						if (guiRect.z > windowSize.x)
							guipos.x = windowSize.x - 450;

						if (guiRect.w > windowSize.y)
							guipos.y = windowSize.y - 275;

						if (guiRect.x < 0)
							guipos.x = 450;

						if (guiRect.y < 0)
							guipos.y = 275;
					}
					dragStart2 = mousePos;
				}
				else {
					isDragging2 = false;
				}
			}
			else if (containsPos && isLeftClickDown) {
				dragStart2 = mousePos;
				isDragging2 = true;
			}
			float yOffset = 0.f;
			static float positionY = guiRect.y + 71.f;
			static float opa[6] = { 0,0,0,0,0,0 };
			int index = 0;
			for (auto& cat : categories) {
				++index;
				float hP = 0.f;
				ImVec4 catRect = ImVec4(guiRect.x + 10.f, guiRect.y + 70.f + yOffset, guiRect.x + 238.f, guiRect.y + 123.f + yOffset);
				Vec4 checkHoverRect = Vec4(catRect.x, catRect.y, catRect.z, catRect.w);

				const bool hovered = checkHoverRect.contains(&mousePos);

				ImVec4 prev(catRect.x, catRect.y + 1.f, catRect.x + 10.f, catRect.w - 1.f);

				if (hovered) {
					if (shouldToggleLeftClick) {
						selectedCategory = cat;
						scrollDirection = 0;
						selectedModule = nullptr;
						settingscrollDirection = 0;
						shouldReset = true;
						issettingOpen = false;
					}
					opa[index] = static_cast<float>(smoothLerp(0.0, opa[index], 0.12));
				}
				else {
					opa[index] = static_cast<float>(smoothLerp(0.0, opa[index], 0.12));
				}
				if (opa[index] <= 0.1f) {
					opa[index] = 0.0f;
				}
				else {
					d->AddRectFilled(ImVec2(catRect.x, catRect.y), ImVec2(catRect.z, catRect.w), UIColor(34, 34, 39, static_cast<int>(255 * percent * opa[index])).toImColor(), 5.f, ImDrawFlags_RoundCornersAll);
				}

				if (selectedCategory == cat) {
					if (isDragging2) {
						positionY = prev.y;
					}
					d->AddRectFilled(ImVec2(prev.x, positionY + 1.4f), ImVec2(prev.z, positionY + (catRect.w - catRect.y) - 1.4f), UIColor(44, 41, 230, static_cast<int>(255 * percent)).toImColor(), 5.f, ImDrawFlags_RoundCornersAll);
					positionY = static_cast<float>(smoothLerp(prev.y, positionY, 0.15));
				}
				ImGuiUtils::drawText(Vec2(catRect.x + 25.f, catRect.y + 11.5f), ClickGui::catToName(cat), UIColor(255, 255, 255, static_cast<int>(255 * percent)), 1.2f);
				yOffset += 60.f;
			}

			float modYOffset = 0.f;
			float modXOffset = 0.f;

			ImVec4 moduleRect(modRect.x + 33 + modXOffset, modRect.y + 65 + modYOffset - scrollDirection, modRect.x + 320 + modXOffset, modRect.y + 187 + modYOffset - scrollDirection);
			std::vector<std::shared_ptr<IModule>> ModuleList;
			getModuleListByCategory(selectedCategory, &ModuleList);

			float spacing = 134.5f;

			for (auto& mod : ModuleList) {
				if (!issettingOpen) {
					settingcanScroll = false;
					if (moduleRect.y < (modRect.y + 65)) {
						canScroll = true;
					}

					ImRect modImRect(ImVec2(moduleRect.x, moduleRect.y), ImVec2(moduleRect.z, moduleRect.w));

					d->AddRectFilled(ImVec2(moduleRect.x, moduleRect.y), ImVec2(moduleRect.z, moduleRect.w), UIColor(40, 40, 41, static_cast<int>(255 * percent)).toImColor(), 5.f, ImDrawFlags_RoundCornersAll);
					ImGuiUtils::drawText(Vec2(moduleRect.x + 10.f, moduleRect.y + 7.f), mod->getModuleName(), UIColor(255, 255, 255, static_cast<int>(255 * percent)), 1.55f);

					// logic

					ImVec4 toggleRect(moduleRect.x + 10.f, moduleRect.w - 39.f, moduleRect.x + 65.f, moduleRect.w - 14.f);

					float start = mod->isEnabled() ? (toggleRect.x + 12.4f) : (toggleRect.z - 13.6f);
					float end = mod->isEnabled() ? (toggleRect.z - 13.6f) : (toggleRect.x + 12.4f);
					if (!mod->init) {
						mod->_toggle = end;
						mod->init = true;
					}
					if (shouldReset) {
						mod->init = false;
					}
					mod->_toggle = static_cast<float>(smoothLerp(end, mod->_toggle, 0.45f));

					UIColor toggleColor(mod->isEnabled() ? UIColor(44, 41, 230, static_cast<int>(255 * percent)) : UIColor(128, 132, 142, static_cast<int>(255 * percent)));
					d->AddRectFilled(ImVec2(toggleRect.x, toggleRect.y), ImVec2(toggleRect.z, toggleRect.w), UIColor(toggleColor).toImColor(), 50.f, ImDrawFlags_RoundCornersAll);

					d->AddCircleFilled(ImVec2(mod->_toggle, toggleRect.w - 12.3f), 9.15f, UIColor(255, 255, 255, static_cast<int>(255 * percent)).toImColor());
					//fixed
					if (Vec4(moduleRect.x, moduleRect.y, moduleRect.z, moduleRect.w).contains(&mousePos) && shouldToggleLeftClick) {
						mod->toggle();
						shouldToggleLeftClick = false;
					}

					if (Vec4(moduleRect.x, moduleRect.y, moduleRect.z, moduleRect.w).contains(&mousePos) && shouldToggleRightClick) {
						issettingOpen = true;
						selectedModule = mod;
						shouldToggleRightClick = false;
					}
					moduleRect.x += spacing * 2.2f;
					moduleRect.z += spacing * 2.2f;

					if (modRect.z <= moduleRect.z) {
						moduleRect.x = modRect.x + 33;
						moduleRect.z = modRect.x + 320;
						moduleRect.y += spacing + 2.05f;
						moduleRect.w += spacing + 2.05f;
					}

					if (scrollDirection < 0) {
						canScroll = true;
					}
					else if (moduleRect.w > modRect.w)
					{
						canScroll = true;
					}
					//
				}
			}

			if (issettingOpen) {
				auto io = ImGui::GetIO();

				static float Enumlength = 0.f;
				static float textYoff = 0.f;

				std::vector<SettingEntry*>* settings = selectedModule->getSettings();

				std::sort(settings->begin(), settings->end(), [](const auto& a, const auto& b) {
					return a->valueType < b->valueType;
					});

				float yoff = 0;
				ImVec4 settingRect(modRect.x + 30, modRect.y + 80 - settingscrollDirection, modRect.z, modRect.w + 80 - settingscrollDirection);

				if (settingRect.y < modRect.y) {
					settingcanScroll = true;
				}

				for (auto& setting : *settings) {
					if (strcmp(setting->name, "enabled") == 0 || strcmp(setting->name, "keybind") == 0)
						continue;
					switch (setting->valueType)
					{
					case ValueType::FLOAT_T: {
						ImGuiUtils::drawSliderFloat(setting->name, setting->minValue->_float, setting->maxValue->_float, setting->defaultValue->_float, &setting->value->_float, setting->isDragging, ImVec2(settingRect.x, settingRect.y));
						setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();

						settingRect.y += spacing * 0.45f;
						settingRect.w += spacing * 0.45f;
						break;
					}
					case ValueType::INT_T: {
						ImGuiUtils::drawSliderInt(setting->name, setting->minValue->_int, setting->maxValue->_int, setting->defaultValue->_int, &setting->value->_int, setting->isDragging, ImVec2(settingRect.x, settingRect.y));
						setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();

						settingRect.y += spacing * 0.45f;
						settingRect.w += spacing * 0.45f;
						break;
					}
					case ValueType::BOOL_T: {

						float theight = ImGuiUtils::getTextHeight(1.2f) + 6.f;

						float circleX = setting->_toggle;

						float start = setting->value->_bool ? (settingRect.x + 12.4f) : (settingRect.x + 41.6f);
						float end = setting->value->_bool ? (settingRect.x + 41.6f) : (settingRect.x + 12.4f);

						setting->_toggle = static_cast<float>(smoothLerp(end, setting->_toggle, 0.45f));

						circleX = setting->_toggle;

						UIColor boolColor(setting->value->_bool ? UIColor(44, 41, 230, static_cast<int>(255 * percent)) : UIColor(128, 132, 142, static_cast<int>(255 * percent)));

						ImGuiUtils::drawText(Vec2(settingRect.x, settingRect.y), setting->name, UIColor(255, 255, 255, 255 * percent), 1.2f);

						d->AddRectFilled(ImVec2(settingRect.x, settingRect.y + theight), ImVec2(settingRect.x + 55.f, settingRect.y + 25.f + theight), UIColor(boolColor).toImColor(), 50.f, ImDrawFlags_RoundCornersAll);

						d->AddCircleFilled(ImVec2(circleX, settingRect.y + 12.3f + theight), 9.15f, UIColor(255, 255, 255, 255 * percent).toImColor());

						{
							if (Vec4(settingRect.x, settingRect.y + theight, settingRect.x + 55.f, settingRect.y + 25.f + theight).contains(&mousePos) && shouldToggleLeftClick && !shouldToggleRightClick)
							{
								shouldToggleLeftClick = false;
								setting->value->_bool = !setting->value->_bool;
							}
						}

						settingRect.y += spacing * 0.51f;
						settingRect.w += spacing * 0.51f;
						break;
					}
					case ValueType::ENUM_T: {

						int e = 0;

						ImGuiUtils::drawText(Vec2(settingRect.x, settingRect.y + 20.f), setting->name, UIColor(255, 255, 255, 255 * percent), 1.2f);

						Vec4 enumRect(settingRect.x + 250.f, settingRect.y + 10.f, settingRect.x + 490.f, settingRect.y + 65.f);
						Vec4 enumSettingRect(enumRect.x, enumRect.w + 5.f, enumRect.z, enumRect.w + 5.f);

						d->AddRectFilled(ImVec2(enumRect.x, enumRect.y), ImVec2(enumRect.z, enumRect.w), UIColor(30, 30, 30, 255 * percent).toImColor(), 7.f, ImDrawFlags_RoundCornersAll);

						auto enumData = reinterpret_cast<SettingEnum*>(setting->extraData);
						if (extended && auau >= enumSettingRect.w) {
							d->PushClipRect(ImVec2(enumSettingRect.x, enumSettingRect.y), ImVec2(enumSettingRect.z, auau + 52.2f + ImGuiUtils::getTextHeight(1.2f)), true);
							d->AddRectFilled(ImVec2(enumSettingRect.x, enumSettingRect.y), ImVec2(enumSettingRect.z, auau + 52.2f + ImGuiUtils::getTextHeight(1.2f)), UIColor(30, 30, 30, 255 * percent).toImColor(), 7.f, ImDrawFlags_RoundCornersAll);
							d->PopClipRect();
						}

						for (auto it = enumData->Entrys.begin(); it != enumData->Entrys.end(); it++, e++)
						{
							char name[500];
							sprintf_s(name, 500, "%s", enumData->GetEntry(e).GetName().c_str());

							if (extended)
							{
								ImVec2 textPos(enumRect.x + 25.f, enumSettingRect.w + 17.5f);
								d->PushClipRect(ImVec2(enumSettingRect.x, enumSettingRect.y), ImVec2(enumSettingRect.z, auau + 52.2f + ImGuiUtils::getTextHeight(1.2f)), true);

								Vec4 enumHoverRect(textPos.x - 20.f, textPos.y - 12.f, enumSettingRect.z - 6.f, textPos.y + ImGuiUtils::getTextHeight(1.2f) + 20.f);

								if (enumHoverRect.contains(&mousePos)) {
									d->AddRectFilled(ImVec2(enumHoverRect.x, enumHoverRect.y), ImVec2(enumHoverRect.z, enumHoverRect.w), UIColor(34, 34, 39, 255 * percent * 0.7f).toImColor(), 2.f, ImDrawFlags_RoundCornersAll);
									if (shouldToggleLeftClick) {

										setting->value->_int = e;
										extended = false;
										shouldToggleLeftClick = false;
									}
								}

								ImGuiUtils::drawText(Vec2(textPos.x, textPos.y), name, UIColor(255, 255, 255, 255 * percent), 1.2f);
								d->PopClipRect();

								enumSettingRect.w = enumSettingRect.w + 66.f;
								auau = enumSettingRect.w - 2.f;
							}
						}

						if (enumRect.contains(&mousePos) && shouldToggleLeftClick) {
							extended = !extended;
							shouldToggleLeftClick = false;
						}

						char intName[500];
						sprintf_s(intName, 500, "%s", enumData->GetEntry(setting->value->_int).GetName().c_str());

						d->PushClipRect(ImVec2(enumRect.x, enumRect.y), ImVec2(enumRect.z, enumRect.w), true);
						ImGuiUtils::drawText(Vec2(enumRect.x + 15.f, settingRect.y + 24.5f), intName, UIColor(255, 255, 255, 255 * percent), 1.2f);
						d->PopClipRect();

						settingRect.y += spacing * 0.53f;
						settingRect.w += spacing * 0.53f;
						break;
					}
					default: {
						break;
					}
					}

					if (settingscrollDirection < 0) {
						settingcanScroll = true;
					}
					else if (settingRect.w > modRect.w)
					{
						settingcanScroll = true;
					}
				}

				if (shouldToggleLeftClick && back.contains(&mousePos)) {
					selectedModule = nullptr;
					scrollDirection = 0;
					settingscrollDirection = 0;
					issettingOpen = false;
					shouldToggleLeftClick = false;
				}
			}

			//Draw rect auau
			d->AddRectFilled(ImVec2(modRect.x, guiRect.y), ImVec2(guiRect.z, guiRect.y + 64.f), UIColor(28, 28, 31, static_cast<int>(255 * percent)).toImColor(), 20.f, ImDrawFlags_RoundCornersTopRight);
			if (selectedModule != nullptr && issettingOpen) {
				ImGuiUtils::drawText(Vec2(backRect.Min.x + 50.f, backRect.Min.y + 2.f), selectedModule->getModuleName(), UIColor(255, 255, 255, static_cast<int>(255 * percent)), 1.5f);
				ImGuiUtils::drawText(Vec2(backRect.Min.x + 50.f + ImGuiUtils::getTextWidth(selectedModule->getModuleName(), 1.55f) + 40.f, backRect.Min.y + 7.5f), selectedModule->getTooltip(), UIColor(128, 132, 142, static_cast <int>(255 * percent)), 1.05f);
				ImGuiUtils::drawText(Vec2(backRect.Min.x, backRect.Min.y), "<", UIColor(backButtonColor), 1.5f);
			}
			//

			d->PopClipRect();
			shouldReset = false;
			shouldToggleLeftClick = false;
			shouldToggleRightClick = false;
		}
	}
}*/