#pragma once

class Player : public Actor {
public:
	BUILD_ACCESS(this, GameMode*, gamemode, 0xF10); //good for 1.20.51
public:
	virtual void prepareRegion(void);
	virtual void destroyRegion(void);
	virtual void suspendRegion(void);
	virtual void _fireDimensionChanged(void);
	virtual void changeDimensionWithCredits(__int64);
	virtual void tickWorld(void);
	virtual void Function5();
	virtual void getTickingOffsets(void);
	virtual void moveView(void);
	virtual void moveSpawnView(Vector3<float> const&, __int64);
	virtual void checkMovementStats(Vector3<float> const&);
	virtual void getCurrentStructureFeature(void);
	virtual bool isAutoJumpEnabled(void);
	virtual void respawn(void);
	virtual void resetRot(void);
	virtual bool isInTrialMode(void);
	virtual void openPortfolio(void);
	virtual void openBook(int, bool, int, void*);
	virtual void openTrading(void*, bool);
	virtual void openChalkboard(void*, bool);
	virtual void openNpcInteractScreen();
	virtual void openInventory(void);
	virtual void displayChatMessage(std::string const&, std::string const&);
	virtual void displayClientMessage(std::string const&);
	virtual void displayTextObjectMessage(std::string const&, std::string const&);
	virtual void displayTextObjectWhisperMessage(std::string const&, std::string const&);
	virtual void displayTextObjectWhisperMessage(std::string const&, std::string const&, std::string const&);
	virtual void displayWhisperMessage(std::string const&, std::string const&, std::string const&, std::string const&);
	virtual void startSleepInBed(Vector3<int> const&);
	virtual void stopSleepInBed(bool, bool);
	virtual bool canStartSleepInBed(void);
	virtual void openSign(Vector3<int> const&, bool);
	virtual void playEmote(std::string const&, bool);
	virtual bool isHostingPlayer(void);
	virtual bool isLoading(void);
	virtual bool isPlayerInitialized(void);
	virtual void stopLoading(void);
	virtual void setPlayerGameType();
	virtual void initHUDContainerManager(void);
	virtual void _crit(Actor*);
	virtual void getEventing(void);
	virtual void getUserId(void);
	virtual void addExperience(int);
	virtual void addLevels(int);
	virtual void Function43();
	virtual void Function44();
	virtual void Function45();
	virtual void deleteContainerManager(void);
	virtual bool isActorRelevant(Actor const&);
	virtual void Function48();
	virtual void onSuspension(void);
	virtual void onLinkedSlotsChanged(void);
	virtual void Function51();
	virtual void Function52();
	virtual void sendNetworkPacket(Packet&);
	virtual void Function54();
	virtual void reportMovementTelemetry();
	virtual bool isSimulated(void);
	virtual void getXuid(void);
	virtual void getMovementSettings(void);
	virtual void getMaxChunkBuildRadius(void);
	virtual void onMovePlayerPacketNormal(Vector3<float> const&, Vector2<float> const&, float);
	virtual void _createChunkSource();
	virtual void setAbilities();
	virtual void Function63();
	virtual void Function64();

};