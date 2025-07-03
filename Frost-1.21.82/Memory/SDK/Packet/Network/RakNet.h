#pragma once

class NetworkIdentifier;

namespace RakNet{}

namespace RakNet { 
	struct SystemAddress {
	public:
		struct {
			unsigned char filler[0x80];
		} address;                  // this+0x0
		unsigned short debugPort;   // this+0x80
		unsigned short systemIndex; // this+0x82
	};
}

namespace RakNet { 
	struct RakNetGUID {
		NetworkIdentifier* mNetworkIdentifier;
		unsigned __int16   mSystemIndex;

		bool operator!=(const RakNetGUID& a1) {
			return *this != a1;
		}
	};
}

namespace RakNet { 
	struct AddressOrGUID {
		RakNetGUID    mGuid;
		SystemAddress mAddr;
	};
}