#include "Killaura.h"
bool showRots;
using namespace std;
Killaura::Killaura() : IModule(0, Category::COMBAT, "Automatically attack the nearest entities")
{
	registerEnumSetting("Rotations", &rotations, 0, "Controls the clients rotations");
	rotations.addEntry("None", 0);
	rotations.addEntry("Silent", 1); // Silently rotate serversided
	rotations.addEntry("Client", 2); // ClientSided roation only
	rotations.addEntry("Actual", 3); // Do strafing things
	rotations.addEntry("Horion", 4); // Do strafing things

	registerEnumSetting("Mode", &mode, 0, "Controls if the client should attack multiple enemies at once or just one enemy");
	mode.addEntry("Single", 0);
	mode.addEntry("Multi", 1);

	registerEnumSetting("Switch", &AutoWeapon, 0, "Controls how the client should switch to a weapon");
	AutoWeapon.addEntry("None", 0);
	AutoWeapon.addEntry("Regular", 1);
	AutoWeapon.addEntry("NukkitSpoof", 2);
	AutoWeapon.addEntry("NukkitSmart", 3); // Switches to trident when on water :0000

	registerEnumSetting("Main Weapon", &mainW, 0, "What weapon should switch use?, use all to switch to the weapon with most damage");
	mainW.addEntry("Sword", 0);
	mainW.addEntry("Trident", 1);
	mainW.addEntry("All", 2);

	registerEnumSetting("Timer", &timer, 0, "Should use timer");
	timer.addEntry("None", 0);
	timer.addEntry("Nukkit", 1);
	timer.addEntry("Vanilla", 2);

	renderType = SettingEnum(this)
					 .addEntry(EnumEntry("Default", 0))
					 .addEntry(EnumEntry("Strip", 1))
					 .addEntry(EnumEntry("ESP", 4))
					 .addEntry(EnumEntry("None", 5));
	registerEnumSetting("Render", &this->renderType, 0, "Controls how the client should render stuff");

	registerBoolSetting("Attack All", &mobs, mobs, "Attack all entities"); // "Mobs" doesnt just attack mobs it attaks arrows, exp and all that shit
	registerBoolSetting("Hold", &hold, hold, "Only attack when holding left click");
	registerBoolSetting("Show Rotations", &showRots, showRots, "Make rotations show client sided");
	registerFloatSetting("Range", &range, range, 3.f, 20.f, 0.1f, "The range where the client will try to attack the target");
	registerIntSetting("Delay", &delayka, delayka, 0, 20, "The delay between each attacks");
	registerFloatSetting("Vanilla TPS", &tps, tps, 20.f, 500.f, 0.1f, "How fast should timer be");
	registerBoolSetting("Swing", &swingKA, swingKA, "Should swing");
	registerBoolSetting("HurtTime", &hurttimeka, hurttimeka, "Takes hit cooldown into account");
}

void Killaura::findWeapon()
{
	C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory *inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack *stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage && stack->getItem()->itemId == 316 && mainW.getSelectedValue() == 0)
			{
				damage = currentDamage;
				slot = n;
			}
			if (currentDamage > damage && stack->getItem()->itemId == 546 && mainW.getSelectedValue() == 1)
			{
				damage = currentDamage;
				slot = n;
			}
			if (currentDamage > damage && mainW.getSelectedValue() == 2)
			{
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}
void Killaura::findWeaponSmart()
{
	C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory *inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 36; n++)
	{
		C_ItemStack *stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage && stack->getItem()->itemId == 546)
			{
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

const char *Killaura::getRawModuleName()
{
	return "Aura";
}

struct CompareTargetEnArray
{
	bool operator()(C_Entity *lhs, C_Entity *rhs)
	{
		C_LocalPlayer *localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

static vector<C_Entity *> targetList;
void findEntity(C_Entity *currentEntity, bool isRegularEntity)
{
	static auto killaura = moduleMgr->getModule<Killaura>();
	static auto antiDesync = moduleMgr->getModule<AntiDesync>();
	if (antiDesync->mode.getSelectedValue() == 0 && antiDesync->isEnabled() && g_Data.isRightClickDown())
		return;

	if (currentEntity == nullptr)
		return;
	if (currentEntity == g_Data.getLocalPlayer())
		return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;
	if (!g_Data.getLocalPlayer()->isAlive())
		return;
	if (!currentEntity->isAlive())
		return;
	if (currentEntity->getEntityTypeId() == 66) // falling block
		return;
	if (currentEntity->getEntityTypeId() == 69) // XP
		return;
	if (currentEntity->getEntityTypeId() == 64) // item
		return;
	if (currentEntity->getEntityTypeId() == 80 || currentEntity->getEntityTypeId() == 69)
		return;

	if (killaura->mobs)
	{
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 319)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)
			return;
		// if (currentEntity->getEntityTypeId() == 64) return;
		if (currentEntity->getEntityTypeId() == 0xC00050) // Arrows - new item id
			return;
		if (currentEntity->getEntityTypeId() == 307) // NPC
			return;
	}
	else
	{
		if (!TargetUtil::isValidTarget(currentEntity))
			return;
		if (currentEntity->getEntityTypeId() == 51 || currentEntity->getEntityTypeId() == 1677999)
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < killaura->range)
	{
		targetList.push_back(currentEntity);
		sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	}
}
const char *Killaura::getModuleName()
{
	return "";
}

const char* Killaura::getModName()
{
	switch (mode.getSelectedValue())
	{
	case 0:
		name = " Single";
		break;
	case 1:
		name = " Multi";
		break;
	}
	return name;
}

void Killaura::onEnable()
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	targetListEmpty = true;
	targetList.clear();
}

