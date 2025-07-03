#pragma once

class UpdateBlockPacket : public Packet
{
public:
	UpdateBlockPacket() {
		init();
	}

public:
	int32_t x;
	int32_t y;
	int32_t z;
	uint32_t dataLayerId;
	uint8_t flags;
	uint32_t blockRuntimeId;

	void init()
	{
		static void* sig = nullptr;
		if (!sig) sig = (void*)findSig("48 8D 05 ? ? ? ? C7 43 ? ? ? ? ? 48 89 43 10 C7 43 ? ? ? ? ? C7 43 ? ? ? ? ? 66 89 73 20 48 89 73 28 48 89 73 30 89 73 38 48 89 73 40 48 89 73 48 40 88 73 50 89 73 54 EB 86");
		this->setVTable<UpdateBlockPacket>(reinterpret_cast<uintptr_t*>(sig));
	}
};