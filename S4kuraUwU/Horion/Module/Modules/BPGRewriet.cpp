#include <tuple>

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "BPGRewrite.h"

bool shouldPlace = false;
bool shouldDestroy = false;

bool disableAutoplace = false;

bool rotate = false;
vec2_t rotateTarget = {0.f, 0.f};

bool packetSente = false;
bool isPlayerAuthInpute = false;

int switchCooldown = 0;

int renderTimer;

void BPGRewrite::onWorldTick(C_GameMode* gm) {
	if (renderTimer > 0)
		renderTimer--;

	renderCrystal = (renderTimer > 0);

	packetSente = false;
	isPlayerAuthInpute = false;
	rotate = false;

	if (switchCooldown > 0)
		return;

	if (!gm->player->isAlive())
		return;

	if (gm->player->getHealth() < (float)minHealth)
		return;

	if (!GameData::canUseMoveKeys() && InvDisable)  // TODO: Find out if 2b2e.org kicks you for placing in inventory
		return;

	if (!GameData::isRightClickDown() && rightClick) {
		if (previousSlot != -1) {
			gm->player->getSupplies()->selectedHotbarSlot = previousSlot;
		}
		previousSlot = -1;
		return;
	}

	if (!autoPlace)
		disableAutoplace = true;

	static int oBDelay = 0;
	static int oPDelay = 0;

	bool placed = false;

	vec3_t playerPos = *gm->player->getPos();

	std::vector<C_Entity*> targetList;
	std::vector<C_Entity*> allEnts;

	if (oPDelay >= placeDelay && !disableAutoplace) {
		oPDelay = 0;

		g_Data.forEachEntity([&](C_Entity* ent, bool isNormal) {
			if (!ent->isAlive())
				return;

			if (ent->getPos()->dist(*gm->player->getPos()) > 12.f + maxDistance)
				return;

			if (ent == g_Data.getLocalPlayer()) {
				allEnts.push_back(ent);
				return;
			}
			
			if (!Target::isValidTarget(ent))
				return;

			allEnts.push_back(ent);
			targetList.push_back(ent);
		});

		if (!targetList.empty()) {
			if (findCrystal(gm)) {
				if (switchCooldown > 0)
					return;

				placed = doPlace(targetList, allEnts, gm);

				if (rotations.GetSelectedEntry().GetValue() == rotations_antikick && placeDelay == breakDelay) {
					// Allow AutoDestroy to work if we are going to take priority over it again
					disableAutoplace = true;
				}
			}
		} else if (!rightClick && !flash) {
			if (previousSlot != -1) {
				gm->player->getSupplies()->selectedHotbarSlot = previousSlot;
				previousSlot = -1;
			}
		}
	} else if (oPDelay >= placeDelay && disableAutoplace) {
		oPDelay = 0;
		disableAutoplace = false;
	} else {
		oPDelay++;
	}

	if (oBDelay >= breakDelay) {
		oBDelay = 0;

		std::vector<C_Entity*> crystalList;
		g_Data.forEachEntity([&](C_Entity* ent, bool isNormal) {
			if (ent->getEntityTypeId() != 71)  // Crystals only
				return;

			if (!ent->isAlive())
				return;

			if (ent->getPos()->dist(*gm->player->getPos()) > maxDistance)
				return;

			if (destroyEnum.GetSelectedEntry().GetValue() == destroy_check) {
				if (ent->getPos()->dist(*gm->player->getPos()) < minDistance)
					return;

				float dmg = computeExplosionDamage(ent, gm->player, gm->player->region, dmgEnum.GetSelectedEntry().GetValue());

				if (dmg > (float)maxSelfDmg)
					return;

				if (gm->player->getHealth() - dmg <= 1.f && suicide)
					return;

				if (targetList.empty())
					return;

				bool enoughTargets = false;
				bool enoughDmg = false;

				for (auto i : targetList) {
					if (i->getPos()->dist(*ent->getPos()) < 12.f) {
						enoughTargets = true;
					}

					if (computeExplosionDamage(ent, i, gm->player->region, dmgEnum.GetSelectedEntry().GetValue()) >= minDamage) {
						enoughDmg = true;
					}
				}

				if (!enoughTargets && !enoughDmg)
					return;
			}

			if (botCheck) {
				// Crystal Y level changes depending on the server but their x and y are always 0.5

				float offX = ent->getPos()->sub(ent->getPos()->floor()).x;
				float offZ = ent->getPos()->sub(ent->getPos()->floor()).z;

				if (offX != 0.5f || offZ != 0.5f)
					return;
			}

			//if (ent->getPos()->floor().add(0.5f, 0.5f, 0.5f).dist(*ent->getPos()) > 0.2f) {
			//	logF("Bot check failed");
			//	return;
			//} Uncomment when 2b2e inevitably adds bot crystals

			crystalList.push_back(ent);
		});

		doDestroy(crystalList, gm, placed);
	} else {
		oBDelay++;

	}
}

