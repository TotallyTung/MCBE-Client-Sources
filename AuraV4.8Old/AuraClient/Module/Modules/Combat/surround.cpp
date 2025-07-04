#include "Surround.h"

//#include "../../DrawUtils.h"

Surround::Surround() : IModule(0, Category::COMBAT, "Protect yourself from crystals :)") {
	registerBoolSetting("Jump-disable", &this->jumpDisable, false);
	registerBoolSetting("City-Switch", &this->citySwitch, true);
	registerBoolSetting("Show placements", &this->renderSurround, true);
	registerBoolSetting("AirPlace", &this->doAirplace, false);
	registerBoolSetting("Center", &this->center, this->center);
}

Surround::~Surround() {
}

const char* Surround::getModuleName() {
	return "Surround";
}
/*
bool Surround::isHoldMode() {
	return false;
}
*/

void getObby() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {  // select obsid
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}


bool Surround::isSideEmpty() {
	bool sendBack = false;

	for (auto checkThis : sidesBlocksOnly) {
		auto playerOn = g_Data.getLocalPlayer()->getPos();
		auto rndOffPlayerOn = playerOn->floor();
		rndOffPlayerOn.y -= 1.f;

		auto calcSides = rndOffPlayerOn.add(checkThis);
		if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(calcSides))->toLegacy()->blockId == 0) {
			sendBack = true;
		}
	}
	if (sendBack == true) {
		return true;
	} else {
		return false;
	}
}

void Surround::onMove(C_MoveInputHandler* hand) {
	if (g_Data.getLocalPlayer() == nullptr) return;

	if (jumpDisable && hand->isJumping) this->setEnabled(false);
}

bool tryPlace(vec3_t blkPlacement) {
	blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

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
			g_Data.getCGameMode()->buildBlock(&blok, i); // if it breaks, then idk
			return true;
		}
	}
	return false;
}

void Surround::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) return;
	origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	if (!citySwitch)
		getObby();
}

bool doReset = true;
std::vector<vec3_t> surrArr;
bool stopSwitch = false;

void Surround::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) return;

	pn = false;


	if (citySwitch && stopSwitch && g_Data.getLocalPlayer()->getSelectedItemId() != 49) {  // make sure that YOU actually have an obsidian in your hand before placing -> allows you to multitask eat, crystal, mining etc..
		getObby();
		return;
	}

	if (citySwitch && isSideEmpty() && !stopSwitch) {  // auto grab blocks when side empty/broken

		//clientMessageF(" You have been citied! Replacing block...");
		origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
		getObby();
		stopSwitch = true;
		return;
	}

	if (surrArr.empty()) {
		doReset = true;
	} else {
		doReset = false;
	}
	if (doReset == true) {
		surrArr.clear();

		for (auto surroundBlock : regularBlocks) {
			auto playerPosition = g_Data.getLocalPlayer()->getPos();
			auto flooredPos = playerPosition->floor();
			flooredPos.y -= 1.f;
			auto mathsBlockPos = flooredPos.add(surroundBlock);

			if (g_Data.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->blockId == 0)  // if that *block* empty, don't place blok
				surrArr.push_back(surroundBlock);
		}
	}

	// LOCK PLAYER POS
	vec3_t yR = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	yR.x += 0.5f;
	yR.z += 0.5f;
	if (center) {
		g_Data.getLocalPlayer()->setPos(yR);
	}

	// GET BUILD BLOCK
	if (isSideEmpty()) {
		pn = true;
		for (vec3_t surroundBlock : surrArr) {
			auto playerPosition = g_Data.getLocalPlayer()->getPos();
			auto flooredPos = playerPosition->floor();
			flooredPos.y -= 1.f;
			auto mathsBlockPos = flooredPos.add(surroundBlock);
			/*
			if (rotate.GetSelectedEntry().GetValue() == 1 || rotate.GetSelectedEntry().GetValue() == 2) {  // regular rotations
				rotAngle = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(mathsBlockPos);
				if (rotate.getSelectedValue() == 1) {
					gm->player->yawUnused1 = rotAngle.y;
					gm->player->pitch = rotAngle.x;
					gm->player->bodyYaw = rotAngle.y;
				}
			}
			*/
			if (doAirplace) {
				gm->buildBlock(&vec3_ti(mathsBlockPos), 0);  // ah so this causes the airplace
			} else if (!doAirplace) {
				tryPlace(mathsBlockPos);
			}
			surrArr.erase(surrArr.begin());
			break;
		}
	}
	if (citySwitch && !isSideEmpty() && stopSwitch) {
		stopSwitch = false;
		g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = origSlot;
	}
}

void Surround::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	auto interfacekk = ColorUtil::interfaceColor(1);
	if (g_Data.getLocalPlayer() == nullptr) return;

	if (renderSurround) {
		for (auto surroundBlock : regularBlocks) {
			DrawUtils::setColor(interfacekk.r, interfacekk.g, interfacekk.b, 1.f);  // 231,125,17

			auto playerPosition = g_Data.getLocalPlayer()->getPos();

			auto flooredPos = playerPosition->floor();
			flooredPos.y -= 1.f;

			auto mathsBlockPos = flooredPos.add(surroundBlock);

			auto vec1 = vec3_t(1.f, 0.2f, 1.f);

			if (g_Data.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->blockId == 0)
				DrawUtils::drawBox(mathsBlockPos, mathsBlockPos.add(1.f), 0.75f, true);
		}
	}
}

/*
void Surround::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && rotate.GetSelectedEntry().GetValue() == 2) {  // silent rotations
		if (pn) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			vec2_t angle = rotAngle;
			//clientMessageF("Rotating now lol %f %f", angle.x, angle.y);
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}
*/
