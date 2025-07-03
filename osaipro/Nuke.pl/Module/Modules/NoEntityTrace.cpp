#include "NoEntityTrace.h"

NoEntityTrace::NoEntityTrace() : IModule(0, Category::VISUAL, "Lets you place and break behind players and mobs") {
	registerBoolSetting("No Weapon", &this->noWeapon, this->noWeapon);
	registerBoolSetting("PickaxeOnly", &this->pickOnly, this->pickOnly);
}

NoEntityTrace::~NoEntityTrace() {
}

const char *NoEntityTrace::getModuleName() {
	return ("NoEntityTrace");
}

static std::vector<Entity*> playerList2;

void findEntityButCooler(Entity *currentEntity, bool isRegularEntity)
{
	static auto noEntityTraceMod = moduleMgr->getModule<NoEntityTrace>();
	if (currentEntity == nullptr)
		return;

	if (currentEntity == Game.getLocalPlayer()) // Skip Local player
		return;

	if (!Game.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!Game.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (noEntityTraceMod->allMobs)
	{
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->getEntityTypeId() == 64) // item
			return;
		if (currentEntity->getEntityTypeId() == 69) // xp_orb
			return;
		if (currentEntity->getEntityTypeId() == 80) // arrow
			return;
	}
	else
	{
		if (currentEntity->getEntityTypeId() != 63)

			return;
	}
	playerList2.push_back(currentEntity);
}

void NoEntityTrace::onEnable()
{
	if (Game.getLocalPlayer() == nullptr)
		return;
}

void NoEntityTrace::onDisable()
{
	if (Game.getLocalPlayer() == nullptr)
		return;

	if (!playerList2.empty())
	{
		for (auto &i : playerList2)
		{
			i->height = 1.8f;
			i->width = 0.6f;
		}
	}
}

void NoEntityTrace::onTick(GameMode *gm)
{
	playerList2.clear();
	LocalPlayer *localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr)
		return;
	Game.forEachEntity(findEntityButCooler);

	if (playerList2.empty())
	{
		return;
	}
	auto selectedItemId = localPlayer->getSelectedItemId();
	if (noWeapon && (selectedItemId == 308 || selectedItemId == 307 || selectedItemId == 312 || selectedItemId == 316 || selectedItemId == 322 || selectedItemId == 592 || selectedItemId == 536 /*swords*/
					 || selectedItemId == 311 || selectedItemId == 315 || selectedItemId == 319 || selectedItemId == 325 || selectedItemId == 298 || selectedItemId == 595 /*axes*/))
	{

		for (auto &i : playerList2)
		{
			i->height = 1.8f;
			i->width = 0.6f;
		}
	}
	else
	{
		for (auto &i : playerList2)
		{
			i->height = 0.f;
			i->width = 0.f;
		}
	}
	if (pickOnly && (selectedItemId == 310 || selectedItemId == 314 || selectedItemId == 297 || selectedItemId == 318 || selectedItemId == 324))
	{

		for (auto &i : playerList2)
		{
			i->height = 0.f;
			i->width = 0.f;
		}
	}
	else
	{
		for (auto &i : playerList2)
		{
			i->height = 1.8f;
			i->width = 0.6f;
		}
	}
}

void NoEntityTrace::onJoiningServer() {
	playerList2.clear();
}