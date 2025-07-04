#include "AutoTntSpammer.h"

AutoTntSpammer::AutoTntSpammer() : Module("AutoTntSpammer", "Auto attack players / mobs arround u.", Category::COMBAT) {
	addSlider<float>("Target Range", "Players/Mobs have range lower than this will be targeted", ValueType::FLOAT_T, &targetRange, 3.f, 12.f);
	addSlider<float>("Wall Range", "NULL", ValueType::FLOAT_T, &wallRange, 0.f, 12.f);		
	addBoolCheck("Attack Mob", "If u want attack mob or not", &attackMob);
}

int AutoTntSpammer::getBestWeaponSlot() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	float damage = 0.f;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) { // or you can do itemStack->getItemPtr()->hasRecipeTag("minecraft:is_sword") for sword find only 
			/*
			* here some step that you can get pick and things
			* item->getItemPtr()->hasRecipeTag("minecraft:digger") <- this is how you get like a things that can digs like shovel and picks and axe maybe
			* if (item->hasRecipeTag("minecraft:is_sword"))
			* item->hasRecipeTag("minecraft:is_pickaxe") <- get pickaxe
			* item->hasRecipeTag("minecraft:is_axe") <- get axe
			* item->hasRecipeTag("minecraft:is_shovel") <- get some shovel
			* item->hasRecipeTag("minecraft:is_hoe")<- get hoe like a riel bitch
			* item->hasRecipeTag("minecraft:is_food") <- get food
			*/
			// enchant can be found on /
			// TODO# https://www.digminecraft.com/lists/enchantment_list_pe.php   
			float currentDamage = itemStack->getItemPtr()->getAttackDamage() + (4.25f * itemStack->getEnchantLevel(EnchantID::fire_aspect));
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = i;
			}
		}
	}
	return slot;
}
int AutoTntSpammer::getObsidian() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 49) {
				return i;
			}
		}
	}
	return slot;
}
int AutoTntSpammer::getTNT() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 46) {
				return i;
			}
		}
	}
	return slot;
}
#include <algorithm>
#include <vector>
#include <ranges>

