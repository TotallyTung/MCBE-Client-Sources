#pragma once

// Visual
#include "Visual/Animations.hpp"
#include "Visual/ArrayList.hpp"
#include "Visual/ClickGUI.hpp"
#include "Visual/ServerRotations.hpp"
#include "Visual/Watermark.hpp"

// Player

// Combat
#include "Combat/Killaura.hpp"

// Motion
#include "Motion/Fly.hpp"
#include "Motion/Speed.hpp"
#include "Motion/Velocity.hpp"

// Misc
#include "Misc/Disabler.hpp"
#include "Misc/Regen.hpp"
#include "Misc/Scaffold.hpp"
#include "Misc/TestModule.hpp"
#include "Misc/Timer.hpp"

class ModuleManager {
public:
	static ModuleManager& getInstance() {
		static ModuleManager instance;
		return instance;
	}

	static void SortCategories() 
	{
		mCategories.clear();  // Clear and re-sort categories

		for (const auto& mod : mModules) {
			const std::string& category = mod->getCategory();
			if (std::find(mCategories.begin(), mCategories.end(), category) == mCategories.end()) {
				mCategories.push_back(category);
			}
		}

		std::sort(mCategories.begin(), mCategories.end());
	}

	static void LoadModules() {
		// Visual
		mModules.emplace_back(std::make_shared<Animations>());
		mModules.emplace_back(std::make_shared<ArrayList>());
		mModules.emplace_back(std::make_shared<ClickGUI>());
		mModules.emplace_back(std::make_shared<ServerRotations>());
		mModules.emplace_back(std::make_shared<Watermark>());

		// Player

		// Combat
		mModules.emplace_back(std::make_shared<Killaura>());

		// Motion
		mModules.emplace_back(std::make_shared<Fly>());
		mModules.emplace_back(std::make_shared<Speed>());
		mModules.emplace_back(std::make_shared<Velocity>());

		// Misc
		mModules.emplace_back(std::make_shared<Disabler>());
		mModules.emplace_back(std::make_shared<Regen>());
		mModules.emplace_back(std::make_shared<Scaffold>());
		mModules.emplace_back(std::make_shared<TestModule>());
		mModules.emplace_back(std::make_shared<Timer>());
	}

	static void InitializeModules() {
		LoadModules();
		SortCategories();
		ModernGUI::sortCategory();
	}

	static void UninitializeMods() {
		for (const auto& mod : mModules) {
			if (mod->isEnabled()) {
				mod->toggle();
			}
		}

		mModules.clear();
	}

	const std::vector<std::shared_ptr<Module>>& getModules() const { return mModules; }
	const std::vector<std::string>& getCategories() const { return mCategories; }
};