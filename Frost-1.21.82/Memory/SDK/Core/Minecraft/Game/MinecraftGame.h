#pragma once

// MouseGrabbed: 80 B9 ? ? ? ? ? 0F 84 ? ? ? ? 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 85 C0

class MinecraftGame
{
public:
	bool getMouseGrabbed() {
		return hat::member_at<bool>(this, 0x1A0); // MinecraftGame_mMouseGrabbed 0xCE8
	}

	void setMouseGrabbed(bool mGrabbed) {
		hat::member_at<bool>(this, 0x1A0) = mGrabbed; // MinecraftGame_mMouseGrabbed 0xCE8
	}
};