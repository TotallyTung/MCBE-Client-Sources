#pragma once

class AutoBow : public Module
{
public:
	AutoBow(int keybind = Keys::NONE, bool enabled = false) :
		Module("AutoBow", "Combat", "a automatical bow shot (only works when Killaura is enabled)", keybind, enabled)
	{
		addSlider("Target Range", "The max range within target", &maxRange, 0, 10, 0.1);
		addSlider("SPS", "set the shot per seconds", &shotPerSec, 0.5, 20, 0.1);
		addBool("Only Hotbar", "Only use a bow in hotbar", &onlyHotbar);
	}
	// Settings
	float maxRange = 5;
	float shotPerSec = 0.7;
	bool onlyHotbar = true;

	// Status
	bool nowPreparing = false;
	void onEnabled() override {
		nowPreparing = false;
	}
	void onDisabled() override {
		nowPreparing = false;
	}
	void onEvent(ActorBaseTickEvent* event) override {
		auto player = Address::getLocalPlayer();
		if (player == nullptr) return;
		auto killaura = (Aura*)getModuleByName("Killaura");
		if (!killaura->isEnabled()) return;
		auto target = getNearestPlayer(killaura->targetList, player);
		if (target == nullptr) return;
		auto bow = getBowStack(player, onlyHotbar);
		if (bow == nullptr) return;
		if (nowPreparing) {
			if (TimeUtils::hasTimeElapsed("autobowTimer", 1 / shotPerSec * 1000, true)) {
				auto localPos = player->getStateVector()->Position;
				auto targetPos = target->getStateVector()->Position;
				if (localPos.distance(targetPos) > maxRange) return;
				auto angle = killaura->CalcAngle(localPos, targetPos);
				player->setPitch(angle.x);
				player->setHeadYaw(angle.y);
				player->setBodyRotation(angle.y);
				player->getGamemode()->releaseUsingItem();
			}
		} else {
			player->getGamemode()->baseUseItem(*bow);
			nowPreparing = true;
			TimeUtils::resetTime("autobowTimer");


			player->getGamemode()->baseUseItem(*bow);
			nowPreparing = false;
		}
	}
	void onEvent(PacketEvent* event) override {
		if (!nowPreparing) return;
		if (event->Packet->getId() == PacketID::InventoryTransaction) {
			auto player = Address::getLocalPlayer();
			if (player == nullptr) return;
			auto pkt = (InventoryTransactionPacket*)(event->Packet);
			if (pkt->mTransaction.get()->type == ComplexInventoryTransaction::Type::ItemUseTransaction) {
				auto transac = (ItemUseInventoryTransaction*)(pkt->mTransaction.get());
				auto bowSlot = findBowSlot(player, onlyHotbar);
				auto bowStack = getBowStack(player, onlyHotbar);
				transac->slot = (int32_t)bowSlot;
			}
			if (pkt->mTransaction.get()->type == ComplexInventoryTransaction::Type::ItemReleaseTransaction) {
				auto transac = (ItemReleaseInventoryTransaction*)(pkt->mTransaction.get());
				auto bowSlot = findBowSlot(player, onlyHotbar);
				auto bowStack = getBowStack(player, onlyHotbar);
				transac->slot = bowSlot;
				ChatUtils::sendMessage("item release pkt");
			}
		}
	}
ItemStack* getBowStack(Actor* player, bool onlyHotbar = true) {
		auto slot = findBowSlot(player, onlyHotbar);
		if (slot == -1) return nullptr;
		else return player->getSupplies()->inventory->getItem(slot);
		Misc::SpoofSwitch(slot);
	}
	int findBowSlot(Actor* player, bool onlyHotbar = true) {
		auto inv = player->getSupplies()->inventory;
		for (int slot = 0; slot < (onlyHotbar ? 9 : 36); ++slot) {
			auto stack = inv->getItem(slot);
			if (stack->item == nullptr) continue;
			if (stack->getItem()->nameContains("bow")) {
				player->getSupplies()->hotbarSlot = slot;
				return slot;
			}
		}
		return -1;
	}
	Actor* getNearestPlayer(vector<Actor*> players, Player* me) {
		if (players.size() == 0) return nullptr;
		auto localPos = me->getStateVector()->Position;
		float minRange = -1.f;
		Actor* returnValue = players[0];
		for (Actor* target : players) {
			float range = target->getStateVector()->Position.distance(localPos);
			if (minRange == -1.f || minRange > range) {
				returnValue = target;
			}
		}
		return returnValue;
	}


};