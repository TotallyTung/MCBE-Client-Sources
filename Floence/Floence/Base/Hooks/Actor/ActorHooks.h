#pragma once

// Declare a void pointer(Empty object) called onActorRenderDispatcher which is the CallBack for ActorRenderDispatcherDetour.
void* onActorRenderDispatcher; // Defined an orignal for onActorRenderDispatcher

// Declare a void pointer(Empty object) called onIntersects which is the CallBack for IntersectsDetour.
void* onIntersects; // Defined an orignal for onIntersects

// Declare a void pointer(Empty object) called onIsImmobile which is the CallBack for IsImmobileDetour.
void* onIsImmobile; // Defined an orignal for onIsImmobile

// Declare a void pointer(Empty object) called onIsInvisible which is the CallBack for IsInvisibleDetour.
void* onIsInvisible; // Defined an orignal for onIsInvisible

void* onIsOnFire;

void* onCanBeEffected;

void* onDestroySpeed;

void* onActorBaseTick;

void* ApplyRotationChanges(void* _this, void* a2, Actor* actor, Vector3<float> cameraPos, Vector3<float> actorPosition, Vector2<float>* a6, bool renderCurrentPos) {
	bool displayRotations = getModuleByName("serverrotations")->isEnabled();

	if (!displayRotations) {
		return Memory::CallFunc<void*, void*, void*, Actor*, Vector3<float>, Vector3<float>, Vector2<float>*, bool>( // CallFunc to call the original.
			onActorRenderDispatcher, _this, a2, actor, cameraPos, actorPosition, a6, renderCurrentPos
		);
	}

	float pitch = actor->getPitch();
	float bodyYaw = actor->getBodyRotation();
	float headYaw = actor->getHeadYaw();

	float oldPitch = actor->getComponent<ActorRotationComponent>()->oldRotation.x;
	float oldHeadYaw = actor->getComponent<ActorHeadRotationComponent>()->rotation.y;
	float oldBodyYaw = actor->getComponent<MobBodyRotationComponent>()->prevBodyRot;

	// Pitch
	actor->setPitch(Global::easedPitch);
	actor->getComponent<ActorRotationComponent>()->oldRotation.x = Global::easedPitch;

	// Head Yaw
	actor->setHeadYaw(Global::easedHeadYaw);
	actor->getComponent<ActorHeadRotationComponent>()->rotation.y = Global::easedHeadYaw;

	// Body Yaw
	actor->setBodyRotation(Global::easedBodyYaw);
	actor->getComponent<MobBodyRotationComponent>()->prevBodyRot = Global::easedBodyYaw;

	if (getModuleByName("testmodule")->isEnabled()) {
		//actor->getSupplies()->hotbarSlot = 1;
	}

	auto ret = Memory::CallFunc<void*, void*, void*, Actor*, Vector3<float>, Vector3<float>, Vector2<float>*, bool>( // CallFunc to call the original.
		onActorRenderDispatcher, _this, a2, actor, cameraPos, actorPosition, a6, renderCurrentPos
	);

	if (getModuleByName("testmodule")->isEnabled()) {
		//actor->getSupplies()->hotbarSlot = 0;
	}

	// Pitch
	actor->setPitch(pitch);
	actor->getComponent<ActorRotationComponent>()->oldRotation.x = oldPitch;

	// Head Yaw
	actor->setHeadYaw(headYaw);
	actor->getComponent<ActorHeadRotationComponent>()->rotation.y = oldHeadYaw;

	// Body Yaw
	actor->setBodyRotation(bodyYaw);
	actor->getComponent<MobBodyRotationComponent>()->prevBodyRot = oldBodyYaw;

	return ret;
}