bool findCryX() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 637) {
				C_MobEquipmentPacket a(id, *stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}

bool slotold() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	auto stack = g_Data.getLocalPlayer()->getSelectedItem();
	int n = supplies->selectedHotbarSlot;
	if (stack->item != nullptr) {
		if ((*stack->item)->itemId != 0) {
			C_MobEquipmentPacket a(id, *stack, n, n);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
			return true;
		}
	}
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}

void BPGRewrite::onTick(C_GameMode* gm) {
	if (switchCooldown > 0)
		switchCooldown--;

	int rotationMode = rotations.GetSelectedEntry().GetValue();

	if (rotate && !packetSente && !isPlayerAuthInpute && rotationMode == rotations_antikick) {
		C_MovePlayerPacket pkt(g_Data.getLocalPlayer(), *gm->player->getPos());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&pkt);

		rotate = false;
		packetSente = false;
	}

	if ((!toBreak.empty() || !toPlace.empty()) && !moduleMgr->getModule<NoSwing>()->isEnabled())
		gm->player->swing();

	// The preTick code did the funny for us so we can just blindly use the toPlace and toBreak stuff
	for (auto it = toPlace.begin(); it != toPlace.end(); ++it) {
		if (rotationMode == rotations_normal) {
			C_MovePlayerPacket pkt(g_Data.getLocalPlayer(), *gm->player->getPos());

			vec2_t calcAngle = gm->player->getPos()->CalcAngle((*it).toVec3t().add(0.5f, 0.5f, 0.5f));

			pkt.pitch = calcAngle.x;
			pkt.yaw = calcAngle.y;
			pkt.headYaw = calcAngle.y;

			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&pkt);
		}
		if (silent) findCryX();
		gm->buildBlock(it._Ptr, 0, false);
		if (silent && !g_Data.getLocalPlayer()->getSelectedItem()->getItem()->isPickaxe()) {
			__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			C_Inventory* inv = supplies->inventory;
			C_ItemStack* stack = inv->getItemStack(supplies->selectedHotbarSlot);
			if (stack->item != nullptr) {
				if (!(*stack->item)->isPickaxe() && !(*stack->item)->isWeapon() && !(*stack->item)->isShooter()) {
					C_MobEquipmentPacket a(id, *stack, supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				}
			}
		}

		renderTimer = 10;
	}

	toPlace.clear();

	std::vector<C_Entity*> swap;

	g_Data.forEachEntity([&](C_Entity* ent, bool isNormal) {
		for (auto i : toBreak) {
			if (ent == i)
				swap.push_back(i);
		}
	});

	toBreak = swap;  // Fix crash

	for (auto i : toBreak) {
		if (rotationMode == rotations_antikick) {
			C_MovePlayerPacket pkt(g_Data.getLocalPlayer(), *gm->player->getPos());

			vec2_t calcAngle = gm->player->getPos()->CalcAngle(*i->getPos());

			pkt.pitch = 270.f;
			pkt.yaw = 270.f;
			pkt.headYaw = calcAngle.y;

			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&pkt);
		}
		gm->attack(i);
	}

	toBreak.clear();
}

