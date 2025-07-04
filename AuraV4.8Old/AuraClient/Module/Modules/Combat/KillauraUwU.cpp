#include "KillauraUwU.h"
#include "../../../../SDK/CAttribute.h"
#include "../../SDK/CEntity.h"

KillauraUwU::KillauraUwU() : IModule('P', Category::COMBAT, "Attacks entities around you automatically") {
	mode.addEntry(EnumEntry("Single", 0))
		.addEntry(EnumEntry("Multi", 1));
	registerEnumSetting("Mode", &mode, 1);
	autoweapon.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Switch", 1))
		.addEntry(EnumEntry("Silent", 2));
	registerEnumSetting("AutoWeapon", &autoweapon, 0);
	selectmode.addEntry(EnumEntry("Damage", 0))
		.addEntry(EnumEntry("Smart", 1));
	registerEnumSetting("SelectMode", &selectmode, 0);
	rotation.addEntry(EnumEntry("Actual", 0))
		.addEntry(EnumEntry("Nuke", 1));
	registerEnumSetting("Rotation", &rotation, 0);
	priority = SettingEnum(this)
		.addEntry(EnumEntry("Distance", 0))
		.addEntry(EnumEntry("Angle", 1))
		//.addEntry(EnumEntry("Health", 2))
		.addEntry(EnumEntry("Armor", 3));
	registerEnumSetting("Priority", &priority, 0);
	registerFloatSetting("Attack Range", &attackrange, attackrange, 3.f, 20.f);
	registerFloatSetting("Swing Range", &swingRange, swingRange, 3.f, 25.f);
	registerIntSetting("APS", &this->APS, this->APS, 1, 50);
	registerBoolSetting("APSBoost", &this->boost, this->boost);
	registerIntSetting("Boost", &this->apsBoost, this->apsBoost, 1, 200);
	//registerBoolSetting("Rotations", &this->rotations, this->rotations);
	registerFloatSetting("Rotation Speed", &rotationSpeed, rotationSpeed, 10.f, 180.f);
	registerBoolSetting("RandomPitch", &randomPitch, randomPitch);
	registerBoolSetting("RandomYaw", &randomYaw, randomYaw);
	registerBoolSetting("Swing", &this->swing, this->swing);
	registerBoolSetting("SwingSound", &this->swingSound, this->swingSound);
	registerBoolSetting("TargetESP", &this->targetESP, this->targetESP);
	registerBoolSetting("MobAura", &this->isMobAura, this->isMobAura);
	registerBoolSetting("Hurttime", &this->hurttime, this->hurttime);

}

KillauraUwU::~KillauraUwU() {
}

const char* KillauraUwU::getModuleName() {
	return ("KillauraUwU");
}

static std::vector<C_Entity*> targetList;

void findEntity2(C_Entity* currentEntity, bool isRegularEntity) {
	static auto KillauraUwUMod = moduleMgr->getModule<KillauraUwU>();
	if (!g_Data.getLocalPlayer()->isAlive())
		return;
	if (KillauraUwUMod->isMobAura) {
		if (currentEntity == nullptr)
			return;

		if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
			return;

		if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
			return;

		if (!currentEntity->isAlive())
			return;

		if (currentEntity->timeSinceDeath > 0)
			return;

		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;

		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) // Don't hit this pesky antibot on 2b2e.org
			return;

		if (currentEntity->getEntityTypeId() == 64) // item
			return;

		if (currentEntity->getEntityTypeId() == 80)  // Arrows
			return;

		if (currentEntity->getEntityTypeId() == 51) // NPC
			return;

		if (currentEntity->getEntityTypeId() == 69)  // XP
			return;
	}
	else {
		if (!TargetUtil::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (KillauraUwUMod->swingRange < KillauraUwUMod->attackrange)
		KillauraUwUMod->swingRange = KillauraUwUMod->attackrange;

	if (!KillauraUwUMod->canswing && dist < KillauraUwUMod->swingRange)
		KillauraUwUMod->canswing = true;

	if (dist < KillauraUwUMod->attackrange) {
		targetList.push_back(currentEntity);
	}
}


void KillauraUwU::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;

	int Sword = -1;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				Sword = n;
			}
		}
	}
	if (Sword != -1)
		supplies->selectedHotbarSlot = Sword;
}

void KillauraUwU::findWeaponWater() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	int Sword = -1;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr && (*stack->item)->isTrident()) {
			Sword = n;
		}
	}
	if (Sword != -1)
		supplies->selectedHotbarSlot = Sword;
}

void KillauraUwU::findWeaponAir() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	int Sword = -1;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr && (*stack->item)->isSword()) {
			Sword = n;
		}
	}
	if (Sword != -1)
		supplies->selectedHotbarSlot = Sword;
}

