#include "ExamModule.h"

ExamModule::ExamModule() : IModule(0, Category::MISC, "JohnTWD skidded this hack XDXD") {
	calcDmgType = SettingEnum(this)
					  .addEntry(EnumEntry("Java", 1))
					  .addEntry(EnumEntry("Bedrock", 2));
	registerEnumSetting("calcDmgType", &this->calcDmgType, 1);
	registerBoolSetting("CrystalDmg", &this->testCrystalDamage, this->testCrystalDamage);
	registerBoolSetting("CrystalValidity", &this->testPlaceSuitable, this->testPlaceSuitable);

	registerBoolSetting("Test raytrace", &this->rayTraceTest, this->rayTraceTest);

	registerBoolSetting("What is my health", &this->SELF_getHealth, true);
	registerBoolSetting("Pointed Ent Health", &this->OTHERS_getHealth, true);

	registerBoolSetting("Draw Text at obj", &this->testDTIW, this->testDTIW);

	registerBoolSetting("Obj in Hand?", &this->tryFindId, true);
	registerBoolSetting("Find ID of Ent?", &this->tryLookEnt, true);
	registerBoolSetting("Test Unlookable", &this->testUnlookableEnt, false);
	registerBoolSetting("vec2 test", &this->testLook, false);
}

ExamModule::~ExamModule() {
}

const char* ExamModule::getModuleName() {
	return "ExamModule";
}

float ExamModule::computeExplosionDamage(vec3_t crystalPos, C_Entity* target, C_BlockSource* reg) {
	float explosionRadius = 12.f;  // 6 * 2

	vec3_t pos = *target->getPos();

	float dist = pos.dist(crystalPos) / explosionRadius;


	if (dist <= 1.f) {
		float exposure = 0;
		float impact;

		int armorPoints = 0;
		int epf = 0;

		using getSeenPercent_t = float(__fastcall*)(C_BlockSource*, vec3_t const&, AABB const&);
		static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(FindSignature("40 53 55 41 56 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4"));
		//float seenPX = getSeenPercent(reg, crystalPos.add(0.5f, 0.f, 0.5f), target->aabb);
		//clientMessageF("%f", seenPX);

		if (calcDmgType.GetSelectedEntry().GetName() == "Java") {  // calcDmgType == JAVA
			float toughness = 0.f;

			float distedsize = target->getPos()->dist(crystalPos) / explosionRadius;
			double blockDensity = getSeenPercent(reg, crystalPos.add(0.5f, 0.f, 0.5f), target->aabb);
			double v = (1.0 - distedsize) * blockDensity;

			float damage = (float)(int)((v * v + v) / 2.0 * 7.0 * explosionRadius + 1.0); // raw damage
			damage = damage * 3 / 2; // assume that difficulty is set to hard (yes im serious)
			
			bool hasArmor = false;
			for (int i = 0; i < 4; i++) { // process armor
				C_ItemStack* armor = target->getArmor(i);

				if (armor->item == nullptr)
					continue;

				int armorId = armor->getItem()->itemId;

				/*	GEAR		TURT	LEAT	GOLD	CHML	IRON	DIAM	NETH
				*	Helm		573		335		351		339		343		347		609
				*	Tunic		null	336		352		340		344		348		610
				*	Pants		null	337		353		341		345		349		611
				*	Boots		null	338		354		342		346		350		612
				*/

				switch (armorId) {
				case 573:	// turt helm
					armorPoints += 2;
					break;

				case 335:	// leat helm
					armorPoints += 1;
					break;
				case 336:	// leat ches
					armorPoints += 3;
					break;
				case 337:	// leat pant
					armorPoints += 2;
					break;
				case 338:	// leat boot
					armorPoints += 1;
					break;

				case 351:  // gold helm
					armorPoints += 2;
					break;	  
				case 352:  // gold ches
					armorPoints += 5;
					break;	  
				case 353:  // gold pant
					armorPoints += 3;
					break;	  
				case 354:  // gold boot
					armorPoints += 1;
					break;

				case 339:  // chml helm
					armorPoints += 2;
					break;	  
				case 340:  // chml ches
					armorPoints += 5;
					break;	  
				case 341:  // chml pant
					armorPoints += 4;
					break;	  
				case 342:  // chml boot
					armorPoints += 1;
					break;

				case 343:  // iron helm
					armorPoints += 2;
					break;	  
				case 344:  // iron ches
					armorPoints += 6;
					break;	  
				case 345:  // iron pant
					armorPoints += 5;
					break;	  
				case 346:  // iron boot
					armorPoints += 2;
					break;
				default:
					if (armorId == 347 || armorId == 609 || armorId == 350 || armorId == 612) {	// neth/diam helm/boot
						armorPoints += 3;
					}
					else if (armorId == 348 || armorId == 610) {	// neth/diam ches
						armorPoints += 8;
					}
					else if (armorId == 349 || armorId == 611) {	// neth/diam pant
						armorPoints += 6;
					}
					
					if (347 <= armorId && armorId <= 350) {	// armor diam
						toughness += 2.f;
					} else if (609 <= armorId && armorId <= 612) { // armor neth
						toughness += 3.f;
					}
					
				}
				hasArmor = true;
				epf += (int)getBlastDamageEnchantReduction(armor);
			}

			if (hasArmor) {
				float F_epf = fminf((float)epf, 15.f) / 25.f;

				// damage, with armor, no ench
				//damage = damage * (1 / ( fmaxf(((float)armorPoints/5), (armorPoints - (4*damage)/(toughness+8))))); // seems reasonable
				damage = damage * fminf(((float)armorPoints/5), (1 / ((float)armorPoints / 5)));

				// damage, with ench
				damage = damage * (1 - F_epf);
			}
				return damage;

		} else if ((calcDmgType.GetSelectedEntry().GetName() == "Bedrock")) {

			exposure = getSeenPercent(reg, crystalPos.add(0.5f, 0.f, 0.5f), target->aabb);
			impact = (1 - dist) * exposure;
			int damage = (int)((impact * impact * 3.5f + impact * 0.5f * 7.f) * explosionRadius + 1.f);

			float finalDamage = (float)damage;

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

			return finalDamage;
		}
	} else {
		return 0.f;
	}
}

