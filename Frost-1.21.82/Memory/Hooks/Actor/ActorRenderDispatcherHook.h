#pragma once
#include "../../../Client/Modules/Visual/ServerRotations.hpp"

void* onActorRenderDispatcher;

void* applyRotationChanges(ActorRenderDispatcher* dispatcher, BaseActorRenderContext* renderContext, Actor* actor, Vector3<float> cameraPos, Vector3<float> actorPos, Vector2<float>* rotation, bool ignoreLighting) {
	const auto serverRotationsModule = getModuleByType<ServerRotations>();

	if (!serverRotationsModule || !serverRotationsModule->isEnabled()) {
		return Memory::CallFunc<void*, ActorRenderDispatcher*, BaseActorRenderContext*, Actor*, Vector3<float>, Vector3<float>, Vector2<float>*, bool>(
			onActorRenderDispatcher, dispatcher, renderContext, actor, cameraPos, actorPos, rotation, ignoreLighting
		);
	}

	// Lambda to backup and restore original actor rotation state.
	auto applyRotation = [&](auto applyFunc, auto restoreFunc) {
		auto getRotationComponent = [&](auto componentType) {
			auto* component = actor->getComponent<decltype(componentType)>();
			if (!component) throw std::runtime_error("Component not found");
			return component;
			};

		auto pitchComponent = getRotationComponent(ActorRotationComponent());
		auto headYawComponent = getRotationComponent(ActorHeadRotationComponent());
		auto bodyYawComponent = getRotationComponent(MobBodyRotationComponent());

		auto backupOriginalState = [&](auto& pitch, auto& headYaw, auto& bodyYaw) {
			pitch = actor->getPitch();
			headYaw = actor->getHeadYaw();
			bodyYaw = actor->getBodyRotation();
			};

		auto restoreOriginalState = [&](float pitch, float headYaw, float bodyYaw) {
			actor->setPitch(pitch);
			pitchComponent->mPrevRotation.x = pitch;

			actor->setHeadYaw(headYaw);
			headYawComponent->mRotation.y = headYaw;

			actor->setBodyRotation(bodyYaw);
			bodyYawComponent->mPrevBodyRot = bodyYaw;
			};

		float originalPitch, originalHeadYaw, originalBodyYaw;
		backupOriginalState(originalPitch, originalHeadYaw, originalBodyYaw);

		applyFunc(serverRotationsModule, pitchComponent, headYawComponent, bodyYawComponent);

		auto ret = Memory::CallFunc<void*, ActorRenderDispatcher*, BaseActorRenderContext*, Actor*, Vector3<float>, Vector3<float>, Vector2<float>*, bool>(
			onActorRenderDispatcher, dispatcher, renderContext, actor, cameraPos, actorPos, rotation, ignoreLighting
		);

		restoreFunc(originalPitch, originalHeadYaw, originalBodyYaw);

		return ret;
		};

	auto applyEasedRotations = [&](ServerRotations* rotations, auto* pitchComponent, auto* headYawComponent, auto* bodyYawComponent) {
		actor->setPitch(rotations->mEasedPitch);
		pitchComponent->mPrevRotation.x = rotations->mEasedPitch;

		actor->setHeadYaw(rotations->mEasedHeadYaw);
		headYawComponent->mRotation.y = rotations->mEasedHeadYaw;

		actor->setBodyRotation(rotations->mEasedBodyYaw);
		bodyYawComponent->mPrevBodyRot = rotations->mEasedBodyYaw;
		};

	auto restoreOriginalValues = [&](float pitch, float headYaw, float bodyYaw) {
		actor->setPitch(pitch);
		actor->getComponent<ActorRotationComponent>()->mPrevRotation.x = pitch;

		actor->setHeadYaw(headYaw);
		actor->getComponent<ActorHeadRotationComponent>()->mRotation.y = headYaw;

		actor->setBodyRotation(bodyYaw);
		actor->getComponent<MobBodyRotationComponent>()->mPrevBodyRot = bodyYaw;
		};

	return applyRotation(applyEasedRotations, restoreOriginalValues);
}

void* ActorRenderDispatcherDetour(ActorRenderDispatcher* _this, BaseActorRenderContext* entityRenderContext, Actor* actor, Vector3<float> cameraTargetPos, Vector3<float> actorPosition, Vector2<float>* rotation, bool ignoreLighting) {
	InstanceManager::set<ActorRenderDispatcher>(_this);

	Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
	if (!mPlayer)
		return nullptr;

	if (actor != mPlayer) {
		return Memory::CallFunc<void*, ActorRenderDispatcher*, BaseActorRenderContext*, Actor*, Vector3<float>, Vector3<float>, Vector2<float>*, bool>( // CallFunc to call the original.
			onActorRenderDispatcher, _this, entityRenderContext, actor, cameraTargetPos, actorPosition, rotation, ignoreLighting);
	}

	applyRotationChanges(_this, entityRenderContext, actor, cameraTargetPos, actorPosition, rotation, ignoreLighting);

	return nullptr;
}

class ActorRenderDispatcherHook : public FuncHook 
{
public:
	bool Initialize() override
	{
		Memory::HookFunction(SigManager::ActorRenderDispatcher_render, (void*)&ActorRenderDispatcherDetour, &onActorRenderDispatcher, "ActorRenderDispatcher::render");

		return true;
	}
};