void BPGRewrite::onPlayerTick(C_Player* plr) {
	if (aurora && renderCrystal) {
	vec2_t ang = g_Data.getLocalPlayer()->getPos()->CalcAngle(latestCrystal.pos.toVec3t().add(0.5f, 0.5f, 0.5f));
		plr->pitch = 170.f;
	

	}
}

void BPGRewrite::onPreRender(C_MinecraftUIRenderContext* ctx) {
	

	if (!g_Data.isInGame()) {
		previousSlot = -1;
		if (flash)
			setEnabled(false);
	} else if (renderPlaced && GameData::canUseMoveKeys()) {
		if (renderCrystal) {
			static constexpr float astolfocolorIncrease = 0.025f;
			static float astolfoColor = 0.f;
			static bool isIncreasingColor2 = true;
			static bool isIncreasingcurrColor2 = true;
			static float astolfo = 0.f;
			astolfo = astolfoColor;
			isIncreasingcurrColor2 = !isIncreasingColor2;
			// astolfo color updates
			{
				astolfoColor += (isIncreasingColor2 ? 0.025f : -0.025f) * DrawUtils::getLerpTime();
				if (astolfoColor < 0.5f) {
					astolfoColor = 0.5;
					isIncreasingColor2 = true;
				}
				if (astolfoColor > 1.f) {
					astolfoColor = 1.f;
					isIncreasingColor2 = false;
				}
			}
			astolfo += (isIncreasingcurrColor2 ? astolfocolorIncrease : -astolfocolorIncrease);
			if (astolfo < 0.5f) {
				astolfo = 0.5f;
				isIncreasingcurrColor2 = true;
			}
			if (astolfo > 1.f) {
				astolfo = 1.f;
				isIncreasingcurrColor2 = false;
			}

			float s = 0.3f;
			float v = 1.f;
			MC_Color c;
			Utils::ColorConvertHSVtoRGB(astolfo, s, v, c.r, c.g, c.b);

			 DrawUtils::setColor(0.f, 1.f, 0.f, 1.f);
			 DrawUtils::drawBox(latestCrystal.pos.toVec3t(), latestCrystal.pos.add(1, 1, 1).toVec3t(), .4f);
			
			vec2_t textPos = DrawUtils::worldToScreen(latestCrystal.pos.toVec3t().add(0.5f, 0.5f, 0.5f));
			vec2_t textPos1 = DrawUtils::worldToScreen(latestCrystal.pos.toVec3t().add(0.5f, 0.6f, 0.5f));
			vec2_t textPos2 = DrawUtils::worldToScreen(latestCrystal.pos.toVec3t().add(0.5f, 0.4f, 0.5f));
			
			
			
				std::string text = std::to_string((int)latestCrystal.enemyDmg);
				std::string text1 = std::to_string((int)computeExplosionDamage(latestCrystal.pos.toVec3t(), g_Data.getLocalPlayer(), g_Data.getLocalPlayer()->region, dmgEnum.selected));

				DrawUtils::drawText(textPos2, &text, MC_Color(102, 0, 102), 0.8f);
				DrawUtils::drawText(textPos1, &text1, MC_Color(66, 238, 238), 0.8f);
			}
		}
	}

void BPGRewrite::onEnable() {
	if (flash && g_Data.getCGameMode() != nullptr) {
		findCrystal(g_Data.getCGameMode());
	}
}

void BPGRewrite::onDisable() {
	if (previousSlot != -1 && g_Data.getLocalPlayer() != nullptr) {
		g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = previousSlot;
	}
	previousSlot = -1;
}

bool BPGRewrite::isTooFar(vec2_t left, vec2_t right) {
	return right.sub(left).magnitude() > 10.f;
}

