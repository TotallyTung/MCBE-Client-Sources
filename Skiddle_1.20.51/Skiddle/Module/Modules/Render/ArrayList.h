#pragma once
#include <algorithm>

class ArrayList : public Module
{
public:
	ArrayList(int keybind = Keyboard::NONE, bool enabled = false) :
		Module("ArrayList", "Render", "Display enabled modules in a list", keybind, enabled)
	{
		registerEnumSetting("Style", "Change the look of the arraylist", { "Outline", "Flux", "Simple", "None" }, &style);
		registerEnumSetting("Animation", "Animation Mode", { "Normal","Expo","Back" }, &animationMode);
		registerFloatSetting("Glow Amount", "How thick the glow will be", &shadowIterations, 100.f, 300.f);
		registerFloatSetting("Glow Opacity", "The opacity ye idk", &shadowOpacity, 0.f, 1.f);
		registerFloatSetting("Offset", "why isnt this in iris?", &offset, 0.f, 50.f);
		registerFloatSetting("Size", "size", &sizeee, 1.f, 2.f);
		registerFloatSetting("Opacity", "background opacity", &opacity, 0.f, 1.f);
		registerBoolSetting("Mode", "None", &mode);
		registerBoolSetting("Lowercase", "None", &lowercase);
	}

	bool lowercase = false;
	bool mode = true;
	bool orientation = false;
	bool fancy = false;
	int style = 0;
	int animationMode = 0;

	float shadowIterations = 100;
	float shadowOpacity = 0.5f;
	float sizeee = 1.4f;
	float offset = 4.f;
	float opacity = 0.6f;

	void onEvent(UpdateEvent* event) {
		if (mode) {
			Game::Skiddle::showArrayMode = true;
		}
		else {
			Game::Skiddle::showArrayMode = false;
		}
	}

