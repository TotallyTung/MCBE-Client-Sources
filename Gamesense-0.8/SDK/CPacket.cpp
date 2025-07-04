#include "CPacket.h"
#include "../Utils/Utils.h"

TextHolder* C_Packet::getName() {
	return Utils::CallVFunc<2, TextHolder*>(this, new TextHolder());
}

LevelSoundEventPacket::LevelSoundEventPacket() {
	static uintptr_t** LevelSoundEventPacketVtable = 0x0;
	if (LevelSoundEventPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 4D ? C7 45 ? ? ? ? ? F2 0F 10 00 F2 0F 11 45 ? 8B 40 ? 89 45 ? C7 45");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		LevelSoundEventPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (LevelSoundEventPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(LevelSoundEventPacket));  // Avoid overwriting vtable
	vTable = LevelSoundEventPacketVtable;
	this->entityType.setText("minecraft:player");
}
PlayerAuthInputPacket::PlayerAuthInputPacket() {
	static uintptr_t** PlayerAuthInputPacketVtable = 0x0;
	if (PlayerAuthInputPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 0F 57 C0 0F 11 00 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 48 8D 05 ? ? ? ? 48 89 02 33 C0 48 89 42");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		PlayerAuthInputPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (PlayerAuthInputPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerAuthInputPacket));  // Avoid overwriting vtable
	vTable = PlayerAuthInputPacketVtable;
}
PlayerAuthInputPacket::PlayerAuthInputPacket(vec3_t pos, float pitch, float yaw, float yawUnused) {
	static uintptr_t** PlayerAuthInputPacketVtable = 0x0;
	if (PlayerAuthInputPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 0F 57 C0 0F 11 00 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 48 8D 05 ? ? ? ? 48 89 02 33 C0 48 89 42");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		PlayerAuthInputPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (PlayerAuthInputPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerAuthInputPacket));  // Avoid overwriting vtable
	vTable = PlayerAuthInputPacketVtable;
	this->pos = pos;
	this->pitch = pitch;
	this->yaw = yaw;
	this->yawUnused = yawUnused;
	this->InputAD = 0.f;
	this->InputWS = 0.f;
}

C_MobEquipmentPacket::C_MobEquipmentPacket() {
	static uintptr_t** MobEquipmentPacketVtable = 0x0;
	if (MobEquipmentPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 06 48 8B C6 4C 8D 9C 24");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		MobEquipmentPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (MobEquipmentPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_MobEquipmentPacket));  // Avoid overwriting vtable
	vTable = MobEquipmentPacketVtable;
}

C_MobEquipmentPacket::C_MobEquipmentPacket(__int64 entityRuntimeId, C_ItemStack* item, int hotbarSlot, int inventorySlot) {
	memset(this, 0x0, sizeof(C_MobEquipmentPacket));
	using MobEquimentPacketConstructor_t = void(__fastcall*)(C_MobEquipmentPacket*, __int64, C_ItemStack*, int, int, char);
	static MobEquimentPacketConstructor_t MobEquimentPacketConstructor = reinterpret_cast<MobEquimentPacketConstructor_t>(Utils::getBase() + 0x19AA9B0);

	if (MobEquimentPacketConstructor != 0)
		MobEquimentPacketConstructor(this, entityRuntimeId, item, hotbarSlot, inventorySlot, 0);
}

C_InventoryTransactionPacket::C_InventoryTransactionPacket() {
	static uintptr_t** InventoryTransactionPacketVtable = 0x0;
	if (InventoryTransactionPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 89 48 40 48 89 48 48 48 89 48 50");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		InventoryTransactionPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (InventoryTransactionPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_InventoryTransactionPacket));  // Avoid overwriting vtable
	vTable = InventoryTransactionPacketVtable;
}

C_MovePlayerPacket::C_MovePlayerPacket() {
	static uintptr_t** movePlayerPacketVtable = 0x0;
	if (movePlayerPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 49 89 40 08 48 8B 47 30");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		movePlayerPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (movePlayerPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_MovePlayerPacket));  // Avoid overwriting vtable
	vTable = movePlayerPacketVtable;
}

C_MovePlayerPacket::C_MovePlayerPacket(C_LocalPlayer* player, vec3_t pos) {
	static uintptr_t** movePlayerPacketVtable = 0x0;
	if (movePlayerPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 49 89 40 08 48 8B 47 30");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		movePlayerPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (movePlayerPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_MovePlayerPacket));  // Avoid overwriting vtable
	vTable = movePlayerPacketVtable;
	entityRuntimeID = player->entityRuntimeId;
	Position = pos;
	pitch = player->pitch;
	yaw = player->yaw;
	headYaw = player->yaw;
	onGround = true;
	mode = 0;
}

C_MovePlayerPacket::C_MovePlayerPacket(C_LocalPlayer* player, vec3_t pos, bool onground = true) {
	static uintptr_t** movePlayerPacketVtable = 0x0;
	if (movePlayerPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 49 89 40 08 48 8B 47 30");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		movePlayerPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (movePlayerPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_MovePlayerPacket));  // Avoid overwriting vtable
	vTable = movePlayerPacketVtable;
	entityRuntimeID = player->entityRuntimeId;
	Position = pos;
	pitch = player->pitch;
	yaw = player->yaw;
	headYaw = player->yaw;
	onGround = onground;
	mode = 0;
}

C_PlayerActionPacket::C_PlayerActionPacket() {
	static uintptr_t** playerActionPacketVtable = 0x0;
	if (playerActionPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 4D ? 66 0F 7F 45 ? C7 45 ? ? ? ? ? 48 89 45 ? 49 8B 8D");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		playerActionPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (playerActionPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_PlayerActionPacket));  // Avoid overwriting vtable
	vTable = playerActionPacketVtable;
}

C_SubChunkRequestPacket::C_SubChunkRequestPacket() {
	static uintptr_t** subChunkRequestPacketVtable = 0x0;
	if (subChunkRequestPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 8B F9 48 89 01 8B DA 48 83 C1 50 E8 ? ? ? ? 48 8D 4F 38 E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 07");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		subChunkRequestPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (subChunkRequestPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_SubChunkRequestPacket));  // Avoid overwriting vtable
	vTable = subChunkRequestPacketVtable;
}

C_EmotePacket::C_EmotePacket() {
	static uintptr_t** emotePacketVtable = 0x0;
	if (emotePacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 8D ? ? ? ? 48 89 85 ? ? ? ? 48 89 BD");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		emotePacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (emotePacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_EmotePacket));  // Avoid overwriting vtable
	vTable = emotePacketVtable;
}
//48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 89 48 40 48 89 48 48 48 89 48 58 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 66 C7 40 ? ? ? 48 89 50 10 48 C7 40 ? ? ? ? ? EB 04 33 C9 8B C1 48 8D 48 10 48 8D 15 ? ? ? ? 48 89 51 20 48 89 43 08 48 8B C3 48 89 0B 48 83 C4 30 5B C3 48 8D 0D ? ? ? ? E8 ? ? ? ? 83 3D ? ? ? ? ? 0F 85 ? ? ? ? 48 8D 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 89 05 ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC 30 65 48 8B 04 25 ? ? ? ? 48 8B D9 48 89 4C 24 ? BA ? ? ? ? 48 8B 08 8B 04 0A 39 05 ? ? ? ? 0F 8F ? ? ? ? E8 ? ? ? ? 48 8B C8 BA ? ? ? ? 48 8B 00 FF 50 08 48 89 44 24 ? 48 85 C0 74 4F
//Model Form Response packet
C_AnimatePacket::C_AnimatePacket() {
	static uintptr_t** animatePacketVtable = 0x0;
	if (animatePacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 4D ? 48 8B 8E");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		animatePacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (animatePacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_AnimatePacket));  // Avoid overwriting vtable
	vTable = animatePacketVtable;
}
/*
C_AnimatePacket::C_AnimatePacket(AnimatePacket Action, __int64 entityRuntimeId, float) {
}
*/

C_NPCRequestPacket::C_NPCRequestPacket() {
	static uintptr_t** npcRequestPacketVtable = 0x0;
	if (npcRequestPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 03 48 89 53 30");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		npcRequestPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (npcRequestPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_NPCRequestPacket));  // Avoid overwriting vtable
	vTable = npcRequestPacketVtable;
}

PlayerSkinPacket::PlayerSkinPacket() {
	static uintptr_t** playerSkinPacketVtable = 0x0;
	if (playerSkinPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 48 89 79 30 48 89 79 38 48 83 C1 40");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		playerSkinPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (playerSkinPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerSkinPacket));  // Avoid overwriting vtable
	vTable = playerSkinPacketVtable;
}

NetworkLatencyPacket::NetworkLatencyPacket() {
	static uintptr_t** networkLatencyPacketVtable = 0x0;
	if (networkLatencyPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 C7 41 ? ? ? ? ? 66 C7 41 ? ? ? FF 15");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		networkLatencyPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (networkLatencyPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(NetworkLatencyPacket));  // Avoid overwriting vtable
	vTable = networkLatencyPacketVtable;
}

NetworkLatencyPacket::NetworkLatencyPacket(long timestamp, bool sendback) {
	static uintptr_t** networkLatencyPacketVtable = 0x0;
	if (networkLatencyPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 C7 41 ? ? ? ? ? 66 C7 41 ? ? ? FF 15");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		networkLatencyPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (networkLatencyPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(NetworkLatencyPacket));  // Avoid overwriting vtable
	vTable = networkLatencyPacketVtable;
	this->timeStamp = timestamp;
	this->sendBack = sendback;
}

CommandRequestPacket::CommandRequestPacket() {
	static uintptr_t** commandRequestPacketVtable = 0x0;
	if (commandRequestPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ?? ?? ?? ?? 0F 11 00 C7 40 ?? ?? ?? ?? ?? C7 40 ?? ?? ?? ?? ?? 48 8D 05 ?? ?? ?? ?? 48 89 02 33 C0 48 89 42 28 48 89 42 30 89 42 38 48 89 42 40 48 89 42 50 88 42 60");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		commandRequestPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
#ifdef _DEBUG
		if (commandRequestPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(CommandRequestPacket));
	vTable = commandRequestPacketVtable;
}

C_TextPacket::C_TextPacket() {
	static uintptr_t** textPacketVtable = 0x0;
	if (textPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 44 24 ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8B 4D");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		textPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (textPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_TextPacket));  // Avoid overwriting vtable
	vTable = textPacketVtable;

	messageType = 1;  // TYPE_CHAT
}

C_InteractPacket::C_InteractPacket(/*enum InteractPacket::Action, class ActorRuntimeID, vec3_t const&*/) {
	static uintptr_t** interactPacketVtable = 0x0;
	if (interactPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 88 48 40 48 89 48 48 48 89 48 50");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		interactPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (interactPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_InteractPacket));  // Avoid overwriting vtable
	vTable = interactPacketVtable;
}

ActorEventPacket::ActorEventPacket(uint64_t entityRuntimeId, char eventId, int16_t itemId) {
	static uintptr_t** actorEvenPacketVtable = 0x0;
	if (actorEvenPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 4D ? 48 89 45 ? C6 45 ? ? 44 89 7D");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		actorEvenPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (actorEvenPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(ActorEventPacket));  // Avoid overwriting vtable
	vTable = actorEvenPacketVtable;
	this->two = 2;
	this->one = 1;
	this->entityRuntimeId = entityRuntimeId;
	this->eventId = eventId;
	this->itemId = itemId;
}