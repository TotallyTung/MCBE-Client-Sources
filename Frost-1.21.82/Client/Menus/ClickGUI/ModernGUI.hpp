#pragma once
#include "Manager/ClickGUIManager.hpp"

class ModernGUI {
public:
	enum class Tabs {
		Search,
		
		Scripting
	};

	void render(float animation, float inScale, int& scrollDirection) noexcept {
		const float guiWidth = 480.f;
		const float guiHeight = 315.f;
		const float catOffset = 280.f;

		const Vector2<float> guiPos = ImRenderUtil::getScreenSize() / 2;

		Vector4<float> baseRect(guiPos.x - guiWidth, guiPos.y - guiHeight, guiPos.x + guiWidth, guiPos.y + guiHeight);
		Vector4<float> catRect(baseRect.x, baseRect.y, baseRect.x + catOffset, baseRect.w);
		Vector4<float> mainRect(baseRect.x + catOffset, baseRect.y, baseRect.z, baseRect.w);

		static float scrollEased = 0, scroll = 0;
		bool isEnabled = getModuleByName("clickgui")->isEnabled();
		scrollEased = MathUtil::animate(scroll, scrollEased, ImRenderUtil::getDeltaTime() * 10.f);

		handleScrolling(mainRect, scrollDirection, scroll);

		static std::string currentCategory = "Visual";
		static std::vector<std::shared_ptr<Module>> modList = ClickGUIManager::getModulesInCategory(currentCategory, mModules);

		// Background Panels
		ImRenderUtil::fillRectangle(catRect, Color(22, 22, 24), inScale);
		ImRenderUtil::fillRectangle(mainRect, Color(16, 16, 18), inScale);
	    //ImRenderUtil::fillGradient(mainRect, Color(20, 20, 24), Color(10, 10, 12), true, inScale * 0.5f);

		// Render Client Title
		drawTitle(catRect, animation);

		// Render Categories
		drawCategories(catRect, currentCategory, modList, scroll, animation);

		// Render Modules List
		drawModules(mainRect, modList, scrollEased, isEnabled, animation);

		// Release Mouse If GUI Is Open
		if (isEnabled) {
			InstanceManager::get<ClientInstance>()->releaseMouse();
		}
	}

public:
	static void sortCategory() {
		mFrostGUICategories.push_back("Search");
		mFrostGUICategories.insert(mFrostGUICategories.end(), mCategories.begin(), mCategories.end());
		mFrostGUICategories.push_back("Scripting");
	}

	void handleScrolling(const Vector4<float>& mainRect, int& scrollDirection, float& scroll) {
		if (ImRenderUtil::isMouseOver(mainRect)) {
			scroll += scrollDirection * 20;
			if (scroll < 0) scroll = 0;
			scrollDirection = 0;
		}
	}

	void drawTitle(const Vector4<float>& catRect, float animation) {
		static std::string clientName = "Frost";
		ImRenderUtil::fillShadowCircle(Vector2<float>(catRect.x + 25.f + ImRenderUtil::getTextWidth(&clientName, 2.5f) / 2,
			catRect.y + 15.f + ImRenderUtil::getTextHeight(2.5f) / 2), 1.f, Color(255, 255, 255), animation * 0.7F, 100, 0);
		ImRenderUtil::drawText(Vector2<float>(catRect.x + 25.f, catRect.y + 15.f), &clientName, Color(255, 255, 255), 2.5f, animation, true);
	}

	void drawCategories(Vector4<float>& catRect, std::string& currentCategory, std::vector<std::shared_ptr<Module>>& modList, float& scroll, float animation) {
		int ind = 0;
		for (std::string category : mFrostGUICategories) {
			Vector2<float> textPosition(catRect.x + 25.f, catRect.y + 90.f + ind);
			Vector4<float> catTab(textPosition.x - 7.f, textPosition.y - 7, textPosition.x + ImRenderUtil::getTextWidth(&category, 1.2f) + 32.f, textPosition.y + ImRenderUtil::getTextHeight(1.2f) + 7);

			if (ImRenderUtil::isMouseOver(catTab) && Util::mLeftClick) {
				Util::mLeftClick = false;
				currentCategory = category;
				modList = currentCategory == "Search" ? mModules : ClickGUIManager::getModulesInCategory(currentCategory, mModules);
				scroll = 0;
			}

			if (category == currentCategory) {
				ImRenderUtil::fillShadowRectangle(catTab.scale(2), ColorUtil::getClientColor(5, 1, 1, 0), animation * 0.5f, 40.f, 0, 10);
				ImRenderUtil::fillRectangle(catTab, ColorUtil::getClientColor(5, 1, 1, 0), animation, 8.f);
			}

			std::string symbol = getCategorySymbol(category);
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
			ImRenderUtil::drawText(Vector2<float>(textPosition.x, textPosition.y), &symbol, Color(255, 255, 255), 1.f, animation, false);
			ImGui::PopFont();

			ImRenderUtil::drawText(Vector2<float>(textPosition.x + 25.f, textPosition.y), &category, Color(255, 255, 255), 1.2f, animation, false);
			ind += (ImRenderUtil::getTextHeight(1.2f) + 20);
		}
	}

