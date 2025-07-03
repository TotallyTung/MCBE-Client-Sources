#pragma once
#include "../../FuncHook.h"

struct ActorPartModel {
	Vec3<float> mPos;  // this+0x00
	Vec3<float> mRot;  // this+0x0C
	Vec3<float> mSize; // this+0x18
};

struct ActorModel {
	ActorPartModel* models[6];

	ActorModel() {
		memset(this, 0, sizeof(ActorModel));
	}

	__forceinline bool isValid() {
		for (int i = 0; i < 6; i++) {
			if (models[i] == nullptr) return false;
		}
		return true;
	}
};

class Model {
public:
	static inline class std::map<Actor*, ActorModel> actorModels;
	static inline bool set = false;
	static inline Vec3<float> armPos = Vec3<float>(1, 1, 1);
	static inline Vec3<float> armSize = Vec3<float>(1, 1, 1);
	static inline Vec3<float> armRot = Vec3<float>(1, 1, 1);

	static inline bool canFindPlayer(Actor* ent) {
		auto list = Game.getLocalPlayer()->level->getRuntimeActorList();
		for (Actor* p : list) {
			if (p == ent) return true;
		}
		return false;
	}
};



class ActorModelHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(__int64 a1, __int64 a2, __int64 a3, float a4, int a5);
	static inline func_t oFunc;

	static void ActorModelCallBack(__int64 a1, __int64 a2, __int64 a3, float a4, int a5) {

			auto ent = *reinterpret_cast<Actor**>(a2 + 0x38);
			if (!ent || !Game.getClientInstance() || !Game.canUseMoveKeys()) return;

			if (!Model::canFindPlayer(ent) || ent == Game.getLocalPlayer()) return;

			const char* name = *reinterpret_cast<const char**>(a3 + 0x10);
			ActorPartModel* part = reinterpret_cast<ActorPartModel*>(a3 + 0xEC);

			if (!name || !part) return;

			auto& model = Model::actorModels[ent];

			if (strcmp(name, "head") == 0) model.models[0] = part;
			else if (strcmp(name, "leftarm") == 0) model.models[1] = part;
			else if (strcmp(name, "rightarm") == 0) model.models[2] = part;
			else if (strcmp(name, "leftleg") == 0) model.models[3] = part;
			else if (strcmp(name, "rightleg") == 0) model.models[4] = part;
			else if (strcmp(name, "body") == 0) model.models[5] = part;

		oFunc(a1, a2, a3, a4, a5);
	}
public:
	ActorModelHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorModelCallBack;
	}
};