bool BPGRewrite::tryRaytrace(vec3_t vec3_t1, vec3_t vec3_t2, C_BlockSource* region) {
	struct HitResult {
		vec3_t startPos;
		vec3_t relativeEndPos;
		int hitType;  // 0 is block, 1 is actor, 3 is air
		uint32_t blockFace;
		vec3_ti blockPos;
		vec3_t blockHitPos;  // If no block is found it just extends to the max ray position
		uintptr_t someActorHitPointer;
		uintptr_t someOtherActorHitPointer;
		uint32_t actorThingy;  // Could contain ids or something, 0xFFFFFFFF when not facing an actor
		char idk[0x4];
		bool isHittingLiquid;
		uint8_t liquidBlockFace;  // If the liquid were a solid block, what block face would it be
		char nothing[0x2];
		vec3_ti liquidBlockPos;
		vec3_t liquidBlockHitPos;  // Full of zeroes if there is no liquid block
		bool isExtendedBlockHit;   // Is the bedrock bridge mechanic thingy focusing a block
		char pad[0x3];
	};

	auto result = new HitResult();

	using BlockSource_ClipT = HitResult*(__fastcall*)(C_BlockSource*, HitResult*, vec3_t&, vec3_t&, bool, bool, int, bool, bool);
	static auto clip = reinterpret_cast<BlockSource_ClipT>(FindSignature("48 8B C4 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 0F 29 70 ?? 0F 29 78 ?? 44 0F 29 40 ?? 44 0F 29 88 ?? ?? ?? ?? 44 0F 29 90 ?? ?? ?? ?? 44 0F 29 98 ?? ?? ?? ?? 44 0F 29 A0 ?? ?? ?? ?? 44 0F 29 A8 ?? ?? ?? ?? 44 0F 29 B0 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4D 8B D9"));
	
	return false;
}

void BPGRewrite::doDestroy(std::vector<C_Entity*> crystalList, C_GameMode* gm, bool didPlace) {
	if (crystalList.empty())
		return;

	vec3_t playerPos = *gm->player->getPos();
	vec2_t playerRot = gm->player->viewAngles;

	switch (rotations.GetSelectedEntry().GetValue()) {
	case rotations_off:
	case rotations_normal: {
		toBreak = crystalList;
	} break;
	case rotations_antikick: {
		if (rotate) {
			for (auto i : crystalList) {
				vec2_t targetAngle = playerPos.CalcAngle(*i->getPos());
				if (!isTooFar(rotateTarget, targetAngle)) {
					toBreak.push_back(i);
				}
			}
			break;
		}
		if (didPlace) {
			for (auto i : crystalList) {
				vec2_t targetAngle = playerPos.CalcAngle(*i->getPos());
				if (!isTooFar(playerRot, targetAngle)) {
					toBreak.push_back(i);
				}
			}
			break;
		}

		// Find the best crystal
		struct CrystalCompareStruct {
			vec3_t playerPos;
			vec2_t playerRot;
			std::vector<C_Entity*>* vec;

			CrystalCompareStruct(vec3_t pPos, vec2_t pRot, std::vector<C_Entity*>* v) : playerPos(pPos), playerRot(pRot), vec(v) {}

			bool operator()(C_Entity* left, C_Entity* right) {
				vec2_t leftAngle = playerPos.CalcAngle(*left->getPos());
				vec2_t rightAngle = playerPos.CalcAngle(*right->getPos());

				std::vector<C_Entity*> leftEnts;  // Crystals we can hit if we rotate to left
				std::vector<C_Entity*> rightEnts;

				for (auto i : *vec) {
					vec2_t tgtRot = playerPos.CalcAngle(*i->getPos());

					if (!BPGRewrite::isTooFar(leftAngle, tgtRot))
						leftEnts.push_back(i);

					if (!BPGRewrite::isTooFar(rightAngle, tgtRot))
						rightEnts.push_back(i);
				}

				return leftEnts.size() > rightEnts.size();
			}
		};

		std::sort(crystalList.begin(), crystalList.end(), CrystalCompareStruct(playerPos, playerRot, &crystalList));

		C_Entity* gamerCrystal = crystalList[0];
		vec2_t angle = playerPos.CalcAngle(*gamerCrystal->getPos());

		for (auto i : crystalList) {
			vec2_t ang = playerPos.CalcAngle(*i->getPos());

			if (!isTooFar(ang, angle))
				toBreak.push_back(i);
		}

		rotate = true;
		rotateTarget = angle;
	} break;
	}
}