float ExamModule::getBlastDamageEnchantReduction(C_ItemStack* armor) {
	float epf = 0.f;

	if (calcDmgType.GetSelectedEntry().GetName() == "Bedrock") {
		if (armor->getEnchantValue(0) != 4) {
			epf += armor->getEnchantValue(0);  // protection
		} else
			epf += 5; //  yeah prot in BE is OP AF
		epf += armor->getEnchantValue(3) * 2.f;  // blast protection
	}

	else if (calcDmgType.GetSelectedEntry().GetName() == "Java") {  // java getBlastReduction
		//float rprot_level = armor->getEnchantValue(0);
		//float blast_level = armor->getEnchantValue(3) * 2.f; 

		epf += armor->getEnchantValue(0);
		epf += armor->getEnchantValue(3) * 2.f;
	}

	return epf;
}

bool RTSC_EM(vec3_t start, vec3_t end) {
	if (!isnan(start.x) && !isnan(start.y) && !isnan(start.z)) {  // Vec3d start == (NaN, NaN, NaN); return false
		if (!isnan(end.x) && !isnan(end.y) && !isnan(end.z)) {    // Vec3d end   == (Nan, NaN, NaN); return false
			return true;
		}
	}

	return false;
}

// its actually useless without RTSC_EM actually working
float ExamModule::calculateBlockDensity(C_Entity* targetEntity, vec3_t explosionPosition) {
	vec3_t entityPosition = *targetEntity->getPos();

	float explosionPower = 12.0f;
	double distanceToSize = entityPosition.dist(explosionPosition) / explosionPower;
	double blockDensity = 0.0;

	AABB bbox = *targetEntity->getAABB();  // targetEntity.getEntityBoundingBox().offset(targetEntity.getPositionVector().subtract(entityPosition));
	vec3_t bboxDelta = vec3_t(
		1.0 / ((bbox.upper.x - bbox.lower.x) * 2.0 + 1.0),
		1.0 / ((bbox.upper.y - bbox.lower.y) * 2.0 + 1.0),
		1.0 / ((bbox.upper.z - bbox.lower.z) * 2.0 + 1.0));

	double xOff = (1.0 - floor(1.0 / bboxDelta.x) * bboxDelta.x) / 2.0;
	double zOff = (1.0 - floor(1.0 / bboxDelta.z) * bboxDelta.z) / 2.0;

	if (bboxDelta.x >= 0.0 && bboxDelta.y >= 0.0 && bboxDelta.z >= 0.0) {
		int nonSolid = 0;
		int total = 0;

		for (double x = 0.0; x <= 1.0; x += bboxDelta.x) {
			for (double y = 0.0; y <= 1.0; y += bboxDelta.y) {
				for (double z = 0.0; z <= 1.0; z += bboxDelta.z) {
					vec3_t startPos = vec3_t(
						xOff + bbox.lower.x + (bbox.upper.x - bbox.lower.x) * x,
						bbox.lower.y + (bbox.upper.y - bbox.upper.y) * y,
						zOff + bbox.lower.z + (bbox.upper.z - bbox.lower.z) * z);

					if (!RTSC_EM(startPos, explosionPosition)) ++nonSolid;
					++total;
				}
			}
		}
		blockDensity = (double)nonSolid / (double)total;
	}

	//double densityAdjust = (1.0 - distanceToSize) * blockDensity;
	//float damage = (float)(int)((densityAdjust * densityAdjust + densityAdjust) / 2.0 * 7.0 * explosionPower + 1.0);

	return blockDensity;
}

