#include "PointBreak.h"

using namespace std;
PointBreak::PointBreak() : IModule(0x0, Category::COMBAT, "AutoBreak Enemy Hole/Downs/Ups (by ReaQwQ | Required Packetmine | Improved by Rusher)") {
	mode = SettingEnum(this)
		.addEntry("AutoCity", 0)
		.addEntry("TargetDown", 1)
		.addEntry("HeadBreak", 2)
		.addEntry("None", 3);
	registerEnumSetting("Mode", &mode, 1);
	registerKeybindSetting("ModeChangeKey", &keybinds, keybinds);
	registerIntSetting("Range", &range, range, 1, 7);
}

PointBreak::~PointBreak() {
}

const char* PointBreak::getModuleName() {
	return "PointBreak";
}
const char* PointBreak::getModName() {
	const char* modes = "";
	if (mode.getSelectedValue() == 0) {
		modes = "AutoCity";
	}
	else if (mode.getSelectedValue() == 1) {
		modes = "TargetDown";
	}
	else if (mode.getSelectedValue() == 2) {
		modes = "HeadBreak";
	}
	else {
		modes = "None";
	};
	return modes;
}

void PointBreak::onKeyUpdate(int keyb, bool isDown) {
	if (Game.getLocalPlayer() == nullptr) return;
	if (!isDown) return;
	if (keyb == this->getKeybind()) this->toggle();
	if (!this->isEnabled()) return;
	if (keyb == keybinds) {
		if (mode.getSelectedValue() == 0) {
			mode.selected = 1;
			Notifications::addNotifBox("Changed PointBreak Mode! {TargetDown}", 5.f);
		}
		else if (mode.getSelectedValue() == 1) {
			mode.selected = 2;
			Notifications::addNotifBox("Changed PointBreak Mode! {HeadBreak}", 5.f);
		}
		else if (mode.getSelectedValue() == 2) {
			mode.selected = 3;
			Notifications::addNotifBox("Changed PointBreak Mode! {None}", 5.f);
		}
		else if (mode.getSelectedValue() == 3) {
			mode.selected = 0;
			Notifications::addNotifBox("Changed PointBreak Mode! {AutoCity}", 5.f);
		}
	}
}
static std::vector<Entity*> targetListPB;
void findEntityPB(Entity* currentEntityav, bool isRegularEntity) {
	static auto PointBreakMod = moduleMgr->getModule<PointBreak>();
	if (currentEntityav == nullptr) return;
	if (currentEntityav == Game.getLocalPlayer()) return;
	if (!Game.getLocalPlayer()->canAttack(currentEntityav, false) || !Game.getLocalPlayer()->isAlive()) return;
	if (!currentEntityav->isAlive()) return;
	if (!Target::isValidTarget(currentEntityav)) return;
	if ((*currentEntityav->getPos()).dist(*Game.getLocalPlayer()->getPos()) < PointBreakMod->range) targetListPB.push_back(currentEntityav);
}
float Wait = 0.f;
void PointBreak::onTick(GameMode* gm) {
	targetListPB.clear();
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	Game.forEachEntity(findEntityPB);
	static PacketMine* PacketMineMod = moduleMgr->getModule<PacketMine>();
	if (Wait <= 0.1f && Wait >= 0.1f) Wait -= 0.1f;
	if (Wait >= 0.1f) return;
	if (moduleMgr->AutoDesync()) return;
	if (!targetListPB.empty()) {
		if (PacketMineMod->blockPos == Vec3i(0, 0, 0)) {
			if (mode.getSelectedValue() == 0) { // AutoCity
				Vec3i EnemyPosition = targetListPB[0]->getPos()->floor();
				Vec3i CityBase = EnemyPosition;
				CityBase.y -= 1;
				Vec3i xplusPos = Vec3i(CityBase.x + 1, CityBase.y, CityBase.z);
				Vec3i xminusPos = Vec3i(CityBase.x - 1, CityBase.y, CityBase.z);
				Vec3i zplusPos = Vec3i(CityBase.x, CityBase.y, CityBase.z + 1);
				Vec3i zminusPos = Vec3i(CityBase.x, CityBase.y, CityBase.z - 1);
				Block* xplus = Game.getLocalPlayer()->region->getBlock(xplusPos);
				Block* xminus = Game.getLocalPlayer()->region->getBlock(xminusPos);
				Block* zplus = Game.getLocalPlayer()->region->getBlock(zplusPos);
				Block* zminus = Game.getLocalPlayer()->region->getBlock(zminusPos);
				if (xplus != nullptr && xminus != nullptr && zplus != nullptr && zminus != nullptr) {
					int xplusID = xplus->toLegacy()->blockId;
					int xminusID = xminus->toLegacy()->blockId;
					int zplusID = zplus->toLegacy()->blockId;
					int zminusID = zminus->toLegacy()->blockId;
					bool awa = true;
					if (xplusID != 0 && xplusID != 7) {
						PacketMineMod->setBreakPos(gm, &xplusPos, 0);
						gm->startDestroyBlock(xplusPos, 0, awa);
						PacketMineMod->setBreakPos(gm, &xplusPos, 0);
						PacketMineMod->updateBreak(player, gm);
					}
					if (xminusID != 0 && xminusID != 7) {
						PacketMineMod->setBreakPos(gm, &xminusPos, 0);
						gm->startDestroyBlock(xminusPos, 0, awa);
						PacketMineMod->setBreakPos(gm, &xminusPos, 0);
						PacketMineMod->updateBreak(player, gm);
					}
					if (zplusID != 0 && zplusID != 7) {
						PacketMineMod->setBreakPos(gm, &zplusPos, 0);
						gm->startDestroyBlock(zplusPos, 0, awa);
						PacketMineMod->setBreakPos(gm, &zplusPos, 0);
						PacketMineMod->updateBreak(player, gm);
					}
					if (zminusID != 0 && zminusID != 7) {
						PacketMineMod->setBreakPos(gm, &zminusPos, 0);
						gm->startDestroyBlock(zminusPos, 0, awa);
						PacketMineMod->setBreakPos(gm, &zminusPos, 0);
						PacketMineMod->updateBreak(player, gm);
					}
				}
				breaking = true;
			}
			else if (mode.getSelectedValue() == 1) { // TargetDown
				Vec3i EnemyPosition = targetListPB[0]->getPos()->floor();
				Vec3i DownPos = EnemyPosition;
				DownPos.y -= 2;
				Block* down = Game.getLocalPlayer()->region->getBlock(DownPos);
				if (down != nullptr) {
					int downID = down->toLegacy()->blockId;
					if (downID != 0 && downID != 7) {
						bool awa = true;
						PacketMineMod->setBreakPos(gm, &DownPos, 0);
						gm->startDestroyBlock(DownPos, 0, awa);
						PacketMineMod->setBreakPos(gm, &DownPos, 0);
						PacketMineMod->updateBreak(player, gm);
					}
				}
				breaking = true;
			}
			else if (mode.getSelectedValue() == 2) { // HeadBreak
				Vec3i EnemyPosition = targetListPB[0]->getPos()->floor();
				Vec3i DownPos = EnemyPosition;
				DownPos.y += 1;
				Block* UP = Game.getLocalPlayer()->region->getBlock(DownPos);
				if (UP != nullptr) {
					int UPID = UP->toLegacy()->blockId;
					if (UPID != 0 && UPID != 7) {
						bool awa = true;
						PacketMineMod->setBreakPos(gm, &DownPos, 0);
						gm->startDestroyBlock(DownPos, 0, awa);
						PacketMineMod->setBreakPos(gm, &DownPos, 0);
						PacketMineMod->updateBreak(player, gm);
					}
				}
				breaking = true;
			}
			else {
				breaking = 3;
			};
		}
		else {
			breaking = false;
		}
	}
}
void PointBreak::onJoiningServer() {
	targetListPB.clear();
}