bool BPGRewrite::doPlace(std::vector<C_Entity*> targetList, std::vector<C_Entity*> allEnts, C_GameMode* gm) {
	struct CrystalStruct {
		vec3_ti blockPos;
		vec3_t entPos;
		float selfDamage;
		float enemyDmgAvg;
		float maxDamage;
	};

	std::vector<CrystalStruct> candidates;

	vec3_t playerPos = *gm->player->getPos();
	vec3_ti posFloored = playerPos.floor();

	int rangeInt = (int)ceilf(maxDistance);

	int dmgMode = dmgEnum.GetSelectedEntry().GetValue();

	for (int x = posFloored.x - rangeInt; x < posFloored.x + rangeInt; x++) {
		for (int y = posFloored.y - rangeInt; y < posFloored.y + rangeInt; y++) {
			for (int z = posFloored.z - rangeInt; z < posFloored.z + rangeInt; z++) {
				if (y < 0 || y >= 256)
					continue;

				vec3_ti blockPos(x, y, z);
				vec3_t center = blockPos.toVec3t().add(0.5f, 0.5f, 0.5f);

				if (center.dist(playerPos) > maxDistance || center.add(0.f, 1.f, 0.f).dist(playerPos) > maxDistance)  // Make sure we can place AND destroy
					continue;

				if (center.add(0.f, 1.f, 0.f).dist(playerPos) < minDistance)
					continue;

				// We want to make sure we can place here before doing an expensive string compare
				if (gm->player->region->getBlock(blockPos.add(0, 1, 0))->blockLegacy->blockId != 0)
					continue;

				if (gm->player->region->getBlock(blockPos.add(0, 2, 0))->blockLegacy->blockId != 0 && dmgMode != dmg_nukkit /* Nukkit lets us do this but 2b2e and vanilla does not */)
					continue;

				auto name = gm->player->region->getBlock(blockPos)->blockLegacy->name.getText();

				if (strcmp(name, "obsidian") != 0 && strcmp(name, "bedrock") != 0)
					continue;

				// Time for an even more expensive calculation
				AABB aabb(
					blockPos.add(0, 1, 0).toVec3t(),
					blockPos.add(0, 1, 0).toVec3t().add(1.f, dmgMode == dmg_nukkit ? 1.f : 2.f, 1.f));
				bool valid = true;
				for (auto ent : allEnts) {
					if (aabb.intersects(ent->aabb)) {
						valid = false;
						break;
					}
				}
				if (!valid)
					continue;

				valid = false;
				bool closeEnough = true;  // TODO

				// Enemy damage
				float enemyDmgAvg = 0.f;
				float maxDamage = 0.f;
				{
					int count = 0;  // How many enemies where damaged
					for (auto i : targetList) {
						float dmg = computeExplosionDamage(center.add(0.f, 1.f, 0.f), i, gm->player->region, dmgMode);
						if (!tryRaytrace(center.add(0.f, 1.f, 0.f), playerPos, gm->player->region)) {
							if (center.dist(playerPos) > wallRange)
								continue;
							if (center.add(0.f, 1.f, 0.f).dist(playerPos) > wallRange)
								continue;
						}
						if (dmg > 0.f) {
							maxDamage = fmaxf(maxDamage, dmg);
							enemyDmgAvg += dmg;
							count++;
						}
						if (dmg >= (float)minDamage)
							valid = true;
					}

					if (count != 0 && enemyDmgAvg > 0.f)
						enemyDmgAvg /= (float)count;
				}

				float selfDmg = computeExplosionDamage(center.add(0.f, 1.f, 0.f), gm->player, gm->player->region, dmgMode);

				if (!valid || selfDmg > (float)maxSelfDmg || (gm->player->getHealth() - selfDmg <= 1.f && suicide) || !closeEnough)
					continue;

				if (!tryRaytrace(center.add(0.f, 1.f, 0.f), playerPos, gm->player->region)) {
					if (center.dist(playerPos) > wallRange)
						continue;
					if (center.add(0.f, 1.f, 0.f).dist(playerPos) > wallRange)
						continue;
				}

				CrystalStruct crystal{};

				crystal.blockPos = blockPos;
				crystal.entPos = center.add(0.f, 1.f, 0.f);
				crystal.enemyDmgAvg = enemyDmgAvg;
				crystal.selfDamage = selfDmg;
				crystal.maxDamage = maxDamage;

				if (crystal.selfDamage >= crystal.enemyDmgAvg)
					continue;

				candidates.push_back(crystal);
			}
		}
	}

	if (candidates.empty())
		return false;

	struct CrystalCompareStruct {
		bool operator()(CrystalStruct& left, CrystalStruct& right) {
			return (left.enemyDmgAvg) > (right.enemyDmgAvg);
		}
	};

	std::sort(candidates.begin(), candidates.end(), CrystalCompareStruct());

	vec3_ti bestPos = candidates[0].blockPos;
	vec2_t targetRot = playerPos.CalcAngle(bestPos.toVec3t().add(0.5f, 0.5f, 0.5f));

	if (isTooFar(gm->player->viewAngles, targetRot) && rotations.GetSelectedEntry().GetValue() == rotations_antikick) {
		rotate = true;
		rotateTarget = targetRot;
	}

	CrystalRenderHolder crystal{};

	crystal.enemyDmg = candidates[0].maxDamage;  // TODO
	crystal.playerDmg = 0.f;
	crystal.pos = bestPos;

	latestCrystal = crystal;
	renderCrystal = true;
	toPlace.push_back(bestPos);

	return true;
}

