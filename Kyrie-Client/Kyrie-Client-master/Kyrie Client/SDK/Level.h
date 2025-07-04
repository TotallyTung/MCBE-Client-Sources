#pragma once
#include <string>
#include "../Utils/Utils.h"
class LevelSettings;
class LevelData;
class Experiments;

class Actor;

class Level {
public:

	class std::vector<Actor*> getRuntimeActorList() {
		return Utils::CallVFunc<297, class std::vector<Actor*>>(this);
	}

};


