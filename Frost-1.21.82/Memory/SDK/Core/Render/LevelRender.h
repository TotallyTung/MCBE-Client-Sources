#pragma once

class LevelRendererPlayer
{
public:
	Vector3<float> getCameraPos() {
		return hat::member_at<Vector3<float>>(this, OffsetManager::LevelRendererPlayer_mCameraPos); // 1.21.50
	}

	//CLASS_FIELD(float, fovMultiplier, 0xD78);
	//CLASS_FIELD(float, lastFovMultiplier, 0xD7C);
};

class LevelRender {
public:
	LevelRendererPlayer* getLevelRendererPlayer()
	{
		return hat::member_at<LevelRendererPlayer*>(this, OffsetManager::LevelRenderer_mRendererPlayer); // 1.21.50
		//return *reinterpret_cast<LevelRendererPlayer**>((uintptr_t)(this) + 0x308); // 1.21.30 // Under level renderer index
		/*
		* v8 = *(_DWORD **)(v7 + 0x308);
		*(_DWORD *)(a1 + 132) = v8[383];
		*(_DWORD *)(a1 + 136) = v8[384];
		 (_DWORD *)(a1 + 140) = v8[385];
		*/
	};

	Vector3<float> getOrigin() {
		return getLevelRendererPlayer()->getCameraPos();
	};
};