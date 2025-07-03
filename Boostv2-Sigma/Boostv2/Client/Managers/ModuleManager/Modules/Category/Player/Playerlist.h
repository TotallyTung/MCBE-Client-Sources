#pragma once  
#include "../../ModuleBase/Module.h"  
#include "../Misc/PlayerJoin.h"  

class PlayerList : public Module {
private:
	float size = 1.0f;
	float backgroundOpacity = 0.85f;
	float rounding = 12.0f;
	float accentColor[3] = { 1.0f, 0.4f, 0.8f }; // Default pink accent  
	float textSize = 0.85f;
	float spacing = 4.0f;
	bool glow = false; // Add this line to define the glow variable  

public:
	PlayerList();
	~PlayerList();

	void onD2DRender() override;
	std::vector<std::string> getCurrentPlayerList();
};