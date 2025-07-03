#pragma once
#include "../../ModuleBase/Module.h"

class PacketMine : public Module {
private:
	float mineSpeed = 1.f;
	float continueReset = 1.f;
	float mineRange = 6.f;
	int mineType = 0;
	int multiTask = 0;
	bool switchBack = false;
private:
	std::pair<BlockPos, uint8_t> breakPair;
public:
	BlockPos getBreakPos() {
		return breakPair.first;
	}
	uint8_t getFace() {
		return breakPair.second;
	}
	int getMultiTask() {
		return multiTask;
	}
public:
	void reset();
	bool canBreakBlock(const BlockPos& blockPos);
	bool breakBlock(const BlockPos& blockPos, uint8_t face = 0);
	void mineBlock(const BlockPos& blockPos, uint8_t face);
	bool sendBreakPacket(const BlockPos& blockPos, uint8_t face, LocalPlayer* localPlayer);
	std::pair<float, uint8_t> getMineTool(const BlockPos& blockPos);
public:
	PacketMine();
	virtual void onNormalTick(LocalPlayer* localPlayer) override;
	void onLevelRender() override;
};