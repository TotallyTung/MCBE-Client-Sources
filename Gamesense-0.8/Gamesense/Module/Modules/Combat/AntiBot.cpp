#include "AntiBot.h"

using namespace std;
AntiBot::AntiBot() : IModule(0, Category::EXPLOIT, "Ignores anticheat generated NPCs")
{
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Basic", 0);
	mode.addEntry("Advanced", 1);
	registerBoolSetting("Invisible Check", &invisibleCheck, invisibleCheck, "Checks if the entity is vanished");
	registerBoolSetting("EntityID Check", &entityIdCheck, entityIdCheck, "Checks the entities ID");
	registerBoolSetting("Hitbox Check", &hitboxCheck, hitboxCheck, "Checks if the hitbox is normal");
	registerBoolSetting("Other Check", &otherCheck, otherCheck, "(Advanced only) Checks if the entity is(Flying, swimming and all that stuff)");
	registerBoolSetting("Extra Check", &extraCheck, extraCheck, "(Advanceed only) Does some extra checkings");
}

const char *AntiBot::getRawModuleName()
{
	return "AntiBot";
}

const char *AntiBot::getModuleName()
{
	name = string("AntiBot ") + string(GRAY) + string("Advanced") + string(GRAY);
	return name.c_str();
}
