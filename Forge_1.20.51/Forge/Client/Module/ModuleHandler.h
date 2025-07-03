#pragma once

#pragma region Module imports

//Combat;
#include "Modules/Velocity.h"

// Movement;
#include "Modules/Jetpack.h"

// Visual;
#include "Modules/ArrayList.h"
#include "Modules/Interface.h"
#include "Modules/Ambience.h"
#include "Modules/Animations.h"

//Misc;

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
	// Combat;
	modules.push_back(new Velocity(Keyboard::NONE, true));

	// Movement;
	modules.push_back(new Jetpack(Keyboard::F, false));
	
	// Visual;
	modules.push_back(new ArrayList(Keyboard::NONE, true));
	modules.push_back(new Interface(Keyboard::NONE, true));
	modules.push_back(new Ambience(Keyboard::NONE, true));
	modules.push_back(new Animations(Keyboard::NONE, true));
	//modules.push_back(new Interface(Keyboard::NONE, true));

	// Misc;
	modules.push_back(new Module("AntiBot", "Misc", "No more bots :)", Keyboard::NONE, true));
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