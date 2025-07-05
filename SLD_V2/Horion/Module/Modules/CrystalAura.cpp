#include "CrystalAura.h"

bool findCry() {
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

bool SetOldSlot() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_ItemStack* stack = inv->getItemStack(supplies->selectedHotbarSlot);
	if (stack->item != nullptr) {
		C_MobEquipmentPacket a(id, *stack, supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
		return true;
	}

	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}
CrystalAura::CrystalAura() : IModule(VK_NUMPAD0, Category::COMBAT, "horion good CA") {
	mode = SettingEnum(this)
			   .addEntry(EnumEntry("Distance", 0))
			   .addEntry(EnumEntry("ExplosionDamage", 1));
	registerEnumSetting("Calculation", &mode, 0);

	registerBoolSetting("MultiAura", &multiaura, multiaura);
	registerBoolSetting("SilentSwitch", &silentswitch, silentswitch);
	registerIntSetting("Delay", &delay2, delay2, 0, 10);
	registerIntSetting("BreakDistance", &range, range, 1, 10);
	registerIntSetting("Break Range", &cRange, cRange, 1, 15);
	registerIntSetting("Place range", &eRange, eRange, 1, 5);
	registerIntSetting("Detect Range", &pRange, pRange, 1, 10);
	registerBoolSetting("AutoSwitch", &AutoSelect, AutoSelect);
	registerBoolSetting("Autoplace", &autoplace, autoplace);
	registerBoolSetting("Render", &Preview, Preview);
	delay = 0;
}
CrystalAura::~CrystalAura() {
}
const char* CrystalAura::getModuleName() {
	return ("CrystalAuraNomal");
}

static std::vector<C_Entity*> targetList;

void CrystalAura::onEnable() {
	targetList.clear();
	delay = 0;
}

bool CfindEntity(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;  // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;  // falling block
	if (curEnt->getEntityTypeId() == 64) return false;  // item
	if (curEnt->getEntityTypeId() == 69) return false;  // xp orb
	if (!Target::isValidTarget(curEnt)) return false;

	float dist = (*curEnt->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist <= moduleMgr->getModule<CrystalAura>()->pRange) {
		targetList.push_back(curEnt);
		return true;
	}
	return false;
}
bool space = true;
vec3_t _pos;
bool CanPlaceC(vec3_ti* pos) {
	space = true;
	_pos = pos->toVec3t();
	g_Data.forEachEntity([](C_Entity* ent, bool b) {
		if (!space)
			return;
		if (ent->aabb.intersects(AABB(_pos, _pos.add(1.f))))
			space = false;
	});
	return space;
}

void CrystalAura::CPlace(C_GameMode* gm, vec3_t* pos) {
	vec3_ti bestPos;
	bool ValidPos = false;
	for (int x = (int)pos->x - eRange; x < pos->x + eRange; x++) {
		for (int z = (int)pos->z - eRange; z < pos->z + eRange; z++) {
			for (int y = (int)pos->y - eRange; y < pos->y + eRange; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				vec3_ti upperBlockPos = vec3_ti(x, y + 1, z);
				C_Block* block = gm->player->region->getBlock(blockPos);
				C_Block* upperBlock = gm->player->region->getBlock(upperBlockPos);
				if (block != nullptr) {
					auto blockId = block->toLegacy()->blockId;
					auto upperBlockId = upperBlock->toLegacy()->blockId;
					if ((blockId == 49 || blockId == 7) && upperBlockId == 0 && CanPlaceC(&blockPos)) {  //Check for awailable block
						bool bestPosSelected = mode.selected == 0 ? (blockPos.toVec3t().dist(*pos) < bestPos.toVec3t().dist(*pos)) : (getExplosionDamage(blockPos.toVec3t(), *pos) < getExplosionDamage(bestPos.toVec3t(), *pos));

						if (!ValidPos) {
							ValidPos = true;
							bestPos = blockPos;
						} else if (bestPosSelected) {
							bestPos = blockPos;
						}
					}
				}
			}
		}
	}
	if (ValidPos) {
		bool idk = true;
		if (silentswitch) findCry();

		gm->buildBlock(&bestPos, 0, idk);
		getRenderPositon = bestPos;
		if (silentswitch) SetOldSlot();
	}
}

void CrystalAura::DestroyC(C_Entity* ent, int range) {
	for (auto& i : targetList)
		if (ent->getPos()->dist(*i->getPos()) < range) {
			g_Data.getCGameMode()->attack(ent);
			g_Data.getLocalPlayer()->swingArm();
			return;
		}
}

bool shouldChange = false;
void CrystalAura::onTick(C_GameMode* gm) {
	if (!GameData::canUseMoveKeys())  // TODO: Find out if 2b2e.org kicks you for placing in inventory
		return;

	if (shouldChange) {
		shouldChange = false;
	}
	delay++;
	if (supplies == nullptr)
		supplies = g_Data.getLocalPlayer()->getSupplies();
	if (inv == nullptr)
		inv = supplies->inventory;
	targetList.clear();
	g_Data.forEachEntity(CfindEntity);
	if (delay == 0) {
		// place block around players?
		return;
	}

	if (AutoSelect) {
		prevSlot = supplies->selectedHotbarSlot;
		FinishSelect = true;
		for (int n = 0; n < 9; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 637) {
					if (prevSlot != n)
						supplies->selectedHotbarSlot = n;
					return;
				}
			}
		}
		return;
	}
	if (delay == delay2 + 1) {
		if (autoplace || g_Data.getLocalPlayer()->getSelectedItemId() == 637) {  // endcrystal
			for (auto& i : targetList)
				if (multiaura)
					CPlace(gm, &i->getPos()->add(0, -1, 0));  // iを使うことによって複数の相手にCrystalAuraをすることができます
				else
					CPlace(gm, &targetList[0]->getPos()->add(0, -1, 0));  // シンプルな単体のターゲットにクリスタルをおきます。
			if (targetList.empty()) getRenderPositon = vec3_ti(0, 0, 0);    //これは0,0,0にある場合はレンダリングしないので、レンダリングをしないよう、0,0,0にしています
		}
		return;
	}
	if (FinishSelect && AutoSelect && targetList.empty()) {
		supplies->selectedHotbarSlot = prevSlot;
		FinishSelect = false;
		return;
	}
	g_Data.forEachEntity([](C_Entity* ent, bool b) {
		if (ent->getEntityTypeId() != 71)
			return;
		int range;
		if (moduleMgr->getModule<CrystalAura>()->dEnhanced)
			range = moduleMgr->getModule<CrystalAura>()->cRange;
		else
			range = moduleMgr->getModule<CrystalAura>()->range;
		moduleMgr->getModule<CrystalAura>()->DestroyC(ent, range);
	});

	if (delay >= delay2 + 2) {
		delay = 0;
		return;
	}
}

void CrystalAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!Preview ||
		g_Data.getClientInstance() == nullptr ||
		g_Data.getPtrLocalPlayer() == nullptr ||
		g_Data.getLocalPlayer() == nullptr)
		return;

	if (getRenderPositon != vec3_ti(0, 0, 0)) {
		if (g_Data.getLocalPlayer()->region->getBlock(getRenderPositon)->toLegacy()->blockId == 49 ||
			g_Data.getLocalPlayer()->region->getBlock(getRenderPositon)->toLegacy()->blockId == 7) {
			DrawUtils::setColor(.75f, .25f, .5f, 1.f);
			DrawUtils::drawBox(getRenderPositon.toVec3t().add(0.f, 0.f, 0.f),
							   getRenderPositon.add(1).toVec3t().add(0.f, 0.f, 0.f), .3f);
		}
	}
}
void CrystalAura::onDisable() {
	delay = 0;
}