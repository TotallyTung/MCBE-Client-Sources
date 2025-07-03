#include "Surround.h"

// #include "../../DrawUtils.h"
bool instants = false;
Surround::Surround() : IModule(0, Category::COMBAT, "Automatically makes a hole to surround u for cpvp")
{
	registerEnumSetting("Attack", &attack, 0, "Attack crystal to place obby");
	attack.addEntry("Off", 0);
	attack.addEntry("Always", 1);
	attack.addEntry("Smart", 2);
	registerFloatSetting("Attack Health", &attackHealth, attackHealth, 0.f, 20.f, 0.1f, "If health is less than this then we wont attack crystal");
	registerEnumSetting("Toggle", &toggle, 0, "How we should toggle surround");
	toggle.addEntry("Normal", 0);
	toggle.addEntry("onSneak", 1);
	registerBoolSetting("Instant", &instants, instants, "Place the blocks instantly");
	registerBoolSetting("JumpDisable", &this->jumpDisable, false, "Disable when you jump");
	registerBoolSetting("Swap", &this->citySwitch, true, "Will switch to obby silently");
	registerBoolSetting("AirPlace", &this->doAirplace, false, "Should place on air");
	registerBoolSetting("Center", &this->center, this->center, "Automatically centers you");
	registerBoolSetting("Place Button", &this->placeAButton, this->placeAButton, "Places a button to stop people from anvilling you(or sanding)");
	registerEnumSetting("Render", &render, 0, "How the client will render placements");
	render.addEntry("None", 0);
	render.addEntry("Both", 1);
	render.addEntry("Fill", 2);
	render.addEntry("Box", 3);
	registerFloatSetting("Opacity", &ropacity, 0.4f, 0.f, 1.f, 0.1f, "Opacity of render");
}

Surround::~Surround()
{
}

const char *Surround::getRawModuleName()
{
	return "Surround";
}
const char* Surround::getModName()
{
	if (toggle.getSelectedValue() ==1)
		name = " Sneak";
	else if (toggle.getSelectedValue() == 0)
		name = " Normal";
	return name;
}
void getObby()
{
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if (stack->getItem()->itemId == 49)
			{
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
// Find best button
void Surround::findButton() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 143) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967152) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967155) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967153) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967156) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967154) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 77) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967036) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967035) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967000) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 373) {
				supplies->selectedHotbarSlot = n;
				break;
			}
		}
	}
}
bool Surround::isSideEmpty()
{
	bool sendBack = false;

	for (auto checkThis : sidesBlocksOnly)
	{
		auto playerOn = g_Data.getLocalPlayer()->getPos();
		auto rndOffPlayerOn = playerOn->floor();
		rndOffPlayerOn.y -= 1.f;

		auto calcSides = rndOffPlayerOn.add(checkThis);
		if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(calcSides))->toLegacy()->blockId == 0)
		{
			sendBack = true;
		}
	}
	if (sendBack == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Surround::onMove(C_MoveInputHandler *hand)
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (jumpDisable && hand->isJumping)
		this->setEnabled(false);
	if (toggle.getSelectedValue() == 1 && hand->isSneakDown)
		isSneaked = true;
	else if (toggle.getSelectedValue() == 1 && !hand->isSneakDown)
		isSneaked = false;
}
bool tryButton(vec3_t blkPlacement) {
	blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = block->blockLegacy;
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static const std::vector<vec3_ti> checklist = {
			//vec3_ti(1, -1, 0),
			vec3_ti(0, -1, 0),
			vec3_ti(0, 1, 0),
			vec3_ti(0, 0, -1),
			vec3_ti(0, 0, 1),
			vec3_ti(-1, 0, 0),
			vec3_ti(1, 0, 0) };

		bool foundCandidate = false;

		for (const auto& current : checklist) {
			vec3_ti calc = blok.sub(current);
			bool Y = (g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy)->material->isReplaceable;
			if (!(g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy)->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}

		if (foundCandidate && !instants) {
			g_Data.getCGameMode()->buildBlock(&blok, i);
			return true;
		}
		else if (foundCandidate && instants) {
			g_Data.getCGameMode()->buildBlock(&blok, 0);
			return true;
		}
	}

	return false;
}
bool tryButtonBelow(const vec3_t& blockPos) {
	vec3_t blockPosBelow1(blockPos.x - 0, blockPos.y - 1, blockPos.z - 0);
	vec3_t blockPosBelow(blockPos.x - 0, blockPos.y - 2, blockPos.z - 0);
	if (tryButton(blockPosBelow1))
		return true;
	if (tryButton(blockPosBelow))
		return true;
	return false;
}
bool tryPlace(vec3_t blkPlacement)
{
	blkPlacement = blkPlacement.floor();

	C_Block *block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy *blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable)
	{
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti *> checklist;
		bool foundCandidate = false;
		if (checklist.empty())
		{
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist)
		{
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable)
			{
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate)
		{
			g_Data.getCGameMode()->buildBlock(&blok, i); // if it breaks, then idk
			return true;
		}
	}
	return false;
}
void Surround::attackCrystal() {
	g_Data.forEachEntity([](C_Entity* ent, bool b)
		{
			int id = ent->getEntityTypeId();
			auto playerHealth = g_Data.getLocalPlayer()->getHealth();
			auto minAttack = moduleMgr->getModule<Surround>()->attackHealth;
			auto mode = moduleMgr->getModule<Surround>()->attack;
			if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= 3.5) {
				if (mode.getSelectedValue() == 2 && minAttack < playerHealth) {
					g_Data.getCGameMode()->attack(ent);
					g_Data.getLocalPlayer()->swingArm();
				}
				else if (mode.getSelectedValue() == 1) {
					g_Data.getCGameMode()->attack(ent);
					g_Data.getLocalPlayer()->swingArm();
				}
			}
		}
	);
	return;
}
void Surround::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;
}

