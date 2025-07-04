#pragma once
#include "../Module.h"

class StorageESP : public Module {
private:
public:
	float opacity = 255.f;
	std::vector<Vec3<int>> blocks;
	std::mutex listLock;
	int mode;
	bool fill = false;
	bool box;
	int esese = 20;
	int radius = 20;

	StorageESP();
	void getBlocks(std::vector<Vec3<int>>& blocks);
	virtual void onRender(MinecraftUIRenderContext* renderCtx);
};