void AutoTntSpammer::Buildblocksfortnt(std::vector<Vec3<int>> buildvector) {
	GameMode* gm = mc.getGameMode();
	auto* lp = mc.getLocalPlayer();
	for (Vec3<int>& buildseq : buildvector) 
	{
		if (gm->tryPlaceBlock(buildseq)) {
			return;
		}

	}
}
bool buildobbyfirst = true;
bool buildtntseccond = false;
bool touchwithmagicsword = false;
void AutoTntSpammer::onNormalTick(Actor* actor) {
	auto localPlayer = mc.getLocalPlayer();
	auto plrInv = localPlayer->getPlayerInventory();
	auto inv = plrInv->inventory;
	auto gm = mc.getGameMode();
	auto region = localPlayer->dimension->blockSource;
	auto level = localPlayer->getLevel();
	



	int bestSwordSlot = getBestWeaponSlot();
	int oldSlotSword = plrInv->selectedSlot;
	bool shouldSwitchSword = (bestSwordSlot != plrInv->selectedSlot);

	int bestObiSlot = getObsidian();
	int oldSlotObi = plrInv->selectedSlot;
	bool shouldSwitchObi = (bestObiSlot != plrInv->selectedSlot);

	int bestTNTSlot = getTNT();
	int oldSlotTNT = plrInv->selectedSlot;
	bool shouldSwitchTNT= (bestTNTSlot != plrInv->selectedSlot);
	targetList.clear();
	if (!level) return;

	for (auto& target : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(target, attackMob)) {
			float seenPercent = region->getSeenPercent(localPlayer->stateVectorComponent->pos, *target->getAABB());
			float dist = target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
			float rangeCheck = (seenPercent > 0.f) ? targetRange : wallRange;
			if (dist < rangeCheck) targetList.push_back(target);
		}
	}

	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), TargetUtils::sortByDist);
		auto pos = targetList[0]->getPosition()->toInt();
		std::vector<Vec3<int>> vectorbuild{
			Vec3<int>(pos.x, pos.y + 0, pos.z - 1),
			Vec3<int>(pos.x, pos.y + 1, pos.z - 1),
			Vec3<int>(pos.x, pos.y + 2, pos.z - 1),

			Vec3<int>(pos.x - 1 , pos.y + 0, pos.z),
			Vec3<int>(pos.x - 1 , pos.y + 1, pos.z),
			Vec3<int>(pos.x - 1 , pos.y + 2, pos.z),

			Vec3<int>(pos.x , pos.y + 0, pos.z + 1),
			Vec3<int>(pos.x, pos.y + 1, pos.z + 1),
			Vec3<int>(pos.x, pos.y + 2, pos.z + 1),

			Vec3<int>(pos.x + 1, pos.y + 0, pos.z),
			Vec3<int>(pos.x + 1, pos.y + 1, pos.z),
			Vec3<int>(pos.x + 1, pos.y + 2, pos.z),

			Vec3<int>(pos.x + 1, pos.y + 3, pos.z),
			


			
			Vec3<int>(pos.x , pos.y + 3, pos.z),

		};
		Vec3<float> Targetpos = targetList[0]->getPosition()->floor().sub(Vec3<float>(0.f, 1.f, 0.f));
		if (buildobbyfirst)
		{
			//Vec3<int> abovePos = (Targetpos.add(Vec3<float>(0.f, 4.f, 0.f))).toInt();

			if (shouldSwitchObi) {
				plrInv->selectedSlot = bestObiSlot;

				MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestObiSlot), bestObiSlot, bestObiSlot);
				mc.getLocalPlayer()->sendNetworkPacket(pk);
				mc.getClientInstance()->loopbackPacketSender->send(&pk);
			}
			Buildblocksfortnt(vectorbuild);
			buildobbyfirst = false;
			buildtntseccond = true;
			touchwithmagicsword = false;
			if (shouldSwitchObi) {
				plrInv->selectedSlot = oldSlotObi;
			}

		}
		if (buildtntseccond)
		{
			Vec3<int> abovePos = (Targetpos.add(Vec3<float>(0.f, 2.f, 0.f))).toInt();

			if (shouldSwitchTNT) {
				plrInv->selectedSlot = bestTNTSlot;

				MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestTNTSlot), bestTNTSlot, bestTNTSlot);
				mc.getLocalPlayer()->sendNetworkPacket(pk);
				mc.getClientInstance()->loopbackPacketSender->send(&pk);
			}
			gm->tryPlaceBlock(abovePos);
			buildobbyfirst = false;
			buildtntseccond = false;
			touchwithmagicsword = true;
			if (shouldSwitchTNT) {
				plrInv->selectedSlot = oldSlotTNT;
			}

		}
		if (touchwithmagicsword)
		{
			Vec3<int> abovePos = (Targetpos.add(Vec3<float>(0.f, 2.f, 0.f))).toInt();

			if (shouldSwitchSword) {
				plrInv->selectedSlot = bestSwordSlot;

				MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSwordSlot), bestSwordSlot, bestSwordSlot);
				mc.getLocalPlayer()->sendNetworkPacket(pk);
				mc.getClientInstance()->loopbackPacketSender->send(&pk);

			}
			InteractPacket inter(InteractAction::RIGHT_CLICK, mc.getLocalPlayer()->getRuntimeID(), abovePos.toFloat());

			mc.getClientInstance()->loopbackPacketSender->send(&inter);
			gm->buildBlock(abovePos, Math::random(0, 5), false);
			buildobbyfirst = true;
			buildtntseccond = false;
			touchwithmagicsword = false;
			if (shouldSwitchSword) {
				plrInv->selectedSlot = oldSlotSword;
			}

		}
		

	}
}

void AutoTntSpammer::onSendPacket(Packet* packet, bool& shouldCancel) {
}

void AutoTntSpammer::onImGuiRender(ImDrawList* d) {
}




