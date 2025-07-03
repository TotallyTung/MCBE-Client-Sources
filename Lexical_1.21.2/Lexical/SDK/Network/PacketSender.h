#pragma once

class Packet;

class PacketSender {
public:
	virtual ~PacketSender();
	virtual void send(Packet* packet);
	virtual void sendToServer(Packet* packet);
	virtual __int64 sendToClient(__int64 UserEntityIdentifierComponent, Packet* const&);
	virtual __int64 sendToClient(const void* networkIdentifier, const Packet* packet, int a4);
	virtual __int64 sendToClients(const void* networkIdentifier, const Packet* packet);
	virtual void sendBroadcast(Packet* packet);

	// Temporary we just need this
};