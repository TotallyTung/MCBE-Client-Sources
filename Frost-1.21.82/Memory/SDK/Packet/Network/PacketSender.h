#pragma once

class PacketSender : public Bedrock::EnableNonOwnerReferences {
public:
	SubClientId mSenderSubId; // this+0x18
};

#include "ClientOrServerNetworkSystemRef.h"
#include "RakNet.h"
#include "NetworkIdentifier.h"
#include "NetworkIdentifierWithSubId.h"
#include "LoopbackPacketSender.h"
#include "NetworkPeer.h"
#include "RakPeer.h"
#include "NetworkBlockPosition.h"