bool checkCornerHitboxCollisionForExamMod(vec3_t* block, C_Entity* ent) {  // THANK SB HOLY SHIT I WAS TRYING TO MAKE THIS BY MYSELF FOR HOURS!!11!1
	std::vector<vec3_t*> corners;
	corners.clear();

	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.lower.z));
	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.lower.z));

	if (!corners.empty())
		for (auto corner : corners) {
			if ((floor(corner->x) == floor(block->x)) && (floor(corner->y) == floor(block->y)) && (floor(corner->z) == floor(block->z))) {
				return true;
			}
		}

	return false;
}

bool ExamModule::isPlaceValid(vec3_t location) {
	if (g_Data.getLocalPlayer()->region->getBlock(location)->toLegacy()->blockId != 0) { 
		clientMessageF("location is not air");
		return false;
	}
	auto friendly = g_Data.getLocalPlayer();

	if (g_Data.getLocalPlayer()->region->getBlock(location.add(0, 1, 0))->toLegacy()->blockId != 0) {
		clientMessageF("location+(0,1,0) is not air");
		return false;
	}  

	if (checkCornerHitboxCollisionForExamMod(&location, friendly)) {
		clientMessageF("opponent's hitbox intersects location ");
		return false;
	}
	if (g_Data.getLocalPlayer()->region->getBlock(location.sub(0, 1, 0))->toLegacy()->blockId != 49 &&
		g_Data.getLocalPlayer()->region->getBlock(location.sub(0, 1, 0))->toLegacy()->blockId != 7) {
		clientMessageF("floor is not obsidian or bedrock");
		return false;
	}

	float distToLoc = (location).dist(*g_Data.getLocalPlayer()->getPos());
	if (distToLoc > 10) {
		clientMessageF("place location is too far (>10m)");
		return false;
	}

	if (friendly->getHealth() < 5) {
		clientMessageF("PlayerHealth less than minHealth");
		return false;
	}

	float possibleDmg = computeExplosionDamage(location, friendly, friendly->region);

	if (possibleDmg >= 10) {
		clientMessageF("crystalDamage > 10");
		return false;
	}
	if ((friendly->eyePos0.floor().y + 2) <= location.y) {
		clientMessageF("Z-axis is too high");
		return false;
	}

	return true;
}


