#include "ActorTick.h"

#include "../../Manager/Manager.h"
#include "../../Manager/Module.h"
#include "../../Manager/Category.h"
#include "../../Utils/Utils.h"
#include "../../SDK/Classes/Actor.h"
#include "../../SDK/Classes/Level.h"
#include "../../SDK/Classes/Actor.h"

//I pasted from nrg
#define playerVtSig "48 8d 05 ? ? ? ? 48 89 01 b8 ? ? ? ? 8d 50 ? 44 8d 48 ? 44 8d 40 ? 66 89 44 24 ? e8 ? ? ? ? 48 8b 8b"
#define baseTickIndex 44 //idk why vtable is 44 maybe it 45(baseTick) - 1
//#define swingIndex 196

Manager* aMgr = nullptr;

typedef void(__thiscall* Actortick)(Actor*);
Actortick _tick;

static std::vector<Actor*> entities;
static std::vector<Actor*> players;

/*bool _collectPlayerToList(class Actor const& a1) {
    Actor* a2 = (Actor*)&a1;
    entities.push_back(a2);
    players.push_back(a2);
    return true;
}*/

void ActorBasetick_Callback(Actor* actor) {
    auto instance = Minecraft::getClientInstance();
    auto localPlayer = instance->getLocalPlayer();

	if (aMgr != nullptr) {
        if (localPlayer == actor)
        {
            /*if (localPlayer == nullptr || actor == nullptr) {
                entities.erase(std::remove(entities.begin(), entities.end(), actor), entities.end());
                players.erase(std::remove(players.begin(), players.end(), actor), players.end());
            }*/
            Level* level = actor->level;
            GameMode* gm = localPlayer->gamemode;

            // Collect entities
            /*entities.clear();
            players.clear();

            level->forEachPlayer(&_collectPlayerToList);
            for (Actor* a1 : level->getRuntimeActorList()) 
            {
                if (a1 != nullptr && !a1->isPlayer()) entities.push_back(a1);
                else if (a1 != nullptr && a1->isPlayer() && a1->isAlive()) players.push_back(a1);
            }
            entities.erase(std::remove(entities.begin(), entities.end(), actor), entities.end());
            players.erase(std::remove(players.begin(), players.end(), actor), players.end());
            */

            for (auto category : aMgr->categories) {
                for (auto mod : category->modules) {
                    if (mod->isEnabled) {
                        mod->onBaseTick(actor, level, gm);
                    }
                }
            }
        }
        
	}

	_tick(actor);
}

auto Hook_Actor::init(void) -> StatusData {

	aMgr = this->mgr;
	auto sig = Utils::findSig(playerVtSig);

    int offset = *reinterpret_cast<int*>(sig + 3);
    uintptr_t** Vtable = reinterpret_cast<uintptr_t**>(sig + offset + 7);

	if (!sig)
		return StatusData(MethodStatus::Error, "[Actor sig] Failed to find Signature!");

	if (MH_CreateHook((void*)Vtable[baseTickIndex], &ActorBasetick_Callback, reinterpret_cast<LPVOID*>(&_tick)) != MH_OK)
		return StatusData(MethodStatus::Error, "[Actor Hook] Failed to create hook!");

	if (MH_EnableHook((void*)Vtable[baseTickIndex]) != MH_OK)
		return StatusData(MethodStatus::Error, "[Actor Hook] Failed to enable hook!");


	return StatusData(MethodStatus::Success, "[Actor Hook] Successfully hooked!");



};