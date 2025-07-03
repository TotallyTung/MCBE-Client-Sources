#pragma once
class ItemStack;

namespace Containers
{
	static std::string Inventory = "inventory_items";
	static std::string Hotbar = "hotbar_items";
	static std::string Container = "container_items";
	static std::string Armor = "armor_items";
};

class UIPropertyBag;

class ScreenController {
public:
	char                                                                                                               pad_0x0000[2512];
	std::unordered_map<unsigned int, std::function<void(std::string const&, UIPropertyBag&)>>                          mBindCallbacks;
	std::unordered_map<unsigned int, std::function<void(int, std::string const&, UIPropertyBag&)>>                     mCollectionBindCallbacks;
	std::unordered_map<unsigned int, std::function<void(std::string const&, int, std::string const&, UIPropertyBag&)>> mAnyCollectionBindCallbacks;
};

class ChatScreenController : public ScreenController {
private:
	virtual __int64 Destructor();
private:
	virtual __int64 tick(void);
	virtual __int64 handleEvent(class ScreenEvent* screenEvent);
	virtual std::optional<std::string>* getRoute(std::optional<std::string>* result);
	virtual void setScreenState(const std::vector<std::pair<std::string, std::string>>* routeQueryParameters);
	virtual void onOpen();

public:
	void displayClientMessage(const std::string& msg) {
		static void* signature = SigManager::ChatScreenController_displayClientMessage;
		Memory::CallFunc<void>(signature, this, msg);
	}
};


class ContainerScreenController : public ScreenController { // indexes cuz im gonna make them methods that aren't virtual
private:
	virtual __int64 Destructor();
private:
	virtual __int64 tick(void);
	virtual __int64 handleEvent(__int64&);
	virtual __int64 getRoute(void);
	virtual __int64 setScreenState(__int64, std::string const&);
	virtual __int64 onOpen(void);
	virtual __int64 onTerminate(void);
	virtual __int64 onInit(void);
	virtual __int64 onDelete(void);
	virtual __int64 unknown1();

public:
	virtual bool canExit(void);
	virtual __int64 tryExit(void);

private:
	virtual __int64 areControllerTabsEnabled(void);
	virtual __int64 onCreation(void);
	virtual __int64 logCreationTime(std::string const&, double, double, unsigned char);
	virtual __int64 unknown2();
	virtual __int64 unknown3();
	virtual __int64 unknown4();
	virtual __int64 unknown5();
	virtual __int64 unknown6();

public:
	virtual __int64 onLeave(void);
	virtual __int64 leaveScreen(void);

private:
	virtual __int64 handleGameEventNotification(__int64);
	virtual __int64 bind(std::string const&, unsigned int, int, std::string const&, unsigned int, std::string const&, __int64&);
	virtual __int64 bind(std::string const&, unsigned int, std::string const&, __int64&);
	virtual __int64 handleLicenseChanged(void);
	virtual __int64 onDictationEvent(std::string const&);

public:
	virtual void setAssociatedBlockPos(Vector3<int> const&);
	virtual void setAssociatedEntityUniqueID(__int64);
	virtual void setSuspendInput(bool);

private:
	virtual __int64 getCallbackInterval(void);
	virtual __int64 onRender(void);
	virtual __int64 addStaticScreenVars(__int64&);
	virtual __int64 getAdditionalScreenInfo(void);
	virtual __int64 getTelemetryOverride(void);
	virtual __int64 addEventProperties(__int64) const;
	virtual __int64 getSceneType(void);
	virtual __int64 getScreenVersion(void);
	virtual __int64 screenHandlesGamepadMenuButton(void);
	virtual __int64 getProxy(void);
	virtual __int64 onEntered(void);
	virtual __int64 getNameId(std::string const&);
	virtual __int64 _doesScreenHaveExitBehavior(void);
	virtual __int64 _isStillValid(void);
	virtual __int64 _getGamepadHelperVisible(void);
	virtual __int64 _getMixedHelperVisible(void);
	virtual __int64 _getKeyboardHelperVisible(void);
	virtual __int64 _getButtonADescription(void);
	virtual __int64 _getButtonBDescription(void);
	virtual __int64 _getButtonXDescription(void);
	virtual __int64 _getButtonYDescription(void);
	virtual __int64 _getButtonKeyboardDescription(void);

public:
	virtual __int64 _handlePlaceAll(std::string const&, int);
	virtual __int64 _handlePlaceOne(std::string const&, int);
	virtual __int64 _handleSelectSlot(std::string const&, int);
	virtual __int64 _getSelectedSlotInfo(void);
	virtual __int64 unknown7();
	virtual __int64 unknown8();
	virtual __int64 unknown9();
	virtual __int64 unknown10();
	virtual __int64 unknown11();
	virtual __int64 unknown12();
	virtual ItemStack* _getItemStack(std::string, int) const;

private:
	virtual __int64 _getVisualItemStack(std::string const&, int);
	virtual __int64 _getTakeableItemStackBase(std::string const&, int);
	virtual __int64 _onContainerSlotHovered(std::string const&, int);
	virtual __int64 _onContainerSlotSelected(std::string const&, int);
	virtual __int64 _onContainerSlotPressed(std::string const&, int);
	virtual __int64 _shouldSwap(std::string const&, int, std::string const&, int);
	virtual __int64 _getCollectionName(__int64*);
	virtual __int64 _canSplit(std::string const&, int);
	virtual __int64 _sendFlyingItem(__int64 const&, std::string const&, int, std::string const&, int);

public:
	//void handleAutoPlace(uintptr_t a1, std::string name, int slot);
	//void handleTakeAll(std::string name, int slot);
	//void handlePlaceAll(std::string name, int slot);
	//void handleSwap(std::string name1, int slot1, std::string name2, int slot2);
	//void sendFlyingItem(ItemStack*, std::string, int, std::string, int);
	//ItemStack* getContainerItem(int slot);
	//void takeContainerItem(int slot);
public:
	void shiftItems(std::string containerName, int slots) {
		Memory::CallFunc<void, ContainerScreenController*, uintptr_t, std::string, int>(
			SigManager::ContainerScreenController_handleAutoPlace, this, 0x7FFFFFFF, containerName, slots);
	}

	// handleSwap
	void handleSwap(std::string name1, int slot1, std::string name2, int slot2) {
		this->shiftItems(name1, slot1);
		this->shiftItems(name2, slot2);
	}

	// tryExit
	void closeContainer() {
		Memory::CallVFunc<12, void*>(this);
	}
};

class ChestScreenController : public ContainerScreenController {
};