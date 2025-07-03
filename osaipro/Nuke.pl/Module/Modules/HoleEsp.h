#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class HoleEsp : public IModule {
private:
	bool obsidian = true; // show obsidian holes
	bool bedrock = true; // show bedrock holes
	bool mixed = true; // show mixed holes
	float Opacity = 1.f; // opacity of the fill
	float lineOpacity = 0.6f; // opacity of the outline
	bool outline = true; // adds outlines
	bool osync = false; // Makes obsidian holes have same color as the Interface module
	bool bsync = false; // Makes bedrock holes have same color as the Interface module
	bool msync = false; // Makes mix holes have same color as the Interface module
	float HoleEsp::obr{ 180 }; // red color of obsidian hole
	float HoleEsp::obg{ 0 }; // green color of obsidian hole
	float HoleEsp::obb{ 0 }; // blue color of obsidian hole

	float HoleEsp::br{ 0 }; // red color of bedrock hole
	float HoleEsp::bg{ 180 }; // green color of bedrock hole
	float HoleEsp::bb{ 0 }; // blue color of bedrock hole

	float HoleEsp::mr{ 180 }; // red color of mixed hole
	float HoleEsp::mg{ 180 }; // green color of mixed hole
	float HoleEsp::mb{ 0 }; // blue color of mixed hole

public:
	int range = 5;
	float LineSize = 0.4f;

	HoleEsp();
	~HoleEsp();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
};