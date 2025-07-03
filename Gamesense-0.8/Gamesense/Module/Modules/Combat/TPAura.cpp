#include "TPAura.h"

using namespace std;
TPAura::TPAura() : IModule(0x0, Category::COMBAT, "Automatically teleports on enemy")
{
	this->registerBoolSetting("Attack Mobs", &this->isMobAura, this->isMobAura);
	this->registerBoolSetting("Smooth", &this->lerp, this->lerp);
	this->registerFloatSetting("Range", &this->range, this->range, 0.5f, 20.f);
	this->registerIntSetting("Delay", &this->delay, this->delay, 0, 20);
	this->registerIntSetting("Attack delay", &this->delay1, this->delay1, 0, 20);
	this->registerBoolSetting("Silent Rotations", &this->silent, this->silent);
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Single", 0);
	mode.addEntry("Multi", 1);
	registerEnumSetting("Switch", &AutoWeapon, 0);
	AutoWeapon.addEntry("None", 0);
	AutoWeapon.addEntry("Regular", 1);
	AutoWeapon.addEntry("NukkitSpoof", 2);
	AutoWeapon.addEntry("NukkitSmart", 3);
}

TPAura::~TPAura()
{
}

const char *TPAura::getRawModuleName()
{
	return ("TPAura");
}
const char *TPAura::getModuleName()
{
	switch (mode.getSelectedValue())
	{
	case 0:
		name = string("TPAura ") + string(GRAY) + string("Single");
		break;
	case 1:
		name = string("TPAura ") + string(GRAY) + string("Multi");
		break;
	}
	return name.c_str();
}

static std::vector<C_Entity *> targetList;

void findEntity0(C_Entity *currentEntity, bool isRegularEntity)
{
	static auto TPMod = moduleMgr->getModule<TPAura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer()) // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (TPMod->isMobAura)
	{
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64) // item
			return;
	}
	else
	{
		if (!TargetUtil::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (!TPMod->unlim)
	{
		if (dist < TPMod->range)
		{
			targetList.push_back(currentEntity);
		}
	}
	else
	{
		if (dist < TPMod->unlimRange)
		{
			targetList.push_back(currentEntity);
		}
	}
}

void TPAura::findWeapon2()
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
			if (currentDamage > damage && stack->getItem()->itemId == 316)
			{
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}
void TPAura::findWeaponSmart2()
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

void TPAura::onTick(C_GameMode *gm)
{

	// Loop through all our players and retrieve their information
	targetList.clear();
	C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();

	g_Data.forEachEntity(findEntity0);

	Odelay++;
	slotKA = supplies->selectedHotbarSlot;
	slotKASmart = supplies->selectedHotbarSlot;
	if (!targetList.empty() && Odelay >= delay)
	{
		vec3_t pos = *targetList[0]->getPos();
		if (turn == 3)
		{
			pos.y += 2;
			pos.x += -1.f;
			pos.z += 1.f;
			turn = 0;
		}
		else if (turn == 2)
		{
			pos.y += 2.f;
			pos.x += -1.f;
			pos.z += -1.f;
			turn = 3;
		}
		else if (turn == 1)
		{
			pos.y += 2.f;
			pos.x += 1.f;
			pos.z += -1.f;
			turn = 2;
		}
		else if (turn == 0)
		{
			pos.y += 2.f;
			pos.x += 1.f;
			pos.z += 1.f;
			turn = 1;
		}
		if (AutoWeapon.getSelectedValue() == 1, 2 && !targetList.empty() && !AutoWeapon.getSelectedValue() == 0)
			findWeapon2();
		if (AutoWeapon.getSelectedValue() == 3 && !targetList.empty() && gm->player->isInWater() && !AutoWeapon.getSelectedValue() == 0)
			findWeaponSmart2();
		if (AutoWeapon.getSelectedValue() == 3 && !targetList.empty() && !gm->player->isInWater() && !AutoWeapon.getSelectedValue() == 0)
			findWeapon2();
		// if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
		C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
		//}
		C_LocalPlayer *player = g_Data.getLocalPlayer();
		if (!lerp)
		{
			player->setPos(pos);
		}
		else
		{
			float dist2 = gm->player->getPos()->dist(pos);
			// player->lerpMotion(pos);
			player->lerpTo(pos, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
			player->lerpMotion(pos);
		}
		if (AutoWeapon.getSelectedValue() == 2)
			supplies->selectedHotbarSlot = slotKA;
		// if in water then it selects trident
		if (AutoWeapon.getSelectedValue() == 3 && gm->player->isInWater())
			supplies->selectedHotbarSlot = slotKASmart;
		if (AutoWeapon.getSelectedValue() == 3 && !gm->player->isInWater())
			supplies->selectedHotbarSlot = slotKA;
		Odelay = 0;
	}

	delay2++;
	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	if (!targetList.empty() && delay2 >= delay1)
	{

		if (mode.getSelectedValue() == 1)
		{
			for (auto &i : targetList)
			{
				g_Data.getLocalPlayer()->swing();
				g_Data.getCGameMode()->attack(i);
			}
		}
		if (mode.getSelectedValue() == 0)
		{
			g_Data.getLocalPlayer()->swing();
			g_Data.getCGameMode()->attack(targetList[0]);
		}
		delay2 = 0;
	}
}

void TPAura::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
}

void TPAura::onSendPacket(C_Packet *packet)
{
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && silent)
	{
		if (!targetList.empty())
		{
			auto *movePacket = reinterpret_cast<C_MovePlayerPacket *>(packet);
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}
