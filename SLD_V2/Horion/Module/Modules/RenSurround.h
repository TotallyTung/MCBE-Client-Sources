#pragma once
#include "Module.h"
class RenSurround : public IModule {
private:
	bool Spoof();
	bool Ez();
	bool Center();
	int slot = false;
	bool realspoof = false;
	bool realspoof2 = true;
	bool AAnvilSwap();
	bool spoofC = true;
	bool AntiAnvilSwap = false;
	bool antianvil = false;
	int Odelay2 = 0;
	bool bypass2 = true;
	int delay2 = 2;
	bool center = true;
	bool Sneak = true;
	bool KindaSpoof = true;
	bool Bypass = true;
	bool rot1 = false;
	bool java = true;
	bool vanilla = true;
	bool anticity = true;
	bool selfweb = false;
	bool rotate = false;
	bool triggerable = true;
	bool tryAirPlace(vec3_t blockBelow);
	bool tryAntiAnvil(vec3_t blockBelow);
	bool tryPlace33(vec3_t blockBelow);
	int delay = 0;
	float rSelect2 = 0.f;
	float gSelect2 = 1.f;
	float bSelect2 = 0.f;
	float tick_for_place = 4;
	float tick_timeout = 20;


public:
	float float1 = 0;

	SettingEnum enum1;
	SettingEnum enum2;
	SettingEnum enum3;
	bool renderRenSurround = false;
	bool doAirplace = false;
	RenSurround();
	~RenSurround();
	bool blockSwap = true;
	int prevSlot3 = -1;
	bool block_head = false;
	bool jumpDisable = false;
	bool disableOnComplete2 = false;

	bool mustPitchUp = false;

	std::vector<vec3_t> regularBlocks = {

		vec3_t(0, -1, 0),

		vec3_t(1, -1, 0),
		vec3_t(1, 0, 0),

		vec3_t(0, -1, 1),
		vec3_t(0, 0, 1),

		vec3_t(-1, -1, 0),
		vec3_t(-1, 0, 0),

		vec3_t(0, -1, -1),
		vec3_t(0, 0, -1),

	};

	std::vector<vec3_t> antiCityBlocks = {

		vec3_t(0, -1, 0),

		vec3_t(1, -1, 0),
		vec3_t(1, 0, 0),
		vec3_t(2, 0, 0),

		vec3_t(0, -1, 1),
		vec3_t(0, 0, 1),
		vec3_t(0, 0, 2),

		vec3_t(-1, -1, 0),
		vec3_t(-1, 0, 0),
		vec3_t(-2, 0, 0),

		vec3_t(0, -1, -1),
		vec3_t(0, 0, -1),
		vec3_t(0, 0, -2),

	};
	std::vector<vec3_t> javaBlocks = {

		vec3_t(0, -1, 0),

		vec3_t(1, -1, 0),
		vec3_t(1, 0, 0),

		vec3_t(0, -1, 1),
		vec3_t(0, 0, 1),

		vec3_t(-1, -1, 0),
		vec3_t(-1, 0, 0),

		vec3_t(0, -1, -1),
		vec3_t(0, 0, -1),

	};
	std::vector<vec3_t> RenSurround_targets = {

		vec3_t(0, -1, 0),

		vec3_t(1, -1, 0),
		vec3_t(1, 0, 0),

		vec3_t(0, -1, 1),
		vec3_t(0, 0, 1),

		vec3_t(-1, -1, 0),
		vec3_t(-1, 0, 0),

		vec3_t(0, -1, -1),
		vec3_t(0, 0, -1),

	};
	std::vector<vec3_t> RenSurround_targets_face = {

		vec3_t(1, 1, 0),
		vec3_t(0, 1, 1),
		vec3_t(-1, 1, 0),
		vec3_t(0, 1, -1),
		vec3_t(1, 0, 0),
		vec3_t(0, 0, 1),
		vec3_t(-1, 0, 0),
		vec3_t(0, 0, -1),
		vec3_t(1, -1, 0),
		vec3_t(0, -1, 1),
		vec3_t(-1, -1, 0),
		vec3_t(0, -1, -1),
		vec3_t(0, -1, 0)};

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual bool isFlashMode() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	bool tryScaffold(vec3_t blockBelow);
	virtual void onDisable() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onMove(C_MoveInputHandler* hand) override;
};