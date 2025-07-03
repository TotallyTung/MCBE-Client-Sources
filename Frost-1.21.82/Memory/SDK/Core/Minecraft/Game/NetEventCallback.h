#pragma once

enum class ShowStoreOfferRedirectType {

};

enum class PacketViolationResponse {

};

class NetEventCallback {
private:
	uintptr_t** vtable;
public:
    // vIndex: 0
    virtual ~NetEventCallback();

    // vIndex: 1
    virtual void onPlayerReady(class Player&);

    // vIndex: 2
    virtual void onConnect(class NetworkIdentifier const&);

    // vIndex: 3
    virtual void onUnableToConnect(Connection::DisconnectFailReason, std::string const&);

    // vIndex: 4
    virtual void onTick();

    // vIndex: 5
    virtual void onStoreOfferReceive(::ShowStoreOfferRedirectType, std::string const& offerID);

    // vIndex: 6
    virtual void onDisconnect(class NetworkIdentifier const&, Connection::DisconnectFailReason, std::string const&, bool, std::string const&);

    // vIndex: 7
    virtual void __unk_vfn_7() = 0;

    // vIndex: 8
    virtual void onWebsocketRequest(std::string const&, std::string const&, std::function<void()>);

    // vIndex: 9
    virtual void sendPacketViolationDetectedTelemetryData(
        uint64_t,
        std::string,
        ::PacketViolationResponse,
        ::PacketID,
        class NetworkIdentifier const&,
        unsigned int,
        ::SubClientId,
        ::SubClientId,
        unsigned int
    );
};