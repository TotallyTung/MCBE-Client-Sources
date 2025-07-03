#pragma once

class Packet;

class LoopbackPacketSender { // 1.21.60
public:
    SubClientId                                             mSenderSubId;       // this+0x18
    class NetworkSystem*                                    mNetwork;           // this+0x20
    std::vector<class NetEventCallback*>                    mLoopbackCallbacks; // this+0x30
    std::vector<class OwnerPtr<class EntityContext>> const* mUserList;          // this+0x48
    std::vector<NetworkIdentifierWithSubId>                 mTempUserIds;       // this+0x50
public:
    virtual void Destructor() {}; // Destructor
    virtual bool isInitialized() {};
    virtual void send(::Packet* packet) {};
    virtual void sendTo(::NetworkIdentifier const& id, ::SubClientId subid, ::Packet& packet) {};
    virtual void sendToServer(::Packet* packet) {};
    virtual void sendToClient(class NetworkIdentifier const&, void* Packet, SubClientId id) {};
    virtual void sendToClient(class UserEntityIdentifierComponent const*, void* Packet) {};
    virtual void sendToClients(std::vector<class NetworkIdentifierWithSubId, std::allocator<class NetworkIdentifierWithSubId>> const&, void* Packet) {};
    virtual void sendBroadcast(void* Packet) {};
    virtual void sendBroadcast(class NetworkIdentifier const&, unsigned char, void* Packet) {};
    virtual void flush(class NetworkIdentifier const&) {};
};

struct SocialGameConnectionInfo
{
    CLASS_FIELD(std::string, realIp, 0x8);
    CLASS_FIELD(std::string, serverIp, 0x28);
};

class NetworkSystem {
public:
    class RemoteConnectorComposite* getRemoteConnectorComposite() {
        return hat::member_at<class RemoteConnectorComposite*>(this, 0x90); // 1.21.50
    }

    virtual ~NetworkSystem() {};
    virtual SocialGameConnectionInfo* getConnectionInfo(void) {};
    virtual void useIPv4Only(void) {};
    virtual void useIPv6Only(void) {};
    virtual unsigned __int16 getDefaultGamePort(void) {};
    virtual unsigned __int16 getDefaultGamePortv6(void) {};
    virtual void _onDisable(void) {};
    virtual void _onEnable(void) {};
    virtual void onNewIncomingConnection(class NetworkIdentifier const&, std::shared_ptr<class NetworkPeer>) {};
    virtual void onNewOutgoingConnection(class NetworkIdentifier const&, std::shared_ptr<class NetworkPeer>) {};
    virtual void onConnectionClosed(NetworkIdentifier const&, const std::string&, bool) {};
    virtual void onAllConnectionsClosed(const std::string&, bool) {};
    virtual void onAllRemoteConnectionsClosed(const std::string&, bool) {};
    virtual void onOutgoingConnectionFailed(void) {};
    virtual void onWebsocketRequest(const std::string&, const std::string&) {};
    virtual void onNewIncomingLocalConnection(NetworkIdentifier const&, std::shared_ptr<class NetworkPeer>) {};
    virtual void onNewOutgoingLocalConnection(NetworkIdentifier const&, std::shared_ptr<class NetworkPeer>) {};
};

class RemoteConnectorComposite {
public:
    class RaknetConnector* getRaknetConnector() {
        return hat::member_at<class RaknetConnector*>(this, 0x70); // 1.21.60
    }
};