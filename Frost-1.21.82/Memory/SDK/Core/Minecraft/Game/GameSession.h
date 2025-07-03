#pragma once

class GameSession {
public:
	ClientOrServerNetworkSystemRef              mNetwork;                    // this+0x0
	OwnerPtr<class EntityContext>               mLevelEntity;                // this+0x10
	Bedrock::NonOwnerPointer<class Level>       mLevel;                      // this+0x28
	std::unique_ptr<class ServerNetworkHandler> mServerNetworkHandler;       // this+0x38
	std::unique_ptr<class NetEventCallback>     mLegacyClientNetworkHandler; // this+0x40
	std::unique_ptr<class NetEventCallback>     mClientNetworkHandler;       // this+0x48
	class LoopbackPacketSender&                 mLoopbackPacketSender;       // this+0x50
	SubClientId                                 mClientSubId;                // this+0x58
};