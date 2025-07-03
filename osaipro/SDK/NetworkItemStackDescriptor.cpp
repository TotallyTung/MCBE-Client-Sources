#include "NetworkItemStackDescriptor.h"
#include "../Utils/Utils.h"

NetworkItemStackDescriptor::NetworkItemStackDescriptor(ItemStack* stack) {
	memset(this, 0, sizeof(NetworkItemStackDescriptor));
	using NetworkItemStackDescriptorConstructor_t = void(__fastcall*)(NetworkItemStackDescriptor*, ItemStack*);
	static NetworkItemStackDescriptorConstructor_t NetworkItemStackDescriptorConstructor = reinterpret_cast<NetworkItemStackDescriptorConstructor_t>(Utils::getBase() + 0x23DA1D0);
	NetworkItemStackDescriptorConstructor(this, stack);
}