decltype(GetTickCount64()) start2;
bool Counter2(double a1) {
	if ((GetTickCount64() - start2) >= a1) {
		start2 = GetTickCount64();
		return true;
	}
	return false;
}

struct Distance {
	bool operator()(C_Entity* target, C_Entity* target2) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*target->getPos()).dist(*localPlayer->getPos()) < (*target2->getPos()).dist(*localPlayer->getPos());
	}
};

struct Angle {
	bool operator()(C_Entity* target, C_Entity* target2) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		vec2_t angle = localPlayer->getPos()->CalcAngle(*target->getPos()).normAngles();
		vec2_t angle2 = localPlayer->getPos()->CalcAngle(*target2->getPos()).normAngles();
		vec2_t appl = localPlayer->viewAngles.normAngles();

		auto normAngles = [](float num) noexcept -> float {
			while (num > 180.0f)
				num -= 360.0f;
			while (num < -180.0f)
				num += 360.0f;
			return num;
			};


		return abs(g_Data.getLocalPlayer()->pitch) > 60.f ? abs(appl.sub(angle).normAngles().magnitude()) < abs(appl.sub(angle2).normAngles().magnitude()) : abs(normAngles(angle.y - appl.y)) < abs(normAngles(angle2.y - appl.y));
	}
};

//struct Health {
	//bool operator()(C_Entity* target, C_Entity* target2) {
		//return target->getAttribute2(&HealthAttribute())->currentValue < target2->getAttribute2(&HealthAttribute())->currentValue;
	//}
//};

struct Armor {
	bool operator()(C_Entity* target, C_Entity* target2) {
		float armorValue = 0.f, armorValue2 = 0.f;

		for (int i = 0; i < 4; i++) {
			C_ItemStack* stack = target->getArmor(i);
			C_ItemStack* stack2 = target2->getArmor(i);

			if (stack->item != nullptr) {
				armorValue += stack->getArmorValueWithEnchants();
			}
			if (stack2->item != nullptr) {
				armorValue2 += stack2->getArmorValueWithEnchants();
			}
		}
		return armorValue < armorValue2;
	}
};

void KillauraUwU::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	//if (rotations) {
		switch (rotation.selected) {
		case 0: {
			vec3_t centrePos = targetList[0]->aabb.centerPoint();
			angle = localPlayer->getPos()->CalcAngle(centrePos);

			localAngle = vec2_t(localPlayer->pitch, localPlayer->bodyYaw);
			vec2_t appl = angle.sub(localAngle).normAngles();
			if (abs(appl.y) > rotationSpeed) {
				if (appl.y > 0.f) {
					localAngle.y += rotationSpeed;
					angle.y = localAngle.y;
				}
				else if (appl.y < 0.f) {
					localAngle.y -= rotationSpeed;
					angle.y = localAngle.y;
				}
				return;
			}
			else {
				if (randomPitch) {
					centrePos.y += randomFloat(-0.85f, 0.85f);
				}

				if (randomYaw) {
					centrePos.x += randomFloat(-0.25f, 0.25f);
					centrePos.z += randomFloat(-0.25f, 0.25f);
				}

				angle = localPlayer->getPos()->CalcAngle(centrePos);
			}
		} break;
		case 1: {
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		} break;
		}
	}
//}

/* void KillauraUwU::onAttack(Entity* attackEnt) {

}*/


