#pragma once
#include "../../Utils/MemoryUtil.h"
#include "../World/Actor.h"

class BaseActorRenderContext;

class ActorRenderDispatcher {
public:
	void render(BaseActorRenderContext* entityRenderContext, Actor* entity, Vec3<float>* cameraTargetPos, Vec3<float>* pos, Vec2<float>* rot, bool ignoreLighting) {
		using func_t = void(__thiscall*)(ActorRenderDispatcher*, BaseActorRenderContext*, Actor*, Vec3<float>*, Vec3<float>*, Vec2<float>*, bool);
		static func_t Func = (func_t)MemoryUtil::getFuncFromCall(MemoryUtil::findSignature("E8 ? ? ? ? 0F 57 C0 0F 11 85 ? ? ? ? 48 8D 95 ? ? ? ? 48 8B 8F"));
		Func(this, entityRenderContext, entity, cameraTargetPos, pos, rot, ignoreLighting);
	}

	void render(BaseActorRenderContext* entityRenderContext, Actor* entity, bool ignoreLighting) {
		using func_t = void(__thiscall*)(ActorRenderDispatcher*, BaseActorRenderContext*, Actor*, bool);
		static func_t Func = (func_t)MemoryUtil::findSignature("48 8B C4 53 55 56 57 41 56 48 81 EC ? ? ? ? 0F 29 70 C8 48 8B F2");
		Func(this, entityRenderContext, entity, ignoreLighting);
	}
};