bool BPGRewrite::findCrystal(C_GameMode* gm) {
	C_PlayerInventoryProxy* supplies = gm->player->getSupplies();

	int prevSlot = supplies->selectedHotbarSlot;

	auto funny = supplies->inventory->getItemStack(prevSlot);
	if (funny->item != nullptr) {
		if (strcmp(funny->getItem()->tileName.getText(), "item.end_crystal") == 0 || silent)  // Try blocking that nukkit
			return true;
		else if (!autoSelect)
			return false;
	} else if (!autoSelect) {
		return false;
	}

	for (auto i = 0; i < 9; i++) {
		C_ItemStack* item = supplies->inventory->getItemStack(i);

		if (item->item != nullptr) {
			if (strcmp(item->getItem()->tileName.getText(), "item.end_crystal") == 0) {
				supplies->selectedHotbarSlot = i;
				switchCooldown = 3;
				previousSlot = prevSlot;
				return true;
			}
		}
	}

	return false;
}

float BPGRewrite::computeExplosionDamage(vec3_t crystalPos, C_Entity* target, C_BlockSource* reg, int mode) {
	float explosionRadius = 12.f;  // 6 * 2

	vec3_t pos = *target->getPos();

	float dist = pos.dist(crystalPos) / explosionRadius;

	if (dist <= 1.f) {
		float exposure;
		float impact;
		int damage;
		if (mode != dmg_vanilla) {
			exposure = 1;
			if (mode == dmg_java) {
				exposure = calculateBlockDensity(crystalPos, AABB(target->aabb.lower, vec3_t(target->aabb.upper.x, fminf(target->aabb.lower.y + 1, target->aabb.upper.y), target->aabb.upper.z)), reg);
			}
			
			impact = (1 - dist) * exposure;
			damage = (int)(((impact * impact + impact) / 2) * 8 * explosionRadius + 1);
		} else {
			using getSeenPercent_t = float(__fastcall*)(C_BlockSource*, vec3_t const&, AABB const&);
			static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(FindSignature("40 53 55 41 56 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4"));

			exposure = getSeenPercent(reg, crystalPos, target->aabb);
			impact = (1 - dist) * exposure;
			damage = (int)((impact * impact * 3.5f + impact * 0.5f * 7.f) * explosionRadius + 1.f);
		}
		float finalDamage = (float)damage;

		int armorPoints = 0;
		int epf = 0;
		//int toughness = 0;

		for (int i = 0; i < 4; i++) {
			C_ItemStack* armor = target->getArmor(i);

			if (armor->item == nullptr)
				continue;

			armorPoints += (*armor->item)->getArmorValue();
			epf += (int)getBlastDamageEnchantReduction(armor);
		}

		finalDamage -= finalDamage * armorPoints * 0.04f;  // basic armor reduction

		finalDamage -= finalDamage * std::min(ceilf(std::min(epf, 25) * ((float)75 /* it's unneccessary to simulate randomness */ / 100)), 20.f) * 0.04f;  // enchant reduction

		// absorption and resistance are impossible to predict client side

		int targetY = (int)floorf(target->aabb.lower.y);
		int crystalY = (int)floorf(crystalPos.y);

		if (targetY < crystalY && mode == dmg_2b2e)
			finalDamage /= 2.f;
		else if (mode == dmg_2b2e)
			finalDamage *= 2.f;

		return finalDamage;
	} else {
		return 0.f;
	}
}

