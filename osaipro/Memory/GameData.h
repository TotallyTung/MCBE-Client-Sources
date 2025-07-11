#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <unordered_set>

#include "../SDK/ChestBlockActor.h"
#include "../SDK/ClientInstance.h"
#include "../SDK/GameMode.h"
#include "../SDK/HIDController.h"
#include "../SDK/MoveInputHandler.h"
#include "../SDK/RakNetInstance.h"
#include "../Utils/SkinUtil.h"
#include "../Utils/TextFormat.h"
#include "SlimMem.h"

struct SkinData;

struct AABBHasher {
	size_t operator()(const AABB& i) const;
};

class GameData {
private:
	ClientInstance* clientInstance = nullptr;
	LocalPlayer* localPlayer = nullptr;
	GameMode* gameMode = nullptr;
	EntityList* entityList = nullptr;
	RakNetInstance* raknetInstance = nullptr;
	void* hDllInst = 0;
	std::unordered_set<AABB, AABBHasher> chestList;
	std::vector<std::string> textPrintList;
	std::mutex textPrintLock;
	std::mutex chestListMutex;
	int lastRequestId = 0;
	std::shared_ptr<std::string> customGeometry;
	bool customGeoActive = false;
	std::shared_ptr<std::tuple<std::shared_ptr<unsigned char[]>, size_t>> customTexture;
	bool customTextureActive = false;

private:
	bool injectorConnectionActive = false;
	const SlimUtils::SlimModule* gameModule = 0;
	bool shouldTerminateB = false;
	bool shouldHideB = false;
	bool isAllowingWIPFeatures = false;
	__int64 lastUpdate;
	static void retrieveClientInstance();
	TextHolder* fakeName;

public:
	HIDController* hidController = nullptr;
	static SlimUtils::SlimMem* slimMem;
	static bool keys[0x256];
	int fps = 0;
	int frameCount = 0;
	int time = 0;
	bool initisfinished();
	void loading();

	static bool canUseMoveKeys();
	static bool isKeyDown(int key);
	static bool isKeyPressed(int key);
	static bool isRightClickDown();
	static bool isLeftClickDown();
	static bool isWheelDown();
	static bool shouldTerminate();
	static bool shouldHide();
	static void hide();
	static void terminate();
	static void updateGameData(GameMode* gameMode);
	static void initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem, void* hDllInst);
	static void addChestToList(ChestBlockActor* ChestBlock2);
	static void EntityList_tick(EntityList* list);
	static void setHIDController(HIDController* Hid);
	static void setRakNetInstance(RakNetInstance* raknet);
	static void displayMessages(GuiData* guiData);
	static void log(const char* fmt, ...);
	float fov = 0.f;

	void clearChestList() {
		std::lock_guard<std::mutex> listGuard(chestListMutex);
		chestList.clear();
	}
	inline void setCustomGeometryOverride(bool setActive, std::shared_ptr<std::string> customGeoPtr) {
		customGeoActive = setActive;
		if (setActive)
			customGeometry.swap(customGeoPtr);
		else
			customGeometry.reset();
	}
	inline std::tuple<bool, std::shared_ptr<std::string>> getCustomGeoOverride() {
		return std::make_tuple(customGeoActive, customGeometry);
	}
	inline void setCustomTextureOverride(bool setActive, std::shared_ptr<std::tuple<std::shared_ptr<unsigned char[]>, size_t>> customTexturePtr) {
		customTextureActive = setActive;
		if (setActive)
			customTexture.swap(customTexturePtr);
		else
			customTexture.reset();
	}
	inline auto getCustomTextureOverride() {
		return std::make_tuple(customTextureActive, customTexture);
	}
	inline void* getDllModule() { return hDllInst; };
	inline ClientInstance* getClientInstance() { return clientInstance; };
	inline GuiData* getGuiData() { return clientInstance->getGuiData(); };
	inline LocalPlayer* getLocalPlayer() {
		localPlayer = clientInstance->getCILocalPlayer();
		if (localPlayer == nullptr)
			gameMode = nullptr;
		return localPlayer;
	};
	bool isInGame() {
		return localPlayer != nullptr;
	}
	const SlimUtils::SlimModule* getModule() {
		return gameModule;
	};
	const SlimUtils::SlimMem* getSlimMem() {
		return slimMem;
	};
	GameMode* getGameMode() { return gameMode; };
	EntityList* getEntityList() { return entityList; };
	HIDController** getHIDController() { return &hidController; };
	RakNetInstance* getRakNetInstance() { return raknetInstance; };
	std::unordered_set<AABB, AABBHasher>& getChestList() { return chestList; };
	auto lockChestList() { return std::lock_guard<std::mutex>(chestListMutex); }
	void setFakeName(TextHolder* name) { fakeName = name; };
	TextHolder* getFakeName() { return fakeName; };
	inline __int64 getLastUpdateTime() { return lastUpdate; };
	void forEachEntity(std::function<void(Entity*, bool)>);
	void forEachPlayer(std::function<void(Entity*, bool)>);
	void forEachMob(std::function<void(Entity*, bool)>);
	int getFPS() { return fps; };
};

extern GameData Game;