void Killaura::onTick(C_GameMode *gm)
{
	auto player = g_Data.getLocalPlayer();
	int dbrr = g_Data.getLocalPlayer()->getSelectedItemId();
	if (player == nullptr)
		return;

	PointingStruct *pointing = g_Data.getLocalPlayer()->pointingStruct;
	C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();
	targetListEmpty = targetList.empty();

	targetList.clear();

	g_Data.forEachEntity(findEntity);

	Odelay++;
	slotKA = supplies->selectedHotbarSlot;
	slotKASmart = supplies->selectedHotbarSlot;
	if (timer.getSelectedValue() == 2)
	{
		g_Data.getClientInstance()->minecraft->setTimerSpeed(tps);
	}
	if (timer.getSelectedValue() == 1)
	{
		g_Data.getClientInstance()->minecraft->setTimerSpeed(26.f);
	}
	if (hold && !g_Data.isLeftClickDown())
		return;
	// Normal
	if (AutoWeapon.getSelectedValue() == 1, 2 && !targetList.empty() && AutoWeapon.getSelectedValue() != 0)
		findWeapon();
	// If in water
	if (AutoWeapon.getSelectedValue() == 3 && !targetList.empty() && gm->player->isInWater() && AutoWeapon.getSelectedValue() != 0)
		findWeaponSmart();
	// If in air
	if (AutoWeapon.getSelectedValue() == 3 && !targetList.empty() && !gm->player->isInWater() && AutoWeapon.getSelectedValue() != 0)
		findWeapon();
	if (!targetList.empty() && Odelay >= delayka)
	{
		if (mode.getSelectedValue() == 0)
		{
			for (auto &i : targetList)
			{
				if (!(i->damageTime > 1 && hurttimeka))
				{
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					if (swingKA)
					{
						g_Data.getLocalPlayer()->swingArm();
						player->swing();
					}
				}
			}
		}
		if (mode.getSelectedValue() == 1)
		{
			for (auto &i2 : targetList)
			{
				if (!(i2->damageTime > 1 && hurttimeka))
				{
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					if (swingKA)
					{
						g_Data.getLocalPlayer()->swingArm();
						player->swing();
					}
				}
			}
		}
		if (AutoWeapon.getSelectedValue() == 2)
			supplies->selectedHotbarSlot = slotKA;
		// if in water then it selects trident
		if (AutoWeapon.getSelectedValue() == 3 && gm->player->isInWater())
			supplies->selectedHotbarSlot = slotKASmart;
		if (AutoWeapon.getSelectedValue() == 3 && !gm->player->isInWater())
			supplies->selectedHotbarSlot = slotKA;
		if (rotations.getSelectedValue() == 3)
		{
			tro = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		}
		Odelay = 0;
	}
}

void Killaura::onPlayerTick(C_Player *plr)
{
	static auto antiDesync = moduleMgr->getModule<AntiDesync>();
	if (plr == nullptr)
		return;
	auto player = g_Data.getLocalPlayer();
	if (rotations.getSelectedValue() == 4) {
		sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		if (!targetList.empty()) {
			vec2_t angle = player->getPos()->CalcAngle(*targetList[0]->getPos());
			vec3_t origin = g_Data.getClientInstance()->levelRenderer->getOrigin();
			vec2_t appl = angle.sub(player->viewAngles).normAngles();
			player->setRot(angle);
			if (showRots)
			player->resetRot();
		}
	}

	if (!targetList.empty())
	{
		if (hold && !g_Data.isLeftClickDown())
			return;

		for (auto &i : targetList)
		{
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			vec2_t pos = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());

			if (rotations.getSelectedValue() == 2)
			{
				plr->yawUnused1 = angle.y;
				plr->pitch = angle.x;
				plr->bodyYaw = angle.y;
			}

			// Strafe
			if (rotations.getSelectedValue() == 3)
			{
				plr->setRot(tro);
				plr->yawUnused1 = angle.y;
				if (showRots)
					player->resetRot();
			}
		}
	}
}