	void onEvent(ImGUIRenderEvent* event) override
	{
		ImDrawList* d = ImGui::GetBackgroundDrawList();
		//if (getModuleByName("clickgui")->isEnabled() or !d) return;
		if (!Game::Core::showMenu) return;

		// Create a copy of the modules list
		static size_t previousSize;
		static std::vector<Module*> sortedModules;
		static std::vector<Module*> currentModules;

		for (auto mod : modules)
		{
			// Hide modules that aren't enabled or visible
			if (!mod->isEnabled() && mod->animPercentage <= 0.01 || !mod->isVisible()) continue;
			sortedModules.push_back(mod);
		}

		// Sort the modules if the vector size has changed (this is so scuffed lmao)
		if (sortedModules.size() != previousSize)
		{
			sortModules(sortedModules);
			currentModules.clear();
			currentModules = sortedModules;
			previousSize = sortedModules.size();
		}

		// Define the textSize and module index
		const float textSize = sizeee;
		const float textHeight = ImRenderUtil::getTextHeight(textSize);
		const float padding = 1.5f * textSize;
		int index = 0;

		Vector4<float> underline = Vector4<float>();
		float textWid = 0;
		float easeAnim = 0;
		float lineWidth = 2;
		float textPosY = offset;
		std::vector<std::tuple<Vector4<float>, std::string, std::string, Vector2<float>, float, float>> sexDraw;
		for (Module* mod : currentModules)
		{
			float targetAnim = mod->isEnabled() ? 1.f : 0.f;
			mod->animPercentage = Math::animate(targetAnim, mod->animPercentage, ImRenderUtil::getDeltaTime() * (animationMode == 0 ? 10 : 6));
			mod->animPercentage = Math::clamp(mod->animPercentage, 0.f, 1.f);
			if (animationMode == 0) {
				easeAnim = mod->animPercentage;
			}
			else if (animationMode == 1) {
				easeAnim = easeInOutExpo(mod->animPercentage);
			}
			else if (animationMode == 2) {
				easeAnim = easeOutBack(mod->animPercentage);
			}

			std::string modName = mod->getName();
			std::string modMode = mod->getModeName();
			float modWidth = ImRenderUtil::getTextWidth(&modName, textSize);
			float modeWidth = !modMode.empty() ? ImRenderUtil::getTextWidth(&modMode, textSize) : 0.f;
			
			float modulePosX;
			if (mode) {
				modulePosX = ImGui::GetIO().DisplaySize.x - modWidth - modeWidth - padding - (style == 1 ? 3 : 0);
			}
			else {
				modulePosX = ImGui::GetIO().DisplaySize.x - modWidth - padding - (style == 1 ? 3 : 0);
			}
			mod->pos.x = Math::lerp(ImGui::GetIO().DisplaySize.x + 5.f, modulePosX, easeAnim) - offset;
			mod->pos.y = textPosY;

			UIColor arrayColor = ColorUtil::getClientColor(2, 1, 1, index * 80);
			Vector4<float> boxRect(mod->pos.x - padding - 2.f, textPosY, ImRenderUtil::getScreenSize().x - offset, textPosY + textHeight);
			Vector4<float> rightSide(boxRect.x + modWidth + modeWidth + 1.f, boxRect.y, boxRect.x + modWidth + modeWidth + lineWidth, boxRect.w);
			Vector4<float> rightSide2(ImRenderUtil::getScreenSize().x - offset, boxRect.y, ImRenderUtil::getScreenSize().x - offset + lineWidth, boxRect.w);
			Vector4<float> leftSide(boxRect.x - lineWidth, boxRect.y, boxRect.x, boxRect.w + lineWidth - 2);
			if (style == 1) {
				rightSide = Vector4<float>(ImRenderUtil::getScreenSize().x - offset - 3, boxRect.y, ImRenderUtil::getScreenSize().x - offset, boxRect.w + 1);
			}
			underline = Vector4<float>(boxRect.z - textWid, boxRect.y - lineWidth, boxRect.x, boxRect.y);

			if (style < 3)ImRenderUtil::fillRectangle(boxRect, UIColor(0, 0, 0), easeAnim * opacity);

			switch (style)
			{
			case 0:
				ImRenderUtil::fillRectangle(leftSide, arrayColor, easeAnim);
				ImRenderUtil::fillRectangle(underline, arrayColor, 1.f);
				ImRenderUtil::fillRectangle(rightSide2, arrayColor, easeAnim, 1.f);
				break;
			case 1:
				ImRenderUtil::fillRectangle(rightSide, arrayColor, easeAnim, 3.f);
				break;
			}

			sexDraw.push_back(std::make_tuple(boxRect, modName, modMode, Vector2<float>(mod->pos.x, mod->pos.y), modWidth, easeAnim));
			textWid = boxRect.z - boxRect.x + 1.f;
			textPosY = Math::lerp(textPosY, textPosY + textHeight, easeAnim);
			underline = Vector4<float>(boxRect.x, boxRect.w, boxRect.z + lineWidth, boxRect.w + lineWidth);

			++index;
		}
		if (!style) ImRenderUtil::fillRectangle(Vector4<float>(underline), ColorUtil::getClientColor(2, 1, 1, index * 80), easeAnim);

		int colorIndex = 0;
		for (const auto& pos : sexDraw) {
			Vector4<float> boxPos = std::get<0>(pos);
			std::string modName = std::get<1>(pos);
			std::string modMode = std::get<2>(pos);
			Vector2<float> modPos = std::get<3>(pos);
			float modWidth = std::get<4>(pos);
			float anim = std::get<5>(pos);
			modPos.y -= 2;
			UIColor arrayColor = ColorUtil::getClientColor(2, 1, 1, colorIndex * 80);

			ImRenderUtil::fillShadowRectangle(boxPos, UIColor(0, 0, 0), anim * shadowOpacity, shadowIterations, 0);


			if (lowercase) {
				ImRenderUtil::toLower(modName);
				ImRenderUtil::toLower(modMode);
			}

			if (mode) {
				ImRenderUtil::drawText(modPos, &modName, arrayColor, textSize, anim, true);
				ImRenderUtil::drawText(Vector2<float>(modPos.x + modWidth, modPos.y), &modMode, UIColor(255, 255, 255), textSize, anim, true);
			}
			else {
				ImRenderUtil::drawText(modPos, &modName, arrayColor, textSize, anim, true);
			}
			colorIndex++;
		}

		sortedModules.clear();
	}

private:
	inline float easeInOutExpo(float x) {
		return x == 0
			? 0
			: x == 1
			? 1
			: x < 0.5 ? pow(2, 20 * x - 10) / 2
			: (2 - pow(2, -20 * x + 10)) / 2;
	};
	static inline float easeOutBack(float x) {
		float c1 = 1.70158;
		float c3 = c1 + 1;

		return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
	};
	static inline void sortModules(std::vector<Module*>& sortedModules)
	{
		std::sort(sortedModules.begin(), sortedModules.end(), [](Module* a, Module* b) {
			std::string nameA;
			std::string nameB;

			if (Game::Skiddle::showArrayMode) {
				nameA = a->getName() + a->getModeName();
				nameB = b->getName() + b->getModeName();
			}
			else {
				nameA = a->getName();
				nameB = b->getName();
			}
			float textSizeA = ImRenderUtil::getTextWidth(&nameA, 1.2f);
			float textSizeB = ImRenderUtil::getTextWidth(&nameB, 1.2f);
			return textSizeA > textSizeB;
			});
	}

};