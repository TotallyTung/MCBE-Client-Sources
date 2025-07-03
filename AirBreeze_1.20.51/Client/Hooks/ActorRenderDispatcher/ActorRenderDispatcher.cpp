#include "ActorRenderDispatcher.h"

#include "../../Manager/Manager.h"
#include "../../Manager/Module.h"
#include "../../Manager/Category.h"
#include "../../Utils/Utils.h"
#include "../../SDK/Classes/Actor.h"

Manager* aRMgr = nullptr;

typedef void(__thiscall* ActorRender)(void* _this, void* a2, Actor* a3, Vec3<float>* cameraPos, Vec3<float>* actorPosition, Vec2<float>* a6, bool a7);
ActorRender _render;

void ActorRender_Callback(void* _this, void* a2, Actor* actor, Vec3<float>* cameraPos, Vec3<float>* actorPosition, Vec2<float>* a6, bool a7) {
    auto instance = Minecraft::getClientInstance();
    auto localPlayer = instance->getLocalPlayer();

    /*
        auto newPos = Vector3(50, 100, 50);
        auto newPosCalc = *actorPosition - *cameraPos - *actorPosition + newPos;
    */

    if (aRMgr != nullptr) {
        for (auto category : aRMgr->categories)
        {
            for (auto mod : category->modules)
            {
                if ((mod->name.rfind("TestModule") != std::string::npos) && mod->isEnabled && localPlayer != actor)
                {
                    actor = localPlayer;
                }
            }
        }
        
    }

    _render(_this, a2, actor, cameraPos, actorPosition, a6, a7);
}

auto Hook_ActorRender::init(void) -> StatusData {

    aRMgr = this->mgr;
    auto sig = Utils::findSig("48 89 5C 24 20 55 56 57 41 54 41 ? ? ? 41 57 48 8D 6C 24 F1 48 81 EC ? ? ? ? 48 8B 05 6D 32 4E 04");

    if (!sig)
        return StatusData(MethodStatus::Error, "[Actor sig] Failed to find Signature!");

    if (MH_CreateHook((void*)sig, &ActorRender_Callback, reinterpret_cast<LPVOID*>(&_render)) != MH_OK)
        return StatusData(MethodStatus::Error, "[Actor Hook] Failed to create hook!");

    if (MH_EnableHook((void*)sig) != MH_OK)
        return StatusData(MethodStatus::Error, "[Actor Hook] Failed to enable hook!");


    return StatusData(MethodStatus::Success, "[Actor Hook] Successfully hooked!");



};