bool doReset = true;
std::vector<vec3_t> surrArr;

void Surround::onTick(C_GameMode *gm)
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!isSneaked && toggle.getSelectedValue() == 1)
		return;

	pn = false;
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;
	slotOB = supplies->selectedHotbarSlot;
	vec3_t playerFeetPos = g_Data.getLocalPlayer()->getPos()->floor();
	vec3_t buttonPos = vec3_t(playerFeetPos.x, playerFeetPos.y - 1, playerFeetPos.z);
	if (citySwitch)
		getObby();
	if (surrArr.empty())
	{
		doReset = true;
	}
	else
	{
		doReset = false;
	}
	if (doReset == true)
	{
		surrArr.clear();

		for (auto surroundBlock : regularBlocks)
		{
			auto playerPosition = g_Data.getLocalPlayer()->getPos();
			auto flooredPos = playerPosition->floor();
			flooredPos.y -= 1.f;
			auto mathsBlockPos = flooredPos.add(surroundBlock);

			if (g_Data.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->blockId == 0) // if that *block* empty, don't place blok
				surrArr.push_back(surroundBlock);
		}
	}

	// LOCK PLAYER POS
	vec3_t yR = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
	yR.x += 0.5f;
	yR.z += 0.5f;
	bool pressed = MoveUtil::keyPressed();
	if (center && !pressed)
	{
		g_Data.getLocalPlayer()->setPos(yR);
	}
	if (attack.getSelectedValue() != 0)
		attackCrystal();
	// GET BUILD BLOCK
	if (isSideEmpty())
	{
		pn = true;
		for (vec3_t surroundBlock : surrArr)
		{
			auto playerPosition = g_Data.getLocalPlayer()->getPos();
			auto flooredPos = playerPosition->floor();
			flooredPos.y -= 1.f;
			auto mathsBlockPos = flooredPos.add(surroundBlock);
			if (doAirplace)
			{
				if (attack.getSelectedValue() != 0)
					attackCrystal();
				gm->buildBlock(&vec3_ti(mathsBlockPos), 0); // ah so this causes the airplace
			}
			else if (!doAirplace)
			{
				if (attack.getSelectedValue() != 0)
					attackCrystal();
				tryPlace(mathsBlockPos);
			}
			surrArr.erase(surrArr.begin());
			break;
		}
	}
	// Prioritize surround before switching to button
	if (citySwitch && placeAButton)
		findButton();
	if (placeAButton)
	tryButtonBelow(playerFeetPos);
	if (citySwitch)
		supplies->selectedHotbarSlot = slotOB;
}

void Surround::onPreRender(C_MinecraftUIRenderContext *renderCtx)
{
	auto interfacekk = ColorUtil::interfaceColor(1);
	if (g_Data.getLocalPlayer() == nullptr)
		return;
		for (auto surroundBlock : regularBlocks)
		{
			DrawUtils::setColor(interfacekk.r, interfacekk.g, interfacekk.b, ropacity);

			auto playerPosition = g_Data.getLocalPlayer()->getPos();

			auto flooredPos = playerPosition->floor();
			flooredPos.y -= 1.f;

			auto mathsBlockPos = flooredPos.add(surroundBlock);
			auto mathsBlockPos2 = mathsBlockPos.add(0.5, 0.5, 0.5);

			auto vec1 = vec3_t(1.f, 0.2f, 1.f);

			// Sneak only off
			if (g_Data.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->blockId == 0 && toggle.getSelectedValue() != 1) {
				// Box and Both
				if (render.getSelectedValue() == 1 || render.getSelectedValue() == 3)
				DrawUtils::drawBox(mathsBlockPos.sub(0, 0, 0), mathsBlockPos.add(1, 1, 1), .5f, false);
				if (render.getSelectedValue() == 1 || render.getSelectedValue() == 2)
					DrawUtils::drawFilledBox(mathsBlockPos2);
			}
			// Sneak only on
			if (g_Data.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->blockId == 0 && toggle.getSelectedValue() == 1 && isSneaked) {
				// Box and Both
				if (render.getSelectedValue() == 1 || render.getSelectedValue() == 3)
					DrawUtils::drawBox(mathsBlockPos.sub(0, 0, 0), mathsBlockPos.add(1, 1, 1), .5f, false);
				if (render.getSelectedValue() == 1 || render.getSelectedValue() == 2)
					DrawUtils::drawFilledBox(mathsBlockPos2);
			}
	}
}
