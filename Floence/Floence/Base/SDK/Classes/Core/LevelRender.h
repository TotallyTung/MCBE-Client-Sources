#pragma once
class MaterialPtr;
class LevelRendererPlayer
{
public:
	BUILD_ACCESS(Vector3<float>, cameraPos, 0x65C); // 1.21.73
};

class LevelRender {
public:
	LevelRendererPlayer* getLevelRendererPlayer()
	{
		return *reinterpret_cast<LevelRendererPlayer**>((uintptr_t)(this) + 0x328); // 1.21.73 // Under level renderer index
		/*
		* v8 = *(_DWORD **)(v7 + 0x308);
        *(_DWORD *)(a1 + 132) = v8[383];
        *(_DWORD *)(a1 + 136) = v8[384];
         (_DWORD *)(a1 + 140) = v8[385];
		*/
	};

	Vector3<float> getOrigin() {
		return getLevelRendererPlayer()->cameraPos;
	};
};