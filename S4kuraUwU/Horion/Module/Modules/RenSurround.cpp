#include "RenSurround.h"

#include "../../DrawUtils.h"
int prevSlot;
vec3_t* center_block;
int y_level = 0;

int tick_runs = 0;

int offset_step = 0;

int old_slot = -1;
RenSurround::RenSurround() : IModule(0, Category::COMBAT, "RenSurrounds you with Obsidian") {
	enum1 = SettingEnum(this)
				.addEntry(EnumEntry("Anti-City", 1))
				.addEntry(EnumEntry("Regular", 2))
				.addEntry(EnumEntry("Java", 3));
	enum3 = SettingEnum(this)
			   .addEntry(EnumEntry("None", 1))
			   .addEntry(EnumEntry("Regular", 2))
			   .addEntry(EnumEntry("Java", 3))
			   .addEntry(EnumEntry("PitchUp", 4));
	enum2 = SettingEnum(this)
				.addEntry(EnumEntry("Normal", 1))
				.addEntry(EnumEntry("Aurora", 2));
	registerEnumSetting("Mode", &enum1, 2);
	registerEnumSetting("Rotations", &enum3, 1);
	registerBoolSetting("Swap", &this->spoofC, true);
	registerEnumSetting("Swap", &enum2, 2);
	registerBoolSetting("Render", &this->renderRenSurround, true);
	registerBoolSetting("JumpDisable", &this->jumpDisable, false);
	registerBoolSetting("AutoCenter", &this->center, true);
	registerBoolSetting("AntiFacePlace", &this->block_head, false);
	registerBoolSetting("DisableComplete", &this->disableOnComplete2, true);
	registerBoolSetting("AntiAnvil", &this->antianvil, true);
	registerBoolSetting("Packet", &this->triggerable, true);
	registerBoolSetting("SelfWeb", &this->selfweb, true);
	registerIntSetting("SwapDelay", &this->delay2, this->delay2, 1, 10);
	registerFloatSetting("Blocks Per Tick", &this->tick_for_place, this->tick_for_place, 1, 12);
	registerFloatSetting("Ticks til timeout", &this->tick_timeout, this->tick_timeout, 10, 50);

}

RenSurround::~RenSurround() {
}

const char* RenSurround::getModuleName() {
	return "RenSurround";
}
const char* RenSurround::getModName() {
	if (java) {
		return " [Java]";
	}
	if (triggerable) {
		return " [Packet]";
	}
	if (vanilla) {
		return " [Nukkit]";
	}
	if (anticity) {
		return " [Anticity]";
	} else {
		return "";
	}
}
int slot = false;

bool RenSurround::isFlashMode() {
	return false;
}
	bool RenSurround::tryAirPlace(vec3_t blockBelow) {
	blockBelow = blockBelow.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);

		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}

		if (foundCandidate) {
			if (spoofC) Spoof();
			bool idk = true;
			g_Data.getCGameMode()->buildBlock(&blok, i, idk);

			return true;
		}
	}
	return false;
}
	bool RenSurround::tryScaffold(vec3_t blockBelow) {
		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

		if (input == nullptr)
			return false;

		blockBelow = blockBelow.floor();

		C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
		C_BlockLegacy* blockLegacy = (block->blockLegacy);
		if (blockLegacy->material->isReplaceable) {
			vec3_ti blok(blockBelow);

			// Find neighbour
			static std::vector<vec3_ti*> checklist;
			if (checklist.empty()) {
				checklist.push_back(new vec3_ti(0, -1, 0));
				checklist.push_back(new vec3_ti(0, 1, 0));

				checklist.push_back(new vec3_ti(0, 0, -1));
				checklist.push_back(new vec3_ti(0, 0, 1));

				checklist.push_back(new vec3_ti(-1, 0, 0));
				checklist.push_back(new vec3_ti(1, 0, 0));
			}
			bool foundCandidate = false;
			int i = 0;
			for (auto current : checklist) {
				vec3_ti calc = blok.sub(*current);
				if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
					// Found a solid block to click
					foundCandidate = true;
					blok = calc;
					break;
				}
				i++;
			}

			return true;
		}
		return false;
	}
	int switchCooldown69 = 0;
int previousSlot69 = -1;

bool RenSurround::Spoof() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();

	int prevSlot = supplies->selectedHotbarSlot;

	auto funny = supplies->inventory->getItemStack(prevSlot);
	if (funny->item != nullptr) {
		if (strcmp(funny->getItem()->tileName.getText(), "item.obsidian") == 0)  // Try blocking that nukkit
			supplies->selectedHotbarSlot = prevSlot;

		for (auto i = 0; i < 9; i++) {
			C_ItemStack* item = supplies->inventory->getItemStack(i);

			if (item->item != nullptr) {
				if (strcmp(item->getItem()->tileName.getText(), "item.obsidian") == 0) {
					supplies->selectedHotbarSlot = i;
					switchCooldown69 = 2;
					previousSlot69 = prevSlot;
					return true;
				}
			}
		}

		return false;
	}
}

