#pragma once

class Player : public Mob { // Server/Player Class 1.21.60
public:
	GameMode* getGameMode() {
		return hat::member_at<GameMode*>(this, OffsetManager::Player_getGameMode);
	}

	SerializedSkin* getSkin() {
		return hat::member_at<SerializedSkin*>(this, OffsetManager::Player_getSkin);
	}
	
	PlayerInventory* getSupplies() {
		return hat::member_at<PlayerInventory*>(this, OffsetManager::Player_getSupplies);
	}

	ContainerManagerModel* getContainerManager() {
		return hat::member_at<ContainerManagerModel*>(this, OffsetManager::Player_getContainerManager);
	}
};

class LocalPlayer : public Player {};