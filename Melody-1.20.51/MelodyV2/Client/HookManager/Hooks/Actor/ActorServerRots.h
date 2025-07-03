// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../SDK/Classes/Packet.h"
#include "../../../../SDK/Classes/StrictEntityContext.h"
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/ActorRotate.h"
#include "../../../ModuleManager/Modules/Movement/NoSlowDown.h"

class ActorServerRots {
protected:
	using func_t = void(__fastcall*)(class ActorRenderDispatcher*, class BaseActorRenderContext*, Actor*, Vec3<float>*, Vec3<float>*, Vec2<float>*, bool);
	static inline func_t func;

	static void Actor_ServerRots(class ActorRenderDispatcher* self, class BaseActorRenderContext* context, Actor* actor, Vec3<float>* param_3, Vec3<float>* param_4, Vec2<float>* param_5, bool param_6) {
		//return true;

		std::array<Vec2<float>, 4> restoreRotation;
		restoreRotation[0] = actor->rotationComponent->rotation;
		restoreRotation[1] = actor->rotationComponent->oldRotation;
		auto* headRotComponent = actor->getActorHeadRotationComponent();
		if (headRotComponent)
		{
			restoreRotation[2] = { headRotComponent->headRot, headRotComponent->oldHeadRot };
		}

		auto* bodyRotComponent = actor->getMobBodyRotationComponent();
		if (bodyRotComponent)
		{
			restoreRotation[3] = { bodyRotComponent->yBodyRot, bodyRotComponent->yOldBodyRot };
		}

		//auto event = nes::make_holder<ActorRotateEvent>(actor, actor->rotationComponent->rotation, actor->rotationComponent->oldRotation, RotationType::RENDER);
		PlayerAuthInputPacket p;
		actor->rotationComponent->rotation = p.rotation;
		//actor->rotationComponent->oldRotation = event->getOldRotation();
		if (headRotComponent)
		{
			headRotComponent->headRot = p.headYaw;
			//headRotComponent->oldHeadRot = event->getOldYaw();
		}
		if (bodyRotComponent)
		{
			bodyRotComponent->yBodyRot = p.rotation.y;
			//bodyRotComponent->yOldBodyRot = event->getOldYaw();
		}

		return func(self, context, actor, param_3, param_4, param_5, param_6);

		actor->rotationComponent->rotation = restoreRotation[0];
		actor->rotationComponent->oldRotation = restoreRotation[1];
		if (headRotComponent)
		{
			headRotComponent->headRot = restoreRotation[2].x;
			headRotComponent->oldHeadRot = restoreRotation[2].y;
		}
		if (bodyRotComponent)
		{
			bodyRotComponent->yBodyRot = restoreRotation[3].x; 
			bodyRotComponent->yOldBodyRot = restoreRotation[3].y;
		}

		//static NoSlowDown* NoSlowDownMod = (NoSlowDown*)client->moduleMgr->getModule("NoSlowDown");
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::ServerRotsHook);
		MemoryUtils::CreateHook("Actor_ServerRotsHook", address, (void*)&ActorServerRots::Actor_ServerRots, (void*)&func);
	}
};
