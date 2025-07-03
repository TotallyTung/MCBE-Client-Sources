#include "StorageESP.h"
bool tracer;
bool enabletext;
int range;
StorageESP::StorageESP() : IModule(0, Category::VISUAL, "ESP for but storage blocks.") {
	registerFloatSetting("Opacity", &opacity, opacity, 0.1f, 1.f);
	registerIntSetting("range", &range, 60, 10, 200);
	mode.addEntry(EnumEntry("2D", 0)).addEntry(EnumEntry("3D", 1)).addEntry(EnumEntry("Outline", 2)).addEntry(EnumEntry("Corners", 3));
	registerEnumSetting("Mode", &mode, 2);
	registerBoolSetting("Fill", &fill, fill);
	registerBoolSetting("Tracer", &tracer, true);
	registerBoolSetting("Storage Counts", &enabletext, true);
}

StorageESP::~StorageESP() {
}

const char* StorageESP::getModuleName() {
	return ("BlockScan");
}

int ncc = 0;
int tcc = 0;
int ecc = 0;
int bc = 0;
int sbc = 0;
int ptl = 0;
void StorageESP::onPreRender(MinecraftUIRenderContext* renderCtx) {
	if (!Game.isInGame() || !GameData::canUseMoveKeys() || Game.getLocalPlayer() == nullptr || bufferedChestList.empty() || mode.selected != 1)
		return;
	auto player = Game.getLocalPlayer();
	auto ourListLock = std::scoped_lock(this->listLock);
	for (const auto& chest : bufferedChestList) {
		MC_Color flushColor = MC_Color(1.f, 1.f, 1.f, opacity);

		Vec3 blockPos = chest.lower;
		auto storageID = Game.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId;
		bool NormalChest = true;
		bool TrappedChest = true;
		bool EnderChest = true;
		bool Barrel = true;
		bool ShulkerBox = true;
		if (storageID == 54 && NormalChest) { flushColor = MC_Color(1.f, 1.f, 1.f, opacity); ncc++; }              // Normal Chest
		if (storageID == 146 && TrappedChest) { flushColor = MC_Color(.92f, .14f, .14f, opacity); tcc++; }// Trapped Chest
		if (storageID == 130 && EnderChest) { flushColor = MC_Color(0.435294f, 0.215686f, 0.631372f, opacity); ecc++; }// Ender Chest
		if (storageID == 458 && Barrel) { flushColor = MC_Color(0.62f, 0.31f, 0.04f, opacity); bc++; }// Barrel
		if (storageID == 205 && ShulkerBox) { flushColor = MC_Color(.49f, .17f, .95f, opacity); sbc; }// Undyed Shulker Box 
		if (storageID == 218 && ShulkerBox) { flushColor = MC_Color(.08f, .91f, .99f, opacity); sbc; }               // Shulker Box
		DrawUtils::setColor(flushColor.r, flushColor.g, flushColor.b, flushColor.a);
		if (fill && (mode.selected == 0 || mode.selected == 3)) DrawUtils::draw2DBox(chest.lower, chest.upper, (float)fmax(0.5f, 1 / (float)fmax(1, Game.getLocalPlayer()->eyePos0.dist(chest.lower))), true);
		switch (mode.selected) {
		case 0:
			DrawUtils::draw2DBox(chest.lower, chest.upper, (float)fmax(0.5f, 1 / (float)fmax(1, Game.getLocalPlayer()->eyePos0.dist(chest.lower))));
			break;
		case 2:
			DrawUtils::drawBox(chest.lower, chest.upper, (float)fmax(0.5f, 1 / (float)fmax(1, Game.getLocalPlayer()->eyePos0.dist(chest.lower))), fill, 2);
			break;
		case 3:
			DrawUtils::draw2DBox(chest.lower, chest.upper, (float)fmax(0.5f, 1 / (float)fmax(1, Game.getLocalPlayer()->eyePos0.dist(chest.lower))), false, true);
			break;
		}
	}
}