vec3_t get_center(double posX, double posY, double posZ) {
	vec3_t xyz = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	xyz.x += 0.5f;
	xyz.z += 0.5f;
	g_Data.getLocalPlayer()->setPos(xyz);
	return vec3_t(xyz.x, xyz.y, xyz.z);
}

bool RenSurround::AAnvilSwap() {
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 373) {
				g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = n;
				return true;
			}
		}
	}
}

bool RenSurround::tryAntiAnvil(vec3_t blockBelow) {
	blockBelow = blockBelow.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);

		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			if (AntiAnvilSwap) AAnvilSwap();
			bool idk = true;
			g_Data.getCGameMode()->buildBlock(&blok, i, idk);

			return true;
		}
	}
	return false;
}

bool RenSurround::tryPlace33(vec3_t blockBelow) {
	blockBelow = blockBelow.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);

		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			if (spoofC) Spoof();
			bool idk = true;
			g_Data.getCGameMode()->buildBlock(&blok, i, idk);

			return true;
		}
	}
	return false;
}
void RenSurround::onEnable() {
	old_slot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	if (Spoof() == -1) {
		this->setEnabled(false);
		g_Data.getGuiData()->displayClientMessageF("[%sRenSurround%s] %sNo Obby%s!", GREEN, WHITE, GREEN, GRAY, GRAY);
		return;
	}

	if (g_Data.getLocalPlayer() != nullptr) {
		y_level = (int)round(g_Data.getLocalPlayer()->getPos()->y);

		center_block = &get_center(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
		vec3_t moveVec;

		if (center) {
			moveVec.x = 0;
			moveVec.z = 0;
		}
	}


	if (g_Data.getLocalPlayer() != nullptr) {
		y_level = (int)round(g_Data.getLocalPlayer()->getPos()->y);

		center_block = &get_center(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->y, g_Data.getLocalPlayer()->getPos()->z);
		vec3_t moveVec;

		if (center) {
			moveVec.x = 0;
			moveVec.z = 0;
		}
	}
}

void RenSurround::onDisable() {
	g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
}
bool doResetR = true;
bool iFuckedUp2 = false;
int tDelay2 = 0;
std::vector<vec3_t> surrArrR;

bool Packet(int ne) {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)) {
				C_MobEquipmentPacket a(id, *stack, ne, ne);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}

