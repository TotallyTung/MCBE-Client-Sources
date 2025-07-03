#pragma once


class PlayerActionPacket : public Packet {
public:
	Vector3<int>         mPos;                               // this+0x30
	Vector3<int>         mResultPos;                         // this+0x3C
	int                  mFace;                              // this+0x48
	PlayerActionType     mAction;                            // this+0x4C
	int                  mRuntimeId;                         // this+0x50
	bool                 mtIsFromServerPlayerMovementSystem; // this+0x58
};