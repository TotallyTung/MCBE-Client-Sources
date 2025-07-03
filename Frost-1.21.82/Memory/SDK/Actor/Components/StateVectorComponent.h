#pragma once

struct StateVectorComponent
{
public:
	Vector3<float> mPosition;     // this0x0 
	Vector3<float> mPrevPosition; // this+0xC 
	Vector3<float> mVelocity;     // this+0x18 
};