void StorageESP::onLevelRender() {
	if (!Game.isInGame() || !GameData::canUseMoveKeys() || Game.getLocalPlayer() == nullptr || bufferedChestList.empty() || mode.selected != 1)
		return;
	auto player = Game.getLocalPlayer();
	auto ourListLock = std::scoped_lock(this->listLock);
	for (const auto& chest : bufferedChestList) {
		MC_Color flushColor = MC_Color(1.f, 1.f, 1.f, opacity);

		Vec3 blockPos = chest.lower;
		blockPos.x + 2.0;
		blockPos.y + 0.5;
		blockPos.z + 1.0;
		auto storageID = Game.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId;
		float calcYaw = (player->yaw + 90) * (PI / 180);
		float calcPitch = (player->pitch) * -(PI / 180);

		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * 0.5f;
		moveVec.y = sin(calcPitch) * 0.5f;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * 0.5f;

		const Vec3 origin = Game.getClientInstance()->levelRenderer->getOrigin().add(moveVec);
		if (storageID == 54) {  // Normal Chest
			flushColor = MC_Color(1.f, 1.f, 1.f, opacity);
		}
		if (storageID == 146) {  // Trapped Chest
			flushColor = MC_Color(.92f, .14f, .14f, opacity);
		}
		if (storageID == 130) {  // Ender Chest
			flushColor = MC_Color(0.435294f, 0.215686f, 0.631372f, opacity);
		}
		if (storageID == 458) {  // Barrel
			flushColor = MC_Color(0.62f, 0.31f, 0.04f, opacity);
		}
		if (storageID == 205) { // Undyed Shulker Box
			flushColor = MC_Color(.49f, .17f, .95f, opacity);
		}
		if (storageID == 218) {                            // Shulker Box
			flushColor = MC_Color(.08f, .91f, .99f, opacity);
		}
		if (storageID == 90) {                            // Portal
			flushColor = MC_Color(.238f, .119f, .255f, opacity);
		}
		DrawUtils::setColor(flushColor.r, flushColor.g, flushColor.b, flushColor.a);
		if (!fill) DrawUtils::drawBox3d(chest.lower, chest.upper, 1.f, true); else DrawUtils::drawBox3dFilled(chest.lower, chest.upper, 1.f, true, true);
		if (tracer) {
			DrawUtils::drawLine3d(origin, blockPos);
		};
	}
}
void StorageESP::onImGuiRender(ImDrawList* drawlist) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr) return;
	if (!Game.canUseMoveKeys()) return;
	static Colors* colorsMod = moduleMgr->getModule<Colors>();
	UIColor mainColor = colorsMod->getColor();
	UIColor extraColor = UIColor(255, 255, 255);
	Vec2 windowSize = Game.getGuiData()->windowSizeReal;
	static const float textSize = 1.f;
	static const float textPadding = 1.f * textSize;
	static const float textHeight = ImGuiUtils::getTextHeight(textSize);
	static const float x = 2.f;
	float y = windowSize.y - textHeight - textPadding;
	char blockscan [150] ;
	sprintf_s(blockscan, 150, "Normal Chests: %s\nTrapped Chest: %s\nEnder Chest: %s\nBarrel: %s\nShulker Box: %s", std::to_string(ncc), std::to_string(tcc), std::to_string(ecc), std::to_string(bc), std::to_string(sbc));
	if (enabletext) {
		ImGuiUtils::drawRectFilled(Vec4(windowSize.x - windowSize.x, y - 55, windowSize.x - windowSize.x + 205, y - 165), UIColor(250, 190, 150, 50), textSize);
		ImGuiUtils::drawText(Vec2(windowSize.x - windowSize.x, y - 165), blockscan, extraColor, textSize);
	}
	ncc = 0;
	tcc = 0;
	ecc = 0;
	bc = 0;
	sbc = 0;
	ptl = 0;
}
void StorageESP::onTick(GameMode* gm) {
	// Swap list
	auto listLock = Game.lockChestList();
	auto& chestList = Game.getChestList();
	auto ourListLock = std::scoped_lock(this->listLock);

	this->bufferedChestList.clear();
	this->bufferedChestList.insert(this->bufferedChestList.end(), chestList.begin(), chestList.end());
	chestList.clear();
}