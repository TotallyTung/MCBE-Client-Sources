#pragma once
#include "Packet.h"
#include "../../World/Inventory/Transaction/ComplexInventoryTransaction.h"

class InventoryTransactionPacket : public Packet {
public:
	InventoryTransactionPacket(std::unique_ptr<ComplexInventoryTransaction> transaction, bool isClientSide = false) {
		memset(this, 0x0, sizeof(InventoryTransactionPacket));
		using InvTransactionPacket_ctt = void(__thiscall*)(InventoryTransactionPacket*, std::unique_ptr<ComplexInventoryTransaction>, bool);
		static InvTransactionPacket_ctt constructor = reinterpret_cast<InvTransactionPacket_ctt>(Addresses::InventoryTransactionPacket_Constructor);
		constructor(this, std::move(transaction), isClientSide);
	}
private:
	char pad_0x30[0x28];
public:
	std::unique_ptr<ComplexInventoryTransaction> transaction; // 0x58
	bool isClientSide; // 0x60
	//40 53 48 83 EC ? 45 33 C9 C7 41 08 ? ? ? ?
};