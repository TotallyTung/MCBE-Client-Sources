#pragma once

#include "PermissionsHandler.h"
#include "Abilities.h"
#include "LayeredAbilities.h"
#include "HitResult.h"

class Level { // Level VTable
public:
	std::vector<Actor*> getRuntimeActorList() {
		std::vector<Actor*> mActors;
		//Memory::CallVFunc<308, decltype(&mActors)>(this, &mActors); // 1.21.41
		Memory::CallFastcall<void>(SigManager::Level_getRuntimeActorList, this, &mActors);
		return mActors;
	}

	HitResult* getHitResult() {
		return Memory::CallVirtualFunc<HitResult*>(OffsetManager::Level_getHitResult, this);
	}
};