void RenSurround::onTick(C_GameMode* gm) {
	mustPitchUp = false;

	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	switch (enum2.selected) {
	case 0:
		blockSwap = false;
		AntiAnvilSwap = false;
		break;
	case 1:
		blockSwap = true;
		AntiAnvilSwap = true;
		break;
	}
	switch (enum3.selected) {
	case 0:
		rot1 = false;
		rotate = false;
		break;
	case 1:
		rot1 = true;
		rotate = false;
		break;
	case 2:
		rot1 = false;
		rotate = true;
		break;
	//case 3:
	//	mustPitchUp = true;
	}
	switch (enum1.selected) {
	case 0:
		vanilla = false;
		java = false;
		anticity = true;
		break;
	case 1:
		vanilla = true;
		java = false;
		anticity = false;
		break;
	case 2:
		vanilla = false;
		java = true;
		anticity = false;
		break;
	}
	{}

			vec3_t moveVec;
	if (gm->player != nullptr) {
		if (center_block != 0 && center) {
			double x_diff = abs(center_block->x - g_Data.getLocalPlayer()->getPos()->x);
			double z_diff = abs(center_block->z - g_Data.getLocalPlayer()->getPos()->z);
		}

		if ((int)round(g_Data.getLocalPlayer()->getPos()->y) != y_level) {
			this->setEnabled(false);
			return;
		}

		if (!triggerable && tick_runs >= tick_timeout) {  // timeout time
			tick_runs = 0;
			this->setEnabled(false);
			return;
		}

		int blocks_placed = 0;

		while (blocks_placed < tick_for_place) {
			if (offset_step >= (block_head ? RenSurround_targets_face.size() : RenSurround_targets.size())) {
				offset_step = 0;
				break;
			}

			auto playerPosition = g_Data.getLocalPlayer()->getPos();
			auto flooredPos = playerPosition->floor();
			flooredPos.y -= 1.f;
			vec3_t* offsetPos = new vec3_t(block_head ? RenSurround_targets_face[offset_step] : RenSurround_targets[offset_step]);
			vec3_t* targetPos = new vec3_t(flooredPos.add(offsetPos->x, offsetPos->y, offsetPos->z));
			bool destroy = false;

			bool try_to_place = true;

			if (!g_Data.getLocalPlayer()->region->getBlock(*targetPos)->blockLegacy->material->isReplaceable) {
				try_to_place = false;
			}
			if (enum3.GetSelectedEntry().GetValue() == 3) {
				mustPitchUp = true;
			}
			if (rotate) {
				auto player = g_Data.getLocalPlayer();
				player->pitch = offsetPos->z;
				player->bodyYaw = offsetPos->x;
			}
			vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
			blockBelow.y -= g_Data.getLocalPlayer()->height;
			blockBelow.y -= 0.5f;
			if (this->rot1) {
				for (int i = 0; i < 40; i++) {
					auto player = g_Data.getLocalPlayer();
					player->pitch = blockBelow.y;
					player->bodyYaw = blockBelow.y;
				}
		}
			offset_step++;
			g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = old_slot;
		}

		tick_runs++;
	}
	g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = old_slot;

	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	if (g_Data.getLocalPlayer() == nullptr) return;

	if (surrArrR.empty()) {
		doResetR = true;
	} else {
		doResetR = false;
	}
	if (doResetR == true) {
		surrArrR.clear();

		if (enum1.selected == 1)
			for (auto RenSurroundBlock : regularBlocks) {
				auto playerPosition = g_Data.getLocalPlayer()->getPos();
				auto flooredPos = playerPosition->floor();
				flooredPos.y -= 1.f;
				auto mathsBlockPos = flooredPos.add(RenSurroundBlock);

				if (g_Data.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->blockId == 0)
					surrArrR.push_back(RenSurroundBlock);
			}
		if (enum1.selected == 0)
			for (auto RenSurroundBlock : antiCityBlocks) {
				auto playerPosition = g_Data.getLocalPlayer()->getPos();
				auto flooredPos = playerPosition->floor();
				flooredPos.y -= 1.f;
				auto mathsBlockPos = flooredPos.add(RenSurroundBlock);

				if (g_Data.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->blockId == 0)
					surrArrR.push_back(RenSurroundBlock);
			}
	}
	// cant be arsed looking at this rn

	//	if (enum1.selected == 1) {
	for (auto RenSurroundBlock : surrArrR) {
		auto playerPosition = g_Data.getLocalPlayer()->getPos();
		auto flooredPos = playerPosition->floor();
		flooredPos.y -= 1.f;
		auto mathsBlockPos = flooredPos.add(RenSurroundBlock);

		if (!tryAntiAnvil(mathsBlockPos));
		surrArrR.erase(surrArrR.begin());
		break;
	}
	//}
	if (blockSwap) {
		g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
		iFuckedUp2 = false;
	}

	if (surrArrR.empty() && disableOnComplete2) this->setEnabled(false);

	if (KindaSpoof) {
		if (surrArrR.empty()) g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
	}

	if (blockSwap) {
		Odelay2++;
		if (Odelay2 > delay2 * 30) {
			int slot = 1;
			for (int n = 0; n < 9; n++) {
				C_ItemStack* stack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(n);
				if (stack->item != nullptr) {
					if (stack->getItem()->itemId == 49) {
						slot = n;
						g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = n;
						delay2 = 0;
					}
	if (selfweb) {
			for (int n = 0; n < 9; n++) {
				C_ItemStack* stack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(n);
				if (stack->item != nullptr) {
					if (stack->getItem()->itemId == 30) {
									g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = n;
								}
							}
				int place = 1;

							vec3_t blockSide = g_Data.getLocalPlayer()->eyePos0;

							blockSide.y -= g_Data.getLocalPlayer()->height - 1;

							if (!tryScaffold(blockSide)) {
								blockSide.y -= 0;
								blockSide.x -= 0;
							}
			}
					}
				}
			}
		}
	}

	if (antianvil) {
		vec3_t blockSide1 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide2 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide3 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide4 = g_Data.getLocalPlayer()->eyePos0;

		blockSide.y -= g_Data.getLocalPlayer()->height - 1;

		if (!tryAntiAnvil(blockSide)) {
			blockSide.y -= 0;
		}
	}

	if (enum1.selected == 2) {
		vec3_t blockSide1 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide2 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide3 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide4 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide12 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide23 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide34 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSide45 = g_Data.getLocalPlayer()->eyePos0;

		vec3_t blockSideL1M = g_Data.getLocalPlayer()->eyePos0;

		blockSide1.x -= g_Data.getLocalPlayer()->height - 1;
		blockSide1.y -= g_Data.getLocalPlayer()->height - 1;

		blockSide2.x += g_Data.getLocalPlayer()->height - 1;
		blockSide2.y -= g_Data.getLocalPlayer()->height - 1;

		blockSide3.z -= g_Data.getLocalPlayer()->height - 1;
		blockSide3.y -= g_Data.getLocalPlayer()->height - 1;

		blockSide4.z += g_Data.getLocalPlayer()->height - 1;
		blockSide4.y -= g_Data.getLocalPlayer()->height - 1;

		blockSide12.x -= g_Data.getLocalPlayer()->height - 1;
		blockSide12.y -= g_Data.getLocalPlayer()->height - 0;

		blockSide23.x += g_Data.getLocalPlayer()->height - 1;
		blockSide23.y -= g_Data.getLocalPlayer()->height - 0;

		blockSide34.z -= g_Data.getLocalPlayer()->height - 1;
		blockSide34.y -= g_Data.getLocalPlayer()->height - 0;

		blockSide45.z += g_Data.getLocalPlayer()->height - 1;
		blockSide45.y -= g_Data.getLocalPlayer()->height - 0;

		blockSideL1M.y -= g_Data.getLocalPlayer()->height - 1;

		blockSideL1M.y -= g_Data.getLocalPlayer()->height - 1;

		blockSideL1M.y -= g_Data.getLocalPlayer()->height - 1;

		blockSideL1M.y -= g_Data.getLocalPlayer()->height - 1;

		blockSideL1M.y -= g_Data.getLocalPlayer()->height - 1;

		if (!tryAirPlace(blockSide1)) {
			blockSide1.y -= 0;
			blockSide1.x -= 0;
		}

		if (!tryAirPlace(blockSide2)) {
			blockSide2.y += 0;
			blockSide2.x += 0;
		}

		if (!tryAirPlace(blockSide3)) {
			blockSide3.y -= 0;
			blockSide3.z -= 0;
		}

		if (!tryAirPlace(blockSide4)) {
			blockSide4.y += 0;
			blockSide4.z += 0;
		}

		if (!tryAirPlace(blockSide12)) {
			blockSide12.y -= 0;
			blockSide12.x -= 0;
		}

		if (!tryAirPlace(blockSide23)) {
			blockSide23.y += 0;
			blockSide23.x += 0;
		}

		if (!tryAirPlace(blockSide34)) {
			blockSide34.y -= 0;
			blockSide34.z -= 0;
		}

		if (!tryAirPlace(blockSide45)) {
			blockSide45.y += 0;
			blockSide45.z += 0;
		}
		if (!tryAirPlace(blockSideL1M)) {
			blockSideL1M.y -= 0;
		}
	}
}

