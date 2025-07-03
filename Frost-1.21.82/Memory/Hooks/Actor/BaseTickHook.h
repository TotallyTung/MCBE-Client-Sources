#pragma once

void* onBaseTick;

void BaseTickDetour(Actor* actor)
{
	Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();

	if (mPlayer != nullptr && mPlayer == actor) {
		Level* mLevel = actor->getLevel();
		GameMode* mGameMode = mPlayer->getGameMode();

		//ChatUtil::sendMessage(Util::combine("BlockID: ", InstanceManager::get<ClientInstance>()->getBlockSource()->getBlock(mPlayer->getLevel()->getHitResult()->IBlockPos)->getLegacyBlock()->getBlockID()));

		BaseTickEvent event{ actor, mLevel, mGameMode };
		EventManager::registerEvent(&event);
	}

	Memory::CallFunc<void*, Actor*>(
		onBaseTick, actor);
}

class BaseTickHook : public FuncHook 
{
public:
	bool Initialize() override
	{
		int offset = *reinterpret_cast<int*>((uintptr_t)SigManager::Actor_VTable + 3);
		uintptr_t** VTable = reinterpret_cast<uintptr_t**>((uintptr_t)SigManager::Actor_VTable + offset + 7);

		Memory::HookFunction(VTable[25], (void*)&BaseTickDetour, &onBaseTick, "Actor::baseTick");

		return true;
	}
};