#pragma once

class SetActorMotionPacket : public Packet {
public:
	int64_t mRuntimeID;
	Vector3<float> mMotion;
};