float BPGRewrite::getBlastDamageEnchantReduction(C_ItemStack* armor) {
	float epf = 0.f;

	epf += armor->getEnchantValue(0);        // protection
	epf += armor->getEnchantValue(3) * 2.f;  // blast protection

	return epf;
}

float BPGRewrite::calculateBlockDensity(vec3_t vec, AABB bb, C_BlockSource* region) {
	double d0 = 1.0 / (((double)bb.upper.x - (double)bb.lower.x) * 2.0 + 1.0);
	double d1 = 1.0 / (((double)bb.upper.y - (double)bb.lower.y) * 2.0 + 1.0);
	double d2 = 1.0 / (((double)bb.upper.z - (double)bb.lower.z) * 2.0 + 1.0);
	double d3 = (1.0 - floor(1.0 / d0) * d0) / 2.0;
	double d4 = (1.0 - floor(1.0 / d2) * d2) / 2.0;

	if (d0 >= 0.0 && d1 >= 0.0 && d2 >= 0.0) {
		int i = 0;
		int j = 0;

		for (float f = 0.f; f <= 1.f; f = (float)((double)f + d0)) {
			for (float f1 = 0.f; f1 <= 1.f; f1 = (float)((double)f1 + d1)) {
				for (float f2 = 0.f; f2 <= 1.f; f2 = (float)((double)f2 + d2)) {
					double d5 = bb.lower.x + ((double)bb.upper.x - (double)bb.lower.x) * (double)f;
					double d6 = bb.lower.y + ((double)bb.upper.y - (double)bb.lower.y) * (double)f1;
					double d7 = bb.lower.z + ((double)bb.upper.z - (double)bb.lower.z) * (double)f2;

					if (tryRaytrace(vec3_t(d5 + d3, d6, d7 + d4), vec, region)) {
					++i;
					}
					++j;
				}
			}
		}

		return (float)i / (float)j;
	} else {
		return 0.f;
	}
}

/*bool __forceinline collisionRayTrace(C_Block* block, vec3_t vec3_t1, vec3_t vec3_t2) {


	vec3_td vec3_td = start.subtract((double)pos.getX(), (double)pos.getY(), (double)pos.getZ());
	vec3_td vec3_td1 = end.subtract((double)pos.getX(), (double)pos.getY(), (double)pos.getZ());
	bool raytraceresult = boundingBox.calculateIntercept(vec3_td, vec3_td1);
	return !raytraceresult ? null : new RayTraceResult(raytraceresult.hitVec.addVector((double)pos.getX(), (double)pos.getY(), (double)pos.getZ()), raytraceresult.sideHit, pos);
}*/