void KillauraUwU::onGetPickRange() {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	static auto rainMode = moduleMgr->getModule<RainMode>();

	targetList.clear();
	g_Data.forEachEntity(findEntity2);
	targetListEmpty = targetList.empty();
	switch (priority.selected) {
	case 0:
		sort(targetList.begin(), targetList.end(), Distance());
		break;
	case 1:
		sort(targetList.begin(), targetList.end(), Angle());
		break;
	//case 2:
		//sort(targetList.begin(), targetList.end(), Health());
		//break;
	case 3:
		sort(targetList.begin(), targetList.end(), Armor());
	}
	if (n >= targetList.size() || mode.selected != 2)
		n = 0;

	if (g_Data.canUseMoveKeys() && !targetList.empty()) {
		if (autoweapon.selected == 2 && !targetList.empty())
			slot = supplies->selectedHotbarSlot;
		if (selectmode.selected == 0) {
			if (autoweapon.selected > 0) findWeapon();
		}
		else {
			if (localPlayer->isInWater() || rainMode->isEnabled()) {
				if (autoweapon.selected > 0) findWeaponWater();
			}
			else {
				if (autoweapon.selected > 0) findWeaponAir();
			}
		}
		if (Counter2(1000.f / (float)APS)) {
			for (auto& i : targetList) {
				// Attack all entitys in targetList
				switch (mode.selected) {
				case 0:
					if (boost) {
						for (int i = 0; i < apsBoost; i++) {
							if (!(targetList[n]->damageTime > 1 && hurttime)) {
								g_Data.getCGameMode()->attack(targetList[n]);
								if (!moduleMgr->getModule<NoSwing>()->isEnabled() && canswing && swing)
									localPlayer->swing();
							}
						}
					}
					else {
						if (!(targetList[n]->damageTime > 1 && hurttime)) {
							g_Data.getCGameMode()->attack(targetList[n]);
							if (!moduleMgr->getModule<NoSwing>()->isEnabled() && canswing && swing)
								localPlayer->swing();
						}
					}
					break;
				case 1:
					if (boost) {
						for (int i = 0; i < apsBoost; i++) {
							for (auto& i : targetList) {
								if (!(i->damageTime > 1 && hurttime)) {
									g_Data.getCGameMode()->attack(i);
									if (!moduleMgr->getModule<NoSwing>()->isEnabled() && canswing && swing)
										localPlayer->swing();
								}
							}
						}
					}
					else {
						for (auto& i : targetList) {
							if (!(i->damageTime > 1 && hurttime)) {
								g_Data.getCGameMode()->attack(i);
								if (!moduleMgr->getModule<NoSwing>()->isEnabled() && canswing && swing)
									localPlayer->swing();
							}
						}
					}
					break;
				}
			};
			++n;
		}
		if (autoweapon.selected == 2)
			supplies->selectedHotbarSlot = slot;
	}
}

void KillauraUwU::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (!swingSound) {
		if (packet->isInstanceOf<LevelSoundEventPacket>()) {
			LevelSoundEventPacket* soundEventPacket = reinterpret_cast<LevelSoundEventPacket*>(packet);
			if (soundEventPacket->sound == 43 || soundEventPacket->sound == 42)
				cancelSend = true;
		}
	}
}

void KillauraUwU::onLevelRender() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	static float n = 0;
	static float anim = 0;
	if (g_Data.canUseMoveKeys() && !targetList.empty()) {
		if (!targetESP) return;
		if (!targetList.empty()) {
			anim++;
			DrawUtils::setColor(1, 1, 1, 0.9f);

			vec3_t permutations[56];
			for (int i = 0; i < 56; i++) {
				permutations[i] = { sinf((i * 10.f) / (180 / PI)), 0.f, cosf((i * 10.f) / (180 / PI)) };
			}
			vec3_t permutations2[56];
			n++;
			if (n == 360)
				n = 0;
			for (int i = 0; i < 56; i++) {
				permutations2[i] = { sinf((i * 10.f) / (180 / PI)), sinf((i * 10.f) / (180 / PI)), cosf((i * 10.f) / (180 / PI)) };
			}

			const float coolAnim = 0.9f + 0.9f * sin((anim / 60) * PI * 1);

			vec3_t* start2 = targetList[0]->getPosOld();
			vec3_t* end = targetList[0]->getPos();

			auto te = DrawUtils::getLerpTime();
			vec3_t pos = start2->lerp(end, te);

			auto yPos = pos.y;
			yPos -= 1.6f;
			yPos += coolAnim;

			std::vector<vec3_t> posList;
			posList.reserve(56);
			for (auto& perm : permutations) {
				vec3_t curPos(pos.x, yPos, pos.z);
				posList.push_back(curPos.add(perm));
			}
			//auto interfaceColor = ColorUtil::interfaceColor(1);
			//DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, 1.f);
			DrawUtils::drawLineStrip3D(posList, 5);

			//Draw Sims Esp
			/*vector<Vec3> posList2;
			posList2.clear();
			// Iterate through phi, theta then convert r,theta,phi to XYZ
			for (double phi = 0.; phi < 2 * PI; phi += PI / 100.) // Azimuth [0, 2PI]
			{
				for (double theta = 0.; theta < PI; theta += PI / 100.) // Elevation [0, PI]
				{
					Vec3 point;
					point.x = 0.2 * cos(phi) * sin(theta);
					point.y = 0.2 * sin(phi) * sin(theta);
					point.z = 0.2 * cos(theta);
					Vec3 curPos = targetList[0]->eyePos0;
					curPos.y += 0.5f;
					posList2.push_back(curPos.add(point));
				}
			}
			DrawUtils::setColor(0.1f,0.9f,0.1f,255);
			DrawUtils::drawLineStrip3D(posList2, 4);*/
		}
	}
}


