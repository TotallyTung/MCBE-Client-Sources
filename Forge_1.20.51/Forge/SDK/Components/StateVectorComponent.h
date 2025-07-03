#pragma once

class StateVectorComponent
{
public:
	Vector3<float> currentPos; //0x0000
	Vector3<float> oldPos; //0x000C
	Vector3<float> velocity; //0x0018
	char pad_0024[116]; //0x0024
}; //Size: 0x0098