void ExamModule::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) return;  // dont forget

	clientMessageF("did it work???");

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (tryFindId == true) {
		int iHasId = localPlayer->getSelectedItemId();
		clientMessageF("Item ID: %i", iHasId);
	}
}

static std::vector<C_Entity*> objectList2022;
bool look4fags(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;

	float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (dist <= 7) {
		objectList2022.push_back(curEnt);
		return true;
	}
	return false;
}

void ExamModule::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) return;  // dont forget

	if (SELF_getHealth) {
		float myHealth = g_Data.getLocalPlayer()->getHealth();
		clientMessageF("My health is: %f", myHealth);
	}

	if (testLook) {
		float p1 = gm->player->pitch;
		float p2 = gm->player->pitch2;

		float ang = gm->player->bodyYaw;

		clientMessageF("p1: %f ~ p2: %f", p1, p2);
		clientMessageF("yaw: %f", ang);
	
	}

	if (testUnlookableEnt) {
		objectList2022.clear();
		g_Data.forEachEntity(look4fags);

		for (auto& i : objectList2022) {
			clientMessageF("fag ID: %i", i->getEntityTypeId());
		}
	}

	auto ptr = g_Data.getLocalPlayer()->getlevel();

	if (tryLookEnt) {

		if (ptr->getEntity() != nullptr) {
			pointedEnt = ptr->getEntity()->getEntityTypeId();
			if (!seen) {
				clientMessageF("You are looking at: %i", pointedEnt);
				clientMessageF("It is also at %f %f %f", ptr->getEntity()->getHumanPos().x, ptr->getEntity()->getHumanPos().y, ptr->getEntity()->getHumanPos().z);
				seen = true;
				if (OTHERS_getHealth) {
					float otherHP = ptr->getEntity()->getHealth();
					clientMessageF("This dude's health is: %f", otherHP);
				}
			}
		} else {
			if (ptr != nullptr && ptr->rayHitType == 0) {
				int atBlok = g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId;
				clientMessageF("This block is: %i", atBlok);
				vec3_t thisBlock = ptr->block.toVec3t();
				clientMessageF("it is also @ %f %f %f", thisBlock.x, thisBlock.y, thisBlock.z);
			}
			seen = false;
		}
	}
}

void ExamModule::onDisable() {
}

void ExamModule::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr) return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() != 637 && !testCrystalDamage && !testPlaceSuitable) return;

	auto ptr = g_Data.getLocalPlayer()->getlevel();
	if (ptr != nullptr)
		if (ptr->getEntity() == nullptr && ptr->rayHitType == 0) {
			if (g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId == 49 ||
				g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId == 7) {
				DrawUtils::setColor(.02f, 1.f, .75f, 1.f);
				DrawUtils::drawBox(ptr->block.toVec3t().add(0.f, 1.5f, 0.f),
								   ptr->block.add(1).toVec3t().add(0.f, 1.5f, 0.f), .3f);

				if (testCrystalDamage) {
					float possibleDamage = computeExplosionDamage(ptr->block.toVec3t().add(0, 1, 0), g_Data.getLocalPlayer(), g_Data.getLocalPlayer()->region);
					clientMessageF("Possible damage to yourself at that location: %f", possibleDamage);
				}
				if (testPlaceSuitable) {
					bool isGreat = isPlaceValid(ptr->block.toVec3t().add(0, 1, 0));

					if (isGreat) {
						clientMessageF("Based place location!");
					}
				}
				if (testDTIW) {
					std::string kneegear = "420.69";
					DrawUtils::drawTextInWorld(&kneegear, ptr->block.toVec3t(), .6f, vec3_ti(66, 238, 238), vec3_ti(128, 38, 151), 0.4f);
				}
				if (rayTraceTest) {
					float blocksFromMeToPlace = countBlksAlongLine(g_Data.getLocalPlayer()->eyePos0, ptr->block.toVec3t().add(0, 1, 0)).blockCount;
					clientMessageF("Amt of sh!t in between u and ptr: %f", blocksFromMeToPlace);
				}
			}
		}

}