	void drawModules(Vector4<float>& mainRect, std::vector<std::shared_ptr<Module>>& modList, float scrollEased, bool isEnabled, float animation) {
		float modInd = -scrollEased;
		ImRect clipRect(mainRect.x, mainRect.y, mainRect.z, mainRect.w);
		ImGui::GetBackgroundDrawList()->PushClipRect(clipRect.Min, clipRect.Max, true);

		for (auto mod : modList) {
			float mSettingsHeight = 0;

			float mTargetAnim = mod->mSettingsExtended ? 1.f : 0.f;
			mod->mClickGUIAnim = MathUtil::animate(mTargetAnim, mod->mClickGUIAnim, ImRenderUtil::getDeltaTime() * 15.5);
			mod->mClickGUIAnim = MathUtil::clamp(mod->mClickGUIAnim, 0.f, 1.f);

			for (const auto& setting : mod->getSettings()) {
				if (!setting->shouldRender())
					continue;

				switch (setting->getType())
				{
				case SettingType::BOOL: {
					mSettingsHeight = MathUtil::lerp(mSettingsHeight, mSettingsHeight + 60, mod->mClickGUIAnim);
					break;
				}
				case SettingType::ENUM: {
					mSettingsHeight = MathUtil::lerp(mSettingsHeight, mSettingsHeight + 60, mod->mClickGUIAnim);
					break;
				}
				case SettingType::SLIDER: {
					mSettingsHeight = MathUtil::lerp(mSettingsHeight, mSettingsHeight + 60.f, mod->mClickGUIAnim);
					break;
				}
				}
			}

			Vector2<float> modulePosition(mainRect.x + 15.f, mainRect.y + 10.f + modInd);
			Vector4<float> moduleRect(modulePosition.x, modulePosition.y, mainRect.z - 15.f, (modulePosition.y + 65.f) + mSettingsHeight);

			ImRenderUtil::fillRectangle(moduleRect, Color(35, 35, 40), 1.0f, 10);

			handleModuleInteraction(moduleRect, mod, isEnabled);

			std::string mModuleName = mod->getName();
			std::string mModuleDescription = mod->getDescription();

			ImRenderUtil::drawText(Vector2<float>(moduleRect.x + 10.f, moduleRect.y + 8.f), &mModuleName, mod->isEnabled() ? ColorUtil::getClientColor(5, 1, 1, modInd * 2) : Color(255, 255, 255), 1.4f, animation, true);
			ImRenderUtil::drawText(Vector2<float>(moduleRect.x + 10.f, moduleRect.y + 38.f), &mModuleDescription, Color(160, 160, 160), 1.f, animation, true);

			modInd += 75.f;
			modInd += mSettingsHeight;
		}

		ImGui::GetBackgroundDrawList()->PopClipRect();
	}

	void drawScripting() { // TODO:

	}

	void handleModuleInteraction(const Vector4<float>& moduleRect, std::shared_ptr<Module>& mod, bool isEnabled) {
		if (ImRenderUtil::isMouseOver(moduleRect) && isEnabled) {
			if (Util::mLeftClick) {
				mod->toggle();
				Util::mLeftClick = false;
			}
			else if (Util::mRightClick) {
				mod->mSettingsExtended = !mod->mSettingsExtended;
				Util::mRightClick = false;
			}
		}
	}

	std::string getCategorySymbol(const std::string& category) {
		static std::unordered_map<std::string, std::string> categorySymbols = {
			{"Search", "B"}, {"Visual", "d"}, {"Combat", "c"}, {"Misc", "a"}, {"Motion", "f"}, {"Scripting", ""}
		};
		return categorySymbols.count(category) ? categorySymbols[category] : "";
	}
};
