#pragma once

class ActorRenderDispatcher {
public:
	void render(BaseActorRenderContext* entityRenderContext, Actor* entity, Vector3<float> cameraTargetPos, Vector3<float> position, Vector2<float> rotation, bool ignoreLighting) {
		Memory::CallFunc<void, ActorRenderDispatcher*, BaseActorRenderContext*, Actor*, Vector3<float>, Vector3<float>, Vector2<float>, bool>(
			SigManager::ActorRenderDispatcher_render, this, entityRenderContext, entity, cameraTargetPos, position, rotation, ignoreLighting);
	}
};