void RenSurround::onMove(C_MoveInputHandler* hand) {
	if (g_Data.getLocalPlayer() == nullptr) return;

	if (jumpDisable && hand->isJumping) this->setEnabled(false);
}

void RenSurround::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr) return;

	if (renderRenSurround) {
		if (enum1.selected == 1) {
			for (auto RenSurroundBlock : regularBlocks) {
				DrawUtils::setColor(rSelect2, gSelect2, bSelect2, 1);

				auto playerPosition = g_Data.getLocalPlayer()->getPos();

				auto flooredPos = playerPosition->floor();
				flooredPos.y -= 1.f;

				auto mathsBlockPos = flooredPos.add(RenSurroundBlock);

				auto vec1 = vec3_t(1.f, 0.2f, 1.f);

				if (g_Data.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->blockId == 0)
					DrawUtils::drawBox(mathsBlockPos, mathsBlockPos.add(vec1), 1.f, true);
			}

		} else if (enum1.selected == 0) {
			for (auto RenSurroundBlock : antiCityBlocks) {
				DrawUtils::setColor(rSelect2, gSelect2, bSelect2, 1);

				auto playerPosition = g_Data.getLocalPlayer()->getPos();

				auto flooredPos = playerPosition->floor();
				flooredPos.y -= 1.f;

				auto mathsBlockPos = flooredPos.add(RenSurroundBlock);

				auto vec1 = vec3_t(1.f, 0.2f, 1.f);

				if (g_Data.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->blockId == 0)
					DrawUtils::drawBox(mathsBlockPos, mathsBlockPos.add(vec1), 1.f, true);
			}

		} else {
			for (auto RenSurroundBlock : regularBlocks) {
				DrawUtils::setColor(rSelect2, gSelect2, bSelect2, 1);

				auto playerPosition = g_Data.getLocalPlayer()->getPos();

				auto flooredPos = playerPosition->floor();
				flooredPos.y -= 1.f;

				auto mathsBlockPos = flooredPos.add(RenSurroundBlock);

				auto vec1 = vec3_t(1.f, 0.2f, 1.f);

				if (g_Data.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->blockId == 0)
					DrawUtils::drawBox(mathsBlockPos, mathsBlockPos.add(vec1), 1.f, true);
			}
		}
	}
}