void* ActorRenderDispatcherDetour(void* _this, void* a2, Actor* actor, Vector3<float> cameraPos, Vector3<float> actorPosition, Vector2<float>* a6, bool renderCurrentPos) {
	/*if (actor == Address::getLocalPlayer()) {
		if (getModuleByName("desync")->isEnabled() || getModuleByName("fly")->isEnabled()) {
			auto newPos = Global::testPosition;
			auto newPosCalc = actorPosition.submissive(cameraPos).submissive(actorPosition).add(newPos);

			if (cameraPos.x == 0 && cameraPos.y == 0 && cameraPos.z == 0) {
				Global::stopESP = true;
			}
			else {
				Global::stopESP = false;
			}

			// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
			Memory::CallFunc<void*, void*, void*, Actor*, Vector3<float>, Vector3<float>, Vector2<float>*, bool>( // CallFunc to call the original.
				onActorRenderDispatcher, _this, a2, actor, cameraPos, newPosCalc, a6, false
			);
		}
	}*/

	Player* player = Address::getLocalPlayer();

	bool isLocalPlayer = actor == player;

	if (actor != player) {
		return Memory::CallFunc<void*, void*, void*, Actor*, Vector3<float>, Vector3<float>, Vector2<float>*, bool>( // CallFunc to call the original.
			onActorRenderDispatcher, _this, a2, actor, cameraPos, actorPosition, a6, renderCurrentPos
		);
	}

	if (getModuleByName("desync")->isEnabled()) {
		auto newPos = Global::desyncPosition;
		auto newPosCalc = actorPosition.submissive(cameraPos).submissive(actorPosition).add(newPos);

		return Memory::CallFunc<void*, void*, void*, Actor*, Vector3<float>, Vector3<float>, Vector2<float>*, bool>( // CallFunc to call the original.
			onActorRenderDispatcher, _this, a2, actor, cameraPos, newPosCalc, a6, renderCurrentPos
		);
	}

	if (getModuleByName("freecam")->isEnabled()) {
		auto newPos = Global::freecamPosition;
		auto newPosCalc = actorPosition.submissive(cameraPos).submissive(actorPosition).add(newPos);

		return Memory::CallFunc<void*, void*, void*, Actor*, Vector3<float>, Vector3<float>, Vector2<float>*, bool>( // CallFunc to call the original.
			onActorRenderDispatcher, _this, a2, actor, cameraPos, newPosCalc, a6, renderCurrentPos
		);
	}

	ApplyRotationChanges(_this, a2, actor, cameraPos, actorPosition, a6, renderCurrentPos);

	return nullptr;
}

bool IntersectsDetour(Actor* _this, Vector3<float> pos1, Vector3<float> pos2) {
	IntersectsTickEvent event{}; // IntersectsTickEvent
	event.cancelled = nullptr;
	CallBackEvent(&event); // Call IntersectsTick event for modules to be writen on this hook.

	// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
	return Memory::CallFunc<bool, Actor*, Vector3<float>, Vector3<float>>( // CallFunc to call the original.
		onIntersects, _this, pos1, pos2
	);
}

bool isImmobileDetour(Mob* _this)
{
	if (getModuleByName("antiimmobile")->isEnabled()) {
		return false;
	}

	// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
	Memory::CallFunc<bool, Mob*>( // CallFunc to call the original.
		onIsImmobile, _this
	);
}

bool isOnFireDetour(Actor* _this)
{
	Global::isOnFire = true;

	if (getModuleByName("nofire")->isEnabled()) {
		if (_this == Address::getLocalPlayer()) {
			return false;
		}
	}

	// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
	Memory::CallFunc<bool, Actor*>( // CallFunc to call the original.
		onIsOnFire, _this
	);
}

bool canBeAffected(Actor* _this, int duh) {
	if (_this == Address::getLocalPlayer()) {
		if (duh == 1) {
			Global::effectSwiftness = true;
			TimeUtils::resetTime("SwiftnessBoost");
		}
		ChatUtils::sendMessage(std::to_string(duh));
	}

	return Memory::CallFunc<bool, Actor*, int>(
		onCanBeEffected, _this, duh
	);
}

float DestroySpeedDetour(Player* _this, Block& a1)
{
	if (getModuleByName("fastbreak")->isEnabled()) {
		return Global::BreakSpeed;
	}

	// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
	Memory::CallFunc<float, Player*, Block&>( // CallFunc to call the original.
		onDestroySpeed, _this, a1
	);
}

void ActorBaseTickDetourHook(Actor* actor) // Actor*
{
	Player* localPlayer = Address::getLocalPlayer();

	if (localPlayer != nullptr && localPlayer == actor) {
		Level* level = actor->getLevel();
		GameMode* gm = localPlayer->getGamemode();


		if (Auau) {
			actor->getMoveInputHandler()->mJumping = false;
			actor->getMoveInputHandler()->mInputState.mJumpDown = false;
			actor->getMoveInputHandler()->mRawInputState.mJumpDown = false;
			Auau = false;
		}

		ActorBaseTickEvent event{ actor, level, gm }; // ActorBaseTickEvent
		event.cancelled = nullptr;
		CallBackEvent(&event); // Call ActorBaseTick event for modules to be writen on this hook.
	}

	// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
	Memory::CallFunc<void*, Actor*>( // CallFunc to call the original.
		onActorBaseTick, actor
	);
}

