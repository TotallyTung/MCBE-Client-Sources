#include "PacketMine.h"
PacketMine::PacketMine() : IModule(0, Category::PLAYER, "Mine blocks silently for IGN Servers") {
	// Server for different types of servers, i will add geyser too when i have the time.
	// registerEnumSetting("Protocol", &server, 0);
	// server.addEntry("Nukkit", 0);
	registerEnumSetting("Mode", &mineType, 0, "Type of mining type");
	mineType.addEntry("Continous", 0);
	mineType.addEntry("Instant", 1);
	mineType.addEntry("InstantStrict", 2);
	mineType.addEntry("Once", 3);
	mineType.addEntry("None", 4);
	registerIntSetting("Range", &swapDel, swapDel, 1.f, 10.f, "Range of mine");
	registerEnumSetting("Swap", &swap, 0, "Swap to pickaxe(WIP)");
	swap.addEntry("None", 0);
	swap.addEntry("Regular", 1);
	swap.addEntry("NukkitSpoof", 2);
	//swap.addEntry("MobEquipment", 3); when trong fixes
	registerFloatSetting("Increase", &increase, increase, 0.f, 1.f, 0.01f, "Increases the mining speed on the Second> mine");
	//registerBoolSetting("Increase on Start", &increaseOnStart, increaseOnStart, "Increases the mining speed at the start instead of at the 2nd mine");
	registerEnumSetting("Render", &render, 0, "Render type");
	render.addEntry("None", 0);
	render.addEntry("Fill", 1);
	render.addEntry("Outline", 2);
	render.addEntry("Both", 3);
	registerEnumSetting("Animations", &anim, 0, "Animations of the mine render");
	anim.addEntry("None", 0);
	anim.addEntry("Grow", 1);
	anim.addEntry("Fade", 2);
	registerBoolSetting("Render Progress", &renderProgress, renderProgress, "Shows a text rendering the progress");
	registerFloatSetting("Alpha", &opacity, opacity, 0.0f, 1.f, 0.01f, "Opacity of the render");
}

const char* PacketMine::getRawModuleName() {
	return "PacketMine";
}
const char* PacketMine::getModName() {
	switch (mineType.getSelectedValue()) {
	case 0:
		name = " Continuous";
		break;
	case 1:
		name = " Instant";
		break;
	case 2:
		name = " InstantStrict";
		break;
	case 3:
		name = " Once";
		break;
	}
	return name;
}
void PacketMine::findAPickaxe() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	auto prevSlot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 36; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {

			bool IsUseful = stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(g_Data.getLocalPlayer()->pointingStruct->block)) <= 32767;
			if (IsUseful)
			{
				if (prevSlot != n)
					supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
	//if (stack->getItem()->itemId == 637) {
}

void PacketMine::onTick(C_GameMode* gm) {
}
void PacketMine::onWorldTick(C_GameMode* gm)
{
}
void PacketMine::returnSlot() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++)
	{
		if (origSlot) {
			supplies->selectedHotbarSlot = origSlot;
			return;
		}
	}
}
void PacketMine::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
}
