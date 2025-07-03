#pragma once

namespace GlobalVariables
{
	std::map<uint64_t, bool> mKey = {};
	float mLastPing = 0;
	float mFPS = 0;

	Color mClientMainColor = { 74,144,226 };
	Color mClientSecondColor = { 51, 255, 187 };
	
	int mClientColor = 0;
	std::string mClientVersion = "2.1";

	bool mShouldBlock = false;
	bool mIsMiningRedstone = false;
}

struct InstanceManager {
private:
	static inline std::unordered_map<std::type_index, void*> mInstanceMap = {
		{ typeid(MinecraftUIRenderContext), nullptr },
		{ typeid(Font), nullptr },
		{ typeid(ClientInstance), nullptr },
		{ typeid(ScreenView), nullptr },
		{ typeid(RakNet::RakPeer), nullptr },
		{ typeid(ActorAnimationControllerPlayer), nullptr },
		{ typeid(ActorRenderDispatcher), nullptr },
		{ typeid(Tessellator), nullptr }
	};

public:
	template <typename T>
	static void set(T* instance) {
		mInstanceMap[typeid(T)] = instance;
	}

	template <typename T>
	static T* get() {
		auto it = mInstanceMap.find(typeid(T));
		if (it != mInstanceMap.end()) {
			return static_cast<T*>(it->second);
		}
		throw std::runtime_error("Instance not found");
	}

	static Player* getLocalPlayer() {
		return get<ClientInstance>()->getLocalPlayer();
	}

	static BlockSource* getBlockSource() {
		return get<ClientInstance>()->getLocalPlayer()->getComponent<BlockSourceComponent>()->mRegion;
	}

	static bool isLocalPlayer(Actor* mActor, bool mUseComponent = false) {
		return mUseComponent ? mActor == get<ClientInstance>()->getLocalPlayer() : mActor->hasComponent<LocalPlayerComponent>();
	}

	static bool isAllowedToUseKeys() {
		return !get<ClientInstance>()->isMouseGrabbed();
	}
};