void Killaura::onSendPacket(C_Packet *packet)
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	if (!targetList.empty())
	{
		if (hold && !g_Data.isLeftClickDown())
			return;

		if (packet->isInstanceOf<C_MovePlayerPacket>() && (rotations.getSelectedValue() == 1 || rotations.getSelectedValue() == 2) && !targetList.empty())
		{
			vec2_t anglefd = g_Data.getLocalPlayer()->getPos()->CalcAngle(targetList[0]->eyePos0);
			auto *movePacket = reinterpret_cast<C_MovePlayerPacket *>(packet);

			movePacket->pitch = anglefd.x;
			movePacket->headYaw = anglefd.y;
			movePacket->yaw = anglefd.y;
		}
	}
}
void Killaura::onPreRender(C_MinecraftUIRenderContext* renderCtx)
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;
	if (renderType.getSelectedValue() == 4) {
		if (mode.getSelectedValue() == 0) {
			if (targetList[0]->damageTime > 1)
				DrawUtils::setColor(1.f, 0.f, 0.f, 0.25f);
			else
			DrawUtils::setColor(1.f, 0.f, 0.f, 0.15f);
			DrawUtils::drawEntityBox(targetList[0], (float)fmax(0.2f, 1 / (float)fmax(1, (*player->getPos()).dist(*targetList[0]->getPos()))));
		}
		else if (mode.getSelectedValue() == 1) {
			for (auto& ii : targetList) {
				if (targetList[0]->damageTime > 1)
					DrawUtils::setColor(1.f, 0.f, 0.f, 0.25f);
				else
					DrawUtils::setColor(1.f, 0.f, 0.f, 0.15f);
				DrawUtils::drawEntityBox(ii, (float)fmax(0.2f, 1 / (float)fmax(1, (*player->getPos()).dist(*ii->getPos()))));
			}
		}
	}
}
void Killaura::onLevelRender()
{
	auto interfacrColor = ColorUtil::interfaceColor(1);
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr || !visualize || renderType.getSelectedValue() == 3)
		return;

	static float n = 0;
	static float anim = 0;
	if (g_Data.canUseMoveKeys() && !targetList.empty())
	{
		if (hold && !g_Data.isLeftClickDown())
			return;
		if (!targetList.empty())
		{
			anim++;
			DrawUtils::setColor(interfacrColor.r, interfacrColor.g, interfacrColor.b, 0.9f);

			vec3_t permutations[56];
			for (int i = 0; i < 56; i++)
			{
				permutations[i] = {sinf((i * 10.f) / (180 / PI)), 0.f, cosf((i * 10.f) / (180 / PI))};
			}
			vec3_t permutations2[56];
			n++;
			if (n == 360)
				n = 0;
			for (int i = 0; i < 56; i++)
			{
				permutations2[i] = {sinf((i * 10.f) / (180 / PI)), sinf((i * 10.f) / (180 / PI)), cosf((i * 10.f) / (180 / PI))};
			}

			const float coolAnim = 0.9f + 0.9f * sin((anim / 60) * PI * 1);

			vec3_t *start = targetList[0]->getPosOld();
			vec3_t *end = targetList[0]->getPos();

			auto te = DrawUtils::getLerpTime();
			vec3_t pos = start->lerp(end, te);

			auto yPos = pos.y;
			yPos -= 1.6f;
			yPos += coolAnim;

			vector<vec3_t> posList;
			posList.reserve(56);
			for (auto &perm : permutations)
			{
				vec3_t curPos(pos.x, yPos, pos.z);
				posList.push_back(curPos.add(perm));
			}
			auto interfaceMod = moduleMgr->getModule<Interface>();
			if (renderType.getSelectedValue() == 0)
			{
				DrawUtils::drawLineStrip3D(posList, 5); // full
			}
			if (renderType.getSelectedValue() == 1)
			{
				DrawUtils::drawLineStrip3D(posList, 4); // strip
			}
		}
	}
}

void Killaura::onDisable()
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	targetListEmpty = true;
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
	targetList.clear();
}