//  DOWN HERE HAVE AN ABOVEHEAD PLACE METHOD
/*
 std::vector<Vec3<int>> vectorbuild {
			Vec3<int>(pos.x, pos.y + 0, pos.z - 1),
			Vec3<int>(pos.x, pos.y + 1, pos.z - 1),
			Vec3<int>(pos.x, pos.y + 2, pos.z - 1),

			Vec3<int>(pos.x - 1 , pos.y + 0, pos.z),
			Vec3<int>(pos.x - 1 , pos.y + 1, pos.z),
			Vec3<int>(pos.x - 1 , pos.y + 2, pos.z),

			Vec3<int>(pos.x , pos.y + 0, pos.z + 1),
			Vec3<int>(pos.x, pos.y + 1, pos.z + 1),
			Vec3<int>(pos.x, pos.y + 2, pos.z + 1),

			Vec3<int>(pos.x + 1, pos.y + 0, pos.z),
			Vec3<int>(pos.x + 1, pos.y + 1, pos.z),
			Vec3<int>(pos.x + 1, pos.y + 2, pos.z),

			Vec3<int>(pos.x + 1, pos.y + 3, pos.z),
			Vec3<int>(pos.x, pos.y + 3, pos.z - 1),
			Vec3<int>(pos.x, pos.y + 3, pos.z + 1),
			Vec3<int>(pos.x - 1, pos.y + 3, pos.z),


			Vec3<int>(pos.x + 1, pos.y + 4, pos.z),
			Vec3<int>(pos.x , pos.y + 4, pos.z),

		};
		Vec3<float> Targetpos = targetList[0]->getPosition()->floor().sub(Vec3<float>(0.f, 1.f, 0.f));
		if (buildobbyfirst)
		{
			//Vec3<int> abovePos = (Targetpos.add(Vec3<float>(0.f, 4.f, 0.f))).toInt();

			if (shouldSwitchObi) {
				plrInv->selectedSlot = bestObiSlot;

				MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestObiSlot), bestObiSlot, bestObiSlot);
				mc.getLocalPlayer()->sendNetworkPacket(pk);
				mc.getClientInstance()->loopbackPacketSender->send(&pk);
			}
			Buildblocksfortnt(vectorbuild);
			buildobbyfirst = false;
			buildtntseccond = true;
			touchwithmagicsword = false;
			if (shouldSwitchObi) {
				plrInv->selectedSlot = oldSlotObi;
			}

		}
		if (buildtntseccond)
		{
			Vec3<int> abovePos = (Targetpos.add(Vec3<float>(0.f, 3.f, 0.f))).toInt();

			if (shouldSwitchTNT) {
				plrInv->selectedSlot = bestTNTSlot;

				MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestTNTSlot), bestTNTSlot, bestTNTSlot);
				mc.getLocalPlayer()->sendNetworkPacket(pk);
				mc.getClientInstance()->loopbackPacketSender->send(&pk);
			}
			gm->tryPlaceBlock(abovePos);
			buildobbyfirst = false;
			buildtntseccond = false;
			touchwithmagicsword = true;
			if (shouldSwitchTNT) {
				plrInv->selectedSlot = oldSlotTNT;
			}

		}
		if (touchwithmagicsword)
		{
			Vec3<int> abovePos = (Targetpos.add(Vec3<float>(0.f, 3.f, 0.f))).toInt();

			if (shouldSwitchSword) {
				plrInv->selectedSlot = bestSwordSlot;

				MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSwordSlot), bestSwordSlot, bestSwordSlot);
				mc.getLocalPlayer()->sendNetworkPacket(pk);
				mc.getClientInstance()->loopbackPacketSender->send(&pk);

			}
			InteractPacket inter(InteractAction::RIGHT_CLICK, mc.getLocalPlayer()->getRuntimeID(), abovePos.toFloat());

			mc.getClientInstance()->loopbackPacketSender->sendToServer(&inter);
			gm->buildBlock(abovePos,Math::random(0,5),false);
			buildobbyfirst = true;
			buildtntseccond = false;
			touchwithmagicsword = false;
			if (shouldSwitchSword) {
				plrInv->selectedSlot = oldSlotSword;
			}

		}
 */