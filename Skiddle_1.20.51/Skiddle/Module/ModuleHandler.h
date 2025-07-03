 #pragma once

#pragma region Module imports
// Combat
#include "Modules/Combat/InfiniteAura.h"
#include "Modules/Combat/Velocity.h"
#include "Modules/Combat/AntiBot.h"
#include "Modules/Combat/Reach.h"
#include "Modules/Combat/Regen.h"
#include "Modules/Combat/Aura.h"

// Render
#include "Modules/Render/Notifications.h"
#include "Modules/Render/HitEffects.h"
#include "Modules/Render/TargetInfo.h"
#include "Modules/Render/Animations.h"
#include "Modules/Render/ArrayList.h"
#include "Modules/Render/HUD.h"
#include "Modules/Render/Ambience.h"
#include "Modules/Render/ClickGUI/ClickGUI.h"
#include "Modules/Render/Nametags.h"
#include "Modules/Render/Trail.h"
#include "Modules/Render/ESP.h"
#include "Modules/Render/Watermark.h"
#include "Modules/Render/VirtualReality.h"

// Movement
#include "Modules/Movement/FastStop.h"
#include "Modules/Movement/Sprint.h"
#include "Modules/Movement/Speed.h"
#include "Modules/Movement/Step.h"
#include "Modules/Movement/Flight.h" 

// Player
#include "Modules/Player/SpeedMine.h"
#include "Modules/Player/Scaffold.h"
#include "Modules/Player/Manager.h"
#include "Modules/Player/Stealer.h"
#include "Modules/Player/Glide.h"
#include "Modules/Player/Timer.h"

// Misc
#include "Modules/Misc/ToggleSounds.h"
#include "Modules/Misc/ChatSuffix.h"
#include "Modules/Misc/AutoQueue.h"
#include "Modules/Misc/Disabler.h"
#include "Modules/Misc/Breaker.h"
#include "Modules/Misc/Insults.h"
#include "Modules/Misc/AutoGG.h"
#pragma endregion

void SortCategories()
{
	for (Module* mod : modules)
	{
		if (std::find(categories.begin(), categories.end(), mod->getCategory()) == categories.end())
			categories.push_back(mod->getCategory());
	}
}

void InitializeModules()
{
	// Combat
	modules.push_back(new InfiniteAura(Keyboard::NONE));
	modules.push_back(new Velocity(Keyboard::NONE));
	modules.push_back(new AntiBot(Keyboard::NONE, true));
	modules.push_back(new Reach(Keyboard::NONE));
	modules.push_back(new Regen(Keyboard::NONE));
	modules.push_back(new Aura(Keyboard::NONE));

	// Render
	modules.push_back(new Notifications(Keyboard::NONE));
	modules.push_back(new HitEffects(Keyboard::NONE));
	modules.push_back(new TargetInfo(Keyboard::NONE));
	modules.push_back(new Animations(Keyboard::NONE));
	modules.push_back(new ArrayList(Keyboard::NONE, true));
	modules.push_back(new HUD(Keyboard::NONE, true));
	modules.push_back(new Ambience(Keyboard::NONE));
	modules.push_back(new ClickGUI(Keyboard::INSERT));
	modules.push_back(new Nametags(Keyboard::NONE));
	modules.push_back(new Trail(Keyboard::NONE));
	modules.push_back(new ESP(Keyboard::NONE));
	modules.push_back(new Watermark(Keyboard::NONE));
	modules.push_back(new VirtualReality(Keyboard::NONE));

	// Movement
	modules.push_back(new FastStop(Keyboard::NONE));
	modules.push_back(new Sprint(Keyboard::NONE));
	modules.push_back(new Speed(Keyboard::NONE));
	modules.push_back(new Step(Keyboard::NONE));
	modules.push_back(new Flight(Keyboard::NONE));

	// Player
	modules.push_back(new SpeedMine(Keyboard::NONE));
	modules.push_back(new Scaffold(Keyboard::NONE));
	modules.push_back(new Manager(Keyboard::NONE));
	modules.push_back(new Stealer(Keyboard::NONE));
	modules.push_back(new Glide(Keyboard::NONE));
	modules.push_back(new Timer(Keyboard::NONE));

	// Other
	modules.push_back(new ToggleSounds(Keyboard::NONE));
	modules.push_back(new ChatSuffix(Keyboard::NONE));
	modules.push_back(new AutoQueue(Keyboard::NONE));
	modules.push_back(new Disabler(Keyboard::NONE));
	modules.push_back(new Breaker(Keyboard::NONE));
	modules.push_back(new Insults(Keyboard::NONE));
	modules.push_back(new AutoGG(Keyboard::NONE));
}

void InitializeMods()
{
	InitializeModules();

	SortCategories(); // create categories
}

void UninitializeMods()
{
	for (auto mod : modules)
	{
		if (mod->isEnabled())
			mod->toggle();

		delete mod;
	}
}