bool isInvisibleDetour(Actor* _this) {
	if (getModuleByName("antiinvis")->isEnabled())
		return false;

	// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
	Memory::CallFunc<bool, Actor*>( // CallFunc to call the original.
		onIsInvisible, _this
	);
}

class ActorHooks : public FuncHook { // a Animations class that inherits the FuncHook template we made
public:
	//
	bool Initialize() override // Now override the Initialize function so we can actually hook our stuff
	{
		// ActorRenderDispatcher
		void* ActorRenderDispatcherAddr = Memory::findSig(xorstr_("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 89 4C 24 ? 49 8B F8"));
		
		// Actor::intersects
		//void* intersectsAddr = Memory::findSig(xorstr_("48 83 EC ? 48 8B 81 ? ? ? ? 48 85 C0 74 50"));

		// Actor::isImmobile
		void* isImmobileAddr = Memory::findSig(xorstr_("40 53 48 83 EC 20 48 8D 59 08"));
		
		// Player::getDestroySpeed
		//void* destroySpeedAddr = Memory::findSig(xorstr_("48 89 5C ? ? 57 48 83 EC ? 48 8B FA ? ? 74 24 ? 48 8B 91"));

		// Attempt to hook the function at the address stored in ActorRenderDispatcher by replacing it with the address of ActorRenderDispatcherDetour and store the original function's address in onActorRenderDispatcher
		if (!Memory::HookFunction(ActorRenderDispatcherAddr, (void*)&ActorRenderDispatcherDetour, &onActorRenderDispatcher, xorstr_("ActorRenderDispatcher"))) { return false; }

		// Attempt to hook the function at the address stored in isImmobileAddr by replacing it with the address of isImmobileDetour and store the original function's address in onIsImmobile
		//if (!Memory::HookFunction(intersectsAddr, (void*)&IntersectsDetour, &onIntersects, xorstr_("Intersects"))) { return false; }

		// Attempt to hook the function at the address stored in destroySpeedAddr by replacing it with the address of destroySpeedDetour and store the original function's address in onDestroySpeed
		//if (!Memory::HookFunction(destroySpeedAddr, (void*)&DestroySpeedDetour, &onDestroySpeed, xorstr_("DestroySpeed"))) { return false; }


		std::thread([&] {
			while (!Address::getClientInstance() || !Address::getClientInstance()->getLocalPlayer()) {
				if (!isRunning) break;

				Sleep(0);
			}
			if (!isRunning) return;
			
			auto VTable = *(uintptr_t**)(Address::getClientInstance()->getLocalPlayer());

			// Attempt to hook the function at the address stored in isImmobileAddr by replacing it with the address of isImmobileDetour and store the original function's address in onIsImmobile
			if (!Memory::HookFunction((void*)VTable[40], (void*)&isImmobileDetour, &onIsImmobile, xorstr_("IsImmobile"))) { return; }

			// Attempt to hook the function at the address stored in isOnFireAddr by replacing it with the address of isOnFireDetour and store the original function's address in onIsOnFire
			if (!Memory::HookFunction((void*)VTable[48], (void*)&isOnFireDetour, &onIsOnFire, xorstr_("isOnFire"))) { return; }
			
			// Attempt to hook the function at the address stored in isInvisible by replacing it with the address of isInvisibleDetour and store the original function's address in onIsInvisible
			if (!Memory::HookFunction((void*)VTable[31], (void*)&isInvisibleDetour, &onIsInvisible, xorstr_("isInvisible"))) { return; }

			if (!Memory::HookFunction((void*)VTable[105], (void*)&canBeAffected, &onCanBeEffected, xorstr_("canBeEffected"))) { return; }

			if (!Memory::HookFunction((void*)VTable[25], (void*)&ActorBaseTickDetourHook, &onActorBaseTick, xorstr_("ActorBaseTick"))) { return; }
			}).detach();

		return true;
	}

	static ActorHooks& Instance() { // a class setup function called Instance.
		static ActorHooks instance;
		return instance;
	}
};