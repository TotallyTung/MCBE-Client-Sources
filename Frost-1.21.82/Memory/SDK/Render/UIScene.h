#pragma once

class UIScene {
public:
	uintptr_t         vtable;        // this+0x0
    char              pad_0x8[0x20]; // this+0x8
    std::string       mName;         // this+0x28
	class ScreenView* mScreenView;    // this+0x30
public:
	/*void getScreenName(std::string* txt) {
		Memory::CallVFunc<68, void, std::string*>(this, txt); //84 //92 //107 
	}*/
};

class SceneStack {
public:
	bool pushScene(UIScene* scene, bool unk = false);
};

enum class PlayScreenDefaultTab : int32_t {
	WorldTab,
	FriendsTab,
	ServersTab,
};

class SceneFactory {
public:
	UIScene* createPlayScreen(PlayScreenDefaultTab tab);
	UIScene* createOptionsScreen();
};
