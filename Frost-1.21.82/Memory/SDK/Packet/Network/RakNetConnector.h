#pragma once

#include "RakPeer.h"

class RaknetConnector {
public:
	// ServerScriptManager::getBlockCustomComponentRegistry(void)
	// RaknetConnector::getPeer
	RakNet::RakPeer* getPeer() {
		return hat::member_at<RakNet::RakPeer*>(this, 0x2A0); // 1.21.50
	}

	// Xref:
	CLASS_FIELD(std::string, mNumericalIP, 0x378); // 1.21.50
	CLASS_FIELD(std::string, mServerIP, 0x398); // 1.21.50
	CLASS_FIELD(uint32_t, mServerPort, 0x3B8); // 1.21.50


	virtual void Destructor() {};
	virtual std::vector<std::string*> getLocalIps(void) {};
	virtual std::string* getLocalIp(void) {};
	virtual uint16_t getPort(void) {};
	virtual __int64 getRefinedLocalIps(void) {};
	virtual __int64* getConnectedGameInfo(void) {};
	virtual void setupNatPunch(bool connectToClient) {};
	virtual __int64* getNatPunchInfo(void) {};
	virtual void startNatPunchingClient(std::string const&, short) {};
	virtual void addConnectionStateListener(__int64*) {};
	virtual void removeConnectionStateListener(__int64*) {};
	virtual bool isIPv4Supported(void) {};
	virtual bool isIPv6Supported(void) {};
	virtual uint16_t getIPv4Port(void) {};
	virtual uint16_t getIPv6Port(void) {};
	virtual void host(__int64 const definition) {};
	virtual void connect(__int64 const&, __int64 const&) {};
	virtual void disconnect(void) {};
	virtual void tick(void) {};
	virtual void runEvents(void) {};
	virtual bool isServer(void) {};
	virtual bool isConnected(struct NetworkIdentifier const&) {};
	virtual __int64 closeNetworkConnection(struct NetworkIdentifier const&) {};
	virtual NetworkIdentifier* getNetworkIdentifier(void) {};
	virtual void setApplicationHandshakeCompleted(NetworkIdentifier const&) {};
	virtual RakNet::RakPeer getRakPeer(void) {};
	virtual void getBlockCustomComponentRegistryConst(void) {};
	virtual void _onDisable(void) {};
	virtual void _onEnable(void) {};
public:
	bool isOnServer() { 
		return !(mServerIP.length() == 0);
	}
};