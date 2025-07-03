#include "AutoTotem.h"
#include "../../ModuleManager.h"
#include <chrono>
#include <thread>
using namespace std;
int offDelay = 0;
int offhandDelay = 0;
int offhandTimeout = 0;
AutoTotem::AutoTotem() : IModule(0, Category::PLAYER, "Automatically puts totems in offhand")
{
	registerEnumSetting("Mode", &item, 0);
	item.addEntry("onTick", 0);
	item.addEntry("onPlayer", 2);
	registerEnumSetting("Protocol", &calling, 0, "How the offhand should activate.");
	calling.addEntry("Normal", 0);
	calling.addEntry("Nukkit", 1);
	calling.addEntry("Strict", 2);
	registerIntSetting("MinHealth", &health, health, 1, 20, "If health is on or smart mode");
	registerBoolSetting("Health", &healthMode, healthMode, "Will only active if user is lower than minhealth");
	registerIntSetting("Delay", &offhandDelay, offhandDelay, 0, 20, "Delay of playertick offhand");
	registerIntSetting("Updates", &offhandTimeout, offhandTimeout, 1, 5, "How many seconds we should update when putting autototem");
	registerEnumSetting("Notify", &mode, 0);
	mode.addEntry("Off", 0);
	mode.addEntry("Chat", 1);
	mode.addEntry("Notification", 2);
}

const char *AutoTotem::getRawModuleName()
{
	return ("AutoTotem");
}
const char *AutoTotem::getModuleName()
{
	switch (item.getSelectedValue())
	{
	case 0:
		name = string("Offhand ") + string(GRAY) + string("Vanilla");
		break;
	case 1:
		name = string("Offhand ") + string(GRAY) + string("Smart");
		break;
	}
	return name.c_str();
}
void AutoTotem::offhandNotify() {
	if (mode.getSelectedValue() == 1)
	{
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("<%sAutoTotem%s> %sAdding totem to offhand", GREEN, WHITE, DARK_GREEN);
	}
	if (mode.getSelectedValue() == 2)
	{
		g_Data.addNotification("Adding totem to offhand", "", 2.f);
	}
	return;
}
void AutoTotem::runNormal() {

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
	C_ItemStack* current = g_Data.getLocalPlayer()->getOffhandItem();
	offDelay++;
	bool run = false;
	bool setOffhand = false;
	int healthy = g_Data.getLocalPlayer()->getHealth();
	auto startTimeoutCount = std::chrono::high_resolution_clock::now();
	if (healthMode && current->item == NULL && healthy < health)
			run = true;
	else if (!healthMode && current->item == NULL)
			run = true;
	if (run && offDelay >= offhandDelay)
	{
		for (int i = 0; i < 36; i++)
		{
			C_ItemStack* totem = inv->getItemStack(i);
			if (totem->item != NULL && (*totem->item)->itemId == 568)
			{
				C_InventoryAction first(i, totem, nullptr);
				C_InventoryAction second(37, nullptr, totem);
				g_Data.getLocalPlayer()->setOffhandSlot(totem);
				manager->addInventoryAction(first);
				manager->addInventoryAction(second);
				offhandNotify();
				auto currentTime = std::chrono::high_resolution_clock::now();
				auto timeoutTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTimeoutCount);
				if (timeoutTime >= static_cast<decltype(timeoutTime)>(offhandTimeout) && offhandTimeout != 1)
					break;
				// Timeout disabled skip it.
				else if (offhandTimeout == 1)
					break;
			}
		}
	}
	return;
}
void AutoTotem::runNukkit() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
	C_ItemStack* current = g_Data.getLocalPlayer()->getOffhandItem();
	auto startTimeoutCount = std::chrono::high_resolution_clock::now();
	offDelay++;
	if (current->item == NULL && offDelay >= offhandDelay)
	{
		for (int i = 1; i < 36; i++)
		{
			C_ItemStack* totem = inv->getItemStack(i);
			if (totem->item != NULL && (*totem->item)->itemId == 568)
			{
				g_Data.getLocalPlayer()->setOffhandSlot(totem);
				inv->removeItem(i, 1);
				offhandNotify();
				auto currentTime = std::chrono::high_resolution_clock::now();
				auto timeoutTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTimeoutCount);
				if (timeoutTime >= static_cast<decltype(timeoutTime)>(offhandTimeout) && offhandTimeout != 1)
					break;
				// Timeout disabled skip it.
				else if (offhandTimeout == 1)
					break;
			}
		}
	}
	return;
}
void AutoTotem::runNukkitStrict() {
	int healthy = g_Data.getLocalPlayer()->getHealth();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
	C_ItemStack* current = g_Data.getLocalPlayer()->getOffhandItem();
	auto startTimeoutCount = std::chrono::high_resolution_clock::now();
	offDelay++;
	if (healthMode) {
		if (healthy <= health) {
			if (current->item == NULL && offDelay >= offhandDelay) {
				for (int i = 1; i < 36; i++) {
					C_ItemStack* stack = inv->getItemStack(i);
					if (stack->item != NULL && (*stack->item)->itemId == 568) {
						g_Data.getLocalPlayer()->setOffhandSlot(stack);
						inv->removeItem(i, i);
						auto currentTime = std::chrono::high_resolution_clock::now();
						auto timeoutTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTimeoutCount);
						if (timeoutTime >= static_cast<decltype(timeoutTime)>(offhandTimeout) && offhandTimeout != 1)
							break;
						// Timeout disabled skip it.
						else if (offhandTimeout == 1)
							break;
					}
				}
			}
		}
	}
	else {
		if (current->item == NULL && offDelay >= offhandDelay) {
			for (int i = 1; i < 36; i++) {
				C_ItemStack* stack = inv->getItemStack(i);
				if (stack->item != NULL && (*stack->item)->itemId == 568) {
					g_Data.getLocalPlayer()->setOffhandSlot(stack);
					inv->removeItem(i, i);
					offhandNotify();
					auto currentTime = std::chrono::high_resolution_clock::now();
					auto timeoutTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTimeoutCount);
					if (timeoutTime >= static_cast<decltype(timeoutTime)>(offhandTimeout) && offhandTimeout != 1)
						break;
					// Timeout disabled skip it.
					else if (offhandTimeout == 1)
						break;
				}
			}
		}
	}
	return;
}
void AutoTotem::onTick(C_GameMode *gm)
{
	auto mode = calling.getSelectedValue();
	if (item.getSelectedValue() == 0) {
		if (mode == 0)
			runNormal();
		else if (mode == 1)
			runNukkit();
		else if (mode == 2)
			runNukkitStrict();
	}
}
void AutoTotem::onPlayerTick(C_GameMode *gm)
{
	auto mode = calling.getSelectedValue();
	if (item.getSelectedValue() == 1 && offDelay > offhandDelay) {
		if (mode == 0)
			runNormal();
		else if (mode == 1)
			runNukkit();
		else if (mode == 2)
			runNukkitStrict();
	}
}