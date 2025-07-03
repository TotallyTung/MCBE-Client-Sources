#pragma once

// This should be in Actor/Mob but using Player for a clean base since Plyaer is a child of Actor/Mob.
class Player : public Mob { // Player VTable
public:
	// GameMode Player::getGameMode(void)
	BUILD_ACCESS(GameMode*, Gamemode, 0xAD0); // 1.21.73
	BUILD_ACCESS(SerializedSkin*, Skin, 0x780); // 1.21.2
public:
	void displayClientMessage(std::string message) {
		Memory::CallVFunc<200, void, std::string, std::optional<std::string>>(this, message, std::optional<std::string>());
	}
	void addLevels(int levels) {
		Memory::CallVFunc<219, void, int>(this, levels);
	}
};
